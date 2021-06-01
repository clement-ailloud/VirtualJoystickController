cmake_minimum_required(VERSION 3.0.0)

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

add_library(VirtualJoystickController
    SHARED
        ${CMAKE_SOURCE_DIR}/src/virtual_joystick_controller.cpp
)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_options(VirtualJoystickController
	PUBLIC
	-Wall -Wextra -Wpedantic
    )
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    target_compile_options(VirtualJoystickController
	PUBLIC
	-Wall -Wextra -Wpedantic -Werror
    )
endif()


target_include_directories(VirtualJoystickController PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(VirtualJoystickController PRIVATE Qt5::Widgets Qt5::Core)

set_target_properties(VirtualJoystickController
    PROPERTIES
	CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON

        LINKER_LANGUAGE     CXX
        LINK_WHAT_YOU_USE   TRUE

        AUTOMOC TRUE
)

add_subdirectory(demo)

