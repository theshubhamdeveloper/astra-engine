#include <freetype/ftmm.h>
#include <freetype/tttables.h>
#include <astra/core/resource_manager.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/font_face.hpp>

namespace astra::graphics {
    FontFace::FontFace(core::ResourceManager *resourceManager,
                       const FT_Library library,
                       const std::string_view &fontPath) : resourceManager(resourceManager), m_face(), m_italic(false) {
        const auto status = FT_New_Face(library, fontPath.data(), 0, &m_face);
        ASSERT(status == FT_Err_Ok);

        const auto os2 =
                static_cast<TT_OS2 *>(FT_Get_Sfnt_Table(m_face, FT_SFNT_OS2));

        m_weight = os2->usWeightClass;
        m_width = os2->usWidthClass;
        if (m_face->style_flags & FT_STYLE_FLAG_ITALIC)
            m_italic = true;

        FT_MM_Var *mmVar = nullptr;
        FT_Get_MM_Var(m_face, &mmVar);
        if (mmVar == nullptr) return;
        for (uint32_t i = 0; i < mmVar->num_axis; ++i) {
            const FT_Var_Axis &axis = mmVar->axis[i];
            m_axes.try_emplace(static_cast<uint32_t>(axis.tag),
                               i,
                               axis.minimum >> 16,
                               axis.maximum >> 16,
                               axis.def >> 16);
        }
    }

    // FontFace::~FontFace() {
    //     FT_Done_Face(m_face);
    // }


    uint32_t FontFace::score(const FontStyle &style) const {
        uint32_t score = 0;

        int weight = m_weight;
        int width = m_width;
        bool italic = m_italic;

        if (!m_axes.empty()) {
            const auto weightIt = m_axes.find(font_tags::weight);
            if (weightIt != m_axes.end()) {
                weight = std::clamp(static_cast<int>(style.weight),
                                    weightIt->second.minimum,
                                    weightIt->second.maximum);
            }

            const auto widthIt = m_axes.find(font_tags::width);
            if (widthIt != m_axes.end()) {
                width = std::clamp(static_cast<int>(style.width),
                                   widthIt->second.minimum,
                                   widthIt->second.maximum);
            }

            const auto italicIt = m_axes.find(font_tags::italic);
            if (italicIt != m_axes.end()) {
                italic = std::clamp(style.italic,
                                    static_cast<bool>(italicIt->second.minimum),
                                    static_cast<bool>(italicIt->second.maximum));
            }
        };

        score += std::abs(weight - static_cast<int>(style.weight));
        score += std::abs(width - static_cast<int>(style.width)) * 50;
        if (italic != style.italic) {
            score += 10000;
        }

        return score;
    }

    void FontFace::setStyle(FontStyle style) {
        style.setAxis(font_tags::weight, style.weight);
        style.setAxis(font_tags::width, style.width);
        style.setAxis(font_tags::italic, style.italic);

        FontCache target = {.size = style.size};
        target.coordsValues.resize(m_axes.size());

        for (const auto &[tag, axis]: m_axes) {
            int value = axis.def;

            const auto styleAxisIt = style.axes().find(tag);
            if (styleAxisIt != style.axes().end()) {
                value = std::clamp(
                    styleAxisIt->second,
                    axis.minimum,
                    axis.maximum
                );
            }
            target.coordsValues[axis.cord] = value << 16;
        }

        FT_Set_Pixel_Sizes(m_face, 0, target.size);

        FT_Set_Var_Design_Coordinates(
            m_face,
            target.coordsValues.size(),
            target.coordsValues.data());

        m_currentCacheIndex = -1;
        for (int i = 0; i < m_caches.size(); i++) {
            if (m_caches[i].size == target.size && m_caches[i].coordsValues == target.coordsValues &&
                m_currentCacheIndex != i) {
                m_currentCacheIndex = i;
                break;
            }
        }

        if (m_currentCacheIndex != -1) return;
        target.matrix = {
            static_cast<int>(m_face->size->metrics.ascender >> 6),
            static_cast<int>(m_face->size->metrics.descender >> 6),
            static_cast<int>(m_face->size->metrics.height >> 6)
        };

        m_caches.emplace_back(target);
        m_currentCacheIndex = m_caches.size() - 1;
    }

    const Glyph &FontFace::getGlyph(const char32_t codepoint) {
        const auto it = currentInstance().glyphs.find(codepoint);
        if (it != currentInstance().glyphs.end())
            return it->second;

        return pushGlyph(codepoint);
    }

    const Glyph &FontFace::pushGlyph(const char32_t codepoint) {
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

        FontCache &instance = currentInstance();

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
