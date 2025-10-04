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
    Model(const std::vector<Vertex>& vertices,
          const std::vector<unsigned int>& indices)
        : vertices(vertices), indices(indices) {
        setupMesh();
    }
    ~Model();

    // Load model from file and return pointer to new Model
    static Model* loadModel(const std::string& path);
    static Model* createCube(float size = 1.0f);
    static Model* createPlane(float size = 1.0f);
    static Model* createSphere(float radius = 1.0f, unsigned int sectors = 16, unsigned int stacks = 16);
    static Model* createCapsule(float radius = 0.5f, float height = 2.0f, unsigned int segments = 16);

    // Draw the mesh (geometry + textures)
    void draw();
    void setTexture(const std::string& path);
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
