cmake_minimum_required( VERSION 3.25.0 )
project( Life VERSION 0.0.1 LANGUAGES C CXX )

include( FetchContent )
FetchContent_Declare(
  centurion
  GIT_REPOSITORY https://github.com/albin-johansson/centurion.git
  GIT_TAG v7.3 # v7.3.0
)
FetchContent_MakeAvailable( centurion )

find_package( SDL2 REQUIRED )
include(cmake/CompilerWarnings.cmake)

find_program( CLANG_TIDY_EXE NAMES "clang-tidy" )
find_program( CLANG_CPPCHECK_EXE NAMES "cppcheck" )
find_program( CLANG_IWYU_EXE NAMES "iwyu" )
find_program( CLANG_CPPLINT_EXE NAMES "cppcheck" )

#set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
#set(CMAKE_CXX_CPPCHECK "${CLANG_CPPCHECK_EXE}")
#set(CMAKE_CXX_CPPLINT "${CLANG_CPPLINT_EXE}")
#set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${CLANG_IWYU_EXE}")


# the main program
add_executable( Life main.cpp
  src/game.cpp
  src/board.cpp
 )

target_include_directories( Life PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_include_directories( Life SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS} ${centurion_SOURCE_DIR}/src )
target_link_libraries( Life
  PRIVATE
    ${SDL2_LIBRARIES}
    SDL2_mixer
    SDL2_ttf
    SDL2_image
)

target_compile_features( Life
  PUBLIC
    cxx_std_23
)

target_compile_options( Life
  PRIVATE
    ${PROJECT_WARNINGS_CXX}
)

include( FindDoxygen )