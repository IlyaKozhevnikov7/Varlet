#include "Mesh.h"
#include "RendererAPI.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Mesh::Mesh() :
    positions(0),
    normals(0),
    uvs(0),
    tangents(0),
    indices(0)
{
}

Mesh* Mesh::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        VARLET_LOG(LevelType::Error, "Failed load model: " + *importer.GetErrorString());
        return nullptr;
    }

    std::vector<Varlet::MeshData> vertices;
    ConstructMesh(scene, scene->mRootNode, vertices);

    auto mesh = new Mesh();
    Varlet::RendererAPI::RegisterMesh(mesh, vertices);

    return mesh;
}

void Mesh::CommitChanges() const
{
    Varlet::RendererAPI::RegisterMesh(this, {});
}

void Mesh::ConstructMesh(const aiScene* scene, aiNode* node, std::vector<Varlet::MeshData>& vertices)
{
    for (int32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        vertices.push_back(ConstructSubMesh(scene, mesh));
    }

    for (int32_t i = 0; i < node->mNumChildren; i++)
        ConstructMesh(scene, node->mChildren[i], vertices);
}

Varlet::MeshData Mesh::ConstructSubMesh(const aiScene* scene, aiMesh* mesh)
{
    Varlet::MeshData vertices;
    vertices.positions.resize(mesh->mNumVertices);
    vertices.normals.resize(mesh->mNumVertices);
    vertices.uvs.resize(mesh->mNumVertices);
    vertices.tangents.resize(mesh->mNumVertices);

    for (int32_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.positions[i] = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        if (mesh->HasNormals())
        {
            vertices.normals[i] = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z);
        }

        if (mesh->mTextureCoords[0] != nullptr)
        {
            vertices.uvs[i] = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.uvs[i] = glm::vec2(0.f, 0.f);
        }

        vertices.tangents[i] = glm::vec3(
            mesh->mTangents[i].x,
            mesh->mTangents[i].y,
            mesh->mTangents[i].z);
    }

    for (int32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (int32_t j = 0; j < face.mNumIndices; j++)
            vertices.indices.push_back(face.mIndices[j]);
    }

    return vertices;
}
