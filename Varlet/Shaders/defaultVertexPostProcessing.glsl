#version 460

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main()
{
	texCoord = aTexCoord;
	
	gl_Position = vec4(aPos, 0.f, 1.f);
}