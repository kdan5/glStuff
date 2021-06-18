#include "shader.h"

// read shader code from file
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // stream vertex and fragment shader code from file
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        // read file buffer to stream 
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();
        // read stream as string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;    
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    unsigned int vertex, fragment;

    // initialize and compile shaders        
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT"); 

    // link shaders to program
    pid = glCreateProgram();
    glAttachShader(pid, vertex);
    glAttachShader(pid, fragment);
    glLinkProgram(pid);
    checkCompileErrors(pid, "PROGRAM");

    // shaders no longer needed after linking to program
    glDeleteShader(vertex);
    glDeleteShader(fragment); 
}


void Shader::use() {
    glUseProgram(pid);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(pid, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(pid, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(pid, name.c_str()), value);
}

void Shader::checkCompileErrors(unsigned int shader, std::string obj) {
    int success = 0;
    char infoLog[1024];
    if (obj == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::LINKING_ERROR::" << obj << "\n" << infoLog << "\n" << std::endl;
        }
    }
    else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_ERROR::" << obj << "\n" << infoLog << "\n" << std::endl;
        }
    }
}
