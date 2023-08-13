#include "ghost.hpp"

#include "constants.hpp"
#include "direction.hpp"

Ghost::Ghost(cen::renderer_handle const& renderer, GhostConfigs configs)
  : Entity(configs.start, Direction::none)
  , renderer_ {renderer}
  , configs_ {configs}
  , texture_ {renderer_.make_texture("assets/ghost.png")}
  , eye_texture_ {renderer_.make_texture("assets/ghost_eyes.png")}
{
}

void Ghost::reset()
{
    position_ = configs_.start;
    state_ = State::normal;
}

void Ghost::set_state(State state)
{
    state_ = state;
    switch (state_) {
    case State::normal:
        texture_.set_color_mod(configs_.color);
        break;
    case State::blue:
        texture_.set_color_mod(cen::colors::blue);
        timer_.start();
        break;
    case State::eyes:
        break;
    case State::hidden:
    default:
        break;
    }
}

void Ghost::update_frame()
{
    constexpr auto FRAMES_SHOWN = 2;
    constexpr auto SPRITE_FRAMES = 6;
    frame_++;
    frame_ %= SPRITE_FRAMES * (SPRITE_FRAMES * FRAMES_SHOWN);
    render_frame_ = frame_ / (SPRITE_FRAMES * FRAMES_SHOWN);
}

void Ghost::update(Board&, Pacman const&)
{
    switch (state_) {
    case State::normal:
        update_frame();
        break;
    case State::blue:
        if (timer_.ticks() > scatter_timer_) { // 2 seconds left
            state_ = State::normal;
            timer_.stop();
        }
        update_frame();
        break;
    case State::eyes:
        break;
    case State::hidden:
    default:
        break;
    }
}

void Ghost::render()
{
    switch (state_) {
    case State::normal:
        texture_.set_color_mod(configs_.color);
        eye_texture_.set_color_mod(cen::colors::white);
        renderer_.render(texture_,
            // clip sprite from texture
            cen::irect {cen::ipoint {render_frame_ * SPRITE_SIZE, 0}, SPRITE_AREA},
            // paste to screen at position
            cen::irect {position_ + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        renderer_.render(eye_texture_,
            // clip sprite from texture
            cen::irect {cen::ipoint {eye_frame(Direction::down) * SPRITE_SIZE, 0}, SPRITE_AREA},
            // paste to screen at position
            cen::irect {position_ + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        break;
    case State::blue:
        texture_.set_color_mod(cen::colors::blue);
        eye_texture_.set_color_mod(cen::colors::white);
        if (timer_.ticks() > scatter_timer_ - 2s) { // 2 seconds left
            if ((timer_.ticks() / 250) % 2 == 1ms) { // blink every 250ms
                texture_.set_color_mod(cen::colors::white);
                eye_texture_.set_color_mod(cen::colors::red);
            }
        }
        renderer_.render(texture_,
            // clip sprite from texture
            cen::irect {cen::ipoint {render_frame_ * SPRITE_SIZE, 0}, SPRITE_AREA},
            // paste to screen at position
            cen::irect {position_ + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        renderer_.render(eye_texture_,
            // clip sprite from texture
            cen::irect {cen::ipoint {eye_frame(Direction::none) * SPRITE_SIZE, 0}, SPRITE_AREA},
            // paste to screen at position
            cen::irect {position_ + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        break;
    case State::eyes:
        eye_texture_.set_color_mod(cen::colors::white);
        renderer_.render(eye_texture_,
            // clip sprite from texture
            cen::irect {cen::ipoint {eye_frame(Direction::down) * SPRITE_SIZE, 0}, SPRITE_AREA},
            // paste to screen at position
            cen::irect {position_ + TILE_TO_SPRITE_OFFSET_2D, SPRITE_AREA});
        break;
    case State::hidden:
    default:
        break;
    }
}
