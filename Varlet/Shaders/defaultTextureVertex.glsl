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
};

uniform bool u_FaceToCamera;

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

    if (u_FaceToCamera)
    {
        mat4 finalMatrix = u_ProjectionView * u_Model;

        const float d = sqrt(u_Model[0][0] * u_Model[0][0]
                           + u_Model[1][1] * u_Model[1][1]
                           + u_Model[2][2] * u_Model[2][2]);
        
        finalMatrix[0] = vec4(d,  0.f,    0.f,  u_Model[0][3]);
        finalMatrix[1] = vec4(0.f,  d,    0.f,  u_Model[1][3]);
        finalMatrix[2] = vec4(0.f,  0.f,    d,  u_Model[2][3]);

        gl_Position = finalMatrix * vec4(aPos, 1.f);
    }
    else
    {
        gl_Position = u_ProjectionView * u_Model * vec4(aPos, 1.f);
    }
}