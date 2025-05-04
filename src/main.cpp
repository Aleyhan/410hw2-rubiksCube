// main.cpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../deps/Angel/inlcude/Angel/Angel.h"

#include "RubiksCube.h"
#include "Controls.h"
#include "Globals.h"
#include "Cube.h"
#include "../deps/Angel/src/InitShader.cpp"

// it is used to set the screen width and height
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Rubik's Cube", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Set viewport
    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Compile and link shaders
    shaderProgram = InitShader("../shaders/vertex_shader.glsl" , "../shaders/fragment_shader.glsl");
    glUseProgram(shaderProgram);

    // Set up controls such as keyboard and mouse callbacks
    setupControls(window);

    // load the shaders
    shaderProgram = InitShader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
    glUseProgram(shaderProgram);

    // setup the VAO
    setupCubeVAO();

    // setting the control and then initializing the cube
    setupControls(window);
    cube = RubiksCube();

    // Main loop for rendering
    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.update(); // update for any cases

        // Set viewProjection matrix once per frame
        mat4 ortho = Ortho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
        mat4 viewProjection = ortho * cube.globalRotation;

        GLuint viewProjLoc = glGetUniformLocation(shaderProgram, "viewProjection");
        glUniformMatrix4fv(viewProjLoc, 1, GL_TRUE, viewProjection);

        cube.draw(shaderProgram);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// needed for the framebuffer size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
}
