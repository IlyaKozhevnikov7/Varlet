#version 460

// Macros

#define MAX_POINTS_LIGHT_AMOUNT 10

// Types

struct PointLight
{
    vec4 color;
    vec4 position;
    float linearAttenuation;
    float quadraticAttenuation;
    bool isActive;
};

// Buffers

layout(std140, binding = 0) uniform Camera
{
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_ProjectionView;
    mat4 u_Model;
    vec3 u_CameraPosition;
};

layout(std140, binding = 1) uniform Illumination
{
    PointLight u_pointsLight[MAX_POINTS_LIGHT_AMOUNT];
};