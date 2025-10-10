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

    virtual Error use() = 0;
    virtual ~Shader() = default;
};