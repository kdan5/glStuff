#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // shader program id
    unsigned int pid;
    // read shader code from file
    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexPath;
        std::string fragmentPath;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
        // stream vertex and fragment shader code from file
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            // read file buffer to stream 
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuff();
            fShaderStream << fShaderFile.rdbuff();

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
        int success;
        char infoLog[512];
        // initialize and compile shaders        
        glCreateShader(GL_VERTEX_SHADER);
        glCreateFragment(GL_FRAGMENT_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_SUCCESS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        } 
        // link shaders to program
        pid = glCreateProgram();
        glAttachShader(pid, vertex);
        glAttachShader(pid, fragment);
        glLinkProgram(pid);
        glGetProgramiv(pid, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(pid, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // shaders no longer needed after linking to program
        glDeleteShader(vertex);
        glDeleteShader(fragment); 
    }
    

    void use() {
        glUseProgram(pid);
    }
    
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(pid, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(pid, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1i(glGetUniformLocation(pid, name.c_str()), value);
    }
};
