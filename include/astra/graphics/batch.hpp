#pragma once

#include <vector>

#include <astra/graphics/material.hpp>
#include <astra/math/vertex.hpp>

namespace astra::graphics {
    struct Batch {
        std::vector<math::Vertex> vertices;
        Material material;


        uint32_t addTextureInSlot(const TextureHandel &texture) {
            for (int i = 0; i < material.textures.size(); ++i) {
                if (material.textures[i].id == texture.id) {
                    return i;
                }
            }

            material.textures.push_back(texture);
            return material.textures.size() - 1;
        }
    };
}
