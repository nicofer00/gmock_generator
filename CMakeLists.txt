cmake_minimum_required(VERSION 3.14)
project(gmock_gen_tester)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(PROJECT_LIBRARIES test/complex_class)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/mocks/
    ${CMAKE_SOURCE_DIR}/test/support/
)

# Create a test executable
# and link it against GoogleTest and GoogleMock
enable_testing()

# Initialize an empty list
set(TEST_SOURCE_FILES "")
set(TEST_SUPPORT_FILES "")

# Recursively gather all .cc files in the current source directory
file(GLOB_RECURSE TEST_SOURCE_FILES "./test/tests/*/*.cc")

# gather support fake source files from support directory
file(GLOB_RECURSE TEST_SUPPORT_FILES "./test/support/*.cpp")

# compile all declared mocks assumes python is installed and on PATH
message(STATUS "Generating GMocks...")

# Use virtual environment Python if available, otherwise fall back to system Python
if(DEFINED PYTHON)
    set(PYTHON_CMD ${PYTHON})
else()
    if(UNIX)
        set(PYTHON_CMD python3)
    else()
        set(PYTHON_CMD python)
    endif()
endif()

message(STATUS "Using Python: ${PYTHON_CMD}")

execute_process(COMMAND ${PYTHON_CMD} ${CMAKE_SOURCE_DIR}/scripts/compile_mocks.py
    ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/mocks/ "${TEST_SOURCE_FILES}"
    OUTPUT_VARIABLE PYTHON_OUTPUT
    RESULT_VARIABLE PYTHON_RESULT)

# Check if the Generator script executed successfully
if (PYTHON_RESULT EQUAL 0)
    message(STATUS "Generator script executed successfully. Output: \n${PYTHON_OUTPUT}")
else()
    message(FATAL_ERROR "Generator script failed to execute. Output: ${PYTHON_OUTPUT}")
endif()

add_executable(
  ${PROJECT_NAME} 
  ${TEST_SOURCE_FILES} 
  ${TEST_SUPPORT_FILES}
)

set(LINKED_LIBS "")
list(APPEND LINKED_LIBS 
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    COMPLEX_CLASS_LIB
)

foreach(LIBRARY ${PROJECT_LIBRARIES})
    add_subdirectory(${CMAKE_SOURCE_DIR}/${LIBRARY}
        ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY})
endforeach()
target_link_libraries(${PROJECT_NAME}
    ${LINKED_LIBS}
    )

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})