#include "model.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Model* Model::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace |
        aiProcess_ImproveCacheLocality
        );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    auto model = new Model();

    aiMesh* mesh = scene->mMeshes[0]; // first mesh only for now
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex v;
        const float scale = 0.01f; // scale 1 Blender meter → 0.01 OpenGL units
        v.position = scale * glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        v.normal   = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0])
            v.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            v.texCoords = glm::vec2(0.0f);

        model->vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            model->indices.push_back(face.mIndices[j]);
    }
    // Load textures if are present
    if (scene->mNumMaterials > 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    aiString texPath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
        const aiTexture* tex = scene->GetEmbeddedTexture(texPath.C_Str());
        if (tex) {
            int width, height, channels;
            unsigned char* imageData = nullptr;

            if (tex->mHeight == 0) {
                // Compressed
                imageData = stbi_load_from_memory(
                    reinterpret_cast<unsigned char*>(tex->pcData),
                    tex->mWidth,
                    &width, &height, &channels, 0
                );
            } else {
                // Uncompressed (rare)
                width = tex->mWidth;
                height = tex->mHeight;
                channels = 4;
                imageData = reinterpret_cast<unsigned char*>(tex->pcData);
            }

            if (imageData) {
                glGenTextures(1, &model->textureID);
                glBindTexture(GL_TEXTURE_2D, model->textureID);
                GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                if (tex->mHeight == 0)
                    stbi_image_free(imageData); // only if loaded via stbi
            }
        }
    }
}

    model->setupMesh();
    return model;
}

void Model::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1); // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2); // texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
glDisable(GL_CULL_FACE);
glFrontFace(GL_CCW);  // or GL_CW if your model uses clockwise winding
}

void Model::draw() {
  if (textureID) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
