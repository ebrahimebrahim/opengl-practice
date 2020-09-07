#version 330 core

// This is just a test shader. It only needs to compile correctly for tests to work.
// Does not matter what it does.

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 colorFromVertShader;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    colorFromVertShader = vec4(aColor,1.0);
}
