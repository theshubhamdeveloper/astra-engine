#pragma once
#include "component_storage.hpp"
#include "component_type.hpp"
#include "ecs/entity/entity.hpp"
#include <vector>

namespace astra::ecs::component {
class ComponentManager {
    std::vector<IComponentStorage*> storages;

    template <typename T> ComponentStorage<T>* createStorage() {
        const auto storage = new ComponentStorage<T>();
        storages.push_back(storage);
        return storage;
    }

    template <typename T> bool storageExits(size_t& idOut) const {
        idOut = getComponentTypeId<T>();
        if (idOut >= storages.size())
            return false;

        return storages[idOut] != nullptr;
    }

  public:
    ComponentManager() = default;

    template <typename T> ComponentStorage<T>* getStorage() {
        if (size_t id; storageExits<T>(id))
            return static_cast<ComponentStorage<T>*>(storages[id]);

        return nullptr;
    }

    template <typename T> T& getComponent(const entity::EntityId& entityId) {
        const auto storage = getStorage<T>();
        ASSERT(storage != nullptr);
        return storage->getComponent(entityId);
    }

    template <typename T> void addComponent(const entity::EntityId& entityId, const T& component) {
        auto storage = getStorage<T>();
        if (storage == nullptr)
            storage = createStorage<T>();

        storage->addComponent(entityId, component);
    }

    template <typename T> void removeComponent(const entity::EntityId& entityId) {
        const auto storage = getStorage<T>();
        ASSERT(storage != nullptr);
        storage->removeComponent(entityId);
    }

    template <typename T> [[nodiscard]] bool hasComponent(const entity::EntityId& entityId) const {
        const auto storage = getStorage<T>();
        if (storage == nullptr)
            return false;

        return storage->hasComponent(entityId);
    }

    void removeAll(const entity::EntityId& entityId) const;
};
}
