#include "board.hpp"

#include "constants.hpp"
#include "direction.hpp"
#include "math.hpp"

#include <centurion/common/logging.hpp>
#include <centurion/common/math.hpp>
#include <centurion/common/utils.hpp>
#include <centurion/video/color.hpp>
#include <centurion/video/renderer.hpp>

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <utility>

namespace {

auto count_alive_neighbours(cen::ipoint const& p, Board::tiles_type const& board) -> int
{
    auto const is_alive = [&board](std::size_t i) -> bool {
        return board[i] == Board::Tile::alive;
    };
    auto const neighbours = std::array<std::size_t, 8> {
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH - 1)) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT - 1)) % BOARD_HEIGHT}),
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH    )) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT - 1)) % BOARD_HEIGHT}),
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH + 1)) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT - 1)) % BOARD_HEIGHT}),
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH - 1)) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT    )) % BOARD_HEIGHT}),
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH + 1)) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT    )) % BOARD_HEIGHT}),
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH - 1)) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT + 1)) % BOARD_HEIGHT}),
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH    )) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT + 1)) % BOARD_HEIGHT}),
        to_index(cen::ipoint {(p.x() + (BOARD_WIDTH + 1)) % BOARD_WIDTH, (p.y() + (BOARD_HEIGHT + 1)) % BOARD_HEIGHT}),
    };
    return static_cast<int>(std::ranges::count_if(neighbours, is_alive));
}

auto dice() -> int
{
    static std::discrete_distribution<> distr({70, 30});
    static std::random_device device;
    static std::mt19937 engine {device()};
    return distr(engine);
};

} // namespace

Board::Board(cen::renderer_handle const& renderer)
  : renderer_ {renderer}
{
    randomize();
}

auto Board::operator[](cen::ipoint const& cell) const -> Tile const&
{
    return (*current_board_)[to_index(cell)];
}

auto Board::operator[](cen::ipoint const& cell) -> Tile&
{
    return (*current_board_)[to_index(cell)];
}

void Board::randomize()
{
    std::generate(current_board_->begin(), current_board_->end(), []() { return dice() == 0 ? Tile::dead : Tile::alive; });
}

void Board::clear()
{
    std::fill(current_board_->begin(), current_board_->end(), Tile::dead);
}

void Board::update()
{
    std::swap(current_board_, prev_board_);
    auto const& base {*prev_board_};
    auto& board {*current_board_};
    for (auto const [row, col] : std::ranges::cartesian_product_view {std::views::iota(0, BOARD_HEIGHT), std::views::iota(0, BOARD_WIDTH)}) {
        auto const current_coords {cen::ipoint {col, row}};
        auto const idx {to_index(current_coords)};
        auto const prev_tile {base[idx]};
        auto& current_tile {board[idx]};
        auto const alive_neighbours {count_alive_neighbours(current_coords, base)};
        using enum Tile;
        if (prev_tile == dead && alive_neighbours == 3) {
            current_tile = alive;
        } else if (prev_tile == alive && (alive_neighbours < 2 || alive_neighbours > 3)) {
            current_tile = dead;
        } else {
            current_tile = prev_tile;
        }
    }
}

void Board::render()
{
    for (auto const& [idx, tile] : *current_board_ | std::views::enumerate) {
        renderer_.set_color(tile == Tile::alive ? cen::colors::red : cen::colors::black);
        renderer_.fill_rect(cen::irect {to_coord(static_cast<int>(idx)) * TILE_SIZE, cen::iarea {TILE_SIZE, TILE_SIZE}});
        // render_neighbour_count(to_coord(static_cast<int>(idx)));
    }
}

void Board::render_neighbour_count(cen::ipoint const& cell)
{
    if (SMALL_FONT_SIZE > TILE_SIZE)
        return;
    auto const num {count_alive_neighbours(cell, *current_board_)};
    auto const number_texture_ {renderer_.make_texture(font_.render_solid(std::to_string(num).c_str(), cen::colors::white))};

    renderer_.set_color(cen::colors::white);
    auto const point_x {cell.x() * TILE_SIZE + TILE_SIZE / 2 - number_texture_.width() / 2};
    auto const point_y {cell.y() * TILE_SIZE + TILE_SIZE / 2 - number_texture_.height() / 2};
    renderer_.render(number_texture_, cen::ipoint {point_x, point_y});
}
