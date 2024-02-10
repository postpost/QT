cmake_minimum_required(VERSION 3.5)

project(AirportInspectorApplication VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 COMPONENTS Widgets Concurrent Sql PrintSupport REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        GraphManager.h
        GraphManager.cpp
        DataBaseController.h
        DataBaseController.cpp
)

add_executable(AirportInspectorApplication
      ${PROJECT_SOURCES}
)

#adding dynamic libs of QCustomPlot
add_compile_definitions(QCUSTOMPLOT_USE_LIBRARY)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
target_link_libraries(AirportInspectorApplication PRIVATE Qt6::Widgets
                                                          Qt6::Concurrent
                                                          Qt6::Sql
                                                          Qt6::PrintSupport
                                                          libcrypto-3-x64
                                                          libiconv-2
                                                          libintl-9
                                                          libpq
                                                          libssl-3-x64
                                                          qcustomplot2.dll)


qt_finalize_executable(AirportInspectorApplication)

