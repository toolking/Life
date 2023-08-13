#pragma once

#include "constants.hpp"
#include "timer.hpp"
#include "math.hpp"

#include <centurion/common/math.hpp>
#include <centurion/fonts/font.hpp>
#include <centurion/video/renderer.hpp>
#include <centurion/video/texture.hpp>

#include <array>

constexpr cen::ipoint FRUIT_POS {startpos_of('F')+TILE_TO_SPRITE_OFFSET_2D};

class Board;

class Fruit
{
public:
    enum class State
    {
        shown,
        hidden,
        eaten
    };

    Fruit(cen::renderer_handle const& renderer);

    void update(Board& board, int level, cen::irect const& pacman_position);
    void render();

    // fixed position of fruit
    static constexpr cen::ipoint position {FRUIT_POS};

private:
    static constexpr std::array<int,8> fruit_scores_ {100, 300, 500, 700, 1000, 2000, 3000, 5000};

    State state_ {State::hidden};
    cen::renderer_handle renderer_;
    cen::texture fruit_texture_;
    cen::font font_;
    cen::texture score_texture_;
    int active_fruit_ {0}; // used to select fruit sprite based on level
    Timer timer_; // used to hide fruit after 9s and score after 1s
    int already_shown_for_ {-1}; // used to check if fruit was already shown
};

/*
mermaid
graph LR
    A((Shown)) -->|hit by pacman| C
    B((Hidden)) -->|70 or 200 pills eaten| A
    C((Eaten)) -->|after 1s| B
    A -->|after 9s| B
*/