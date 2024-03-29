cmake_minimum_required(VERSION 3.5)

project(Vtm_sheet_android VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        attributes.h
        attributes.cpp
        attributes.ui
        diceroller.h
        diceroller.cpp
        diceroller.ui
        personaldata.h
        personaldata.cpp
        personaldata.ui
        resources.qrc
        indicators.h
        indicators.cpp
        indicators.ui
        disciplines.h
        disciplines.cpp
        disciplines.ui
        loresheets.h
        loresheets.cpp
        loresheets.ui
        skills.h
        skills.cpp
        skills.ui
        options.h
        options.cpp
        options.ui
        advantagesanddisadvantages.h
        advantagesanddisadvantages.cpp
        advantagesanddisadvantages.ui
        clans.h
        clans.cpp
        clans.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Vtm_sheet_android
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
    set_property(TARGET Vtm_sheet_android APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Vtm_sheet_android SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Vtm_sheet_android
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Vtm_sheet_android PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::CorePrivate)


set_target_properties(Vtm_sheet_android PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Vtm_sheet_android
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Vtm_sheet_android)
endif()
