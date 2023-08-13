cmake_minimum_required( VERSION 3.25.0 )
project( Game VERSION 0.1.0 LANGUAGES C CXX )


include( FetchContent )
FetchContent_Declare(
  centurion
  GIT_REPOSITORY https://github.com/albin-johansson/centurion.git
  GIT_TAG adc604505c9b8b5b22360723adeb3b87e16ab91c # v7.3.0
)
FetchContent_MakeAvailable( centurion )

find_package( SDL2 REQUIRED )
find_package( fmt REQUIRED )



#include(cmake/PreventInSourceBuilds.cmake)
include(cmake/CompilerWarnings.cmake)

find_program( CLANG_TIDY_EXE NAMES "clang-tidy" )

# set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")


# the main program
add_executable( Game main.cpp
  src/game.cpp
  src/entity.cpp
  src/board.cpp
  src/fruit.cpp
  src/timer.cpp
  src/pacman.cpp
  src/ghost.cpp
 )

target_include_directories( Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_include_directories( Game SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS} ${centurion_SOURCE_DIR}/src )
#target_compile_definitions( Game PRIVATE CENTURION_MOCK_FRIENDLY_MODE CENTURION_NO_DEBUG_LOG_MACROS)
target_link_libraries( Game PRIVATE fmt::fmt ${SDL2_LIBRARIES} SDL2_mixer SDL2_ttf SDL2_image )

target_compile_features( Game
  PUBLIC
    cxx_std_23
)

target_compile_options(
  Game
  PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS_CXX}>)

