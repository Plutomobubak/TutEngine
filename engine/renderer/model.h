#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Model {
  public:
    Model() = default;
    ~Model();

    // Load model from file and return pointer to new Model
    static Model* loadModel(const std::string& path);

    // Draw the mesh (geometry + textures)
    void draw();

    // Accessors for animation data
    const aiScene* getScene() const { return scene; }
    const std::map<std::string, unsigned int>& getBoneMapping() const { return boneMapping; }
    const std::vector<glm::mat4>& getBoneOffsets() const { return boneOffsets; }

  private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint textureID = 0;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Assimp data, kept alive by importer
    Assimp::Importer importer;
    const aiScene* scene = nullptr;

    // Bone data extracted during loading, for animations
    std::map<std::string, unsigned int> boneMapping;
    std::vector<glm::mat4> boneOffsets;

    void setupMesh();
    void loadBones(const aiMesh* mesh);
};
