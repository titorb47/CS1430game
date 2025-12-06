cmake_minimum_required(VERSION 3.10)
project(CS1430Game)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Copy Assets folder to build directory
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR})

file(GLOB SOURCES "src/*.cpp")

add_executable(GameExecutable ${SOURCES})

target_link_libraries(GameExecutable 
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)
# --- Auto-Copy Assets ---
# This tells CMake to copy the "Assets" folder to the build folder 
# every time the game finishes compiling.
add_custom_command(TARGET GameExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Assets
    ${CMAKE_BINARY_DIR}/Assets
)