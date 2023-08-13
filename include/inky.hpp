#pragma once

#include "ghost.hpp"
#include "direction.hpp"

class Inky final : public Ghost
{
public:
    Inky(cen::renderer_handle const& renderer)
      : Ghost {renderer, {
                             .color = cen::colors::cyan,
                             .start = startpos_of('2'),
                             .home = startpos_of('2'), // use position in home area
                             .scatter = cen::ipoint {27, 33} * TILE_SIZE // bottom right
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
        auto const pos = pacman.position() + to_point<cen::ipoint::value_type>(pacman.direction()) * dist;
        return pos + pos - blinky_pos_;
    }
private:
    cen::ipoint blinky_pos_{startpos_of('1')};
};