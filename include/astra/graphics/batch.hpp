#pragma once

#include <vector>

#include <astra/core/resource_manager.hpp>

namespace astra::graphics {
    struct Batch {
        core::ShaderHandle shader;
        core::MeshHandle mesh;

        template<typename TVertex>
        void pushVertex(const TVertex &vertex) {
            const size_t oldSize = vertices.size();
            vertices.resize(oldSize + sizeof(TVertex));
            std::memcpy(vertices.data() + oldSize, &vertex, sizeof(TVertex));
            vertexCount++;
        }

        uint32_t pushTexture(const core::TextureHandle &texture) {
            for (int i = 0; i < textures.size(); ++i)
                if (textures[i].id == texture.id)
                    return i;

            textures.push_back(texture);
            return textures.size() - 1;
        }

        void activateTextures(core::ResourceManager &rm, std::vector<int> &slots) const {
            slots.resize(textures.size());
            for (int i = 0; i < textures.size(); ++i) {
                slots.push_back(i);
                rm.textures.get(textures[i]).use(i);
            }
        };

        void clear() {
            shader = {};
            mesh = {};
            vertices.clear();
            textures.clear();
            vertexCount = 0;
        }

        [[nodiscard]] const std::vector<uint8_t> &getVertices() const {
            return vertices;
        }

        [[nodiscard]] uint32_t getVertexCount() const {
            return vertexCount;
        }

        [[nodiscard]] uint32_t textureCount() const {
            return textures.size();
        }

    private:
        std::vector<uint8_t> vertices;
        std::vector<core::TextureHandle> textures;
        uint32_t vertexCount = 0;
    };
}
