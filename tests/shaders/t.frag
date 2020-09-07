#version 330 core

// This is just a test shader. It only needs to compile correctly for tests to work.
// Does not matter what it does.

out vec4 FragColor;

in vec4 colorFromVertShader;

uniform vec4 colorFromApplication;

void main()
{
    FragColor = colorFromVertShader;
    FragColor.g *= colorFromApplication.g;
}
