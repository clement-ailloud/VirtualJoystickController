cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

add_executable(VirtualJoystickControllerDemo
    ${CMAKE_SOURCE_DIR}/include/virtual_joystick_controller.h
    ${CMAKE_SOURCE_DIR}/src/virtual_joystick_controller.cpp
    ${CMAKE_SOURCE_DIR}/demo/virtual_joystick_controller_demo.cpp
)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_options(VirtualJoystickControllerDemo
	PUBLIC
	-Wall -Wextra -Wpedantic
    )
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    target_compile_options(VirtualJoystickControllerDemo
	PUBLIC
	-Wall -Wextra -Wpedantic -Werror
    )
endif()

#target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
	Qt5::Widgets
	Qt5::Core
        VirtualJoystickController
)

target_link_libraries(VirtualJoystickControllerDemo
    PRIVATE
        Qt5::Core
        Qt5::Widgets
)

set_target_properties(VirtualJoystickControllerDemo
    PROPERTIES
	CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON

        LINKER_LANGUAGE     CXX
        LINK_WHAT_YOU_USE   TRUE

        AUTOMOC TRUE
)

target_compile_features(${CMAKE_PROJECT_NAME}
    PUBLIC
	cxx_constexpr
	cxx_final
	cxx_attributes
	cxx_auto_type
	cxx_inheriting_constructors
	cxx_inline_namespaces
	cxx_lambdas
	cxx_lambda_init_captures
	cxx_noexcept
	cxx_nullptr
	cxx_override
	cxx_range_for
	cxx_rvalue_references
	cxx_sizeof_member
	cxx_static_assert
	cxx_template_template_parameters
)

