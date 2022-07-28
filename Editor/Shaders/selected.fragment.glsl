#version 460

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
    mat4 u_Model;
    int u_renderId;
};

out vec4 fragColor;

void main()
{
    const int r = (u_renderId & 0x000000FF) >> 0;
    const int g = (u_renderId & 0x0000FF00) >> 8;
    const int b = (u_renderId & 0x00FF0000) >> 16;

    fragColor = vec4(
    r / 255.f,
    g / 255.f,
    b / 255.f,
    0);
}