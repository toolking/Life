#pragma once

#include "math.hpp"

#include <centurion/common/math.hpp>
#include <centurion/video/color.hpp>

#include <string_view>

constexpr int MIN_FPS = 120;
constexpr int FRAME_DURATION_MS = 1000 / MIN_FPS;

constexpr int PACMAN_SPEED {2};
constexpr int BOARD_WIDTH {28};
constexpr int BOARD_HEIGHT {36};
constexpr int SPRITE_SIZE {32};
constexpr int TILE_SIZE {24};
constexpr int WINDOW_WIDTH {BOARD_WIDTH * TILE_SIZE};
constexpr int WINDOW_HEIGHT {BOARD_HEIGHT * TILE_SIZE};

// center of sprite is center of tile
constexpr int TILE_TO_SPRITE_OFFSET {(SPRITE_SIZE - TILE_SIZE) / 2};
constexpr cen::ipoint TILE_TO_SPRITE_OFFSET_2D {(TILE_SIZE - SPRITE_SIZE) / 2, (TILE_SIZE - SPRITE_SIZE) / 2};

constexpr cen::iarea SPRITE_AREA {SPRITE_SIZE, SPRITE_SIZE};
constexpr cen::iarea TILE_AREA {TILE_SIZE, TILE_SIZE};
constexpr cen::irect HOME_CELLS {cen::ipoint {11, 16}, cen::iarea {6, 3}};
constexpr cen::irect HOME_RECT {HOME_CELLS * TILE_SIZE};

constexpr std::string_view CHAR_BOARD {
    // 23456789012345678901234567
    "                            " //    0
    "                            " //    1
    "                            " //    2
    "############################" //    3
    "#............##............#" //    4
    "#.####.#####.##.#####.####.#" //    5
    "#o#  #.#   #.##.#   #.#  #o#" //    6
    "#.####.#####.##.#####.####.#" //    7
    "#..........................#" //    8
    "#.####.##.########.##.####.#" //    9
    "#.####.##.########.##.####.#" //   10
    "#......##....##....##......#" //   11
    "######.##### ## #####.######" //   12
    "     #.##### ## #####.#     " //   13
    "     #.##    1     ##.#     " //   14
    "     #.## ###==### ##.#     " //   15
    "######.## #      # ##.######" //   16
    "      .   #2 3 4 #   .      " //   17
    "######.## #      # ##.######" //   18
    "     #.## ######## ##.#     " //   19
    "     #.##    F     ##.#     " //   20
    "     #.## ######## ##.#     " //   21
    "######.## ######## ##.######" //   22
    "#............##............#" //   23
    "#.####.#####.##.#####.####.#" //   24
    "#.####.#####.##.#####.####.#" //   25
    "#o..##.......C .......##..o#" //   26
    "###.##.##.########.##.##.###" //   27
    "###.##.##.########.##.##.###" //   28
    "#......##....##....##......#" //   29
    "#.##########.##.##########.#" //   30
    "#.##########.##.##########.#" //   31
    "#..........................#" //   32
    "############################" //   33
    "                            " //   34
    "                            "}; // 35

static_assert(CHAR_BOARD.size() == BOARD_WIDTH * BOARD_HEIGHT);

[[nodiscard]] constexpr auto count_char(char c) -> int
{
    return static_cast<int>(std::count(CHAR_BOARD.begin(), CHAR_BOARD.end(), c));
};

constexpr int PILLS_COUNT {count_char('.')};
constexpr int POWER_PILLS_COUNT {count_char('o')};
constexpr int TOTAL_PILLS_COUNT {PILLS_COUNT + POWER_PILLS_COUNT};

static_assert(count_char('C') == 1, "only one Pacman allowed");
static_assert(count_char('1') == 1, "only one Blinky allowed");
static_assert(count_char('2') == 1, "only one Inky allowed");
static_assert(count_char('3') == 1, "only one Pinky allowed");
static_assert(count_char('4') == 1, "only one Clyde allowed");
static_assert(count_char('F') == 1, "only one fruit allowed");

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

[[nodiscard]] constexpr auto cell_of(char c) -> cen::ipoint
{
    return to_coord(static_cast<int>(CHAR_BOARD.find(c)));
};

[[nodiscard]] constexpr auto startpos_of(char c) -> cen::ipoint // shift half a tile to right
{
    return {cell_of(c) * TILE_SIZE + cen::ipoint {TILE_SIZE / 2, 0}};
}

[[nodiscard]] constexpr inline auto wrapped_distance(cen::ipoint const& pos1, cen::ipoint const& pos2) -> double
{
    auto dist_x = abs(pos1.x() - pos2.x());
    auto const dist_y = pos1.y() - pos2.y();
    dist_x = (dist_x > WINDOW_WIDTH / 2) ? WINDOW_WIDTH - dist_x : dist_x;
    return sqrt(dist_x * dist_x + dist_y * dist_y);
}

[[nodiscard]] constexpr inline auto is_home(cen::ipoint const& pos) -> bool
{
    return HOME_RECT.contains(pos);
}