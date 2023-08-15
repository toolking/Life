#include "game.hpp"
#include "constants.hpp"
#include "direction.hpp"
#include "ghost.hpp"

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
  , board_    {renderer_}
  , fruit_    {renderer_}
  , pacman_   {renderer_}
  , blinky_   {renderer_}
  , inky_     {renderer_}
  , pinky_    {renderer_}
  , clyde_    {renderer_}
{}

void Game::run()
{
    running_ = true;
    while (running_) {
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
            running_ = false;
            break;
        }
        if (handler.is<cen::keyboard_event>()) {
            auto const& event = handler.get<cen::keyboard_event>();
            if (event.released() && event.is_active(cen::keycodes::escape)) {
                CENTURION_LOG_INFO("End!\n");
                running_ = false;
                break;
            }
            namespace sc = cen::scancodes;
            using enum Direction;
            if (event.is_active(sc::right) || event.is_active(sc::d)) {
                pacman_.want_direction(right);
            } else if (event.is_active(sc::up) || event.is_active(sc::w)) {
                pacman_.want_direction(up);
            } else if (event.is_active(sc::left) || event.is_active(sc::a)) {
                pacman_.want_direction(left);
            } else if (event.is_active(sc::down) || event.is_active(sc::s)) {
                pacman_.want_direction(down);
            } else if (event.is_active(sc::k)) {
                pacman_.die();
            }
        }
        if (handler.is<cen::mouse_motion_event>()) {
            auto const& event = handler.get<cen::mouse_motion_event>();
            mouse_position_ = cen::ipoint {event.x(), event.y()};
        }
    }
}

void Game::update()
{
    board_.update();
    auto const [pills_before, power_pills_before] = board_.count_pills();
    pacman_.update(board_);
    auto const [pills, power_pills] = board_.count_pills();
    if (pills + power_pills == 0) { // level completed
        CENTURION_LOG_INFO("Level %d finished!\n", level_);
        ++level_;
        board_.reset();
        fruit_.reset();
        blinky_.reset();
        pinky_.reset();
        inky_.reset();
        clyde_.reset();
        pacman_.reset();
    }
    if (power_pills < power_pills_before) { // pacman ate power pill
        blinky_.start_pacman_power();
        pinky_.start_pacman_power();
        inky_.start_pacman_power();
        clyde_.start_pacman_power();
    }
    fruit_.update(board_, level_, pacman_.box());
    blinky_.update(board_, pacman_);
    pinky_.update(board_, pacman_);
    inky_.set_blinky_pos(blinky_.position());
    inky_.update(board_, pacman_);
    clyde_.update(board_, pacman_);
}

void Game::render()
{
    board_.render();
    fruit_.render();
    pacman_.render();
    blinky_.render();
    pinky_.render();
    inky_.render();
    clyde_.render();

    renderer_.present();
}