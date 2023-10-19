// Bloom DownSampling Shader

.STATE:
{
	Blend: Off
	StensilWrite: Off
	DepthWrite: Off
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
		vec2 u_SourceResolution;
		int u_MipLevel;
		float u_Radius;
	};

	[uniform_set]
	{
		sampler2D u_SourceTexture;
	};

	in vec2 uv;
	out vec3 fragColor;

	vec3 PowVec3(in vec3 v, in float p)
	{
		return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
	}

	const float invGamma = 1.0 / 2.2;
	vec3 ToSRGB(in vec3 v) { return PowVec3(v, invGamma); }
	
	float RGBToLuminance(in vec3 col)
	{
		return dot(col, vec3(0.2126f, 0.7152f, 0.0722f));
	}
	
	float KarisAverage(in vec3 col)
	{
		// Formula is 1 / (1 + luma)
		float luma = RGBToLuminance(ToSRGB(col)) * 0.25f;
		return 1.f / (1.f + luma);
	}

	void main()
	{
		vec2 srcTexelSize = 1.f / u_SourceResolution;
		float x = srcTexelSize.x;
		float y = srcTexelSize.y;

		// Take 13 samples around current texel:
		// a - b - c
		// - j - k -
		// d - e - f
		// - l - m -
		// g - h - i
		// === ('e' is the current texel) ===
		vec3 a = texture(u_SourceTexture, vec2(uv.x - 2 * x, uv.y + 2 * y)).rgb;
		vec3 b = texture(u_SourceTexture, vec2(uv.x,         uv.y + 2 * y)).rgb;
		vec3 c = texture(u_SourceTexture, vec2(uv.x + 2 * x, uv.y + 2 * y)).rgb;

		vec3 d = texture(u_SourceTexture, vec2(uv.x - 2 * x, uv.y)).rgb;
		vec3 e = texture(u_SourceTexture, vec2(uv.x,         uv.y)).rgb;
		vec3 f = texture(u_SourceTexture, vec2(uv.x + 2 * x, uv.y)).rgb;

		vec3 g = texture(u_SourceTexture, vec2(uv.x - 2 * x, uv.y - 2 * y)).rgb;
		vec3 h = texture(u_SourceTexture, vec2(uv.x,		 uv.y - 2 * y)).rgb;
		vec3 i = texture(u_SourceTexture, vec2(uv.x + 2 * x, uv.y - 2 * y)).rgb;

		vec3 j = texture(u_SourceTexture, vec2(uv.x - x, uv.y + y)).rgb;
		vec3 k = texture(u_SourceTexture, vec2(uv.x + x, uv.y + y)).rgb;
		vec3 l = texture(u_SourceTexture, vec2(uv.x - x, uv.y - y)).rgb;
		vec3 m = texture(u_SourceTexture, vec2(uv.x + x, uv.y - y)).rgb;

		if (u_MipLevel == 0)
		{
		    vec3 groups[5];
		
		    groups[0] = (a + b + d + e) * (0.125f / 4.f);
		    groups[1] = (b + c + e + f) * (0.125f / 4.f);
		    groups[2] = (d + e + g + h) * (0.125f / 4.f);
		    groups[3] = (e + f + h + i) * (0.125f / 4.f);
		    groups[4] = (j + k + l + m) * (0.5f / 4.f);
		    groups[0] *= KarisAverage(groups[0]);
		    groups[1] *= KarisAverage(groups[1]);
		    groups[2] *= KarisAverage(groups[2]);
		    groups[3] *= KarisAverage(groups[3]);
		    groups[4] *= KarisAverage(groups[4]);
		    fragColor = groups[0] + groups[1] + groups[2] + groups[3] + groups[4];
		}
		else
		{
		    fragColor = e * 0.125f;
		    fragColor += (a + c + g + i) * 0.03125f;
		    fragColor += (b + d + f + h) * 0.0625f;
		    fragColor += (j + k + l + m) * 0.125f;
		}
	}
}
