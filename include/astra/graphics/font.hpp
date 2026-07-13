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
    struct FontMetrics {
        int ascender;
        int descender;
        int height;
    };

    struct FontAxis {
        int minimum;
        int maximum;
        int def;
    };

    struct FontStyle {
        uint32_t size;

        explicit FontStyle(const uint32_t size) : size(size) {
        }

        FontStyle(const uint32_t size, const std::vector<uint32_t> &tags,
                  const std::vector<int> &values) : size(size), m_tags(tags), m_values(values) {
        }

        void setVariation(const uint32_t tag, const int value) {
            m_tags.push_back(tag);
            m_values.push_back(value);
        }

        [[nodiscard]] const std::vector<uint32_t> &getTags() const {
            return m_tags;
        }

        [[nodiscard]] const std::vector<int> &getValues() const {
            return m_values;
        }

    private:
        std::vector<uint32_t> m_tags;
        std::vector<int> m_values;
    };

    struct FontInstance {
        uint32_t size = 16;
        std::vector<FT_Fixed> coordsValues;

        core::AtlasBuilder atlasBuilder = core::AtlasBuilder{{0, 0}, 4};
        core::TextureHandle currentAtlas{};
        std::unordered_map<char32_t, Glyph> glyphs;
        FontMetrics matrix;
    };

    struct Font {
        struct Desc {
            core::ResourceManager *resourceManager;
            FT_Library library;
            std::string fontPath;
        };

        Font() : resourceManager(nullptr), m_face(nullptr) {
        }

        explicit Font(const Desc &desc);
        ~Font();

        void setStyle(const FontStyle &style);

        const Glyph &getGlyph(char32_t codepoint);

        [[nodiscard]] const FontMetrics &getMatrix() const {
            return m_instances[m_currentInstanceIndex].matrix;
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

        std::vector<uint32_t> m_coordsToTags;
        std::vector<FontAxis> m_coordsToAxes;

        std::vector<FontInstance> m_instances;
        size_t m_currentInstanceIndex = -1;

        FontInstance &currentInstance() {
            return m_instances[m_currentInstanceIndex];
        }

        const Glyph &pushGlyph(char32_t codepoint);
    };

    namespace font_tags {
        constexpr uint32_t weight = Font::makeTag('w', 'g', 'h', 't');
        constexpr uint32_t width = Font::makeTag('w', 'd', 't', 'h');
        constexpr uint32_t slant = Font::makeTag('s', 'l', 'n', 't');
        constexpr uint32_t italic = Font::makeTag('i', 't', 'a', 'l');
        constexpr uint32_t opticalSize = Font::makeTag('o', 'p', 's', 'z');
    }
}
