#include "game.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "timer.hpp"

#include <centurion/common/logging.hpp>
#include <centurion/concurrency/thread.hpp>
#include <centurion/events/event_handler.hpp>
#include <centurion/events/misc_events.hpp>
#include <centurion/events/mouse_events.hpp>
#include <centurion/input/keyboard.hpp>
#include <centurion/system/timer.hpp>
#include <centurion/video/renderer.hpp>

Game::Game(cen::renderer const& renderer)
  : renderer_ {renderer}
  , board_ {renderer_}
{}

void Game::run()
{
    state_ = State::starting;
    auto_update_timer_.start();

    while (state_ != State::finishing) {
        auto const start_loop = cen::now();

        process_events();
        update();
        render();

        auto const end_loop = cen::now();
        auto const elapsed_ms = (end_loop - start_loop) / (cen::frequency() * 1000U);
        cen::thread::sleep(std::chrono::milliseconds {FRAME_DURATION_MS - elapsed_ms});
    }
}

void Game::process_events()
{
    cen::event_handler handler;
    while (handler.poll()) {
        if (handler.is<cen::quit_event>()) {
            CENTURION_LOG_INFO("End!\n");
            state_ = State::finishing;
            break;
        }
        if (handler.is<cen::keyboard_event>()) {
            namespace sc = cen::scancodes;
            auto const& event = handler.get<cen::keyboard_event>();
            if (event.released()) {
                if (event.is_active(cen::keycodes::escape)) {
                    CENTURION_LOG_INFO("End!\n");
                    state_ = State::finishing;
                    break;
                } else if (event.is_active(cen::keycodes::space)) {
                    if (state_ != State::running)
                        state_ = State::running;
                    else if (state_ == State::running)
                        state_ = State::painting;
                    break;
                } else if (event.is_active(sc::r)) {
                    board_.randomize();
                    state_ = State::running;
                    break;
                } else if (event.is_active(sc::c)) {
                    board_.clear();
                    state_ = State::running;
                    break;
                }
            }
            using namespace std::chrono_literals;
            if ((event.is_active(sc::right) || event.is_active(sc::d)) && event.released()) {
                board_.update();
            } else if (event.is_active(sc::up) || event.is_active(sc::w)) {
                auto_update_interval_ += auto_update_interval_ > 5s ? 0s : 10ms;
            } else if (event.is_active(sc::left) || event.is_active(sc::a)) {
            } else if (event.is_active(sc::down) || event.is_active(sc::s)) {
                auto_update_interval_ -= auto_update_interval_ <= std::chrono::milliseconds {FRAME_DURATION_MS} ? 0s : 10ms;
            }
        }
        if (handler.is<cen::mouse_button_event>()) {
            auto const& event = handler.get<cen::mouse_button_event>();
            CENTURION_LOG_DEBUG("mouse button event %s", event);

            if (event.clicks()) {
                if (event.button() == cen::mouse_button::left) {
                    auto const& cell = event.position() / TILE_SIZE;
                    if (board_[cell] == Board::Tile::dead) {
                        board_[cell] = Board::Tile::alive;
                        CENTURION_LOG_DEBUG("set to alive %d,%d", cell.x(), cell.y());
                    } else {
                        board_[cell] = Board::Tile::dead;
                        CENTURION_LOG_DEBUG("set to dead %d,%d", cell.x(), cell.y());
                    }
                }
            }
        }
    }
}

void Game::update()
{
    using namespace std::chrono_literals;
    if (State::running == state_ && auto_update_timer_.ticks() > auto_update_interval_) {
        auto_update_timer_.start();
        board_.update();
    }
}

void Game::render()
{
    board_.render();
    renderer_.present();
}