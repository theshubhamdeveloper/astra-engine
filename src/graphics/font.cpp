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
                                  math::Vec2{
                                      static_cast<float>(face->glyph->bitmap.width),
                                      static_cast<float>(face->glyph->bitmap.rows)
                                  },
                                  math::Vec2{
                                      static_cast<float>(face->glyph->advance.x >> 6),
                                      static_cast<float>(face->glyph->advance.y >> 6)
                                  },
                                  math::Vec2{
                                      static_cast<float>(face->glyph->bitmap_left),
                                      static_cast<float>(face->glyph->bitmap_top)
                                  }).first->second;
    }

    Font::Font(const Desc &desc) : resourceManager(desc.resourceManager), face(),
                                   atlasBuilder(math::Vec2{512}, 4) {
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

    void Font::setSize(const uint32_t size) {
        glyphs.clear();
        FT_Set_Pixel_Sizes(face, 0, size);
        generateBasicGlyph();
        resourceManager->textures.get(atlas).setPixels(atlasBuilder.atlas);
    }

    const Glyph &Font::getGlyph(const char codepoint) {
        const auto it = glyphs.find(codepoint);
        if (it != glyphs.end())
            return it->second;

        return loadGlyph(codepoint);
    }

    int Font::lineHeight() const {
        return face->size->metrics.height >> 6;
    }

    const core::TextureHandle &Font::getAtlas() const {
        return atlas;
    }
}
