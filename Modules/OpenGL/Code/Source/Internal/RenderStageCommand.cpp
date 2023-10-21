#include "Internal/RenderStageCommand.h"
#include "Internal/Resources.h"
#include "Internal/Resources/RenderPipeline.h"
#include "Internal/Resources/Shader.h"
#include "Internal/Resources/Texture.h"
#include "Internal/Resources/Model.h"

#include "glad/glad.h"

namespace Varlet::Graphics::OpenGL
{
	RenderPipeline* currentRenderPipeline = nullptr;

	bool isBlening = false;
	GLenum srcFactor = 0;
	GLenum dstFactor = 0;
	GLenum operation = 0;

	bool depthWrite = true;
	bool stencilWrite = true;
	bool cullFace = true;

	bool RenderStageCommand::BeginRenderPipeline(void* descriptor)
	{
		currentRenderPipeline = Resources::GetRenderPipeline(descriptor);
		assert(currentRenderPipeline);

		currentRenderPipeline->Begin();
		return true;
	}

	bool RenderStageCommand::EndRenderPipeline(void* descriptor)
	{
		currentRenderPipeline = nullptr;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;
	}

	void RenderStageCommand::NextPass()
	{
		assert(currentRenderPipeline);
		currentRenderPipeline->NextPass();
	}

	void RenderStageCommand::BindShader(void* shaderDescriptor, void* pipelineDescriptor, const void* uniformBuffer)
	{
		assert(currentRenderPipeline);

		const auto shader = Resources::GetShader(shaderDescriptor, pipelineDescriptor);
		
		if (shader == nullptr)
			return;

		glUseProgram(shader->id);

		if (shader->blendState.isEnable)
		{
			if (isBlening != shader->blendState.isEnable)
			{
				isBlening = shader->blendState.isEnable;
				glEnable(GL_BLEND);
			}

			if (operation != shader->blendState.operation)
			{
				operation = shader->blendState.operation;
				glBlendEquation(operation);
			}

			if (srcFactor != shader->blendState.srcFactor || dstFactor != shader->blendState.dstFactor)
			{
				srcFactor = shader->blendState.srcFactor;
				dstFactor = shader->blendState.dstFactor;
				glBlendFunc(srcFactor, dstFactor);
			}
		}

		if (cullFace != shader->cullFace)
		{
			cullFace = shader->cullFace;

			if (cullFace)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);
		}

		if (isBlening != shader->blendState.isEnable)
		{
			isBlening = shader->blendState.isEnable;
			glDisable(GL_BLEND);
		}

		if (depthWrite != shader->depthWrite)
		{
			depthWrite = shader->depthWrite;

			if (depthWrite)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
		}

		if (stencilWrite != shader->stencilWrite)
		{
			stencilWrite = shader->stencilWrite;

			if (stencilWrite)
				glEnable(GL_STENCIL_TEST);
			else
				glDisable(GL_STENCIL_TEST);
		}

		if (uniformBuffer == nullptr)
			return;

		auto uniformInfos = Resources::GetShaderUniformInfosInternal(shaderDescriptor);

		int8_t* data = (int8_t*)uniformBuffer;
		uint32_t offset = 0;
		GLenum textureUnit = uniformInfos.firstTextureUnit;

		for (int32_t i = 0; i < uniformInfos.uniformInfos->size(); i++)
		{
			const auto& currentInfo = (*(uniformInfos.uniformInfos))[i];
			const auto& location = (*(uniformInfos.uniformLocations))[i];

			const int8_t* value = data + offset;

#define TWO_ELEMENTS(Type) *std::bit_cast<Type*>(value), *std::bit_cast<Type*>(value + sizeof(Type))
#define THREE_ELEMENTS(Type) TWO_ELEMENTS(Type), *std::bit_cast<Type*>(value + sizeof(Type) * 2)
#define FOUR_ELEMENTS(Type) THREE_ELEMENTS(Type), *std::bit_cast<Type*>(value + sizeof(Type) * 3)

			switch (currentInfo.type)
			{
			case ShaderDataType::Bool:
			case ShaderDataType::Int:		glUniform1i(location, *std::bit_cast<GLint*>(value));	break;

			case ShaderDataType::UInt:		glUniform1ui(location, *std::bit_cast<GLuint*>(value));	break;
			case ShaderDataType::Float:		glUniform1f(location, *std::bit_cast<float*>(value));	break;
			case ShaderDataType::Double:	glUniform1d(location, *std::bit_cast<double*>(value));	break;

			case ShaderDataType::Vec2:		glUniform2f(location, TWO_ELEMENTS(float));		break;
			case ShaderDataType::Vec3:		glUniform3f(location, THREE_ELEMENTS(float));	break;
			case ShaderDataType::Vec4:		glUniform4f(location, FOUR_ELEMENTS(float));		break;

			case ShaderDataType::Mat2:		glUniformMatrix2fv(location, 1, GL_FALSE, (float*)value);	break;
			case ShaderDataType::Mat3:		glUniformMatrix3fv(location, 1, GL_FALSE, (float*)value);	break;
			case ShaderDataType::Mat4:		glUniformMatrix4fv(location, 1, GL_FALSE, (float*)value);	break;

			case ShaderDataType::Sampler2D:
			{
				void* descriptor = *std::bit_cast<void**>(value);
				const auto texture = Resources::GetTexture(descriptor);

				GLuint textureId = texture != nullptr
					? texture->id
					: 0;

				glActiveTexture(GL_TEXTURE0 + textureUnit);
				glBindTexture(GL_TEXTURE_2D, textureId);

				textureUnit++;
			}
			break;
			}

			switch (currentInfo.type)
			{
			case ShaderDataType::Bool:		offset += sizeof(bool);			break;
			case ShaderDataType::Int:		offset += sizeof(int32_t);		break;
			case ShaderDataType::UInt:		offset += sizeof(uint32_t);		break;
			case ShaderDataType::Float:		offset += sizeof(float);		break;
			case ShaderDataType::Double:	offset += sizeof(double);		break;
			case ShaderDataType::Vec2:		offset += sizeof(glm::vec2);	break;
			case ShaderDataType::Vec3:		offset += sizeof(glm::vec3);	break;
			case ShaderDataType::Vec4:		offset += sizeof(glm::vec4);	break;
			case ShaderDataType::Mat2:		offset += sizeof(glm::mat2);	break;
			case ShaderDataType::Mat3:		offset += sizeof(glm::mat3);	break;
			case ShaderDataType::Mat4:		offset += sizeof(glm::mat4);	break;
			case ShaderDataType::Sampler2D:	offset += sizeof(void*);		break;
			}
		}
	}

	void RenderStageCommand::Draw(void* descriptor)
	{
		const auto model = Resources::GetModel(descriptor);
		
		if (model == nullptr)
			return;

		for (auto& mesh : model->meshes)
		{
			glBindVertexArray(mesh.vao);

			if (mesh.ebo > 0)
				glDrawElements(GL_TRIANGLES, mesh.verticesCount, GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, mesh.verticesCount);
		}
	}

	void RenderStageCommand::DrawVertices(uint32_t vertexCount)
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
