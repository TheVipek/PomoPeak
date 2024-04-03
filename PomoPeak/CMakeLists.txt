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
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts)
set(PROJECT_SOURCES
        main.cpp

        ui/pomopeak.h
        ui/pomopeak.cpp
        ui/pomopeak.ui

        ui/taskQT.h
        ui/taskQT.cpp
        ui/taskQT.ui

        ui/pomopeakstats.h
        ui/pomopeakstats.cpp
        ui/pomopeakstats.ui

        ui/pomopeaksettings.h
        ui/pomopeaksettings.cpp
        ui/pomopeaksettings.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PomoPeak
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        data/Resources.qrc

        include/FlowHandler.h
        src/flowhandler.cpp

        include/settings.h
        src/settings.cpp

        include/taskmanager.h
        src/taskmanager.cpp

        include/task.h
        src/task.cpp

        include/TaskInputFilter.h

        include/sqliteHandler.h
        src/sqliteHandler.cpp

        include/databasetables.h
        src/databasetables.cpp

        include/settingsdto.h
        src/settingsdto.cpp

        include/audiomimetypeshelper.h
        src/audiomimetypeshelper.cpp

        include/trayiconhandler.h
        src/trayiconhandler.cpp

        include/Skin.h
        src/Skin.cpp

        include/userstats.h
        src/userstats.cpp

        include/daytaskstats.h
        src/daytaskstats.cpp

        include/userstatsdto.h
        src/userstatsdto.cpp
        include/QObjectInitialization.h
        include/GPTHelper.h
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

target_include_directories(PomoPeak PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(PomoPeak PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ui)
target_link_libraries(PomoPeak PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PomoPeak PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(PomoPeak PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(PomoPeak PRIVATE Qt${QT_VERSION_MAJOR}::Charts)

target_include_directories(PomoPeak PRIVATE ${CMAKE_SOURCE_DIR}/libs/nlohmann/include)
#target_link_libraries(PomoPeak PRIVATE json)

target_include_directories(PomoPeak PRIVATE ${CMAKE_SOURCE_DIR}/libs/curl/include)
#target_link_libraries(PomoPeak PRIVATE curl)

target_include_directories(PomoPeak PRIVATE ${CMAKE_SOURCE_DIR}/libs/liboai)
#target_link_libraries(PomoPeak PRIVATE liboai)

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



set(START_ALARM_SOUNDS_DIRECTORY "${CMAKE_SOURCE_DIR}/data/sounds/sessionAlarms")
set(BUILD_START_ALARM_SOUNDS_DIRECTORY "${CMAKE_BINARY_DIR}/data/sounds/sessionAlarms")

set(BREAK_ALARM_SOUNDS_DIRECTORY "${CMAKE_SOURCE_DIR}/data/sounds/breakAlarms")
set(BUILD_BREAK_ALARM_SOUNDS_DIRECTORY "${CMAKE_BINARY_DIR}/data/sounds/breakAlarms")

set(TEMP_DIR "${CMAKE_SOURCE_DIR}/data/temp")
set(BUILD_TEMP_DIR "${CMAKE_BINARY_DIR}/data/temp")

# Create a custom target that always runs
add_custom_target(CopyAlarmSounds ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${BUILD_START_ALARM_SOUNDS_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${START_ALARM_SOUNDS_DIRECTORY}" "${BUILD_START_ALARM_SOUNDS_DIRECTORY}"
    COMMENT "Copying sound files to build directory")
add_custom_target(CopyBreakSounds ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${BUILD_BREAK_ALARM_SOUNDS_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${BREAK_ALARM_SOUNDS_DIRECTORY}" "${BUILD_BREAK_ALARM_SOUNDS_DIRECTORY}"
    COMMENT "Copying sound files to build directory")

add_custom_target(TempFiles ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${BUILD_TEMP_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${TEMP_DIR}" "${BUILD_TEMP_DIR}"
    COMMENT "Copying sound files to build directory")
# Ensure your main target depends on this custom target
add_dependencies(PomoPeak CopyAlarmSounds)
add_dependencies(PomoPeak CopyBreakSounds)
add_dependencies(PomoPeak TempFiles)
set(RENEW_FILES FALSE CACHE BOOL "Flag to indicate if the build is for production")

if(RENEW_FILES)
     message("RENEWING FILES")

    set(DATABASE_FILE "${CMAKE_SOURCE_DIR}/data/database/applicationData.sqlite")
    set(BUILD_DATABASE_FILE "${CMAKE_BINARY_DIR}/data/database/applicationData.sqlite")

    add_custom_target(CopyDatabase ALL
                       COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/data/database"
                       COMMAND ${CMAKE_COMMAND} -E copy ${DATABASE_FILE} ${BUILD_DATABASE_FILE}
                       COMMENT "Copying database to build directory")

    add_dependencies(CopyDatabase PomoPeak)
endif()
