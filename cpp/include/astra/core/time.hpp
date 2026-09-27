#pragma once

#include <cstdint>

namespace astra::core {
    constexpr double FPS_INTERVAL = 1.0;

    class Time {
        double m_deltaTime;

        uint64_t m_previousCounter;
        const uint64_t m_performanceFrequency;

        double m_elapsedTime;

        uint32_t m_frameCount;
        uint32_t m_fps;

        void evaluateFPS();

        void evaluateDeltaTime();

        void updateFrameCount();

    public:
        Time();

        void update();

        [[nodiscard]] double deltaTime() const;

        [[nodiscard]] uint16_t fps() const;
    };
}
