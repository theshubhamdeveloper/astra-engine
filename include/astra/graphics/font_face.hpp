#pragma once

#include <string>

#include <freetype/tttables.h>
#include <freetype/freetype.h>

#include <astra/core/assert.hpp>
#include <astra/core/resource_handles.hpp>
#include <astra/graphics/glyph.hpp>
#include <astra/core/atlas_builder.hpp>

namespace astra::core {
    class ResourceManager;
}

namespace astra::graphics {
    struct FontMetrics {
        int ascender;
        int descender;
        int height;
    };

    struct FontAxis {
        int cord;
        int minimum;
        int maximum;
        int def;
    };

    struct FontStyle {
        uint32_t size;
        uint32_t width;
        uint32_t weight;
        bool italic;

        void setAxis(const uint32_t tag, const int value) {
            m_axes.try_emplace(tag, value);
        }

        [[nodiscard]] const std::unordered_map<uint32_t, int> &axes() const {
            return m_axes;
        }

    private:
        std::unordered_map<uint32_t, int> m_axes;
    };

    struct FontCache {
        uint32_t size = 16;
        std::vector<FT_Fixed> coordsValues;

        core::AtlasBuilder atlasBuilder = core::AtlasBuilder{{0, 0}, 4};
        core::TextureHandle currentAtlas{};
        std::unordered_map<char32_t, Glyph> glyphs;
        FontMetrics matrix;
    };

    struct FontFace {
        FontFace(core::ResourceManager *resourceManager,
                 FT_Library library,
                 const std::string_view &fontPath);

        // ~FontFace();

        void setStyle(FontStyle style);

        uint32_t score(const FontStyle &style) const;

        const Glyph &getGlyph(char32_t codepoint);

        [[nodiscard]] const FontMetrics &getMatrix() const {
            return m_caches[m_currentCacheIndex].matrix;
        }

        static constexpr uint32_t makeTag(const char a, const char b, const char c, const char d) {
            return (static_cast<uint32_t>(a) << 24) |
                   (static_cast<uint32_t>(b) << 16) |
                   (static_cast<uint32_t>(c) << 8) |
                   static_cast<uint32_t>(d);
        }

    private:
        core::ResourceManager *resourceManager;

        FT_Face m_face;

        uint32_t m_weight;
        uint32_t m_width;
        bool m_italic;

        std::unordered_map<uint32_t, FontAxis> m_axes;

        std::vector<FontCache> m_caches;
        int m_currentCacheIndex = -1;

        FontCache &currentInstance() {
            ASSERT(m_currentCacheIndex >= 0)
            return m_caches[m_currentCacheIndex];
        }

        const Glyph &pushGlyph(char32_t codepoint);
    };

    namespace font_tags {
        constexpr uint32_t weight = FontFace::makeTag('w', 'g', 'h', 't');
        constexpr uint32_t width = FontFace::makeTag('w', 'd', 't', 'h');
        constexpr uint32_t slant = FontFace::makeTag('s', 'l', 'n', 't');
        constexpr uint32_t italic = FontFace::makeTag('i', 't', 'a', 'l');
        constexpr uint32_t opticalSize = FontFace::makeTag('o', 'p', 's', 'z');
    }
}
