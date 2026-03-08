cmake_minimum_required(VERSION 3.10)

project(TerminalTimer C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ttimer ttimer.c)

if (WIN32)
    target_link_libraries(ttimer pdcurses winmm ole32)
else ()
    # for Linux/MacOS
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIRS})

    target_link_libraries(ttimer ${CURSES_LIBRARIES} m pthread dl)
endif()