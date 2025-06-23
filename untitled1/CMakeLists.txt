cmake_minimum_required(VERSION 3.5...4.0)

project(untitled1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WARN_DEPRECATED OFF)

find_library(LIBZIP_LIBRARIES libzippp PATHS "D:/cpp_packages/libzippp/release")
include_directories("D:/cpp_packages/libzippp/release")

find_package(Qt5 COMPONENTS Widgets Multimedia MultimediaWidgets LinguistTools REQUIRED)

set(TS_FILES untitled1_zh_CN.ts)

# 匹配 source 文件夹中的所有 .cpp 文件
file(GLOB SOURCES "*.cpp")

# 匹配 header 文件夹中的所有 .h 文件
file(GLOB HEADERS "*.h")

# 匹配 UI 文件
file(GLOB UI_FILES "*.ui")

# 匹配其他文件
set(OTHER_FILES ${TS_FILES})

if(ANDROID)
    add_library(
        untitled1
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${OTHER_FILES}
    )
else()
    add_executable(
        untitled1 WIN32
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
        ${OTHER_FILES}
    )
endif()

target_include_directories(untitled1 PRIVATE header)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets ${LIBZIP_LIBRARIES})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
