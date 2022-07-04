#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
}

out vec3 normal;
out vec2 texCoord;

void main()
{
    normal = aNormal;
    texCoord = aTexCoord;

	gl_Position = u_ViewProjection * u_Model * vec4(aPos, 0.f);
}