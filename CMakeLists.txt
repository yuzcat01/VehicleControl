cmake_minimum_required(VERSION 3.16)

project(VehicleControl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)
find_package(Qt6 REQUIRED COMPONENTS Core Gui)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebSockets)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VehicleControl
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        View/Home/home.h View/Home/home.cpp View/Home/home.ui
        View/Weather/weather.h View/Weather/weather.cpp View/Weather/weather.ui
        README.md
        Resource.qrc
        View/Music/musicpage.h View/Music/musicpage.cpp View/Music/musicpage.ui
        View/Map/map.h View/Map/map.cpp View/Map/map.ui
        View/Communicate/communicate.h View/Communicate/communicate.cpp View/Communicate/communicate.ui
        View/Vedio/vedio.h View/Vedio/vedio.cpp View/Vedio/vedio.ui
        View/Picture/picture.h View/Picture/picture.cpp View/Picture/picture.ui

        View/Map/canvas.h View/Map/canvas.cpp View/Map/canvas.ui
        View/Weather/weatherdata.h
        View/Weather/weathertool.h
        View/Picture/customlabel.h View/Picture/customlabel.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET VehicleControl APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(VehicleControl SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(VehicleControl
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(VehicleControl PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(VehicleControl PRIVATE Qt6::Network)
target_link_libraries(VehicleControl PRIVATE Qt6::Multimedia)
target_link_libraries(VehicleControl PRIVATE Qt6::MultimediaWidgets)
target_link_libraries(VehicleControl PRIVATE Qt6::Core Qt6::Gui)
target_link_libraries(VehicleControl PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::WebSockets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VehicleControl)
endif()
set_target_properties(VehicleControl PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VehicleControl
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VehicleControl)
endif()
