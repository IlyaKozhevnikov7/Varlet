#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTangent;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
    int u_renderId;
};

out vec2 uv;
out vec3 fragPos;
out mat3 tbn;

void main()
{
    uv = aUV;
    fragPos = vec3(u_Model * vec4(aPos, 1.f));

    const vec3 bitangent = cross(aNormal, aTangent);
    vec3 T = normalize(vec3(u_Model * vec4(aTangent, 0.f)));
    const vec3 N = normalize(vec3(u_Model * vec4(aNormal, 0.f)));
    T = normalize(T - dot(T, N) * N);
    const vec3 B = cross(N, T);
    tbn = mat3(T, B, N);

	gl_Position = u_ProjectionView * u_Model * vec4(aPos, 1.f);
}
