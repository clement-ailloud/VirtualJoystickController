cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(VirtualJoystickControllerDemo VERSION 1.0 LANGUAGES CXX)

# Add gcc colors
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
endif()

find_package(Qt5
    COMPONENTS
        Core      REQUIRED
        Widgets   REQUIRED
)

add_subdirectory(demo)

