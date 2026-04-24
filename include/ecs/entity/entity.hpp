#pragma once
#include <cstdint>

namespace astra::ecs::entity {
using EntityId = uint32_t;
using EntityGeneration = uint32_t;

struct Entity {
    EntityId id;
    EntityGeneration generation;
};
}
