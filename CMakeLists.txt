cmake_minimum_required(VERSION 3.28)
project(HARDQT)

set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
qt_standard_project_setup()

find_package(qt6advanceddocking REQUIRED)


qt_add_executable(HARDQT
        main.cpp
        mainwindow.hpp
        mainwindow.cpp
        datamodel.hpp
        datamodel.cpp)

target_include_directories(HARDQT PRIVATE include)

target_link_libraries(HARDQT PRIVATE
        Qt6::Widgets
        Qt6::Sql
        ads::qt6advanceddocking
        -static
)
