#pragma once

#include <glad/glad.h>
#include <astra/assets/image.hpp>
#include <astra/graphics/shader.hpp>

namespace astra::graphics {
    struct Texture {
    private:
        uint32_t id = 0;

    public:
        Texture() = default;

        explicit Texture(const assets::Image &image);

        void use(uint32_t unit) const;
    };
}
