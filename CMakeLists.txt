cmake_minimum_required(VERSION 3.0.2)


set(PROJECT_NAME onlinePlotter)
project(${PROJECT_NAME})

#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\Qt\\Qt5.4.1\\5.4\\mingw491_32")
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\boost\\")


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_SHARED_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS "-std=c++11")
if (UNIX)
    set( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
endif (UNIX)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)


include_directories(sharedMemory)
add_subdirectory(sharedMemory)


include_directories(qCustomPlot)
add_subdirectory(qCustomPlot)


add_library(mainwindow mainwindow.cpp)
target_link_libraries(mainwindow sharedMemory)
qt5_use_modules(mainwindow Widgets PrintSupport)




add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} mainwindow qCustomPlot)
qt5_use_modules(${PROJECT_NAME} Core Gui)





set(PROJECT_NAME testSender)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} testSender.cpp)
target_link_libraries (${PROJECT_NAME} sharedMemory)