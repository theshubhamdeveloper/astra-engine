#pragma once
#include <cstdint>

namespace astra::core {
    constexpr double FPS_INTERVAL = 1.0;

    class Time {
        double deltaTime_;
        uint64_t previousCounter;
        const uint64_t performanceFrequency;
        uint32_t frameCount;
        double elapsedTime;
        uint32_t fps_;

        void evaluateFPS();

        void evaluateDeltaTime();

        void updateFrameCount();

    public:
        Time();

        void update();

        double deltaTime() const;

        uint16_t fps() const;
    };
}
