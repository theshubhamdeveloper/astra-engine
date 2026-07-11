#include <astra/core/resource_manager.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/font.hpp>

namespace astra::graphics {
    core::AtlasRegion Font::addCurrentGlyphInAtlas() {
        const FT_GlyphSlotRec_ slot = *m_face->glyph;
        std::vector<uint8_t> pixels;

        pixels.resize(slot.bitmap.width * slot.bitmap.rows * 4, 255);

        for (uint32_t y = 0; y < slot.bitmap.rows; y++) {
            for (uint32_t x = 0; x < slot.bitmap.width; x++) {
                pixels[(y * slot.bitmap.width + x) * 4 + 3] = slot.bitmap.buffer[y * slot.bitmap.width + x];
            }
        }

        return atlasBuilder.add({
                                    static_cast<int>(slot.bitmap.width), static_cast<int>(slot.bitmap.rows),
                                    4, pixels
                                }, 0);
    }

    const Glyph &Font::loadGlyph(const char codepoint) {
        auto status = FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER);
        ASSERT(status == FT_Err_Ok);

        return glyphs.try_emplace(codepoint, addCurrentGlyphInAtlas(),
                                  math::uvec2{
                                      m_face->glyph->bitmap.width,
                                      m_face->glyph->bitmap.rows
                                  },
                                  math::uvec2{
                                      static_cast<uint32_t>(m_face->glyph->advance.x >> 6),
                                      static_cast<uint32_t>(m_face->glyph->advance.y >> 6)
                                  },
                                  math::uvec2{
                                      static_cast<uint32_t>(m_face->glyph->bitmap_left),
                                      static_cast<uint32_t>(m_face->glyph->bitmap_top)
                                  }).first->second;
    }

    Font::Font(const Desc &desc) : resourceManager(desc.resourceManager), m_face(),
                                   atlasBuilder(math::uvec2{512}, 4), size(desc.size) {
        auto status = FT_New_Face(desc.library, desc.fontPath.c_str(), 0, &m_face);
        ASSERT(status == FT_Err_Ok);

        FT_Set_Pixel_Sizes(m_face, 0, desc.size);
        generateBasicGlyph();

        m_atlas = resourceManager->textures.load({atlasBuilder.atlas});
    }

    void Font::generateBasicGlyph() {
        for (char c = 32; c < 127; c++)
            loadGlyph(c);
    }

    void Font::setSize(const uint32_t newSize) {
        if (size == newSize) return;
        glyphs.clear();

        FT_Set_Pixel_Sizes(m_face, 0, newSize);
        size = newSize;

        generateBasicGlyph();

        resourceManager->textures.get(m_atlas).setPixels(atlasBuilder.atlas);
    }

    const Glyph &Font::getGlyph(const char codepoint) {
        const auto it = glyphs.find(codepoint);
        if (it != glyphs.end())
            return it->second;

        const auto &glyph = loadGlyph(codepoint);
        resourceManager->textures.get(m_atlas).setPixels(atlasBuilder.atlas);
        return glyph;
    }

    math::vec2 Font::kerning(const char leftChar, const char rightChar) const {
        FT_Vector kerning;

        FT_Get_Kerning(
            m_face,
            FT_Get_Char_Index(m_face, leftChar),
            FT_Get_Char_Index(m_face, rightChar),
            FT_KERNING_DEFAULT,
            &kerning
        );

        return {static_cast<float>(kerning.x >> 6), static_cast<float>(kerning.y >> 6)};
    }

    int Font::ascender() const {
        return m_face->size->metrics.ascender >> 6;
    }

    int Font::height() const {
        return m_face->size->metrics.height >> 6;
    }

    const core::TextureHandle &Font::atlas() const {
        return m_atlas;
    }
}
