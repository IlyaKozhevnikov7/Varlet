#include "Mesh.h"
#include "RendererAPI.h"
#include "VertexArray.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

const std::vector<Varlet::VertexArray*>& Mesh::GetSubMeshes() const
{
    return _subMeshes;
}

std::vector<Varlet::Shader*>& Mesh::GetShaders() const
{
    return _shaders;
}

Mesh* Mesh::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        VARLET_LOG(LevelType::Error, "Failed load model: " + *importer.GetErrorString());
        return nullptr;
    }

    return ConstructMesh(new Mesh, scene, scene->mRootNode);
}

Mesh* Mesh::ConstructMesh(Mesh* processedMesh, const aiScene* scene, aiNode* node)
{
    for (int32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processedMesh->_subMeshes.push_back(ConstructSubMesh(scene, mesh));
    }

    for (int32_t i = 0; i < node->mNumChildren; i++)
        ConstructMesh(processedMesh, scene, node->mChildren[i]);

    return processedMesh;
}

Varlet::VertexArray* Mesh::ConstructSubMesh(const aiScene* scene, aiMesh* mesh)
{
    std::vector<Varlet::Vertex> vertices;
    std::vector<uint32_t> indices;

    for (int32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Varlet::Vertex vertex;

        vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        if (mesh->HasNormals())
        {
            vertex.normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z);
        }

        if (mesh->mTextureCoords[0] != nullptr)
        {
            vertex.textureCoordinate = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.textureCoordinate = glm::vec2(0.f, 0.f);
        }

        vertices.push_back(vertex);
    }

    for (int32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (int32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    return Varlet::RendererAPI::CreateVertexArray({ vertices ,indices });
}
