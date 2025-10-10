#pragma once

#include <glad/gl.h>

#include "Shader.hpp"
#include <string>
#include <expected>

class VertexFragmentShader : public Shader
{
public:
    VertexFragmentShader(const std::string &vs, const std::string &fs);
    VertexFragmentShader(const VertexFragmentShader &) = delete;
    VertexFragmentShader &operator=(const VertexFragmentShader &) = delete;
    // TODO: add move constructor / move assignment operator ?

    Error getError(); // maybe this should be moved to the Shader interface?

    Error use() override;
    ~VertexFragmentShader() override;

private:
    GLuint m_program;
    Error m_error;

    std::expected<GLuint, Error> compileShader(GLenum type, const std::string &source);
    std::expected<GLuint, Error> createProgram(GLuint vs, GLuint fs);
};