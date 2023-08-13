#pragma once

#include "ghost.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "pacman.hpp"

#include <centurion/common/math.hpp>

class Pinky final : public Ghost
{
public:
    Pinky(cen::renderer_handle const& renderer)
      : Ghost {renderer, {
                             .color = cen::colors::pink,
                             .start = startpos_of('3'),
                             .home = startpos_of('3'), // use position in home area
                             .scatter = cen::ipoint {0, 3} * TILE_SIZE // top left
                         }}
    {
    }

    [[nodiscard]] virtual auto calc_target(Pacman const& pacman) const -> cen::ipoint
    {
        // Pinky's target is 4 blocks ahead of Pacman
        constexpr auto dist = 4 * TILE_SIZE;
        return pacman.position() + to_point<cen::ipoint::value_type>(pacman.direction()) * dist;
    }
};