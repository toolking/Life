#pragma once

#include "math.hpp"

#include <centurion/common/math.hpp>
#include <centurion/video/color.hpp>

#include <string_view>

constexpr int MIN_FPS = 60;
constexpr int FRAME_DURATION_MS = 1000 / MIN_FPS;

constexpr int BOARD_WIDTH {32};
constexpr int BOARD_HEIGHT {24};
constexpr int TILE_SIZE {20};
constexpr int WINDOW_WIDTH {BOARD_WIDTH * TILE_SIZE};
constexpr int WINDOW_HEIGHT {BOARD_HEIGHT * TILE_SIZE};

constexpr cen::iarea TILE_AREA {TILE_SIZE, TILE_SIZE};
constexpr const char* const SMALL_FONT_PATH {"assets/VpPixel.ttf"};
constexpr unsigned int SMALL_FONT_SIZE {20U};
constexpr const char* const BIG_FONT_PATH {"assets/emulogic.ttf"};
constexpr unsigned int BIG_FONT_SIZE {TILE_SIZE};

[[nodiscard]] constexpr auto to_coord(int index) -> cen::ipoint
{
    auto const x = index % BOARD_WIDTH;
    auto const y = index / BOARD_WIDTH;
    return {x, y};
};

[[nodiscard]] constexpr auto to_index(cen::ipoint const& cell) -> std::size_t
{
    return std::size_t(cell.y() * BOARD_WIDTH + abs(cell.x() % BOARD_WIDTH));
}