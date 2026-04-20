#pragma once

namespace astra::ecs::component {
    using ComponentTypeId = uint32_t;

    inline ComponentTypeId getNextComponentTypeId() {
        static ComponentTypeId nextId = 0;
        return nextId++;
    }

    template<typename T>
    ComponentTypeId getComponentTypeId() {
        static ComponentTypeId typeId = getNextComponentTypeId();
        return typeId;
    }
}
