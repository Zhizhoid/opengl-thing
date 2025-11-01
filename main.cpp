#include <fstream>
#include <string>
#include <optional>
#include <log.hpp>
#include <VertexFragmentShader.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Function prototypes
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

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
        /* position: */ 0.0f, 0.0f, 0.0f, /* texture: */ 0.0f, 0.0f, //
        /* position: */ 0.5f, 0.0f, 0.0f, /* texture: */ 1.0f, 0.0f, //
        /* position: */ 0.5f, 0.5f, 0.0f, /* texture: */ 1.0f, 1.0f, //
        /* position: */ 0.0f, 0.5f, 0.0f, /* texture: */ 0.0f, 1.0f, //
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void *>(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // texture
    glActiveTexture(GL_TEXTURE0);
    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(true);
    int texWidth, texHeight, texNrChan;
    unsigned char *texData = stbi_load("../res/textures/container.jpg", &texWidth, &texHeight, &texNrChan, 0);
    logs::log(logs::Level::Info, "Texture parameters: w: %d, h: %d, nrChan: %d, dataPtr: %p", texWidth, texHeight, texNrChan, texData);
    if (texData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData + 3*(0));
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        logs::log(logs::Level::Error, "Failed to load texture!");
    }
    stbi_image_free(texData);
    
    glActiveTexture(GL_TEXTURE1);
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    texData = stbi_load("../res/textures/awesomeface.png", &texWidth, &texHeight, &texNrChan, 0);
    logs::log(logs::Level::Info, "Texture (2nd) parameters: w: %d, h: %d, nrChan: %d, dataPtr: %p", texWidth, texHeight, texNrChan, texData);
    if (texData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData + 3*(0));
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        logs::log(logs::Level::Error, "Failed to load 2nd texture!");
    }
    stbi_image_free(texData);

    const std::string vs = loadFileContents("../res/shaders/texture_blend/vs.glsl").value_or("");
    const std::string fs = loadFileContents("../res/shaders/texture_blend/fs.glsl").value_or("");

    VertexFragmentShader shaderProgram(vs.data(), fs.data());
    
    logs::log(logs::Level::Info, "Status: %d", static_cast<int>(shaderProgram.getError()));
    
    shaderProgram.use();
    shaderProgram.setUniform("texture0", 0);
    shaderProgram.setUniform("texture1", 1);

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
