#include <fstream>
#include <string>
#include <optional>
#include <log.hpp>

#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>

// Function prototypes
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

static GLuint compileShader(GLenum type, const std::string &source)
{
    GLuint id = glCreateShader(type);
    const char *sourceRaw = source.c_str();
    glShaderSource(id, 1, &sourceRaw, nullptr);
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
    }

    return id;
}

static GLuint createProgram(const std::string &vsSource, const std::string &fsSource)
{
    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

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
    }

    // FIXME: should shaders still be deleted and detached if shader linking failed?
    glDeleteShader(vs);
    glDeleteShader(fs);

    // FIXME: needed or not?
    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}

static std::optional<std::string> loadFileContents(const std::string &path)
{
    std::ifstream fin(path);

    if (!fin.is_open())
    {
        logs::log(logs::Level::Error, "Failed to read from file: \"%s\"\n", path.c_str());
        return std::nullopt;
    }

    auto res = std::string(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

    return res;
}

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        logs::log(logs::Level::Error, "Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        logs::log(logs::Level::Error, "Failed to initialize OpenGL context");
        return -1;
    }

    // Successfully loaded OpenGL
    logs::log(logs::Level::Info, "Loaded OpenGL %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // VAO needed in core profile
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertices[]{
        0.0f, 0.0f, 0.0f, //
        0.5f, 0.0f, 0.0f, //
        0.5f, 0.5f, 0.0f, //
        0.0f, 0.5f, 0.0f, //
    };

    unsigned int indices[]{
        0, 1, 2, //
        0, 2, 3, //
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);

    const std::string vs = loadFileContents("../res/shaders/basic_color_position/vs.glsl").value_or("");
    const std::string fs = loadFileContents("../res/shaders/basic_color_position/fs.glsl").value_or("");

    GLuint program = createProgram(vs, fs);
    glUseProgram(program);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, 0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
