#pragma once

#include "board.hpp"
#include "timer.hpp"

#include <centurion/video/renderer.hpp>

class Game final
{
public:
    enum class State
    {
        starting,
        painting,
        running,
        finishing
    };

    explicit Game(cen::renderer const& renderer);
    void run();

private:
    void process_events();
    void update();
    void render();

    cen::renderer_handle renderer_;
    Board board_;

    State state_ {State::starting};
    std::chrono::milliseconds auto_update_interval_ {1000};
    Timer auto_update_timer_{};
};