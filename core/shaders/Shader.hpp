#pragma once

// shader interface
class Shader
{
public:
    enum class Error
    {
        Ok = 0,
        CompilationFailed,
        LinkingFailed,
    };

    virtual ~Shader() = default;
    virtual Error use() = 0;

    // uniform setters
    virtual Error setUniform(const char *name, int a) = 0; // TODO: add docs (shader needs to be used before setting the uniforms)
};