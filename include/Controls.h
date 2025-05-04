// Controls.h

#ifndef CONTROLS_H
#define CONTROLS_H

#include <GL/glew.h>
#include "../deps/Angel/inlcude/Angel/Angel.h"
#include <GLFW/glfw3.h>

// lets initialize the controls
void setupControls(GLFWwindow* window);

//  lets add callbacks
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

#endif // CONTROLS_H
