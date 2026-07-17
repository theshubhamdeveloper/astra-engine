#pragma once

#include <freetype/freetype.h>

#include <astra/graphics/font/font_axis.hpp>
#include <astra/graphics/font/font_instance.hpp>
#include <astra/graphics/font/font_style.hpp>

namespace astra::graphics {
    constexpr uint32_t WEIGHT_PENALITY_MULTIPLIER = 1;
    constexpr uint32_t WIDTH_PENALITY_MULTIPLIER = 50;
    constexpr uint32_t CUSTOM_AXIS_PENALITY = 5000;
    constexpr uint32_t ITALIC_PENALTY = 100000;

    struct FontFace {
        FontFace(core::ResourceManager *resourceManager,
                 FT_Library library,
                 const std::string_view &fontPath);

        ~FontFace();

        [[nodiscard]] uint32_t score(const FontStyle &style) const;

        FontInstance &getInstance(const FontStyle &style);

    private:
        bool hasAxis(uint32_t tag) const;

        core::ResourceManager *resourceManager;

        FT_Face m_face;

        uint32_t m_weight;
        uint32_t m_width;
        bool m_italic;

        FontAxes m_axes;
        std::vector<FontInstance> m_instances;
    };

    namespace font_tags {
        constexpr uint32_t makeTag(const char a, const char b, const char c, const char d) {
            return (static_cast<uint32_t>(a) << 24) |
                   (static_cast<uint32_t>(b) << 16) |
                   (static_cast<uint32_t>(c) << 8) |
                   static_cast<uint32_t>(d);
        }

        constexpr uint32_t weight = makeTag('w', 'g', 'h', 't');
        constexpr uint32_t width = makeTag('w', 'd', 't', 'h');
        constexpr uint32_t slant = makeTag('s', 'l', 'n', 't');
        constexpr uint32_t italic = makeTag('i', 't', 'a', 'l');
        constexpr uint32_t opticalSize = makeTag('o', 'p', 's', 'z');
    }
}
