#pragma once

#include "ghost.hpp"

class Blinky final : public Ghost
{
public:
    Blinky(cen::renderer_handle const& renderer)
      : Ghost {renderer, {
                             .color = cen::colors::red,
                             .start = startpos_of('1'),
                             .home = startpos_of('3'), // use position in home area
                             .scatter = cen::ipoint {27, 3} * TILE_SIZE // top right
                         }}
    {
    }

    [[nodiscard]] virtual auto calc_target(Pacman const& pacman) const -> cen::ipoint
    {
        return pacman.position();
    }
};