#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Shaders:
    // ===========================
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);


    // Error logging:
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::SHADER_LINKING::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Delete shader objects:
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // ===========================
}

int Shader::location(const std::string &name) const {
    auto it = uniformCache.find(name);
    if (it != uniformCache.end())
        return it->second;

    int loc = glGetUniformLocation(ID, name.c_str());
    uniformCache[name] = loc;
    return loc;
}

void Shader::use() {
    glUseProgram(ID);
};
void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(location(name), (int)value);
};
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(location(name), value);
};
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(location(name), value);
};
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(location(name), 1, glm::value_ptr(value));
};
void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(location(name), 1, GL_FALSE, glm::value_ptr(value));
};
