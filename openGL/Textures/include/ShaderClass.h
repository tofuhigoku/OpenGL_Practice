#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class ShaderClass
{
private:
    /* data */
public:
    ShaderClass(const char* vertexPath, const char* fragmentPath);
    ~ShaderClass();

    unsigned int ID;

    void use();

    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;

};

ShaderClass::ShaderClass(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        cout << "open shader and vertex scripts" << endl;
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file’s buffer contents into streams
        cout << "read shader and vertex scripts" << endl;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure const& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    cout << "vShaderCode:\t" << vShaderCode << endl;
    cout << "fShaderCode:\t" << fShaderCode << endl;

    // 2. compile shaders
    cout << "start compiling shaders" << endl;
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

        // vertex Shader
        cout << "start compiling vertex shader" << endl;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
            infoLog << std::endl;
        }

        // similiar for Fragment Shader
        cout << "start compiling Fragment shader" << endl;
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
            infoLog << std::endl;
        }
    // shader Program
    cout << "start compiling shader Program" << endl;
    ShaderClass::ID = glCreateProgram();
    glAttachShader(ShaderClass::ID, vertex);
    glAttachShader(ShaderClass::ID, fragment);
    glLinkProgram(ShaderClass::ID);
        // print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
            infoLog << std::endl;
        }
        // delete shaders; they’re linked into our program and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
}

ShaderClass::~ShaderClass()
{
    glDeleteProgram(ID);
}

void ShaderClass::use()
{
    glUseProgram(ID);
}

void ShaderClass::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderClass::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderClass::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

#endif