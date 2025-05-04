#version 330 core

layout(location = 0) in vec3 vPosition;

uniform mat4 model;
uniform mat4 viewProjection;
flat out vec3 vColor;

void main()
{
    vColor = vec3(1.0);
    gl_Position = viewProjection * model * vec4(vPosition, 1.0); // transforms the vertex position
}
