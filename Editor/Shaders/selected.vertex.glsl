#version 460

layout(location = 0) in vec3 aPos;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
    int u_renderId;
};

void main()
{
	gl_Position = u_ProjectionView * u_Model * vec4(aPos, 1.f);
}