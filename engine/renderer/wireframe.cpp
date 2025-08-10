#include "wireframe.h"
#include "../core/registry.h"
#include "./shader.h"
#include <glad/glad.h>
#include <iostream>

const glm::vec3 unitBoxVerts[8] = {
    {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
    {0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f},
    {0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}
};

const unsigned int boxIndices[24] = {
    0,1, 1,2, 2,3, 3,0,
    4,5, 5,6, 6,7, 7,4,
    0,4, 1,5, 2,6, 3,7
};

unsigned int wireVAO = 0, wireVBO = 0, wireEBO = 0;
const unsigned int wireIndexCount = 24;

Shader* wireframeShader = nullptr;

void setupWireframeMesh() {
    wireframeShader = new Shader("shaders/wireframe.vert", "shaders/wireframe.frag");
    wireframeShader->use();
    wireframeShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f)); // red wireframe

    glGenVertexArrays(1, &wireVAO);
    glGenBuffers(1, &wireVBO);
    glGenBuffers(1, &wireEBO);

    glBindVertexArray(wireVAO);

    glBindBuffer(GL_ARRAY_BUFFER, wireVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(unitBoxVerts), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wireEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glLineWidth(2.0f);
    std::cout << "setup wireframe" << std::endl;
}

void drawWireframe(const glm::mat4& worldTransform, const glm::vec3& size) {
    if (!wireframeShader) return;

    wireframeShader->use();

    // Get camera matrices
    glm::mat4 view, proj;
    Registry::instance().view<Camera, GlobalTransform>([&](Entity, Camera& cam, GlobalTransform& gt) {
        proj = cam.getProjection();
        view = glm::inverse(gt.matrix);
    });

    // Set shader uniforms
    wireframeShader->setMat4("model", worldTransform);
    wireframeShader->setMat4("view", view);
    wireframeShader->setMat4("projection", proj);

    // Calculate scaled box vertices (you could skip this if you just use model scale)
    glm::vec3 transformedVerts[8];
    for (int i = 0; i < 8; ++i) {
        transformedVerts[i] = unitBoxVerts[i] * size;
    }

    // Upload updated vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, wireVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(transformedVerts), transformedVerts);

    // Draw
    glBindVertexArray(wireVAO);
    glDrawElements(GL_LINES, wireIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    std::cout << "Draw Wireframe at " << worldTransform[3][0] << ", " << worldTransform[3][1] << ", " << worldTransform[3][2] << std::endl;
}
