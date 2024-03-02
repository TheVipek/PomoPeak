cmake_minimum_required(VERSION 3.5)

project(PomoPeak VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)

set(PROJECT_SOURCES
        main.cpp
        pomopeak.cpp
        pomopeak.h
        pomopeak.ui
        taskQT.ui
        taskQT.h taskQT.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PomoPeak
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        FlowHandler.h flowhandler.cpp
        settings.h settings.cpp
        taskmanager.h taskmanager.cpp
        task.h
        task.cpp
        TaskInputFilter.h

        pomopeaksettings.h pomopeaksettings.cpp pomopeaksettings.ui
        sqliteHandler.h sqliteHandler.cpp
        databasetables.h databasetables.cpp
        settingsdto.h settingsdto.cpp



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PomoPeak APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PomoPeak SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PomoPeak
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PomoPeak PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PomoPeak PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(PomoPeak PRIVATE Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PomoPeak)
endif()
set_target_properties(PomoPeak PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PomoPeak
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PomoPeak)
endif()

set(DATABASE_FILE "${CMAKE_SOURCE_DIR}/database/applicationData.sqlite")
set(BUILD_DATABASE_FILE "${CMAKE_BINARY_DIR}/database/applicationData.sqlite")

add_custom_command(OUTPUT ${BUILD_DATABASE_FILE}
                   COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/database"
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DATABASE_FILE} ${BUILD_DATABASE_FILE}
                   DEPENDS ${DATABASE_FILE})

add_custom_target(CopySQLite ALL
                  DEPENDS ${BUILD_DATABASE_FILE})

add_dependencies(CopySQLite PomoPeak)
