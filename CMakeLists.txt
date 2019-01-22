cmake_minimum_required(VERSION 3.10)

project(QT_MX)

ADD_SUBDIRECTORY(MXSchedule)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-DUSE_QT=1")

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_INCLUDE_CURRENT_DIR  ON)

# qt library
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
# opengl library
find_package(OpenGL REQUIRED)

# find source
file (GLOB_RECURSE SOURCE "UI/*.cpp" "UI/*.c")

# include header
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/UI)
include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/MXSchedule/Inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/MXSchedule/Interface)

# binary target
add_executable(QT_MX ${SOURCE})

# add library
target_link_libraries(QT_MX MXSchedule)
target_link_libraries(QT_MX ${OPENGL_LIBRARIES})
qt5_use_modules(QT_MX Core Gui Multimedia Widgets)
