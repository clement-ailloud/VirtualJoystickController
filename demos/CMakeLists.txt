cmake_minimum_required(VERSION 3.0.0)

add_executable(VirtualJoystickControllerDemo virtual_joystick_controller_demo.cpp)

target_link_libraries(
    VirtualJoystickControllerDemo PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets
                                         VirtualJoystickController)

set_target_properties(
    VirtualJoystickControllerDemo
    PROPERTIES CXX_STANDARD 14
               CXX_STANDARD_REQUIRED ON
               LINKER_LANGUAGE CXX
               LINK_WHAT_YOU_USE TRUE)

install(TARGETS VirtualJoystickControllerDemo RUNTIME COMPONENT Demos)
