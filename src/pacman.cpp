#include "pacman.hpp"
#include "board.hpp"

#include <centurion/common/logging.hpp>

Pacman::Pacman(cen::renderer_handle const& renderer)
  : Entity(PACMAN_STARTPOS, Direction::none)
  , renderer_ {renderer}
  , texture_ {renderer_.make_texture("assets/pacman.png")}
  , dead_texture_ {renderer_.make_texture("assets/pacman_dead.png")}
{}

void Pacman::reset() noexcept
{
    state_ = State::live;
    set_position(PACMAN_STARTPOS);
    set_direction(Direction::none);
    next_direction_ = Direction::none;
    render_frame_ = 0;
    frame_ = 0;
}

void Pacman::update_live_frame(Board const& b)
{
    if (direction() == Direction::none) {
        render_frame_ = 0; // undefined direction
        return;
    }
    if (b.is_wall_at_position(position() + direction())) {
        render_frame_ = 2; // open mouth
        return;
    }
    constexpr auto FRAMES_SHOWN = 4;
    constexpr auto SPRITE_FRAMES = 3; // O G C
    frame_++;
    frame_ %= SPRITE_FRAMES * SPRITE_FRAMES * FRAMES_SHOWN;
    render_frame_ = frame_ / SPRITE_FRAMES / FRAMES_SHOWN;
}

void Pacman::update_dead_frame()
{
    constexpr auto FRAMES_SHOWN = 2;
    constexpr auto SPRITE_FRAMES = 10;
    frame_++;
    frame_ %= SPRITE_FRAMES * SPRITE_FRAMES * FRAMES_SHOWN;
    render_frame_ = frame_ / SPRITE_FRAMES / FRAMES_SHOWN;
    if (render_frame_ == 0 && frame_ == 0) { // dead animation ends
        state_ = State::hidden;
    }
}

void Pacman::die() noexcept
{
    state_ = State::dieing;
    render_frame_ = 0;
    frame_ = 0;
}

void Pacman::want_direction(Direction new_direction) noexcept
{
    using enum Direction;
    //CENTURION_LOG_DEBUG("Pacman::set_direction: %s", new_direction);
    //CENTURION_LOG_DEBUG("Pacman::direction: %s", direction());
    [[maybe_unused]] auto a = direction();
    if (direction() == none) { // no direction direct use new direction
        set_direction(new_direction);
        next_direction_ = none;
    } else if (new_direction == direction()) { // same direction
        next_direction_ = none;
    } else if (new_direction == -direction()) { // opposite direction
        set_direction(new_direction);
        next_direction_ = none;
    } else if (new_direction == none) { // set to no direction
        next_direction_ = none;
    } else { // queue different direction
        next_direction_ = new_direction;
    }
}

void Pacman::update_position(Board const& board)
{
    if (!board.is_wall_at_position(position() + direction())) {
        set_position(position() + direction());
        //position_ += direction_;
        //wrap_position();
    }
    if (Direction::none != next_direction_) {
        if (!board.is_wall_at_position(position() + next_direction_)) {
            set_direction(next_direction_);
            next_direction_ = Direction::none;
        }
    }
}

void Pacman::update(Board& board)
{
    switch (state_) {
    case State::live:
        for (int i = 0; i < speed(); i++) {
            update_position(board);
            board.eat_if_pill(position());
        }
        update_live_frame(board);
        break;
    case State::dieing:
        update_dead_frame();
        break;
    case State::hidden:
    default:
        break;
    }
}

void Pacman::render()
{
    switch (state_) {
    case State::live:
        renderer_.render(texture_,
            // clip sprite from texture
            cen::irect {cen::ipoint {render_frame_ * SPRITE_SIZE, 0}, SPRITE_AREA},
            // paste to screen at position
            cen::irect {position() + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA},
            to_angle(direction()));
        break;
    case State::dieing:
        renderer_.render(dead_texture_,
            // clip sprite from texture
            cen::irect {cen::ipoint {render_frame_ * SPRITE_SIZE, 0}, SPRITE_AREA},
            // paste to screen at position
            cen::irect {position() + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA},
            to_angle(direction()));
        break;
    case State::hidden:
    default: break;
    }
}