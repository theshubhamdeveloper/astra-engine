#pragma once

#include <string>

#include <freetype/freetype.h>

#include <astra/core/assert.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/glyph.hpp>
#include <astra/core/atlas_builder.hpp>

namespace astra::core {
    class ResourceManager;
}

namespace astra::graphics {
    struct Font {
        struct Desc {
            core::ResourceManager *resourceManager;
            FT_Library library;
            std::string fontPath;
            uint32_t size;
        };

        explicit Font(const Desc &desc);

        void generateBasicGlyph();

        void setSize(uint32_t size);

        const Glyph &getGlyph(char codepoint);

        [[nodiscard]] int lineHeight() const;

        [[nodiscard]] const core::TextureHandle &getAtlas() const;

    private:
        core::ResourceManager *resourceManager;
        FT_Face face;
        core::TextureHandle atlas;
        core::AtlasBuilder atlasBuilder;
        std::unordered_map<char, Glyph> glyphs;

        core::AtlasRegion addCurrentGlyphInAtlas();

        const Glyph &loadGlyph(char codepoint);
    };
}
