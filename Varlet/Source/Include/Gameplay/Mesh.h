#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class VertexArray;
	class Shader;
}

struct aiScene;
struct aiNode;
struct aiMesh;

class CORE_API Mesh final
{
private:

	std::vector<Varlet::VertexArray*> _subMeshes;
	mutable std::vector<Varlet::Shader*> _shaders;

public:

	const std::vector<Varlet::VertexArray*>& GetSubMeshes() const;

	std::vector<Varlet::Shader*>& GetShaders() const;

	static Mesh* LoadModel(const std::string& path);

private:

	Mesh() = default;

	static Mesh* ConstructMesh(Mesh* processedMesh, const aiScene* scene, aiNode* node);
	
	static Varlet::VertexArray* ConstructSubMesh(const aiScene* scene, aiMesh* mesh);
};

