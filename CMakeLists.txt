cmake_minimum_required(VERSION 3.10)
project(CS1430Game)
set(CMAKE_CXX_STANDARD 17)

# --- 1. Detect OS and Find Libraries ---
if(WIN32)
    # [WINDOWS] Use 'find_package' in Config mode (Works best with vcpkg)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)

    # Gather the Windows-specific library targets
    set(GAME_LIBS 
        SDL2::SDL2 
        SDL2::SDL2main 
        SDL2_image::SDL2_image 
        SDL2_mixer::SDL2_mixer 
        SDL2_ttf::SDL2_ttf
    )
else()
    # [LINUX / MAC] Use PkgConfig
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

    # Include directories explicitly for Linux
    include_directories(
        ${SDL2_INCLUDE_DIRS} 
        ${SDL2_IMAGE_INCLUDE_DIRS} 
        ${SDL2_MIXER_INCLUDE_DIRS} 
        ${SDL2_TTF_INCLUDE_DIRS}
    )

    # Gather the Linux-specific library variables
    set(GAME_LIBS 
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
    )
endif()

# --- 2. Setup Include Folders ---
include_directories(include)

# --- 3. Gather Source Files ---
file(GLOB SOURCES "src/*.cpp")

# --- 4. Create Executable ---
add_executable(GameExecutable ${SOURCES})

# --- 5. Link Libraries ---
target_link_libraries(GameExecutable PRIVATE ${GAME_LIBS})

# --- 6. Auto-Copy Assets ---
add_custom_command(TARGET GameExecutable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Assets
    ${CMAKE_BINARY_DIR}/Assets
)