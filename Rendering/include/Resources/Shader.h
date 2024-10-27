#pragma once
#include <string>
#include <glad/glad.h>

#include "Resources/Asset.h"

class Shader : Asset
{
    enum ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

public:
    Shader(std::string pFilePath);
    ~Shader();

    void Use();

    Shader& operator=(const Shader& other);

private:
    GLuint mShaderID;

    ShaderProgramSource ParseShader(const std::string& filePath);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
};

