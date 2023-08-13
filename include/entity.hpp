#pragma once

#include "direction.hpp"

#include <centurion/common/math.hpp>
#include <centurion/video/renderer.hpp>
#include <centurion/video/texture.hpp>

class Entity
{
public:
    Entity(cen::ipoint const& position, Direction direction);

    [[nodiscard]] auto position() const -> cen::ipoint const&;
    [[nodiscard]] auto box() const -> cen::irect;
    [[nodiscard]] auto direction() const -> Direction;
    [[nodiscard]] auto speed() const -> int;
    void set_speed(int speed);

protected:

    void wrap_position();

    cen::ipoint position_ {};
    Direction direction_ {Direction::none};
    int speed_ {2};
};

[[nodiscard]] auto is_colliding(Entity const&, Entity const&) -> bool;
[[nodiscard]] auto is_colliding(Entity const&, cen::irect const&) -> bool;