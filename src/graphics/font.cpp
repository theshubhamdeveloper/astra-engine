#include <astra/core/resource_manager.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/font.hpp>

namespace astra::graphics {
    core::AtlasRegion Font::addCurrentGlyphInAtlas() {
        const FT_GlyphSlotRec_ slot = *face->glyph;
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
        auto status = FT_Load_Char(face, codepoint, FT_LOAD_RENDER);
        ASSERT(status == FT_Err_Ok);

        return glyphs.try_emplace(codepoint, addCurrentGlyphInAtlas(),
                                  math::uvec2{
                                      face->glyph->bitmap.width,
                                      face->glyph->bitmap.rows
                                  },
                                  math::uvec2{
                                      static_cast<uint32_t>(face->glyph->advance.x >> 6),
                                      static_cast<uint32_t>(face->glyph->advance.y >> 6)
                                  },
                                  math::uvec2{
                                      static_cast<uint32_t>(face->glyph->bitmap_left),
                                      static_cast<uint32_t>(face->glyph->bitmap_top)
                                  }).first->second;
    }

    Font::Font(const Desc &desc) : resourceManager(desc.resourceManager), face(),
                                   atlasBuilder(math::uvec2{512}, 4), size(desc.size) {
        auto status = FT_New_Face(desc.library, desc.fontPath.c_str(), 0, &face);
        ASSERT(status == FT_Err_Ok);

        FT_Set_Pixel_Sizes(face, 0, desc.size);
        generateBasicGlyph();

        atlas = resourceManager->textures.load({atlasBuilder.atlas});
    }

    void Font::generateBasicGlyph() {
        for (char c = 32; c < 127; c++)
            loadGlyph(c);
    }

    void Font::setSize(const uint32_t newSize) {
        if (size == newSize) return;
        glyphs.clear();

        FT_Set_Pixel_Sizes(face, 0, newSize);
        size = newSize;

        generateBasicGlyph();

        resourceManager->textures.get(atlas).setPixels(atlasBuilder.atlas);
    }

    const Glyph &Font::getGlyph(const char codepoint) {
        const auto it = glyphs.find(codepoint);
        if (it != glyphs.end())
            return it->second;

        return loadGlyph(codepoint);
    }

    math::vec2 Font::getKerning(const char leftChar, const char rightChar) const {
        FT_Vector kerning;

        FT_Get_Kerning(
            face,
            FT_Get_Char_Index(face, leftChar),
            FT_Get_Char_Index(face, rightChar),
            FT_KERNING_DEFAULT,
            &kerning
        );

        return {static_cast<float>(kerning.x >> 6), static_cast<float>(kerning.y >> 6)};
    }

    int Font::lineHeight() const {
        return face->size->metrics.height >> 6;
    }

    const core::TextureHandle &Font::getAtlas() const {
        return atlas;
    }
}
