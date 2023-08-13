#pragma once

#include "ghost.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "pacman.hpp"

#include <centurion/common/math.hpp>

class Clyde final : public Ghost
{
public:
    Clyde(cen::renderer_handle const& renderer)
      : Ghost {renderer, {
                             .color = cen::colors::orange,
                             .start = startpos_of('4'),
                             .home = startpos_of('4'), // use position in home area
                             .scatter = cen::ipoint {0, 33} * TILE_SIZE // bottom left
                         }}
    {
    }

    [[nodiscard]] virtual auto calc_target(Pacman const& pacman) const -> cen::ipoint
    {
        auto const dist = wrapped_distance(position(), pacman.position());
        return (dist > 8 * TILE_SIZE) ? pacman.position() : cen::ipoint {0, 33} * TILE_SIZE;
    }
};