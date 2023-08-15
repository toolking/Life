#pragma once

#include "constants.hpp"
#include "direction.hpp"
#include "math.hpp"
#include "timer.hpp"
#include "entity.hpp"

#include <centurion/video/renderer.hpp>

constexpr cen::ipoint PACMAN_START_POS {startpos_of(PACMAN_CHAR)};

class Board;

class Pacman final : public Entity
{
public:
    enum class State
    {
        live,
        dieing,
        hidden
    };

    explicit Pacman(cen::renderer_handle const& renderer);

    void reset() noexcept;
    void want_direction(Direction new_dir) noexcept;
    void die() noexcept;

    void update(Board& b);
    void render();

private:
    void update_live_frame(Board const& b);
    void update_dead_frame();

    void update_position(Board const& board);

    cen::renderer_handle renderer_;
    cen::texture texture_;
    cen::texture dead_texture_;
    State state_ {State::live};
    Timer timer_;
    Direction next_direction_ {Direction::none};

    int render_frame_ {0};
    int frame_ {0};
};
