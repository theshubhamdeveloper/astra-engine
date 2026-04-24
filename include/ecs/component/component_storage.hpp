#pragma once
#include "core/assert.hpp"
#include "core/type.hpp"
#include "ecs/entity/entity.hpp"
#include "i_component_storage.hpp"
#include <vector>

namespace astra::ecs::component {
template <typename T> class ComponentStorage : public IComponentStorage {
    std::vector<T> components;
    std::vector<entity::EntityId> entities;
    std::vector<size_t> entityToIndex;

  public:
    ComponentStorage() = default;

    [[nodiscard]] bool hasComponent(const entity::Entity &entity) const {
        if (entity.id >= entityToIndex.size() ||
            entityToIndex[entity.id] == core::INVALID_INDEX)
            return false;

        return true;
    }

    T &getComponent(const entity::Entity &entity) {
        ASSERT(hasComponent(entity));
        return components[entityToIndex[entity.id]];
    }

    T &getComponentAt(const size_t index) {
        ASSERT(index < entities.size());
        return components[index];
    }

    [[nodiscard]] entity::EntityId getEntityIdAt(const size_t index) const {
        ASSERT(index < entities.size());
        return entities[index];
    }

    [[nodiscard]] size_t size() const { return components.size(); }

  private:
    friend class ComponentManager;

    void remove(const entity::Entity &entity) override {
        removeComponent(entity);
    }

    void addComponent(const entity::Entity &entity, const T &component) {
        components.push_back(component);
        entities.push_back(entity.id);

        if (entity.id >= entityToIndex.size()) {
            entityToIndex.resize(entity.id + 1, core::INVALID_INDEX);
        }

        entityToIndex[entity.id] = entities.size() - 1;
    }

    void removeComponent(const entity::Entity &entity) {
        if (!hasComponent(entity))
            return;

        if (components.size() == 1) {
            components.pop_back();
            entities.pop_back();
            entityToIndex[entity.id] = core::INVALID_INDEX;
            return;
        }

        const size_t indexToRemove = entityToIndex[entity.id];

        std::swap(components[indexToRemove], components[components.size() - 1]);
        std::swap(entities[indexToRemove], entities[entities.size() - 1]);
        components.pop_back();
        entities.pop_back();

        const size_t entityIdSwaped = entities[indexToRemove];

        entityToIndex[entity.id] = core::INVALID_INDEX;
        entityToIndex[entityIdSwaped] = indexToRemove;
    }
};
}
