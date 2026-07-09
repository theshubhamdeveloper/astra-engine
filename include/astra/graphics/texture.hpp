#pragma once

#include <astra/assets/image.hpp>

namespace astra::graphics {
    struct Texture {
        struct Desc {
            assets::Image image;
        };

        Texture() = default;

        explicit Texture(const Desc &desc);

        void use(uint32_t unit) const;

    private :
        uint32_t id = 0;
    };
}
