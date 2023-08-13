#include "fruit.hpp"
#include "board.hpp"

#include <centurion/common/logging.hpp>

Fruit::Fruit(cen::renderer_handle const& renderer)
  : renderer_ {renderer}
  , fruit_texture_ {renderer_.make_texture("assets/fruits.png")}
  , font_ {"assets/VpPixel.ttf", 20U}
  , score_texture_ {renderer_.make_texture(font_.render_solid(" ", cen::colors::white))}
{}

void Fruit::update(Board& board, int level, cen::irect const& pacman_box)
{
    auto const [pills,power_pills] = board.count_pills();
    auto const pills_eaten = TOTAL_PILLS_COUNT-(pills+power_pills);
    using namespace std::chrono_literals;

    switch (state_) {
    case State::shown:
        // check if pacman hit fruit
        if (cen::overlaps(cen::irect {position, cen::iarea {SPRITE_SIZE, SPRITE_SIZE}}, pacman_box)) {
            state_ = State::eaten;
            auto const score {fruit_scores_[std::size_t(active_fruit_)]};
            board.add_score(score);
            score_texture_ = renderer_.make_texture(font_.render_solid(std::to_string(score).c_str(), cen::colors::white));
            timer_.start(); // show score for 1s
        } else if (timer_.ticks() > 9s) { // hide fruit after 9s
            state_ = State::hidden;
            timer_.stop();
        }
        break;
    case State::hidden:
        // show fruit after 70 or 200 pills eaten
        // if not already shown without eating pills in the meantime
        if ((pills_eaten == 70 || pills_eaten == 200) && !(already_shown_for_ == pills_eaten) ) {
            already_shown_for_ = pills_eaten;
            state_ = State::shown;
            timer_.start();
        }
        active_fruit_ = std::min(level - 1, 21) / 3; // set fruit sprite based on level
        break;
    case State::eaten:
        if (timer_.ticks() > 1s) { // hide score after 1s
            state_ = State::hidden;
            timer_.stop();
        }
        break;
    default:
        break;
    }
}
void Fruit::render()
{
    switch (state_) {
    case State::shown:
        renderer_.render(fruit_texture_,
            // clip sprite from texture
            cen::irect {active_fruit_ * SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE},
            // paste to screen at position
            cen::irect {position, cen::iarea {SPRITE_SIZE, SPRITE_SIZE}});
        break;

    case State::eaten:
        renderer_.render(score_texture_,
            position
                + cen::ipoint {SPRITE_SIZE / 2, 0}
                - cen::ipoint {score_texture_.width() / 2, 0});
        break;

    case State::hidden:
    default:
        break;
    }
    // box render for debugging
    // renderer_.set_color(cen::colors::yellow);
    // renderer_.draw_rect(cen::irect{position, cen::iarea{SPRITE_SIZE, SPRITE_SIZE}});
}