#pragma once

#include "board.hpp"
#include "fruit.hpp"
#include "pacman.hpp"
#include "blinky.hpp"
#include "inky.hpp"
#include "pinky.hpp"
#include "clyde.hpp"

#include <centurion/video/renderer.hpp>

class Game final
{
public:
    enum class State
    {
        starting,
        levelcomplete,
        running
    };

    explicit Game(cen::renderer const& renderer);
    void run();

private:
    void process_events();
    void update();
    void render();

    cen::renderer_handle renderer_;
    Board board_;
    Fruit fruit_;
    Pacman pacman_;
    Blinky blinky_;
    Inky inky_;
    Pinky pinky_;
    Clyde clyde_;

    bool running_ {};
    cen::ipoint mouse_position_ {};
    int level_ {1};
};