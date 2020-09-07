#version 330 core

// A shader used for testing code, not for actual drawing

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 colorFromVertShader;

uniform float test_uniform_1f;
uniform vec4 test_uniform_4f;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    colorFromVertShader = vec4(aColor,1.0);
    gl_Position.x += test_uniform_1f; // Need to use it so compiler doesn't optimize it away
    gl_Position.x += test_uniform_4f.x;
}
