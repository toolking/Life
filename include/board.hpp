#pragma once

#include "constants.hpp"

#include <centurion/fonts/font.hpp>
#include <centurion/video/renderer.hpp>
#include <centurion/video/texture.hpp>

#include <array>
#include <string_view>

class Board final
{
public:
    enum class Tile
    {
        empty = 0,
        wall,
        door,
        pill,
        power_pill,
    };

    using tiles_type = std::array<Tile, BOARD_HEIGHT * BOARD_WIDTH>;

    explicit Board(cen::renderer_handle const& renderer);

    void set_lives(int lives) noexcept;
    void set_score(int score) noexcept;
    void add_score(int score) noexcept;

    [[nodiscard]] auto count_pills() const noexcept -> std::pair<int,int>;

    [[nodiscard]] auto operator[](cen::ipoint const& cell) const -> Tile const&;
    [[nodiscard]] auto operator[](cen::ipoint const& cell) -> Tile&;

    [[nodiscard]] auto is_wall_at_position(cen::ipoint const& position, bool can_use_door = false) const -> bool;
    void eat_if_pill(cen::ipoint const& position);

    void reset();

    void update();
    void render();

private:
    void render_grid(unsigned size, cen::color color);
    void render_pills();
    void render_door();
    void render_scores();
    void render_lives();

    cen::renderer_handle renderer_;

    tiles_type board_ {};

    cen::texture board_texture_;
    cen::texture door_texture_;
    cen::texture pill_texture_;
    cen::texture power_pill_texture_;
    cen::texture lives_texture_;

    cen::font big_font_;
    cen::texture score_label_texture_;
    cen::texture score_texture_;
    cen::texture high_score_label_texture_;
    cen::texture high_score_texture_;

    int lives_ {4};
    int score_ {};
    int high_score_ {};
};
