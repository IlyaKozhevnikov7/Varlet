#pragma once

#include "VarletCore.h"

namespace Varlet
{
	class VertexArray;
	struct Vertex;

	class SubMesh final
	{
	private:

		VertexArray* _vertexArray;

	public:

		SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		~SubMesh();

		const VertexArray* GetVertexArray() const;
	};
}

struct aiScene;
struct aiNode;
struct aiMesh;

class CORE_API Mesh final
{
private:

	std::vector<Varlet::SubMesh*> _subMeshes;

public:

	const std::vector<Varlet::SubMesh*>& GetSubMeshes() const;

	static Mesh* LoadModel(const std::string& path);

private:

	Mesh() = default;

	static Mesh* ConstructMesh(Mesh* processedMesh, const aiScene* scene, aiNode* node);
	
	static Varlet::SubMesh* ConstructSubMesh(const aiScene* scene, aiMesh* mesh);
};

