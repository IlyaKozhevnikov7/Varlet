#pragma once

#include "VarletCore.h"
#include "RenderingCore.h"

namespace Varlet
{
	class OpenGLShader final : public Shader
	{
		enum ShaderType : uint32_t
		{
			Vertex = 0x8B31,
			Fragment = 0x8B30,
			Geometry = 0x8DD9
		};

		enum ObjectType : uint32_t
		{
			VertexShader = 0,
			FragmentShader,
			GeomertyShader,
			Program
		};

	private:

		uint32_t _id;

	public:

		OpenGLShader();

		~OpenGLShader() override;

		virtual void Use() override;

	private:

		void Compile(const uint32_t& objId, ObjectType&& objType);
	};
}
