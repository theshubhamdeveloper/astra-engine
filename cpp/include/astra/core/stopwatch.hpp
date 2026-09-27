#pragma once

#include <SDL3/SDL_timer.h>

namespace astra::core {
    class Stopwatch {
        uint64_t m_performanceFrequency;
        uint64_t m_startCounter;
        uint64_t m_stopCounter;
        double m_elapsed;

    public:
        Stopwatch() : m_performanceFrequency(SDL_GetPerformanceFrequency()), m_startCounter(0), m_stopCounter(0),
                      m_elapsed(0) {
        };

        void start() {
            m_startCounter = SDL_GetPerformanceCounter();
        }

        void stop() {
            m_stopCounter = SDL_GetPerformanceCounter();
            m_elapsed = static_cast<double>(m_stopCounter - m_startCounter) / static_cast<double>(
                            m_performanceFrequency);
        }

        [[nodiscard]] double elapsed() const {
            return m_elapsed;
        }

        void reset() {
            m_startCounter = 0;
            m_stopCounter = 0;
            m_elapsed = 0;
        }
    };
}
