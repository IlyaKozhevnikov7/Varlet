// Bloom UpSampling Shader

.STATE:
{
	Blend: On
	BlendFactor: One One 
	BlendOp: Add
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
	[uniform_set(fast)]
	{
		float u_FilterRadius;
	};

	[uniform_set]
	{
		sampler2D u_SourceTexture;
	};

	in vec2 uv;
	out vec3 fragColor;

	void main()
	{
		float x = u_FilterRadius;
		float y = u_FilterRadius;

		// Take 9 samples around current texel:
		// a - b - c
		// d - e - f
		// g - h - i
		// === ('e' is the current texel) ===
		vec3 a = texture(u_SourceTexture, vec2(uv.x - x, uv.y + y)).rgb;
		vec3 b = texture(u_SourceTexture, vec2(uv.x,     uv.y + y)).rgb;
		vec3 c = texture(u_SourceTexture, vec2(uv.x + x, uv.y + y)).rgb;

		vec3 d = texture(u_SourceTexture, vec2(uv.x - x, uv.y)).rgb;
		vec3 e = texture(u_SourceTexture, vec2(uv.x,     uv.y)).rgb;
		vec3 f = texture(u_SourceTexture, vec2(uv.x + x, uv.y)).rgb;

		vec3 g = texture(u_SourceTexture, vec2(uv.x - x, uv.y - y)).rgb;
		vec3 h = texture(u_SourceTexture, vec2(uv.x,     uv.y - y)).rgb;
		vec3 i = texture(u_SourceTexture, vec2(uv.x + x, uv.y - y)).rgb;
		
		fragColor = e * 4.f;
		fragColor += (b + d + f + h) * 2.f;
		fragColor += (a + c + g + i);
		fragColor *= 1.f / 16.f;
	}
}
