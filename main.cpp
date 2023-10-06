#include "include/game.hpp"
#include <centurion.hpp>

int main(int, char**)
{
    try {
        const cen::sdl sdl;
        const cen::img img;
        const cen::ttf ttf;
        const cen::mix mix;
        cen::window window {"Hello, World!", {WINDOW_WIDTH, WINDOW_HEIGHT}, cen::window::shown};
        cen::renderer renderer {window.make_renderer()};
        cen::set_priority(cen::log_priority::verbose);

        Game game {renderer};

        game.run();

    } catch (std::exception const& excpt) {
        CENTURION_LOG_ERROR("Exception: %s", excpt.what());
    }
}
