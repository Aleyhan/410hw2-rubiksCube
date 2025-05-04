// Controls.cpp

#include "Controls.h"
#include "Globals.h"
#include <iostream>
static int selectedLayer = 0;   // -1,0,+1

void setupControls(GLFWwindow* window) { // setting up the controls
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

// here we define the key callback function for all the keys
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_H:
                std::cout << "Controls:\n";
            std::cout << "- Arrow keys: Rotate view\n";
            std::cout << "- R: Reset cube\n";
            std::cout << "- S: Scramble cube\n";
            std::cout << "- Left click: Turn\n";
            std::cout << "- Right click: Turn back\n";
            std::cout << "- X/Y/Z: select rotation axis\n";
            std::cout << "- 1/2/3: select layer (-1/0/+1) on that axis\n";
            std::cout << "- + / - : rotate selected layer +90° / -90°\n";
            std::cout << "- H: Show this help\n";
            std::cout << "- ESC: Exit\n";

            break;
            case GLFW_KEY_LEFT:
                cube.globalRotation = RotateY(5.0) * cube.globalRotation;
            break;
            case GLFW_KEY_RIGHT:
                cube.globalRotation = RotateY(-5.0) * cube.globalRotation;
            break;
            case GLFW_KEY_UP:
                cube.globalRotation = RotateX(5.0) * cube.globalRotation;
            break;
            case GLFW_KEY_DOWN:
                cube.globalRotation = RotateX(-5.0) * cube.globalRotation;
            case GLFW_KEY_X:
                currentAxis = 'X';
            std::cout << "Axis set to X\n";
            break;
            case GLFW_KEY_Y:
                currentAxis = 'Y';
            std::cout << "Axis set to Y\n";
            break;
            case GLFW_KEY_Z:
                currentAxis = 'Z';
            std::cout << "Axis set to Z\n";
            break;
            case GLFW_KEY_R:
                cube.reset();
            break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
            case GLFW_KEY_1:
                selectedLayer = -1;
            std::cout << "Selected layer " << selectedLayer << " on axis " << currentAxis << "\n";
            break;
            case GLFW_KEY_2:
                selectedLayer =  0;
            std::cout << "Selected layer " << selectedLayer << " on axis " << currentAxis << "\n";
            break;
            case GLFW_KEY_3:
                selectedLayer = +1;
            std::cout << "Selected layer " << selectedLayer << " on axis " << currentAxis << "\n";
            break;
            case GLFW_KEY_S:
                cube.scramble();
            std::cout << "Cube face‐colors randomized!\n";
            break;

            // + rotates positively 90
            case GLFW_KEY_KP_ADD:
            case GLFW_KEY_EQUAL:  // also allow '=/+' key
                cube.rotateAxisLayer(currentAxis, selectedLayer, true);
            break;
            // - rotates negatively 90
            case GLFW_KEY_KP_SUBTRACT:
            case GLFW_KEY_MINUS:
                cube.rotateAxisLayer(currentAxis, selectedLayer, false);
            break;
            default:
                break;


        }
    }
}

// mouse button callback function
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action != GLFW_PRESS) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        // same as +
        cube.rotateAxisLayer(currentAxis, selectedLayer, true);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        // same as –
        cube.rotateAxisLayer(currentAxis, selectedLayer, false);
    }
}
