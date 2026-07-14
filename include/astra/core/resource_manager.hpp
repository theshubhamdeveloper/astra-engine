#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/core/types.hpp>
#include <astra/graphics/font_family.hpp>
#include <astra/graphics/mesh.hpp>
#include <astra/graphics/shader.hpp>
#include <astra/graphics/texture.hpp>

namespace astra::core {
    template<typename T, typename Handle>
    struct AssetStorage {
        AssetStorage() {
            // Making Index 0 as Dummy and Invalid
            data.emplace_back(CreateScope<T>());
            generations.push_back(0);
        }

        Handle load(const T::Desc &desc) {
            uint32_t index;
            if (freeSlots.empty()) {
                data.emplace_back();
                generations.push_back(0);
                index = data.size() - 1;
            } else {
                index = freeSlots.front();
                freeSlots.pop();
            }

            data[index] = CreateScope<T>(desc);
            return {index, generations[index]};
        }

        T &get(const Handle &handle) {
            if (!exits(handle)) return *data[0];
            return *data[handle.id];
        }

        void destroy(const Handle &handle) {
            if (!exits(handle) || handle.id == 0) return;
            data[handle.id].reset();
            generations[handle.id] += 1;
        }

        [[nodiscard]] bool exits(const Handle &handle) const {
            return handle.id < generations.size() && generations[handle.id] == handle.generation;
        }

    private:
        std::vector<Scope<T> > data;
        std::vector<uint32_t> generations;
        std::queue<uint32_t> freeSlots;
    };

    class ResourceManager {
    public:
        AssetStorage<graphics::Texture, TextureHandle> textures;
        AssetStorage<graphics::Shader, ShaderHandle> shaders;
        AssetStorage<graphics::FontFamily, FontFamilyHandle> fontsFamilies;
        AssetStorage<graphics::Mesh, MeshHandle> meshes;
    };
}
