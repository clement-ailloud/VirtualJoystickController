include_guard(GLOBAL)

find_package(
    Python3
    COMPONENTS Interpreter
    REQUIRED)

set(PRE_COMMIT_HOME ${CMAKE_SOURCE_DIR}/.cache/pre-commit)
if(EXISTS ${PRE_COMMIT_HOME})
    return()
endif()

message(STATUS "Checking for 'pre-commit...")
execute_process(
    COMMAND bash "-c" "pip3 list | grep pre-commit"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_QUIET
    RESULT_VARIABLE PRE_COMMIT_NOT_FOUND)
message(STATUS "Checking for 'pre-commit... done")

if(PRE_COMMIT_NOT_FOUND)
    message(STATUS "Installing 'pre-commit...")
    execute_process(
        COMMAND bash "-c" "pip3 install pre-commit"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_QUIET)
endif()

set(PRE_COMMIT_CONFIG_FILE ".pre-commit-config.yaml")
if(NOT (EXISTS PRE_COMMIT_CONFIG_FILE))
    message(STATUS "Installing 'pre-commit...")
    execute_process(
        COMMAND bash "-c" "pre-commit sample-config > ${PRE_COMMIT_CONFIG_FILE}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_QUIET)
    message(STATUS "Installing 'pre-commit... done")
endif()

file(MAKE_DIRECTORY ${PRE_COMMIT_HOME})
message(STATUS "Initializing 'pre-commit environment...")
execute_process(
    COMMAND bash "-c" "pre-commit install --install-hooks"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_QUIET)
message(STATUS "Initializing 'pre-commit environment... done")
message(STATUS "Installing 'pre-commit...")
