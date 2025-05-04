// RubiksCube.cpp

#include "RubiksCube.h"
#include "Colors.h"
#include <ctime>   // for time function

// here is the cube constructor that initializes the cube,
// sets the position and face colors of each cube
// works with each cubes id
RubiksCube::RubiksCube() {
    globalRotation = mat4(1.0);

    int idx = 0;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                vec3 position = vec3(float(x), float(y), float(z));
                cubes[idx].setPosition(position);

                vec3 faceColors[6];

                // +X face (right side)
                faceColors[0] = (x == 1) ? COLOR_RED : COLOR_BLACK;

                // -X face (left side)
                faceColors[1] = (x == -1) ? COLOR_ORANGE : COLOR_BLACK;

                // +Y face (top side)
                faceColors[2] = (y == 1) ? COLOR_WHITE : COLOR_BLACK;

                // -Y face (bottom side)
                faceColors[3] = (y == -1) ? COLOR_YELLOW : COLOR_BLACK;

                // +Z face (front side)
                faceColors[4] = (z == 1) ? COLOR_BLUE : COLOR_BLACK;

                // -Z face (back side)
                faceColors[5] = (z == -1) ? COLOR_GREEN : COLOR_BLACK;

                cubes[idx].setFaceColors(faceColors);
                cubes[idx].pickingID = idx;

                ++idx;
            }
        }
    }
}



void RubiksCube::scramble() {
    srand((unsigned int)time(0)); // seed random number generator

    const int scrambleMoves = 20; // how many random moves

    for (int i = 0; i < scrambleMoves; ++i) {
        int randomCubeID = rand() % 27; // pick random cube (0-26)

        char axes[3] = { 'X', 'Y', 'Z' };
        char randomAxis = axes[rand() % 3]; // pick random axis

        bool positive = (rand() % 2) == 0; // random direction (true = +90, false = -90)

        // immediate rotation, not smooth animation for scrambling
        float angle = positive ? 90.0f : -90.0f;

        vec3 pickedPos( // position of the picked cube is taken
            cubes[randomCubeID].modelMatrix[0][3],
            cubes[randomCubeID].modelMatrix[1][3],
            cubes[randomCubeID].modelMatrix[2][3]
        );

        for (int j = 0; j < 27; ++j) { // check all cubes
            vec3 pos(
                cubes[j].modelMatrix[0][3],
                cubes[j].modelMatrix[1][3],
                cubes[j].modelMatrix[2][3]
            );
            // check if the cube is on the same axis as the picked cube
            bool match = false;
            if (randomAxis == 'X' && abs(pos.x - pickedPos.x) < 0.01) match = true;
            if (randomAxis == 'Y' && abs(pos.y - pickedPos.y) < 0.01) match = true;
            if (randomAxis == 'Z' && abs(pos.z - pickedPos.z) < 0.01) match = true;

            if (match) { // if the cube is on the same axis, rotate it
                mat4 rot;
                if (randomAxis == 'X') rot = RotateX(angle);
                if (randomAxis == 'Y') rot = RotateY(angle);
                if (randomAxis == 'Z') rot = RotateZ(angle);
                // apply rotation to the cube
                cubes[j].modelMatrix = rot * cubes[j].modelMatrix;
            }
        }
    }

    std::cout << "Scramble complete!\n";
}

void RubiksCube::draw(GLuint shaderProgram) const { // draw the cube
    for (int i = 0; i < 27; ++i) {
        cubes[i].draw(shaderProgram);
    }
}

// update function is called every frame
void RubiksCube::update() {
    if (!isRotating) return;

    float angleStep = rotationSpeed * (rotationDirection ? 1.0f : -1.0f);

    mat4 rot; // rotation matrix
    if (rotatingAxis == 'X') rot = RotateX(angleStep);
    if (rotatingAxis == 'Y') rot = RotateY(angleStep);
    if (rotatingAxis == 'Z') rot = RotateZ(angleStep);

    for (int idx : rotatingCubes) { // rotate all cubes in the rotatingCubes vector
        cubes[idx].modelMatrix = rot * cubes[idx].modelMatrix;
    }

    rotatedAngle += rotationSpeed; // increment the rotated angle for smooth animation
    if (rotatedAngle >= 90.0f) {
        isRotating = false;
        rotatedAngle = 0.0f;
        rotatingCubes.clear();
    }
}



void RubiksCube::reset() {
    // inital solved version of the cube

    int index = 0;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) { // for each cube
                mat4 translation = Translate(float(x), float(y), float(z)); // reset position
                cubes[index].modelMatrix = translation; // reset position
                ++index;
            }
        }
    }

    std::cout << "Cube reset to solved state.\n";
}

void RubiksCube::rotateAxisLayer(char axis, int coord, bool positive) {
    if (isRotating) return;
    rotatingCubes.clear(); // clear the vector

    // find all cubes whose modelMatrix translation along current axis is equal to coord
    for (int i = 0; i < 27; ++i) { // check all cubes
        vec3 pos = vec3( // get the position of the cube
            cubes[i].modelMatrix[0][3],
            cubes[i].modelMatrix[1][3],
            cubes[i].modelMatrix[2][3]
        );
        bool match = false;
        if (axis == 'X' && abs(pos.x - coord) < 0.01) match = true;
        if (axis == 'Y' && abs(pos.y - coord) < 0.01) match = true;
        if (axis == 'Z' && abs(pos.z - coord) < 0.01) match = true;
        if (match) rotatingCubes.push_back(i); // add the cube to the vector
    }

    // start smooth rotation
    isRotating       = true;
    rotatingAxis     = axis;
    rotationDirection= positive;
    rotatedAngle     = 0.0f;
}
