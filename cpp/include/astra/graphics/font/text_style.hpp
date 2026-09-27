#pragma once

#include <vector>

#include <astra/core/resource_handles.hpp>
#include <astra/math/color.hpp>

namespace astra::graphics {
    struct FontCustomAxis {
        uint32_t tag;
        int value;
    };

    struct FontStyle {
        core::FontFamilyHandle fontFamily;

        uint32_t width;
        uint32_t weight;
        bool italic;

        void setAxis(const uint32_t tag, const int value) {
            m_axes.emplace_back(tag, value);
        }

        [[nodiscard]] const auto &axes() const {
            return m_axes;
        }

    private:
        std::vector<FontCustomAxis> m_axes;
    };

    struct FontFeatures {
        bool liga = true;
        bool kern = true;
        bool smcp = true;
    };

    struct TextStyle {
        FontStyle fontStyle;

        FontFeatures features;

        math::Color color;

        float size;
        float letterSpacing;
        float wordSpacing;
    };
}
