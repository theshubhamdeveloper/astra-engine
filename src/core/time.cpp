#include "core/time.hpp"
#include "SDL3/SDL_timer.h"

namespace astra::core {
    Time::Time() : deltaTime_(0), previousCounter(SDL_GetPerformanceCounter()),
                   performanceFrequency(SDL_GetPerformanceFrequency()),
                   frameCount(0), elapsedTime(0), fps_(0) {
    }

    void Time::update() {
        evaluateDeltaTime();
        updateFrameCount();
    }

    void Time::evaluateDeltaTime() {
        const uint64_t currentCounter = SDL_GetPerformanceCounter();

        deltaTime_ = static_cast<double>(currentCounter - previousCounter) / static_cast<double>(performanceFrequency);

        previousCounter = currentCounter;
    }

    void Time::updateFrameCount() {
        elapsedTime += deltaTime_;
        frameCount++;

        if (elapsedTime >= FPS_INTERVAL) {
            evaluateFPS();
        }
    }

    void Time::evaluateFPS() {
        fps_ = frameCount;

        frameCount = 0;
        elapsedTime -= FPS_INTERVAL;
    }

    double Time::deltaTime() const {
        return deltaTime_;
    }

    uint16_t Time::fps() const {
        return fps_;
    }
}
