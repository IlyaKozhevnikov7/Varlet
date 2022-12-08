#version 460

layout(std140, binding = 0) uniform Camera
{
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
};

uniform sampler2D u_MainTexture;

in vec2 uv;
in vec3 fragPos;
in mat3 tbn;

out vec4 fragColor;

void main()
{
    fragColor = texture(u_MainTexture, uv);

    if (fragColor.a < 0.01f)
        discard;
}