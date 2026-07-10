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

        Font() : resourceManager(nullptr), face(nullptr), atlasBuilder(math::Vec2{0}, 0), size(0) {
        }

        explicit Font(const Desc &desc);

        void generateBasicGlyph();

        void setSize(uint32_t newSize);

        const Glyph &getGlyph(char codepoint);

        [[nodiscard]] math::Vec2 getKerning(char leftChar, char rightChar) const;

        [[nodiscard]] int lineHeight() const;

        [[nodiscard]] const core::TextureHandle &getAtlas() const;

    private:
        core::ResourceManager *resourceManager;
        FT_Face face;
        core::TextureHandle atlas;
        core::AtlasBuilder atlasBuilder;
        std::unordered_map<char, Glyph> glyphs;
        uint32_t size;

        core::AtlasRegion addCurrentGlyphInAtlas();

        const Glyph &loadGlyph(char codepoint);
    };
}
