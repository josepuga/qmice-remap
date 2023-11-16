cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

if(QT_VERSION_MAJOR LESS 6)
    message(FATAL_ERROR "Qt6 Required.")
endif()

# Versioning
# Create a file named. version.h.in with those lines:
# #define APP_NAME "@PROJECT_NAME@"
# #define VERSION_STRING "@VERSION_STRING@"
set(VERSION_STRING "0.1.0")
set(APP_NAME "QMice Remap")

project(
    qmice-remap
    VERSION ${VERSION_STRING}
    LANGUAGES CXX
)

# Sets compiler, flags, built type and so on...
include(josepuga.cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

find_package(fmt CONFIG REQUIRED)

set(TS_FILES ${PROJECT_NAME}/src/_es_ES.ts)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/include
)


file (GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/include/*.h"
    "${PROJECT_SOURCE_DIR}/src/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    ${TS_FILES}
)

configure_file(${PROJECT_SOURCE_DIR}/version.h.in ${PROJECT_SOURCE_DIR}/src/include/version.hpp)

# Resource icon Does *NOT* work.
qt6_add_resources(RESOURCES "${PROJECT_SOURCE_DIR}/src/resources/resources.qrc")

qt6_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${RESOURCES}
)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt-header-only
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(${PROJECT_NAME})
