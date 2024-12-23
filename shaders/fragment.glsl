#version 330 core

out vec4 fragColour;

in vec2 texCoord;

uniform sampler2D textureMap;

void main()
{
	fragColour = texture(textureMap, texCoord);
}