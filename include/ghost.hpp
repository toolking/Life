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
        chase,
        scatter,
        blue,
        eyes,
        homewait,
        homeleave,
        hidden
    };

    Ghost(cen::renderer_handle const& renderer, GhostConfigs configs);

    virtual ~Ghost() = default;

    void reset();
    void start_pacman_power();

    void update(Board const&, Pacman&);
    void render();

protected:

    GhostConfigs const config;

private:
    [[nodiscard]] virtual auto calc_target(Pacman const&) const -> cen::ipoint = 0;
    void update_frame();
    void update_position(Board const& board);

    void calculate_direction(Board const& board);

    cen::renderer_handle renderer_;
    cen::texture texture_;
    cen::texture eye_texture_;
    State state_ {State::homeleave};
    Timer timer_;
    cen::u64ms scatter_duration_ {SCATTER_DURATION};
    cen::u64ms chase_duration_ {CHASE_DURATION};
    cen::ipoint target_ {DOOR_POINT};

    int render_frame_ {0};
    int frame_ {0};
    bool can_use_door_ {true};
};
/*
flowchart LR
    A((Chase)) -- pacman eats powerpill --> B((Blue))
    B -- hit by pacman --> C((Eyes))
    C -- reach homezone --> D((HomeWait))
    F -- leaved home --> A
    E -- pacman eats powerpill --> B((Blue))
    subgraph colored
        D -- scatter time over --> F((HomeLeave))
        A -- chase time over --> E((Scatter))
        E -- scatter time over --> A
    end
*/