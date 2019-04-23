#include "shaderprogram.h"

ShaderProgram::ShaderProgram() : mHandle(0)
{
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mHandle);
}

GLuint ShaderProgram::createShader(GLuint shaderType, std::string shaderSourcePath)
{
    GLuint shader_t = glCreateShader(shaderType);
    std::string shader_s = fileToString(shaderSourcePath);
    const char* s = shader_s.c_str();
    glShaderSource(shader_t, 1, &s, nullptr);
    glCompileShader(shader_t);
    checkCompileErrors(shader_t, VERTEX);

    return shader_t;
}

bool ShaderProgram::loadShaders(const char *vsFileName, const char *fsFileName)
{
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vsFileName);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fsFileName);

    mHandle = glCreateProgram();
    glAttachShader(mHandle, vertexShader);
    glAttachShader(mHandle, fragmentShader);
    glLinkProgram(mHandle);

    checkCompileErrors(mHandle, PROGRAM);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ShaderProgram::use()
{
    if(mHandle > 0)
        glUseProgram(mHandle);
}

std::string ShaderProgram::fileToString(const std::string &filename)
{
    std::stringstream ss;
    std::ifstream file;

    try {
        file.open(filename, std::ios::in);

        if(!file.fail())
        {
            ss << file.rdbuf();
        }

        file.close();

    } catch (std::exception ex) {
        std::cerr << ex.what() << std::endl;
    }

    return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderProgram::ShaderType type)
{
    int status = 0;
    if(type == PROGRAM)
    {
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint length = 0;
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

            std::string errorLog(length, ' ');
            glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
            std::cerr << "LINK ERROR:" << std::endl << errorLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE)
        {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            std::string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            std::cerr << "COMPILE ERROR:" << std::endl << errorLog << std::endl;
        }
    }
}
