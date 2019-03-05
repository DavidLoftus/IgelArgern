cmake_minimum_required(VERSION 3.0.0)
project(IgelArgern VERSION 0.1.0)

cmake_minimum_required(VERSION 2.8)

file(GLOB game_SRC
    "src/*.c"
)

file(GLOB game_INCLUDE
	"include/*.h"
)

add_executable(IgelArgern ${game_SRC} ${game_INCLUDE})
