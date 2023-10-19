#include "Assets/Mesh.h"
#include "GraphicsAPI.h"
#include "Platform/Current.h"
#include "AssetPath.h"
#include "../../../GraphicsInterface/Code/Include/GraphicsAPI.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

std::unordered_map<std::wstring, Mesh*> Mesh::_loaded;

Mesh* Mesh::LoadModel(const wchar_t* path)
{
    if (_loaded.contains(path) == false)
    {
        auto mesh = new Mesh;
        if (ConstructInternal(path, mesh))
        {
            _loaded[path] = mesh;
        }
        else
        {
            delete mesh;
            return nullptr;
        }
    }

    return _loaded[path];
}

void Mesh::Serialize(ISerializeContext* context)
{
    for (auto entry : _loaded)
        if (entry.second == this)
        {
            auto serializedPath = AssetPath::SerializePath(entry.first);
            context->AddData(serializedPath);
            return;
        }
}

bool Mesh::Deserialize(IDeserializeContext* context)
{
    std::vector<int8_t> data = context->ReadAll();

    std::wstring path = AssetPath::DeserializePath(data);

    const bool isSuccess = ConstructInternal(path.c_str(), this);

    if (isSuccess)
        _loaded[path] = this;

    return isSuccess;
}

void Mesh::ConstructMesh(const aiScene* scene, aiNode* node, std::vector<Varlet::Graphics::SubMeshInfo>& vertices)
{
    for (int32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        vertices.push_back(ConstructSubMesh(scene, mesh));
    }

    for (int32_t i = 0; i < node->mNumChildren; i++)
        ConstructMesh(scene, node->mChildren[i], vertices);
}

Varlet::Graphics::SubMeshInfo Mesh::ConstructSubMesh(const aiScene* scene, aiMesh* mesh)
{
    assert(mesh->HasPositions());
    const bool hasNormals = mesh->HasNormals();
    const bool hasTextureCoords = mesh->HasTextureCoords(0);
    const bool hasTangentsAndBitangents = mesh->HasTangentsAndBitangents();

    const uint32_t dataSize = mesh->mNumVertices * sizeof(glm::vec3)
        + (hasNormals ? mesh->mNumVertices * sizeof(glm::vec3) : 0)
        + (hasTextureCoords ? mesh->mNumVertices * sizeof(glm::vec2) : 0)
        + (hasTangentsAndBitangents ? mesh->mNumVertices * sizeof(glm::vec3) : 0);

    Varlet::Graphics::SubMeshInfo meshInfo;
    meshInfo.verticesCount = mesh->mNumVertices;
    meshInfo.vertexData.resize(dataSize);

    uint32_t offset = 0;
    uint32_t blockSize = mesh->mNumVertices * sizeof(glm::vec3);
    memcpy_s(meshInfo.vertexData.data(), blockSize, &mesh->mVertices[0].x, blockSize);
    offset += blockSize;

    if (hasNormals)
    {
        blockSize = mesh->mNumVertices * sizeof(glm::vec3);
        memcpy_s(meshInfo.vertexData.data() + offset, blockSize, &mesh->mNormals[0].x, blockSize);
        offset += blockSize;
    }

    if (hasTextureCoords)
    {
        for (int32_t i = 0; i < mesh->mNumVertices; i++)
        {
            const glm::vec2 data = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            memcpy_s(meshInfo.vertexData.data() + offset + i * sizeof(glm::vec2), sizeof(glm::vec2) , &data, sizeof(glm::vec2));
        }

        offset += mesh->mNumVertices * sizeof(glm::vec2);
    }

    if (hasTangentsAndBitangents)
    {
        blockSize = mesh->mNumVertices * sizeof(glm::vec3);
        memcpy_s(meshInfo.vertexData.data() + offset, blockSize, &mesh->mTangents[0].x, blockSize);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (int32_t j = 0; j < face.mNumIndices; j++)
            meshInfo.indices.push_back(face.mIndices[j]);
    }

    return meshInfo;
}

bool Mesh::ConstructInternal(const wchar_t* path, Mesh* mesh)
{
    assert(mesh);

    std::string validPath = Varlet::Core::Platform::ConvertFromWide(path);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(validPath.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        VARLET_LOG(Error, WIDE("Failed load model: %s"), Varlet::Core::Platform::ConvertToWide(importer.GetErrorString()).c_str());
        return false;
    }

    std::vector<Varlet::Graphics::SubMeshInfo> meshInfos;
    ConstructMesh(scene, scene->mRootNode, meshInfos);

    Varlet::Graphics::ModelCreateInfo createInfo;
    createInfo.vertexLayout =
    {
        { Varlet::Graphics::ShaderDataType::Vec3, 0 }, // Position
        { Varlet::Graphics::ShaderDataType::Vec3, 1 }, // Normals
        { Varlet::Graphics::ShaderDataType::Vec2, 2 }, // UVs
        { Varlet::Graphics::ShaderDataType::Vec3, 3 }, // Tangents
    };

    createInfo.subMeshInfos = std::move(meshInfos);

    return Varlet::Graphics::CreateModel(mesh, createInfo);
}
