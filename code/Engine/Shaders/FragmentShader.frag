#version 450

// location = 0 is for the frame buffer at index 0
layout(location = 0) out vec4 outColor;

// get the input from Vertex shader color here 
layout(location = 0) in vec3 fragColor;

void main() 
{
    outColor = vec4(fragColor, 1.0);
}