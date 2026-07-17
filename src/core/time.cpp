#include <SDL3/SDL_timer.h>

#include <astra/core/time.hpp>

namespace astra::core {
    Time::Time()
        : m_deltaTime(0), m_previousCounter(SDL_GetPerformanceCounter()),
          m_performanceFrequency(SDL_GetPerformanceFrequency()),
          m_frameCount(0), m_elapsedTime(0), m_fps(0) {
    }

    void Time::update() {
        evaluateDeltaTime();
        updateFrameCount();
    }

    void Time::evaluateDeltaTime() {
        const uint64_t currentCounter = SDL_GetPerformanceCounter();

        m_deltaTime = static_cast<double>(currentCounter - m_previousCounter) / static_cast<double>(m_performanceFrequency);

        m_previousCounter = currentCounter;
    }

    void Time::updateFrameCount() {
        m_elapsedTime += m_deltaTime;
        m_frameCount++;

        if (m_elapsedTime >= FPS_INTERVAL) {
            evaluateFPS();
        }
    }

    void Time::evaluateFPS() {
        m_fps = m_frameCount;

        m_frameCount = 0;
        m_elapsedTime -= FPS_INTERVAL;
    }

    double Time::deltaTime() const {
        return m_deltaTime;
    }

    uint16_t Time::fps() const {
        return m_fps;
    }
}
