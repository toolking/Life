#pragma once

#include <centurion/common/math.hpp>

template <typename T>
[[nodiscard]] inline constexpr auto operator*(cen::basic_point<T> const& point, typename cen::basic_point<T>::value_type scalar) noexcept -> cen::basic_point<T>
{
    return {point.x() * scalar, point.y() * scalar};
}

template <typename T>
[[nodiscard]] inline constexpr auto operator*(cen::basic_rect<T> const& rect, typename cen::basic_point<T>::value_type scalar) noexcept -> cen::basic_rect<T>
{
    return {rect.x() * scalar, rect.y() * scalar, rect.width() * scalar, rect.height() * scalar};
}

template <typename T>
inline constexpr auto operator*=(cen::basic_point<T>& point, typename cen::basic_point<T>::value_type scalar) noexcept -> cen::basic_point<T>&
{
    return point = point * scalar;
}

template <typename T>
[[nodiscard]] inline constexpr auto operator/(cen::basic_point<T> const& point, typename cen::basic_point<T>::value_type scalar) noexcept -> cen::basic_point<T>
{
    return {point.x() / scalar, point.y() / scalar};
}

template <typename T>
inline constexpr auto operator/=(cen::basic_point<T>& point, typename cen::basic_point<T>::value_type scalar) noexcept -> cen::basic_point<T>&
{
    return point = point / scalar;
}

template <typename T>
inline constexpr auto operator+=(cen::basic_point<T>& point, cen::basic_point<T> const& point2) noexcept -> cen::basic_point<T>&
{
    return point = point + point2;
}

template <typename T>
inline constexpr auto operator-=(cen::basic_point<T>& point, cen::basic_point<T> const& point2) noexcept -> cen::basic_point<T>&
{
    return point = point - point2;
}
