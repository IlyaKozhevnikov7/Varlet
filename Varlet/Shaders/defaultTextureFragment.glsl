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

uniform sampler2D u_MainTexture;

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

out vec4 fragColor;

void main()
{
	fragColor = texture(u_MainTexture, texCoord);
}