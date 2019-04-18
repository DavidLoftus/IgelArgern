cmake_minimum_required(VERSION 3.0.0)
project(IgelArgern VERSION 0.1.0)

cmake_minimum_required(VERSION 2.8)

file(GLOB game_SRC
    "src/*.c"
)

file(GLOB game_INCLUDE
	"src/*.h"
)

add_executable(IgelArgern ${game_SRC} ${game_INCLUDE})

set_property(TARGET IgelArgern PROPERTY C_STANDARD 99)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

if(NOT CURSES_HAVE_CURSES_H AND CURSES_HAVE_NCURSES_CURSES_H)
    include_directories(${CURSES_INCLUDE_DIR}/ncurses)
endif()

target_link_libraries(IgelArgern ${CURSES_LIBRARIES})
