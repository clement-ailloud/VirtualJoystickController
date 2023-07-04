cmake_minimum_required(VERSION 3.0.0)

project(
    VirtualJoystickControllerDemo
    VERSION 1.0
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

# Common output directories set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
# ${CMAKE_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>) set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
# ${CMAKE_BINARY_DIR}/lib/$<LOWER_CASE:$<CONFIG>>)

# Ninja Multi-Config
if(CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
    set(CMAKE_CONFIGURATION_TYPES "MinSizeRel;Release;Debug")
    set(CMAKE_CROSS_CONFIGS "MinSizeRel;Release;Debug")
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

add_library(
    VirtualJoystickController SHARED ${CMAKE_SOURCE_DIR}/include/virtual_joystick_controller.h
                                     ${CMAKE_SOURCE_DIR}/src/virtual_joystick_controller.cpp)

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
               AUTOMOC TRUE)

include(CMakePackageConfigHelpers)

# Configure the VirtualJoystickController.cmake config file
set(CONFIG_FILES_SUFFIX_PATH lib/cmake/VirtualJoystickController)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/VirtualJoystickControllerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfig.cmake
    INSTALL_DESTINATION ${CONFIG_FILES_SUFFIX_PATH})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerConfigVersion.cmake
        DESTINATION ${CONFIG_FILES_SUFFIX_PATH})

# Generate the VirtualJoystickControllerTargets.cmake file
export(
    TARGETS VirtualJoystickController
    NAMESPACE VirtualJoystickController::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/VirtualJoystickControllerTargets.cmake)

install(
    TARGETS VirtualJoystickController
    EXPORT VirtualJoystickControllerTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(
    EXPORT VirtualJoystickControllerTargets
    NAMESPACE VirtualJoystickController::
    FILE VirtualJoystickControllerTargets.cmake
    DESTINATION lib/cmake/VirtualJoystickController)

add_subdirectory(demo)

# list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")
# include(CTest)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "1.0.0")
# set(CPACK_PACKAGE_CONTACT "your_email@example.com")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Your package description.")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Clement Ailloud")
include(CPack)
