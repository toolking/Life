#pragma once

#include "ghost.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "pacman.hpp"

#include <centurion/common/math.hpp>

class Inky final : public Ghost
{
public:
    Inky(cen::renderer_handle const& renderer)
      : Ghost {renderer, {
                             .color = cen::colors::cyan,
                             .start = startpos_of('2'),
                             .home = startpos_of('2'), // use position in home area
                             .scatter = BOTTOM_RIGHT * TILE_SIZE // bottom right
                         }}
    {
    }

    void set_blinky_pos(cen::ipoint const& blinky_pos) {
        blinky_pos_=blinky_pos;
    }

    [[nodiscard]] virtual auto calc_target(Pacman const& pacman) const -> cen::ipoint
    {
        // Inky's target is twice the distance between Pac and Blinky in the direction of Pac
        constexpr auto dist = 2 * TILE_SIZE;
        auto const pos = pacman.position() + to_point<int>(pacman.direction()) * dist;
        return pos + pos - blinky_pos_;
    }
private:
    cen::ipoint blinky_pos_{startpos_of('1')};
};