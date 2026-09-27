#pragma once

#include <vector>

#include <astra/core/assert.hpp>
#include <astra/core/types.hpp>
#include <astra/ecs/entity.hpp>
#include <astra/ecs/i_component_storage.hpp>

namespace astra::ecs {
    template<typename T>
    class ComponentStorage : public IComponentStorage {
        std::vector<T> components;
        std::vector<EntityId> entities;
        std::vector<size_t> entityToIndex;

    public:
        ComponentStorage() = default;

        [[nodiscard]] bool hasComponent(const EntityId &entityId) const {
            if (entityId >= entityToIndex.size() || entityToIndex[entityId] == core::INVALID_INDEX)
                return false;

            return true;
        }

        T &getComponent(const EntityId &entityId) {
            ASSERT(hasComponent(entityId));
            return components[entityToIndex[entityId]];
        }

        T &getComponentAt(const size_t index) {
            ASSERT(index < entities.size());
            return components[index];
        }

        [[nodiscard]] EntityId getEntityIdAt(const size_t index) const {
            ASSERT(index < entities.size());
            return entities[index];
        }

        [[nodiscard]] size_t size() const {
            return components.size();
        }

    private:
        friend class ComponentManager;

        void remove(const EntityId &entityId) override {
            removeComponent(entityId);
        }

        void addComponent(const EntityId &entityId, const T &component) {
            components.push_back(component);
            entities.push_back(entityId);

            if (entityId >= entityToIndex.size()) {
                entityToIndex.resize(entityId + 1, core::INVALID_INDEX);
            }

            entityToIndex[entityId] = entities.size() - 1;
        }

        void removeComponent(const EntityId &entityId) {
            if (!hasComponent(entityId))
                return;

            if (components.size() == 1) {
                components.pop_back();
                entities.pop_back();
                entityToIndex[entityId] = core::INVALID_INDEX;
                return;
            }

            const size_t indexToRemove = entityToIndex[entityId];

            std::swap(components[indexToRemove], components[components.size() - 1]);
            std::swap(entities[indexToRemove], entities[entities.size() - 1]);
            components.pop_back();
            entities.pop_back();

            const size_t entityIdSwaped = entities[indexToRemove];

            entityToIndex[entityId] = core::INVALID_INDEX;
            entityToIndex[entityIdSwaped] = indexToRemove;
        }
    };
}
