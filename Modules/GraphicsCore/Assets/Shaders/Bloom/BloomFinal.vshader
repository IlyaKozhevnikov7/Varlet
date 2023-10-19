// Bloom Final Shader

.STATE:
{
	Blend: Off
	DepthWrite: Off
	StensilWrite: Off
}
.VERTEX_STAGE:
{
	out vec2 uv;

	out gl_PerVertex
	{
		vec4 gl_Position;
		float gl_PointSize;
		float gl_ClipDistance[];
	};

	void main()
	{
		uv = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
		gl_Position = vec4(uv * 2.0f + -1.0f, 0.0f, 1.0f);
	}
}
.FRAGMENT_STAGE:
{
	[uniform_set(fast)]
	{
		float exposure;
		float bloomStrength;
	};

	[uniform_set]
	{
		sampler2D u_SceneTexture;
		sampler2D u_BloomBlur;
	};

	in vec2 uv;
	out vec3 fragColor;

	void main()
	{
		vec3 hdrColor = texture(u_SceneTexture, uv).rgb;
		vec3 bloomColor = texture(u_BloomBlur, uv).rgb;
		vec3 result = mix(hdrColor, bloomColor, bloomStrength); // linear interpolation

		// tone mapping
		result = vec3(1.0) - exp(-result * exposure);

		// also gamma correct while we're at it
		const float gamma = 2.2;
		result = pow(result, vec3(1.0 / gamma));
		fragColor = result;
	}
}
