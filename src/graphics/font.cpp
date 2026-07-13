#include <freetype/ftmm.h>
#include <astra/core/resource_manager.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/font.hpp>

namespace astra::graphics {
    Font::Font(const Desc &desc) : resourceManager(desc.resourceManager), m_face() {
        auto status = FT_New_Face(desc.library, desc.fontPath.c_str(), 0, &m_face);
        ASSERT(status == FT_Err_Ok);

        FT_MM_Var *mmVar = nullptr;
        FT_Get_MM_Var(m_face, &mmVar);
        if (mmVar == nullptr) return;
        m_coordsToTags.reserve(mmVar->num_axis);
        m_coordsToAxes.reserve(mmVar->num_axis);

        for (int i = 0; i < mmVar->num_axis; ++i) {
            const FT_Var_Axis &axis = mmVar->axis[i];
            m_coordsToTags.push_back(axis.tag);
            m_coordsToAxes.emplace_back(
                axis.minimum,
                axis.maximum,
                axis.def
            );
        }
    }

    Font::~Font() {
        FT_Done_Face(m_face);
    }

    void Font::setStyle(const FontStyle &style) {
        FontInstance target = {.size = style.size};
        for (size_t i = 0; i < m_coordsToTags.size(); ++i) {
            FT_Fixed value = m_coordsToAxes[i].def;
            for (size_t j = 0; j < style.getTags().size(); ++j) {
                if (style.getTags()[j] == m_coordsToTags[i]) {
                    value = std::clamp(
                        style.getValues()[j] << 16,
                        m_coordsToAxes[i].minimum,
                        m_coordsToAxes[i].maximum
                    );
                    break;
                }
            }
            target.coordsValues.push_back(value);
        }

        FT_Set_Pixel_Sizes(m_face, 0, target.size);
        FT_Set_Var_Design_Coordinates(m_face, target.coordsValues.size(),
                                      target.coordsValues.data());

        m_currentInstanceIndex = -1;
        for (int i = 0; i < m_instances.size(); i++) {
            if (m_instances[i].size == target.size && m_instances[i].coordsValues == target.coordsValues)
                m_currentInstanceIndex = i;
        }

        if (m_currentInstanceIndex != -1) return;
        target.matrix = {
            static_cast<int>(m_face->size->metrics.ascender >> 6),
            static_cast<int>(m_face->size->metrics.descender >> 6),
            static_cast<int>(m_face->size->metrics.height >> 6)
        };
        m_instances.emplace_back(target);
        m_currentInstanceIndex = m_instances.size() - 1;
    }

    const Glyph &Font::getGlyph(const char32_t codepoint) {
        const auto it = currentInstance().glyphs.find(codepoint);
        if (it != currentInstance().glyphs.end())
            return it->second;

        return pushGlyph(codepoint);
    }

    const Glyph &Font::pushGlyph(const char32_t codepoint) {
        const auto status = FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER);
        ASSERT(status == FT_Err_Ok);

        const FT_GlyphSlotRec_ slot = *m_face->glyph;
        std::vector<uint8_t> pixels;

        pixels.resize(slot.bitmap.width * slot.bitmap.rows * 4, 255);

        for (uint32_t y = 0; y < slot.bitmap.rows; y++) {
            for (uint32_t x = 0; x < slot.bitmap.width; x++) {
                pixels[(y * slot.bitmap.width + x) * 4 + 3] = slot.bitmap.buffer[y * slot.bitmap.width + x];
            }
        }

        FontInstance &instance = currentInstance();

        if (!instance.currentAtlas) {
            instance.atlasBuilder = core::AtlasBuilder({512, 512}, 4);
            instance.currentAtlas = resourceManager->textures.load({instance.atlasBuilder.atlas});
        }

        core::AtlasRegion region = instance.atlasBuilder.add({
                                                                 static_cast<int>(slot.bitmap.width),
                                                                 static_cast<int>(slot.bitmap.rows),
                                                                 4, pixels
                                                             }, 0);

        resourceManager->textures.get(instance.currentAtlas).setPixels(instance.atlasBuilder.atlas);

        return instance.glyphs.try_emplace(codepoint,
                                           instance.currentAtlas,
                                           region,
                                           math::uvec2{
                                               m_face->glyph->bitmap.width,
                                               m_face->glyph->bitmap.rows
                                           },
                                           math::ivec2{
                                               static_cast<int32_t>(
                                                   m_face->glyph->advance.x >> 6),
                                               static_cast<int32_t>(
                                                   m_face->glyph->advance.y >> 6)
                                           },
                                           math::ivec2{
                                               m_face->glyph->bitmap_left,
                                               m_face->glyph->bitmap_top
                                           }).first->second;
    }
}
