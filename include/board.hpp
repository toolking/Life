#pragma once

#include "constants.hpp"

#include <centurion/fonts/font.hpp>
#include <centurion/video/renderer.hpp>
#include <centurion/video/texture.hpp>

#include <array>

class Board final
{
public:
    enum class Tile : uint_fast8_t
    {
        dead = 0,
        alive
    };

    using tiles_type = std::array<Tile, BOARD_HEIGHT * BOARD_WIDTH>;

    explicit Board(cen::renderer_handle const& renderer);

    [[nodiscard]] auto operator[](cen::ipoint const& cell) const -> Tile const&;
    [[nodiscard]] auto operator[](cen::ipoint const& cell) -> Tile&;

    void randomize();
    void clear();

    void update();
    void render();

private:

    void render_neighbour_count(cen::ipoint const& cell);

    cen::renderer_handle renderer_;
    tiles_type board1_;
    tiles_type board2_;
    tiles_type* current_board_{&board1_};
    tiles_type* prev_board_{&board2_};

    cen::font font_{SMALL_FONT_PATH, SMALL_FONT_SIZE};
};
