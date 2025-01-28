cmake_minimum_required(VERSION 3.12)
project(CalendarTime_Test C)

# This CMakeLists.txt is for testing purposes only.
add_compile_definitions(TESTING)

# Collect all source files
set(SOURCES
CalendarTime.c)
add_executable(CalendarTime_Test ${SOURCES} CalendarTime_Test.c)

# Setup testing
enable_testing()
include(CTest)

add_test(
    NAME CalendarTime_UnitTest
    COMMAND CalendarTime_Test
)
