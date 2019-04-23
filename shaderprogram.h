#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    enum ShaderType
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    bool loadShaders(const char* vsFileName, const char* fsFileName);
    void use();

private:
    std::string fileToString(const std::string& filename);
    void checkCompileErrors(GLuint shader, ShaderType type);
    GLuint createShader(GLuint shaderType, std::string shaderSourcePath);

    GLuint mHandle;
};

#endif // SHADERPROGRAM_H
