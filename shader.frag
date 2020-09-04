#version 330 core
out vec4 FragColor;

uniform vec4 colorFromApplication;

void main()
{
    FragColor = colorFromApplication;
} 
