#version 460

in vec3 normal;
in vec2 texCoord;

uniform vec3 u_Color;

out vec4 fragColor;

void main()
{
	fragColor = vec4(u_Color, 1.f);
}