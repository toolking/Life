#pragma once

#include "ghost.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "pacman.hpp"

#include <centurion/common/math.hpp>

class Blinky final : public Ghost
{
public:
    Blinky(cen::renderer_handle const& renderer)
      : Ghost {renderer, {
                             .color = cen::colors::red,
                             .start = startpos_of('1'),
                             .home = startpos_of('3'), // use position in home area
                             .scatter = TOP_RIGHT * TILE_SIZE // top right
                         }}
    {
    }

    [[nodiscard]] virtual auto calc_target(Pacman const& pacman) const -> cen::ipoint
    {
        return pacman.position();
    }
};