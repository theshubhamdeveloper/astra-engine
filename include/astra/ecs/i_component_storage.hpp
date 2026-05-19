#pragma once

#include <astra/ecs/entity.hpp>

namespace astra::ecs {
    class IComponentStorage {
    public:
        virtual ~IComponentStorage() = default;

    private:
        friend class ComponentManager;

        virtual void remove(const EntityId &entityId) = 0;
    };
}
