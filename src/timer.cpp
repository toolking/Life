#include "timer.hpp"

#include <centurion/system/timer.hpp>

void Timer::start()
{
    startTicks_ = cen::ticks64();
}

void Timer::stop()
{
    startTicks_ = cen::u64ms{0};
}

auto Timer::ticks() const -> std::chrono::milliseconds
{
    return cen::ticks64() - startTicks_;
}
