cmake_minimum_required(VERSION 3.16)
project(RubiksCube LANGUAGES C CXX OBJC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
        GLEW_STATIC
        GL_SILENCE_DEPRECATION
)

# for the headers, glew and Angel and glfw
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/deps/glew/include
        ${CMAKE_SOURCE_DIR}/deps/Angel/include
)

# Build platform libraries
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/glfw)

# Build GLEW as a static library
add_library(glew STATIC
        ${CMAKE_SOURCE_DIR}/deps/glew/src/glew.c
)

target_include_directories(glew PUBLIC
        ${CMAKE_SOURCE_DIR}/deps/glew/include
)

# build Angel as a static library
add_library(angel_shaders STATIC
        ${CMAKE_SOURCE_DIR}/deps/Angel/src/InitShader.cpp
)
target_include_directories(angel_shaders PUBLIC
        ${CMAKE_SOURCE_DIR}/deps/Angel/include
)

#  my own executable files
add_executable(RubiksCube
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/Cube.cpp
        ${CMAKE_SOURCE_DIR}/src/RubiksCube.cpp
        ${CMAKE_SOURCE_DIR}/src/Controls.cpp
        ${CMAKE_SOURCE_DIR}/src/Globals.cpp
)

# link the libraries to the executable
target_link_libraries(RubiksCube PRIVATE
        glfw
        glew
        angel_shaders
        ${PLATFORM_LIBS}
)

target_link_libraries(RubiksCube PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework OpenGL"
        "-framework GLUT"
)

# Copy all shaders into the build directory before each build
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*")
foreach(SHADER ${SHADER_FILES})
    get_filename_component(NAME ${SHADER} NAME)
    add_custom_command(
            TARGET RubiksCube PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SHADER}"
            "$<TARGET_FILE_DIR:RubiksCube>/${NAME}"
            COMMENT "Copying shader ${NAME}"
    )
endforeach()

# set the output directory for the executable
set_target_properties(RubiksCube PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
