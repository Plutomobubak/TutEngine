#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    GLuint vertShader = compileShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    if (vertShader && fragShader)
        ID = linkProgram(vertShader, fragShader);
    else
        ID = 0;
}

Shader::~Shader() {
    if (ID) glDeleteProgram(ID);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if (loc == -1) {
        std::cerr << "Warning: uniform '" << name << "' not found or not used in shader!\n";
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

// Private helpers

std::string Shader::loadFileToString(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filePath << "\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compileShader(const char* filePath, GLenum shaderType) {
    std::string sourceString = loadFileToString(filePath);
    if (sourceString.empty()) return 0;

    const char* sourceCStr = sourceString.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        std::string infoLog(logLen, ' ');
        glGetShaderInfoLog(shader, logLen, nullptr, &infoLog[0]);
        std::cerr << "ERROR: Shader compilation failed (" << filePath << "):\n" << infoLog << "\n";
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint Shader::linkProgram(GLuint vertShader, GLuint fragShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        std::string infoLog(logLen, ' ');
        glGetProgramInfoLog(program, logLen, nullptr, &infoLog[0]);
        std::cerr << "ERROR: Program linking failed:\n" << infoLog << "\n";
        glDeleteProgram(program);
        return 0;
    }

    glDetachShader(program, vertShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}
