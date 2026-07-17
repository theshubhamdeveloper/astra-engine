#include <freetype/ftmm.h>

#include <astra/core/resource_manager.hpp>
#include <../../include/astra/graphics/font/font_instance.hpp>

namespace astra::graphics {
    FontInstance::FontInstance(core::ResourceManager *resourceManager, FT_Face face, const uint32_t size,
                               const AxisCoordinates &coordValues) : resourceManager(resourceManager), face(face),
                                                               m_size(size),
                                                               m_coordValues(coordValues),
                                                               m_matrics() {
        FT_Set_Pixel_Sizes(face, 0, size);

        FT_Set_Var_Design_Coordinates(
            face,
            m_coordValues.count,
            m_coordValues.values.data());


        m_matrics = {
            static_cast<int>(face->size->metrics.ascender >> 6),
            static_cast<int>(face->size->metrics.descender >> 6),
            static_cast<int>(face->size->metrics.height >> 6)
        };
    }

    const Glyph &FontInstance::getGlyph(const char32_t codepoint) {
        const auto it = m_glyphs.find(codepoint);
        if (it != m_glyphs.end())
            return it->second;

        return pushGlyph(codepoint);
    }

    const Glyph &FontInstance::pushGlyph(const char32_t codepoint) {
        auto status = FT_Set_Pixel_Sizes(face, 0, m_size);
        status = FT_Set_Var_Design_Coordinates(
            face,
            m_coordValues.count,
            m_coordValues.values.data());

        status = FT_Load_Char(face, codepoint, FT_LOAD_RENDER);
        ASSERT(status == FT_Err_Ok);

        const FT_GlyphSlotRec_ slot = *face->glyph;
        std::vector<uint8_t> pixels;

        pixels.resize(slot.bitmap.width * slot.bitmap.rows * 4, 255);

        for (uint32_t y = 0; y < slot.bitmap.rows; y++) {
            for (uint32_t x = 0; x < slot.bitmap.width; x++) {
                pixels[(y * slot.bitmap.width + x) * 4 + 3] = slot.bitmap.buffer[y * slot.bitmap.width + x];
            }
        }

        if (m_currentAtlas.id == 0) {
            m_atlasBuilder = core::AtlasBuilder({512, 512}, 4);
            m_currentAtlas = resourceManager->textures.load({m_atlasBuilder.atlas});
        }

        core::AtlasRegion region = m_atlasBuilder.add({
                                                          static_cast<int>(slot.bitmap.width),
                                                          static_cast<int>(slot.bitmap.rows),
                                                          4, pixels
                                                      }, 0);

        resourceManager->textures.get(m_currentAtlas).setPixels(m_atlasBuilder.atlas);

        return m_glyphs.try_emplace(codepoint,
                                    m_currentAtlas,
                                    region,
                                    math::uvec2{
                                        face->glyph->bitmap.width,
                                        face->glyph->bitmap.rows
                                    },
                                    math::ivec2{
                                        static_cast<int32_t>(
                                            face->glyph->advance.x >> 6),
                                        static_cast<int32_t>(
                                            face->glyph->advance.y >> 6)
                                    },
                                    math::ivec2{
                                        face->glyph->bitmap_left,
                                        face->glyph->bitmap_top
                                    }).first->second;
    }

    const AxisCoordinates &FontInstance::coordValues() const {
        return m_coordValues;
    }

    const FontMetrics &FontInstance::getMatrix() const {
        return m_matrics;
    }
}
