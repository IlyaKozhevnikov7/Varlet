#version 460

in vec3 normal;
in vec2 texCoord;

uniform sampler2D u_Deffuse;

out vec4 fragColor;

void main()
{
	fragColor = texture(u_Deffuse, texCoord);
}