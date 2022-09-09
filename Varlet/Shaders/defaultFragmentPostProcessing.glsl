#version 460

uniform sampler2D u_MainTexture;
uniform float u_Gamma;

in vec2 texCoord;

out vec4 fragColor;

void main()
{
	vec4 color = texture(u_MainTexture, texCoord);

	fragColor = vec4(pow(color.rgb, vec3(1.f / u_Gamma)), color.a);
}