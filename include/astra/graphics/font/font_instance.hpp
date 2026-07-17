#pragma once

#include <freetype/freetype.h>

#include <astra/core/assert.hpp>
#include <astra/core/atlas_builder.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/font/font_axis.hpp>
#include <astra/graphics/glyph.hpp>

namespace astra::core {
    class ResourceManager;
}

namespace astra::graphics {
    struct FontMetrics {
        int ascender;
        int descender;
        int height;
    };

    struct FontInstance {
        FontInstance(core::ResourceManager *resourceManager, FT_Face face, uint32_t size,
                     const AxisCoordinates &coordValues);

        [[nodiscard]] uint32_t size() const {
            return m_size;
        }

        [[nodiscard]] const AxisCoordinates &coordValues() const;

        [[nodiscard]] const FontMetrics &getMatrix() const;

        const Glyph &getGlyph(char32_t codepoint);

    private:
        const Glyph &pushGlyph(char32_t codepoint);


        core::ResourceManager *resourceManager;
        FT_Face face;

        uint32_t m_size;
        AxisCoordinates m_coordValues;

        core::AtlasBuilder m_atlasBuilder = core::AtlasBuilder{{0, 0}, 4};
        core::TextureHandle m_currentAtlas;

        std::unordered_map<char32_t, Glyph> m_glyphs;
        FontMetrics m_matrics;
    };
}
