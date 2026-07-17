#pragma once

#include <array>
#include <vector>

#include <freetype/freetype.h>

namespace astra::graphics {
    constexpr uint32_t MAX_AXES = 16;

    struct AxisCoordinates {
        uint8_t count;
        std::array<FT_Fixed, MAX_AXES> values;

        bool operator==(const AxisCoordinates &other) const {
            if (count != other.count)
                return false;

            return std::equal(values.begin(),
                              values.begin() + count,
                              other.values.begin());
        }
    };

    struct FontAxis {
        uint32_t coord;
        uint32_t tag;
        int minimum;
        int maximum;
        int def;
    };

    struct FontAxes {
        int weight = -1;
        int width = -1;
        int italic = -1;

        std::vector<FontAxis> axes;
        std::vector<uint32_t> customAxisIndices;
        AxisCoordinates defaultCoords;
    };
}
