#pragma once

#include <centurion/system/timer.hpp>
#include <chrono>

class Timer
{
public:
    Timer() = default;

    void start()
    {
        startTicks_ = cen::ticks64();
    }

    void stop()
    {
        startTicks_ = cen::u64ms {0};
    }

    [[nodiscard]] auto ticks() const -> std::chrono::milliseconds
    {
        return cen::ticks64() - startTicks_;
    }

private:
    cen::u64ms startTicks_ {0};
};