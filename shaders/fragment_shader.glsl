#version 330 core

uniform vec3  objectColor;   // normal visible colour
uniform vec3  pickingColor;  // picking colour
uniform bool  isPicking;
flat in vec3 vColor;        // flat so every fragment in the triangle


out vec4 FragColor; // output to framebuffer

void main()
{
    if (isPicking)
    FragColor = vec4(pickingColor, 1.0);   // picking render
    else
    FragColor = vec4(objectColor, 1.0);    // normal render
}
