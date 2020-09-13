#version 330 core

// A shader used for testing code, not for actual drawing

layout (location = 0) in vec3 aPos;

out vec4 colorFromVertShader;

uniform float test_uniform_1f;
uniform vec4 test_uniform_4f;
uniform mat4 test_uniform_mat4;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    colorFromVertShader = vec4(0.5,0.5,0.65,1.0);
    gl_Position.x += test_uniform_1f; // Need to use it so compiler doesn't optimize it away
    gl_Position.x += test_uniform_4f.x;
    gl_Position = test_uniform_mat4 * gl_Position;
}
