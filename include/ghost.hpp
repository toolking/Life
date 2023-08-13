#pragma once

#include "entity.hpp"
#include "constants.hpp"
#include "timer.hpp"

#include <centurion/video/color.hpp>
#include <centurion/video/renderer.hpp>
#include <centurion/video/texture.hpp>

constexpr std::chrono::seconds SCATTER_DURATION {7};
constexpr std::chrono::seconds CHASE_DURATION {20};

struct GhostConfigs final
{
    const cen::color color;
    const cen::ipoint start;
    const cen::ipoint home;
    const cen::ipoint scatter;
};

class Board;
class Pacman;

using namespace std::chrono_literals;

class Ghost : public Entity
{
public:
    enum class State
    {
        normal,
        blue,
        eyes,
        hidden
    };

    Ghost(cen::renderer_handle const& renderer, GhostConfigs configs);

    virtual ~Ghost() = default;

    void reset();
    void set_state(State state);

    void update(Board&, Pacman const&);
    void render();

private:
    [[nodiscard]] virtual auto calc_target(Pacman const&) const -> cen::ipoint = 0;
    void update_frame();

    cen::renderer_handle renderer_;
    GhostConfigs const configs_;
    cen::texture texture_;
    cen::texture eye_texture_;
    State state_ {State::normal};
    Timer timer_;
    cen::u64ms scatter_timer_ {7s};
    cen::u64ms chase_timer_ {20s};
    cen::ipoint target_ {};

    int render_frame_ {0};
    int frame_ {0};
};
