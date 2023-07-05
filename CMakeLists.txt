cmake_minimum_required(VERSION 3.12.0)

project(
    VirtualJoystickController
    VERSION 1.0.0
    LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH cmake/modules)
include(PreCommit)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# set(CMAKE_CXX_CPPCHECK cppcheck --project=${CMAKE_BINARY_DIR}/compile_commands.json --enable=all
# --output-file=${CMAKE_BINARY_DIR}/cppcheck_output.txt) set(CMAKE_CXX_CLANG_TIDY clang-tidy
# --use-color -header-filter=.* --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy -p
# ${CMAKE_BINARY_DIR})
set(CMAKE_LINK_WHAT_YOU_USE iwyu ${CMAKE_BINARY_DIR})
# set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE )

# Ninja Multi-Config
if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
    set(CMAKE_CONFIGURATION_TYPES "Debug;MinSizeRel;Release")
    set(CMAKE_CROSS_CONFIGS "Debug;MinSizeRel;Release")
endif()

# Common compile options
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
    add_compile_options(
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-O3>
        -Wall
        -Wextra
        -Wpedantic
        $<$<CONFIG:Release>:-Werror>
        -fdiagnostics-color=always
        $<$<CONFIG:Debug>:-fprofile-instr-generate>
        $<$<CONFIG:Debug>:-fcoverage-mapping>)
endif()

if(CMAKE_PLATFORM_ID STREQUAL "Linux")
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

find_package(
    Qt5
    COMPONENTS Core
    REQUIRED Widgets REQUIRED)

# find_package(Valgrind REQUIRED)

set(PUBLIC_HEADER_FILES include/virtual_joystick_controller.h)
set(SOURCE_FILES src/virtual_joystick_controller.cpp)

add_library(VirtualJoystickController SHARED ${PUBLIC_HEADER_FILES} ${SOURCE_FILES})

target_include_directories(
    VirtualJoystickController PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                     $<INSTALL_INTERFACE:include>)

target_link_libraries(VirtualJoystickController PRIVATE Qt${QT_VERSION_MAJOR}::Core
                                                        Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(
    VirtualJoystickController
    PROPERTIES VERSION 1.0.0
               CXX_STANDARD 14
               CXX_STANDARD_REQUIRED ON
               LINKER_LANGUAGE CXX
               LINK_WHAT_YOU_USE TRUE
               AUTOMOC TRUE
               PUBLIC_HEADER ${PUBLIC_HEADER_FILES})

include(CMakePackageConfigHelpers)

# Configure the VirtualJoystickController.cmake config file
set(CONFIG_FILES_PREFIX_PATH lib/cmake/VirtualJoystickController)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/VirtualJoystickControllerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfig.cmake
    INSTALL_DESTINATION ${CONFIG_FILES_PREFIX_PATH})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfig.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfigVersion.cmake
    DESTINATION ${CONFIG_FILES_PREFIX_PATH}
    COMPONENT Development)

# Generate the VirtualJoystickControllerTargets.cmake file
export(
    TARGETS VirtualJoystickController
    NAMESPACE VirtualJoystickController::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerTargets.cmake)

install(
    TARGETS VirtualJoystickController
    EXPORT VirtualJoystickControllerTargets
    LIBRARY COMPONENT libraries NAMELINK_COMPONENT Development
    PUBLIC_HEADER COMPONENT Development)

install(
    EXPORT VirtualJoystickControllerTargets
    DESTINATION ${CONFIG_FILES_PREFIX_PATH}
    NAMESPACE VirtualJoystickController::
    FILE VirtualJoystickControllerTargets.cmake
    COMPONENT Development)

add_subdirectory(demos)

# list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")
# include(CTest)

# Generate the binary package
set(CPACK_PACKAGE_CONTACT "clem.ailloud@gmail.com")
set(CPACK_PACKAGE_MAINTAINER "Clement Ailloud")
set(CPACK_PACKAGE_NAME "libinputcontrols")
#set(CPACK_PACKAGE_VERSION "1.0.0")

set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEB_PACKAGE_COMPONENT ON)
set(CPACK_DEB_UNINSTALL ON)

include(CPack)

# Generate the runtime package
cpack_add_install_type(Full DISPLAY_NAME "Everything")

# Generate the demos package
cpack_add_component_group(demos)

cpack_add_component(
    Demos
    DISPLAY_NAME "Demos package"
    DESCRIPTION "Demos package description"
    GROUP demos
    DEPENDS libraries
    INSTALL_TYPES Full)

# Generate the development package component
cpack_add_component_group(dev EXPANDED
                          DESCRIPTION "All of the tools you'll ever need to develop software")

cpack_add_install_type(Developer)

cpack_add_component(
    Development
    DISPLAY_NAME "Development package"
    DESCRIPTION "C++ Libraries development files"
    GROUP dev
    DEPENDS libraries
    INSTALL_TYPES Developer Full)
