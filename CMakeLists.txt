cmake_minimum_required(VERSION 3.16)
project(ShapeSimulation VERSION 1.0.0 LANGUAGES CXX)

# Enforce modern C++17 standard for coding architecture
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CRITICAL FOR QT6: Enable automatic Meta-Object Compiler
set(CMAKE_AUTOMOC ON)

# 1. FIRST: Find and load essential Qt6 components so CMake recognizes Qt commands
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml)

# 2. SECOND: Set global Qt policies now that Qt6 package is successfully loaded
qt_policy(SET QTP0001 NEW) # Enforces safe ':/qt/qml/' prefix
qt_policy(SET QTP0004 NEW) # Properly handles internal module directories

# Set up standard project layouts, paths, and targets according to Qt6 specifications
qt_standard_project_setup()

#set(PROJECT_SOURCES
 #   src/main.cpp
 #   src/shape_model.cpp
 #   src/shape_model.h
 #   src/engine.cpp
 #   src/engine.h
#)

# Define the main executable and its entry point
qt_add_executable(appShapeSimulation src/main.cpp)

# Define the QML module and register source files
qt_add_qml_module(appShapeSimulation
    URI Simulation.Backend
    VERSION 1.0
    QML_FILES
        qml/Main.qml
    SOURCES
        src/engine.cpp
        src/engine.h
        src/shape_model.cpp
        src/shape_model.h
        RESOURCES .gitignore
)

target_include_directories(appShapeSimulation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


# ==============================================================================
# IDE Project Tree Filters (Source Groups)
# ==============================================================================

# Group the main application entry point under "core"
source_group("core" FILES
    src/Main.cpp
)

# Group C++ implementation source files under "src"
source_group("src" FILES
    src/shape_model.cpp
    src/engine.cpp
)

# Group C++ declaration header files under "headers"
source_group("headers" FILES
    src/shape_model.h
    src/engine.h
)

# ==============================================================================

# Enable automatic QML type registration for the static analyzer and IDE linter
set_target_properties(appShapeSimulation PROPERTIES
    QT_QML_GENERATE_QMLTYPES ON
)

# Link essential Qt runtime libraries
target_link_libraries(appShapeSimulation PRIVATE  Qt6::Core Qt6::Gui Qt6::Qml)

if(WIN32)
    set_target_properties(appShapeSimulation PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
