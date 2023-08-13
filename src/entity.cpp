#include "entity.hpp"
#include "constants.hpp"

Entity::Entity(cen::ipoint const& position, Direction direction)
    : position_ {position}
    , direction_ {direction}
{}

auto Entity::position() const -> cen::ipoint const&
{
    return position_;
}

auto Entity::speed() const -> int
{
    return speed_;
}

auto Entity::direction() const -> Direction
{
    return direction_;
}

auto Entity::box() const -> cen::irect
{
    return {position_, TILE_AREA};
}

void Entity::set_speed(int speed)
{
    speed_ = speed;
}

void Entity::wrap_position()
{
    if (position_.x() > WINDOW_WIDTH + TILE_SIZE) {
        position_.set_x(-TILE_SIZE);
    } else if (position_.x() < -TILE_SIZE) {
        position_.set_x(WINDOW_WIDTH + TILE_SIZE);
    }
}