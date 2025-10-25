#pragma once

#include <glad/gl.h>

#include "Shader.hpp"
#include <string>
#include <expected>

class VertexFragmentShader : public Shader
{
public:
    VertexFragmentShader(const char *vs, const char *fs);
    VertexFragmentShader(const VertexFragmentShader &) = delete;
    VertexFragmentShader &operator=(const VertexFragmentShader &) = delete;
    // TODO: add move constructor / move assignment operator ?
    ~VertexFragmentShader() override;
    
    Error getError(); // maybe this should be moved to the Shader interface?

    Error use() override;
    // uniform setters
    Error setUniform(const char *name, int a) override;

private:
    GLuint m_program;
    Error m_error;

    std::expected<GLuint, Error> compileShader(GLenum type, const char *source);
    std::expected<GLuint, Error> createProgram(GLuint vs, GLuint fs);
};