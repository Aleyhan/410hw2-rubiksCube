// Cube.cpp

// Cube.cpp
#include "Cube.h"

// Array of cube vertices (x, y, z coordinates for each vertex)
const GLfloat cubeVertices[] = {
    // Face +X
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,

    // Face -X
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // Face +Y
    -0.5f, 0.5f,  0.5f,
     0.5f, 0.5f,  0.5f,
     0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f,  0.5f,
     0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,

    // Face -Y
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // Face +Z
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,

    // Face -Z
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f
};

// 12 edges of the cube (pairs of vertex indices from cubeVertices)
const GLuint edgeIndices[] = {
    // edges along X-axis
    11,  0, // (-0.5, -0.5, -0.5) to ( 0.5, -0.5, -0.5)
     8,  1, // (-0.5,  0.5, -0.5) to ( 0.5,  0.5, -0.5)
     6,  5, // (-0.5, -0.5,  0.5) to ( 0.5, -0.5,  0.5)
     7,  2, // (-0.5,  0.5,  0.5) to ( 0.5,  0.5,  0.5)

    // edges along Y-axis
    11,  8, // (-0.5, -0.5, -0.5) to (-0.5,  0.5, -0.5)
     0,  1, // ( 0.5, -0.5, -0.5) to ( 0.5,  0.5, -0.5)
     6,  7, // (-0.5, -0.5,  0.5) to (-0.5,  0.5,  0.5)
     5,  2, // ( 0.5, -0.5,  0.5) to ( 0.5,  0.5,  0.5)

    // edges along Z-axis
    11,  6, // (-0.5, -0.5, -0.5) to (-0.5, -0.5,  0.5)
     0,  5, // ( 0.5, -0.5, -0.5) to ( 0.5, -0.5,  0.5)
     8,  7, // (-0.5,  0.5, -0.5) to (-0.5,  0.5,  0.5)
     1,  2  // ( 0.5,  0.5, -0.5) to ( 0.5,  0.5,  0.5)
};

static GLuint edgeEBO = 0;
static GLuint cubeVAO = 0;
static GLuint cubeVBO = 0;

// to setup cube VAO/VBO
void setupCubeVAO() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // glbufferdata is used to copy the vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // create the edge index buffer once
    glGenBuffers(1, &edgeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(edgeIndices),
                 edgeIndices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

Cube::Cube() {
    modelMatrix = mat4(1.0); // matrix identity is the default
    for (int i = 0; i < 6; ++i) {
        faceColors[i] = vec3(1.0, 1.0, 1.0); // for each face make it white
    }
}

void Cube::setPosition(const vec3& position) {
    modelMatrix = Translate(position); // translate to the position
}

void Cube::setFaceColors(const vec3 colors[6]) {
    for (int i = 0; i < 6; ++i) {
        faceColors[i] = colors[i]; // for each face set the color
    }
}

void Cube::draw(GLuint shaderProgram) const { // draw the cube with the shader program
    glBindVertexArray(cubeVAO);

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_TRUE, modelMatrix);

    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    GLuint isPickingLoc = glGetUniformLocation(shaderProgram, "isPicking");

    glUniform1i(isPickingLoc, 0);
    for (int face = 0; face < 6; ++face) { // draw each face
        glUniform3fv(colorLoc, 1, faceColors[face]); // set the color
        glDrawArrays(GL_TRIANGLES, face * 6, 6); // draw 6 vertices
    }

    // tried to assign frames but it didn't work somehow
    glUniform3fv(colorLoc, 1, vec3(0.0f));  // black
    glLineWidth(2.0f);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.0f, -1.0f);
    // draw the edges
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeEBO); // bnind the element buffer
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // draw 12 lines using 24 indices

    glDisable(GL_POLYGON_OFFSET_LINE);

    glBindVertexArray(0);
}






