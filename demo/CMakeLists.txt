cmake_minimum_required(VERSION 3.0.0)

add_executable(VirtualJoystickControllerDemo
               ${CMAKE_SOURCE_DIR}/demo/virtual_joystick_controller_demo.cpp)
target_link_libraries(
    ${CMAKE_PROJECT_NAME} PUBLIC VirtualJoystickController Qt${QT_VERSION_MAJOR}::Core
                                 Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(
    VirtualJoystickControllerDemo
    PROPERTIES CXX_STANDARD 14
               CXX_STANDARD_REQUIRED ON
               LINKER_LANGUAGE CXX
               LINK_WHAT_YOU_USE TRUE)
