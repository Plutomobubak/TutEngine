#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;

    // Uniform helpers
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    GLuint getID() const { return ID; }

private:
    GLuint ID;

    std::string loadFileToString(const char* filePath);
    GLuint compileShader(const char* filePath, GLenum shaderType);
    GLuint linkProgram(GLuint vertShader, GLuint fragShader);
};
