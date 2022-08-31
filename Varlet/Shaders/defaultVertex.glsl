#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
    int u_renderId;
};

out vec3 normal;
out vec2 texCoord;
out vec3 fragPos;

void main()
{
    normal = mat3(u_Model) * aNormal;
    texCoord = aTexCoord;
    fragPos = vec3(u_Model * vec4(aPos, 1.f));

	gl_Position = u_ProjectionView * u_Model * vec4(aPos, 1.f);
}