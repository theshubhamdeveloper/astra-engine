#pragma once

#include <string>
#include <freetype/freetype.h>

#include <astra/core/assert.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/glyph.hpp>

namespace astra::core {
    class ResourceManager;
}

namespace astra::graphics {
    class Font {
        core::ResourceManager &resourceManager;
        FT_Face face;
        std::unordered_map<char, Glyph> glyphs;

        [[nodiscard]] core::TextureHandle generateCurrentGlyphTexture() const;

        const Glyph &loadGlyph(char codepoint);

    public:
        Font(core::ResourceManager &resourceManager, FT_Library library,
             const std::string &fontPath);

        void setFontSize(uint32_t size);

        const Glyph &getGlyph(char codepoint);
    };
}
