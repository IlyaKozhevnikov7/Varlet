#version 460

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
};

uniform uint u_EntityId;

out vec4 fragColor;

void main()
{
    fragColor = vec4(
            (u_EntityId & 0x000000ff) >> 0, 
            (u_EntityId & 0x0000ff00) >> 8,
            (u_EntityId & 0x00ff0000) >> 16,
            (u_EntityId & 0xff000000) >> 24) / 255.f;
}