#include "VertexFragmentShader.hpp"
#include <log.hpp>

VertexFragmentShader::VertexFragmentShader(const char *vs_src, const char *fs_src)
{
    m_error = Error::Ok;

    auto vs = compileShader(GL_VERTEX_SHADER, vs_src);
    auto fs = compileShader(GL_FRAGMENT_SHADER, fs_src);
    if (!vs || !fs)
    {
        m_error = Error::CompilationFailed;
        return;
    }

    auto ret = createProgram(vs.value(), fs.value());
    m_program = ret.value_or(0);
    if (!ret && m_error == Error::Ok)
        m_error = ret.error();
}

VertexFragmentShader::~VertexFragmentShader()
{
    glDeleteProgram(m_program);
}

Shader::Error VertexFragmentShader::getError()
{
    return m_error;
}

Shader::Error VertexFragmentShader::use()
{
    if (m_error != Error::Ok)
        return m_error;

    glUseProgram(m_program);
    // TODO: check for errors?
    return Error::Ok;
}

Shader::Error VertexFragmentShader::setUniform(const char *name, int a)
{
    if (m_error != Error::Ok)
        return m_error;
    glUniform1i(glGetUniformLocation(m_program, name), a);
    return Error::Ok;
}

std::expected<GLuint, Shader::Error> VertexFragmentShader::compileShader(GLenum type, const char *source)
{
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    GLint compileStatus;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLint logBuffSize;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logBuffSize);
        char *logBuff = new char[logBuffSize];

        GLint logWroteLength;
        glGetShaderInfoLog(id, logBuffSize, &logWroteLength, logBuff);

        logs::log(logs::Level::Error, "Failed to compile shader! (type: %u)\nDumping logs:\n%s", type, logBuff);

        delete[] logBuff;

        glDeleteShader(id);
        return std::unexpected(Error::CompilationFailed);
    }

    return id;
}

std::expected<GLuint, Shader::Error> VertexFragmentShader::createProgram(GLuint vs, GLuint fs)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        GLint logBuffSize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logBuffSize);
        char *logBuff = new char[logBuffSize];

        GLint logWroteLength;
        glGetProgramInfoLog(program, logBuffSize, &logWroteLength, logBuff);

        logs::log(logs::Level::Error, "Failed to link shader! \nDumping logs:\n%s", logBuff);

        delete[] logBuff;
        return std::unexpected(Error::LinkingFailed);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    // FIXME: needed or not?
    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}
