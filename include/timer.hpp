#pragma once

#include <centurion/system/timer.hpp>
#include <chrono>

class Timer
{
public:
    Timer() = default;

    void start();
    void stop();

    [[nodiscard]] auto is_started() const -> bool;
    [[nodiscard]] auto ticks() const -> std::chrono::milliseconds;

private:
    cen::u64ms startTicks_ {0};
};