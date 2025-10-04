#include "primitives.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>


Model* Primitives::CreatePlane(float size) {
    std::vector<Vertex> vertices = {
        {{-size, 0.0f, -size}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ size, 0.0f, -size}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ size, 0.0f,  size}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-size, 0.0f,  size}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
    };

    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

    return new Model(vertices, indices);
}

Model* Primitives::CreateCube(float size) {
    float h = size * 0.5f;

    glm::vec3 positions[] = {
        {-h,-h,-h}, { h,-h,-h}, { h, h,-h}, {-h, h,-h}, // back
        {-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}  // front
    };

    unsigned int faces[] = {
        // back
        0,1,2, 2,3,0,
        // front
        4,5,6, 6,7,4,
        // left
        0,4,7, 7,3,0,
        // right
        1,5,6, 6,2,1,
        // bottom
        0,1,5, 5,4,0,
        // top
        3,2,6, 6,7,3
    };

    std::vector<Vertex> vertices;
    vertices.reserve(8);
    for (auto& p : positions) {
        Vertex v;
        v.position = p;
        v.normal = glm::normalize(p);
        v.texCoords = {0.0f, 0.0f};
        vertices.push_back(v);
    }

    std::vector<unsigned int> indices(faces, faces + sizeof(faces) / sizeof(unsigned int));
    return new Model(vertices, indices);
}

Model* Primitives::CreateSphere(float radius, unsigned int sectors, unsigned int stacks) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i <= stacks; ++i) {
        float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * glm::two_pi<float>() / sectors;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            Vertex v;
            v.position = {x, z, y};
            v.normal = glm::normalize(v.position);
            v.texCoords = { (float)j / sectors, (float)i / stacks };
            vertices.push_back(v);
        }
    }

    for (unsigned int i = 0; i < stacks; ++i) {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return new Model(vertices, indices);
}
