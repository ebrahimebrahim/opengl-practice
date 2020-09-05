#version 330 core
out vec4 FragColor;

in vec4 colorFromVertShader;

uniform vec4 colorFromApplication;

void main()
{
    FragColor = colorFromVertShader;
    FragColor.g *= colorFromApplication.g;
} 
