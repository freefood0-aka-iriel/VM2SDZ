cmake_minimum_required(VERSION 4.0)

project(untitled1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WARN_DEPRECATED OFF)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets Multimedia MultimediaWidgets LinguistTools REQUIRED)

set(TS_FILES untitled1_zh_CN.ts)

if(ANDROID)
  add_library(untitled1 SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    mainwindow_method.cpp
    mainwindow_slots.cpp
    shidunzi.cpp
    shidunzi.h
    shidunzistack.cpp
    shidunzistack.h
    settingwindow.cpp
    settingwindow.h
    settingwindow.ui
    addshidunzidialog.cpp
    addshidunzidialog.h
    addshidunzidialog.ui
    ${TS_FILES}
  )
else()
  add_executable(untitled1
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    mainwindow_method.cpp
    mainwindow_slots.cpp
    shidunzi.cpp
    shidunzi.h
    shidunzistack.cpp
    shidunzistack.h
    settingwindow.cpp
    settingwindow.h
    settingwindow.ui
    addshidunzidialog.cpp
    addshidunzidialog.h
    addshidunzidialog.ui
    ${TS_FILES}
  )
endif()

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} )
