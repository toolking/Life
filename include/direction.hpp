#pragma once

#include "math.hpp"

#include <centurion/common/math.hpp>

enum class Direction
{
    none,
    up,
    down,
    left,
    right
};

[[nodiscard]] inline constexpr auto operator-(Direction const dir) noexcept -> Direction
{
    switch (dir) {
        using enum Direction;
    case up: return down;
    case down: return up;
    case left: return right;
    case right: return left;
    default: return none;
    }
}

template <typename T>
[[nodiscard]] inline constexpr auto to_point(Direction const dir) noexcept -> cen::basic_point<T>
{
    constexpr static typename cen::basic_point<T>::value_type one {1};
    constexpr static typename cen::basic_point<T>::value_type zero {0};
    switch (dir) {
        using enum Direction;
    case right: return {one, zero};
    case up: return {zero, -one};
    case left: return {-one, zero};
    case down: return {zero, one};
    default: return {zero, zero};
    }
}

[[nodiscard]] inline constexpr auto to_angle(Direction const dir) noexcept -> double
{
    switch (dir) {
        using enum Direction;
    case right: return 0.0;
    case down: return 90.0;
    case left: return 180.0;
    case up: return 270.0;
    default: return 0.0;
    }
}

[[nodiscard]] inline constexpr auto eye_frame(Direction dir) noexcept -> int
{
    using enum Direction;
    switch (dir) {
    case right: return 0;
    case up: return 1;
    case left: return 2;
    case down: return 3;
    default: return 4;
    }
}

template <typename T>
[[nodiscard]] inline constexpr auto operator+(typename cen::basic_point<T> const& pos, Direction d) -> cen::ipoint
{
    return pos + to_point<T>(d);
}

template <typename T>
inline auto operator+=(typename cen::basic_point<T>& pos, Direction d) -> cen::ipoint&
{
    return pos += to_point<T>(d);
}
