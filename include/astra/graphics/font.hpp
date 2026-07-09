#pragma once

#include <string>

#include <astra/core/assert.hpp>
#include <astra/graphics/glyph.hpp>
#include <freetype/freetype.h>

namespace astra::core {
    class ResourceManager;
}

namespace astra::graphics {
    struct Font {
        struct Desc {
            core::ResourceManager *resourceManager;
            FT_Library library;
            const std::string &fontPath;
        };

        explicit Font(const Desc &desc);

        void setFontSize(uint32_t size);

        const Glyph &getGlyph(char codepoint);

        [[nodiscard]] int lineHeight() const;

    private:
        core::ResourceManager *resourceManager = nullptr;
        FT_Face face = nullptr;
        std::unordered_map<char, Glyph> glyphs;

        [[nodiscard]] core::TextureHandle generateCurrentGlyphTexture() const;

        const Glyph &loadGlyph(char codepoint);
    };
}
