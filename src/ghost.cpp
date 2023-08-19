#include "ghost.hpp"
#include "board.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "pacman.hpp"

#include <algorithm>
#include <array>
#include <ranges>

Ghost::Ghost(cen::renderer_handle const& renderer, GhostConfigs configs)
  : Entity(configs.start, Direction::none)
  , config {configs}
  , renderer_ {renderer}
  , texture_ {renderer_.make_texture("assets/ghost.png")}
  , eye_texture_ {renderer_.make_texture("assets/ghost_eyes.png")}
{
}

void Ghost::reset()
{
    set_position(config.start);
    state_ = State::homeleave;
}

void Ghost::start_pacman_power()
{
    state_ = State::blue;
    timer_.start();
}

void Ghost::calculate_direction(Board const& board)
{
    using dist_dir_type = std::pair<float, Direction>;
    std::vector<dist_dir_type> possibilities;
    possibilities.reserve(4);
    using enum Direction;
    for (Direction const i : {right, up, left, down}) {
        auto const pos = position() + i;
        if (!board.is_wall_at_position(pos, can_use_door_)) {
            possibilities.emplace_back(wrapped_distance(pos, target_), i);
        }
    }

    if (possibilities.size() == 1) {
        set_direction(possibilities.front().second);
        return;
    }
    std::ranges::sort(possibilities, {}, &dist_dir_type::first);

    for (auto const& [dist, dir] : possibilities) {
        if (dir != -direction()) { // no 180 turns for ghosts
            set_direction(dir);
            return;
        }
    }
}

void Ghost::update_frame()
{
    constexpr auto FRAMES_SHOWN = 2;
    constexpr auto SPRITE_FRAMES = 6;
    frame_++;
    frame_ %= SPRITE_FRAMES * SPRITE_FRAMES * FRAMES_SHOWN;
    render_frame_ = frame_ / SPRITE_FRAMES / FRAMES_SHOWN;
}

void Ghost::update_position(Board const& board)
{
    for (auto i = 0; position() != target_ && i < speed(); i++) {
        calculate_direction(board);
        set_position(position() + direction());
    }
}

void Ghost::update(Board const& board, Pacman& pacman)
{
    switch (state_) {
        using enum State;
    case chase:
        target_ = calc_target(pacman);
        can_use_door_ = false;
        set_speed(2);
        update_position(board);
        if (timer_.ticks() > chase_duration_) {
            state_ = scatter;
            timer_.start();
        }
        update_frame();
        break;
    case scatter:
        target_ = config.scatter;
        set_speed(2);
        update_position(board);
        if (timer_.ticks() > scatter_duration_) {
            state_ = chase;
            timer_.start();
        }
        break;
    case blue:
        target_ = config.scatter;
        set_speed(SCATTER_SPEED);
        update_position(board);
        if (timer_.ticks() > scatter_duration_) {
            state_ = chase;
            timer_.start();
        }
        if (cen::overlaps(pacman.box(), box())) {
            state_ = eyes;
            target_ = config.home;
            can_use_door_ = true;
            set_speed(EYE_SPEED);
            break;
        }
        update_frame();
        break;
    case homeleave:
        if (!is_home(position())) {
            can_use_door_ = false;
            state_=chase;
            break;
        }
        can_use_door_ = true;
        set_speed(CHASE_SPEED);
        target_ = DOOR_POINT;
        update_position(board);
        if (position()==target_) {
            can_use_door_ = false;
            state_ = chase;
        }
        break;
    case homewait:
        if (timer_.ticks()>scatter_duration_) {
            can_use_door_ = true;
            state_ = homeleave;
            break;
        }
        can_use_door_ = false;
        target_ = config.scatter;
        set_speed(1);
        update_position(board);
        break;
    case eyes:
        update_position(board);
        if (position()==target_) {
            state_ = homewait;
        }
        break;
    case hidden:
    default:
        break;
    }
}

void Ghost::render()
{
    using cen::ipoint;
    using cen::irect;
    switch (state_) {
        using enum State;
    case chase:
    case scatter:
    case homewait:
    case homeleave:
        texture_.set_color_mod(config.color);
        eye_texture_.set_color_mod(cen::colors::white);
        renderer_.render(texture_,
            irect {ipoint {render_frame_ * SPRITE_SIZE, 0}, SPRITE_AREA},
            irect {position() + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        renderer_.render(eye_texture_,
            irect {ipoint {eye_frame(direction()) * SPRITE_SIZE, 0}, SPRITE_AREA},
            irect {position() + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        break;
    case blue:
        texture_.set_color_mod(cen::colors::blue);
        eye_texture_.set_color_mod(cen::colors::white);
        if (timer_.ticks() > scatter_duration_ - 2s) { // 2 seconds left
            if ((timer_.ticks() / 250) % 2 == 0ms) { // blink every 250ms
                texture_.set_color_mod(cen::colors::white);
                eye_texture_.set_color_mod(cen::colors::red);
            }
        }
        renderer_.render(texture_,
            irect {ipoint {render_frame_ * SPRITE_SIZE, 0}, SPRITE_AREA},
            irect {position() + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        renderer_.render(eye_texture_,
            irect {ipoint {4 * SPRITE_SIZE, 0}, SPRITE_AREA},
            irect {position() + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        break;
    case eyes:
        eye_texture_.set_color_mod(cen::colors::white);
        renderer_.render(eye_texture_,
            irect {ipoint {eye_frame(direction()) * SPRITE_SIZE, 0}, SPRITE_AREA},
            irect {position() + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        break;
    case hidden:
    default:
        break;
    }
}
