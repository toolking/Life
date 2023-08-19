#include "board.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "math.hpp"

#include <centurion/common/logging.hpp>
#include <centurion/common/math.hpp>
#include <centurion/common/utils.hpp>
#include <centurion/video/renderer.hpp>
#include <centurion/video/color.hpp>

#include <algorithm>
#include <numeric>
#include <ranges>
#include <cmath>
#include <string>
#include <utility>

namespace {

inline constexpr auto init_tiles() -> Board::tiles_type
{
    Board::tiles_type tiles;
    std::transform(CHAR_BOARD.begin(), CHAR_BOARD.end(), tiles.begin(), [](char tile_char) -> Board::Tile {
        switch (tile_char) {
            using enum Board::Tile;
        case '#': return wall;
        case '.': return pill;
        case 'o': return power_pill;
        case '=': return door;
        default: return empty;
        }
    });
    return tiles;
}

inline constexpr auto round_per_direction(Direction side_dir, cen::fpoint const& cell_pos) -> cen::ipoint
{
    using enum Direction;
    switch (side_dir) {
    case right: return {static_cast<int>(floor(cell_pos.x())), static_cast<int>(floor(cell_pos.y()))};
    case down: return {static_cast<int>(ceil(cell_pos.x())), static_cast<int>(floor(cell_pos.y()))};
    case left: return {static_cast<int>(floor(cell_pos.x())), static_cast<int>(ceil(cell_pos.y()))};
    case up: return {static_cast<int>(ceil(cell_pos.x())), static_cast<int>(ceil(cell_pos.y()))};
    default: return {};
    }
}

} // namespace

Board::Board(cen::renderer_handle const& renderer)
  : renderer_ {renderer}
  , board_ {init_tiles()}
  , board_texture_ {renderer_.make_texture("assets/board.png")}
  , door_texture_ {renderer_.make_texture("assets/door.png")}
  , pill_texture_ {renderer_.make_texture("assets/pill.png")}
  , power_pill_texture_ {renderer_.make_texture("assets/power_pill.png")}
  , lives_texture_ {renderer_.make_texture("assets/lives.png")}
  , big_font_ {BIG_FONT_PATH, BIG_FONT_SIZE}
  , score_label_texture_ {renderer_.make_texture(big_font_.render_solid("Score", cen::colors::white))}
  , score_texture_ {renderer_.make_texture(big_font_.render_solid("0", cen::colors::white))}
  , high_score_label_texture_ {renderer_.make_texture(big_font_.render_solid("High Scores", cen::colors::white))}
  , high_score_texture_ {renderer_.make_texture(big_font_.render_solid("0", cen::colors::white))}
{}

void Board::set_lives(int lives) noexcept
{
    lives_ = lives;
}

void Board::set_score(int score) noexcept
{
    score_ = score;
    score_texture_ = renderer_.make_texture(big_font_.render_solid(std::to_string(score_).c_str(), cen::colors::white));
    if (score_ > high_score_) {
        high_score_ = score_;
        high_score_texture_ = renderer_.make_texture(big_font_.render_solid(std::to_string(high_score_).c_str(), cen::colors::white));
    }
}

void Board::add_score(int score) noexcept
{
    set_score(score_ + score);
}

auto Board::operator[](cen::ipoint const& cell) const -> Tile const&
{
    return board_.at(to_index(cell));
}

auto Board::operator[](cen::ipoint const& cell) -> Tile&
{
    return board_.at(to_index(cell));
}

void Board::eat_if_pill(cen::ipoint const& position)
{
    auto const cell_f {position.as_f() / TILE_SIZE};
    using enum Direction;
    for (auto const dir : {right, down, left, up}) {
        auto const board_pos {round_per_direction(dir, cell_f)};
        auto& tile {(*this)[board_pos]};
        switch (tile) {
            using enum Tile;
        case power_pill: {
            add_score(POWER_PILL_SCORE);
            tile = empty; // (*this)[board_pos] = empty;
            [[maybe_unused]] auto const [ignore, power_pills] = count_pills();
            CENTURION_LOG_VERBOSE("power pill #%d eaten", POWER_PILLS_COUNT - power_pills);
            return;
        }
        case pill:
            add_score(PILL_SCORE);
            tile = empty;
            return;
        default: break;
        }
    }
}

auto Board::is_wall_at_position(cen::ipoint const& position, bool can_use_door /* false */) const -> bool
{
    auto const cell_f {position.as_f() / TILE_SIZE};
    using enum Direction;
    for (auto const dir : {right, down, left, up}) {
        auto const board_pos {round_per_direction(dir, cell_f)};
        auto const tile {(*this)[board_pos]};
        switch (tile) {
            using enum Tile;
        case wall:
            return true;
        case door:
            return !can_use_door;
        default: break;
        }
    }
    return false;
}

auto Board::count_pills() const noexcept -> std::pair<int, int>
{
    using int_pair = std::pair<int, int>;
    return std::accumulate(board_.begin(), board_.end(), int_pair {0, 0}, [](int_pair const& acc, Tile tile) -> int_pair {
        switch (tile) {
            using enum Tile;
        case pill: return {acc.first + 1, acc.second};
        case power_pill: return {acc.first, acc.second + 1};
        default: return acc;
        }
    });
}

void Board::reset()
{
    board_ = init_tiles();
}

void Board::update()
{
}

void Board::render_grid(unsigned size, cen::color color)
{
    renderer_.set_color(color);
    for (auto row {0U}; row < WINDOW_HEIGHT / size; ++row) {
        for (auto col {0U}; col < WINDOW_WIDTH / size; ++col) {
            renderer_.draw_rect(cen::irect {cen::cast<int>(col * size), cen::cast<int>(row * size), cen::cast<int>(size), cen::cast<int>(size)});
        }
    }
}

void Board::render_door()
{
    renderer_.render(door_texture_, DOOR_CELL * TILE_SIZE + cen::ipoint {0, TILE_SIZE / 2});
}

void Board::render_pills()
{
    using namespace std::views;
    using pair_type = std::pair<Tile, cen::ipoint>;
    auto is_pill = [](auto&& pair) -> bool { return std::get<1>(pair) == Tile::pill || std::get<1>(pair) == Tile::power_pill; };
    auto to_cell = [](auto&& pair) -> pair_type { return {std::get<1>(pair), {int(std::get<0>(pair) % BOARD_WIDTH), int(std::get<0>(pair) / BOARD_WIDTH)}}; };
    auto to_coords = [](auto&& pair) -> pair_type { return {std::get<0>(pair), {std::get<1>(pair) * TILE_SIZE}}; };
    auto pill_coords = board_
        | enumerate
        | filter(is_pill)
        | transform(to_cell)
        | transform(to_coords);

    std::ranges::for_each(pill_coords, [&](auto const& pos) -> void {
        renderer_.render(pos.first == Tile::pill ? pill_texture_ : power_pill_texture_, pos.second);
    });
}

void Board::render_scores()
{
    renderer_.render(score_label_texture_, cen::ipoint {0, 0});
    renderer_.render(score_texture_, cen::ipoint {0, SPRITE_SIZE});

    renderer_.render(high_score_label_texture_, cen::ipoint {WINDOW_WIDTH / 2, 0});
    renderer_.render(high_score_texture_, cen::ipoint {WINDOW_WIDTH / 2, SPRITE_SIZE});
}

void Board::render_lives()
{
    for (auto i {0}; i < lives_; ++i) {
        renderer_.render(lives_texture_, cen::ipoint {i * SPRITE_SIZE, WINDOW_HEIGHT - SPRITE_SIZE} - cen::ipoint {-SPRITE_SIZE / 4, SPRITE_SIZE / 4});
    }
}

void Board::render()
{
    renderer_.clear_with(cen::colors::green);
    // render_grid(TILE_SIZE, cen::colors::dark_red);
    // render_grid(SPRITE_SIZE, cen::colors::dark_blue);
    board_texture_.set_color_mod(cen::colors::blue); // blue wall lines
    renderer_.render(board_texture_, cen::ipoint {0, 0});
    render_door();
    render_pills();
    render_scores();
    render_lives();
}
