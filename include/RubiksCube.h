// RubiksCube.h

#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H

#include "Cube.h"

class RubiksCube {
public:
    mat4 globalRotation;
    Cube cubes[27];

    // For smooth rotation
    bool isRotating = false;
    char rotatingAxis;
    float rotationSpeed = 3.0f; // degrees per frame
    float rotatedAngle = 0.0f;
    bool rotationDirection = true; // true = +90, false = -90
    //end of rotation variables

    std::vector<int> rotatingCubes;
    void scramble(); // randomize cube face-colors

    RubiksCube();
    void draw(GLuint shaderProgram) const; // draws the cube
    void rotateAxisLayer(char axis, int coord, bool positive); // rotate a layer along an axis

    void update(); // update rotation
    void reset(); // reset cube to solved state

};


#endif
