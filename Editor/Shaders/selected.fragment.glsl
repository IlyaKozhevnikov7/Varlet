#version 460

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
    int u_renderId;
};

out vec4 fragColor;

void main()
{
    const vec3 color = vec3(
        (u_renderId & 0x000000FF) >> 0,
        (u_renderId & 0x0000FF00) >> 8,
        (u_renderId & 0x00FF0000) >> 16) / 255.f;

    fragColor = vec4(color, 0.f);
}