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

		enum class ObjectType : uint8_t
		{
			VertexShader = 0,
			FragmentShader,
			GeomertyShader,
			Program
		};

	private:

		uint32_t _id;

	public:

		OpenGLShader(const ShaderInitializer& initializer);

		~OpenGLShader() override;

		virtual void Use() const override;

	private:

		void Compile(const uint32_t& objId, const ObjectType& objType) const;

		std::string Load(const char* path) const;

		const uint32_t GenerateShader(const ShaderType&& type, const char* source) const;

		void TryAttach(const uint32_t& id) const;
	};
}
