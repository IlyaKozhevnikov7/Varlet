// Emission Color Detect Shader

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
		gl_Position = vec4(uv * 2.f + -1.f, 0.f, 1.f);
	}
}
.FRAGMENT_STAGE:
{
	[uniform_set]
	{
		sampler2D u_SourceTexture;
	};

	in vec2 uv;

	layout (location = 0) out vec3 initialColor;
	layout (location = 1) out vec3 emissionColor;

	void main()
	{
		initialColor = texture(u_SourceTexture, uv).rgb;
		const float brightness = dot(initialColor, vec3(0.2126, 0.7152, 0.0722));

		if(brightness > 1.0)
		    emissionColor = initialColor;
		else
			emissionColor = vec3(0.f);
	}
}
