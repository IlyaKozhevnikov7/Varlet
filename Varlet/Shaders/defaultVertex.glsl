#version 460

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aTangent;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
};

out vec2 uv;
out vec3 fragPos;
out mat3 tbn;

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
};

mat3 CalculateTBN(in mat4 model, in vec3 normal, in vec3 tangent)
{
    const vec3 bitangent = cross(normal, tangent);
    vec3 T = normalize(vec3(model * vec4(tangent, 0.f)));
    const vec3 N = normalize(vec3(model * vec4(normal, 0.f)));
    T = normalize(T - dot(T, N) * N);
    const vec3 B = cross(N, T);
    return mat3(T, B, N);
}

void main()
{
    uv = aUV;
    fragPos = vec3(u_Model * vec4(aPos, 1.f));

    tbn = CalculateTBN(u_Model, aNormal, aTangent);

	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.f);
}
