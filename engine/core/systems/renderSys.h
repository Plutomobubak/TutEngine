#pragma once

#include "../registry.h"
#include "../components/mesh.h"
#include "../../renderer/model.h"
#include "../components/transform.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

class RenderSys {
public:
    GLuint shader;

    RenderSys(GLuint shaderProgram) : shader(shaderProgram) {}

    void render() {
      // Get Camera from registry
        glm::mat4 view, proj;
        auto& registry = Registry::instance();

        registry.view<Camera, GlobalTransform>([&](Entity, Camera& cam, GlobalTransform& gt) {
            proj = cam.getProjection();
            glm::mat4 camWorld = gt.matrix;
            view = glm::inverse(camWorld);
        });

        // Set Shader Program and variables
        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);
        GLint modelLoc = glGetUniformLocation(shader, "model");
        GLint viewLoc = glGetUniformLocation(shader, "view");
        GLint projLoc = glGetUniformLocation(shader, "projection");

        // Set the camera matrices once per frame
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);

        registry.view<GlobalTransform, Mesh>([&](Entity, GlobalTransform& tf, Mesh& mc) {
            glm::mat4 model = tf.matrix;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

            if (mc.model) {
                mc.model->draw();
            } else {
                std::cerr << "Mesh model is null. Skipping draw.\n";
            }
        });
    }
};
