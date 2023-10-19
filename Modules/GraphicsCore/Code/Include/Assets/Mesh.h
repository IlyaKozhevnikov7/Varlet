#pragma once

#include "GraphicsCoreCore.h"
#include "Asset.h"
#include "Mesh.generated.h"

struct aiScene;
struct aiNode;
struct aiMesh;

struct MeshData final
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> tangents;
	std::vector<uint32_t> indices;
};

namespace Varlet::Graphics
{
	struct SubMeshInfo;
}

REFLECTABLE()
class GRAPHICSCORE_API Mesh final : public IAsset, public ICustomSerializable
{
	GENERATION

private:

	static std::unordered_map<std::wstring, Mesh*> _loaded;

public:

	Mesh() = default;

	static Mesh* LoadModel(const wchar_t* path);

	void Serialize(ISerializeContext* context) override;
	
	bool Deserialize(IDeserializeContext* context) override;

private:

	static void ConstructMesh(const aiScene* scene, aiNode* node, std::vector<Varlet::Graphics::SubMeshInfo>& vertices);
	
	static Varlet::Graphics::SubMeshInfo ConstructSubMesh(const aiScene* scene, aiMesh* mesh);
	
	static bool ConstructInternal(const wchar_t* path, Mesh* mesh);
};

