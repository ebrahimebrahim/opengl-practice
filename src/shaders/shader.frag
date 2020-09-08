#version 330 core
out vec4 FragColor;

in vec4 colorFromVertShader;
in vec2 texCoord;

uniform vec4 colorFromApplication;
uniform sampler2D currentTexture;

void main()
{
    FragColor = texture(currentTexture, texCoord);
    FragColor *= colorFromVertShader;
    FragColor.g *= colorFromApplication.g;
}
