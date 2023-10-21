#pragma once

#include "OpenGLCore.h"
#include "GraphicsBaseTypes.h"
#include "glad/glad.h"

namespace Varlet::Graphics
{
	class ShaderFormatter;

	enum class ShaderStage : uint8_t;
	enum class ShaderDataType : uint8_t;

	struct ShaderInputAssemblyInfo;
	struct ShaderUniformSetInfo;
	struct ShaderUniformVariablesInfo;

	namespace OpenGL
	{
		struct ShaderCreateResult;

		struct ShaderInfo final
		{
			std::string source;
			std::vector<UniformInfo> uniformInfos;
			std::vector<GLint> uniformLocations;
			GLenum firstTextureUnit;
		};

		struct BlendState
		{
			bool isEnable;
			GLenum srcFactor;
			GLenum dstFactor;
			GLenum operation;
		};

		struct Shader final
		{
			enum class ObjectType : uint8_t
			{
				Shader = 0,
				Program
			};

		public:

			GLuint id;
			BlendState blendState;
			bool depthWrite;
			bool stencilWrite;
			bool cullFace;

		public:

			Shader() = default;

			Shader(Shader&& other) noexcept;

			Shader& operator=(Shader&& other) noexcept;

			~Shader();

			static ShaderCreateResult Create(const std::string& source);

		private:

			Shader(const std::string& source, std::vector<UniformInfo>& uniformInfos, std::vector<GLint>& uniformLocations, GLint& firstTextureUnit);

			GLuint GenerateShader(GLenum type, const char* source) const;

			bool Validate(GLuint objId, ObjectType objType) const;

			std::vector<std::pair<ShaderStage, std::string>> ConvertToGLSources(const std::string& source);

			void InitializeUniforms(std::string& source, std::vector<UniformInfo>& uniformInfos, std::vector<GLint>& uniformLocations, GLint& firstTextureUnit);

			void SetupState(ShaderFormatter& formatter);

			void SetupBlendState(ShaderFormatter& formatter);

			static std::string ReplaceInputAssembly(const ShaderInputAssemblyInfo& info);

			static std::string ReplaceUniformSets(const ShaderUniformSetInfo& info);

			static std::string ConvertToGLShader(std::string source);

			static void ProcessInputAttachments(std::string& source);

			static ShaderDataType TypeNameToShaderDataType(const std::string& typeName);
		};

		struct ShaderCreateResult final
		{
			const bool isSuccess;
			Shader shader;
			std::vector<UniformInfo> uniformInfos;
			std::vector<GLint> uniformLocations;
			GLenum firstTextureUnit;
		};

		struct ShaderUtils final
		{
		public:

			static bool IsUseBlend(ShaderFormatter& formatter);

			static void SetupBlendFactor(BlendState& state, ShaderFormatter& formatter);

			static void SetupBlendOperation(BlendState& state, ShaderFormatter& formatter);
			
			static bool IsUseDepthWrite(ShaderFormatter& formatter);

			static bool IsUseStensilWrite(ShaderFormatter& formatter);

			static GLenum ConvertStringToBlendFactor(const std::string& value);

			static GLenum ConvertStringToBlendOperation(const std::string& value);
		};
	}
}