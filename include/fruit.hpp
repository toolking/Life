#pragma once

#include "constants.hpp"
#include "timer.hpp"
#include "math.hpp"

#include <centurion/common/math.hpp>
#include <centurion/fonts/font.hpp>
#include <centurion/video/renderer.hpp>
#include <centurion/video/texture.hpp>

#include <array>

constexpr auto FRUIT_POS {FRUIT_STARTPOS+TILE_TO_SPRITE_OFFSET_2D};
constexpr auto FRUIT_SCORES {std::to_array<int>({100, 300, 500, 700, 1000, 2000, 3000, 5000})};

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
    void reset() noexcept;
    void update(Board& board, int level, cen::irect const& pacman_box);
    void render();

    // fixed position of fruit
    static constexpr cen::ipoint position {FRUIT_POS};

private:

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