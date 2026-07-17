#pragma once

#include <vector>

namespace astra::graphics {
    struct FontCustomAxis {
        uint32_t tag;
        int value;
    };

    struct FontStyle {
        uint32_t size;
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
}
