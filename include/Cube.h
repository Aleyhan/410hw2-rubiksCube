#ifndef CUBE_H
#define CUBE_H

#include "../deps/Angel/inlcude/Angel/Angel.h"

class Cube {
public:
    Cube();
    int pickingID;
    void setPosition(const Angel::vec3& pos); // sets position of the cube
    void setFaceColors(const Angel::vec3 faceColors[6]); // sets face colors of the cube
    void draw(GLuint shaderProgram) const; // draws the cube
    vec3 color = vec3(1.0, 1.0, 1.0); // default white color

    Angel::vec3 position; // position of the cube
    Angel::vec3 faceColors[6]; // colors of the cube faces
    Angel::mat4 modelMatrix; // model matrix of the cube
};

void setupCubeVAO();

#endif // CUBE_H
