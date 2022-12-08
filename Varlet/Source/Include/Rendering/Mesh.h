#pragma once

#include "VarletCore.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace Varlet
{
	struct MeshData final
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> tangents;
		std::vector<uint32_t> indices;
	};
}

class CORE_API Mesh final
{
public:

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> tangents;
	std::vector<uint32_t> indices;

public:

	Mesh();

	static Mesh* LoadModel(const std::string& path);

	void CommitChanges() const;

private:

	static void ConstructMesh(const aiScene* scene, aiNode* node, std::vector<Varlet::MeshData>& vertices);

	static Varlet::MeshData ConstructSubMesh(const aiScene* scene, aiMesh* mesh);
};

