#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <shaderprogram.h>

static const std::string vPath = "../ModernOpenGL/shaders/shader.vert";
static const std::string fPath = "../ModernOpenGL/shaders/shader.frag";

static const std::string TITLE = "Modern OpenGL";
static const unsigned int WIDTH = 1280;
static const unsigned int HEIGHT = 720;

static bool fullscreen = false;
static bool hasToSwitch = false;
static bool wireframeMode = false;

bool initOpenGL(GLFWwindow*& window, bool& windowInit);
void showFPS(GLFWwindow* window);
void handleKeyboard(GLFWwindow* window, int key, int code, int action, int mode);
void checkForFullscreen(GLFWwindow*& window, bool& windowInit);

static const GLfloat vertices[] = {
    -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,
};

static const GLuint indices[] = {
    0, 1, 2,
    0, 1, 3,
};

int main()
{
    GLFWwindow* window = nullptr;
    bool windowInit = true;
    if(!initOpenGL(window, windowInit))
        return -1;

    GLuint VAO, VBO, EBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*6, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*6, (GLvoid*)(sizeof(GL_FLOAT)*3));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ShaderProgram program;
    program.loadShaders(vPath.c_str(), fPath.c_str());

    while(!glfwWindowShouldClose(window))
    {
        showFPS(window);
        glfwPollEvents();

        checkForFullscreen(window, windowInit);

        glClear(GL_COLOR_BUFFER_BIT);

        program.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

bool initOpenGL(GLFWwindow*& window, bool& windowInit)
{
    if(!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    checkForFullscreen(window, windowInit);

    if(window == nullptr)
    {
        glfwTerminate();
        return false;
    }

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }
    glClearColor(0.23f, 0.23f, 0.65f, 1.0);

    return true;
}

void handleKeyboard(GLFWwindow* window, int key, int code, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if(fullscreen)
        {
            hasToSwitch = true;
        }
        else
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    if(key == GLFW_KEY_F && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
    {
        hasToSwitch = true;
    }

    if(key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        wireframeMode = !wireframeMode;

        if(wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void checkForFullscreen(GLFWwindow*& window, bool& windowInit)
{
    if(hasToSwitch || windowInit)
    {
        if(!windowInit)
            fullscreen = !fullscreen;

        hasToSwitch = false;
        glfwDestroyWindow(window);

        glewInit();

        if(fullscreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

            if(vidmode != nullptr)
            {
                window = glfwCreateWindow(vidmode->width, vidmode->height, TITLE.c_str(), monitor, nullptr);
            }
        }
        else
        {
            window = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(), nullptr, nullptr);
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, handleKeyboard);
    }

    if(windowInit)
        windowInit = false;
}

void showFPS(GLFWwindow* window)
{
    static double prevSeconds = 0.0;
    static int frameCount = 0;

    double elapsedSeconds;
    double currentSeconds = glfwGetTime();

    elapsedSeconds = currentSeconds - prevSeconds;
    if(elapsedSeconds > 0.25)
    {
        prevSeconds = currentSeconds;
        double fps = static_cast<double>(frameCount) / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
             << TITLE
             << " - "
             << "FPS: "
             << fps
             << " - "
             << "Frame time: "
             << msPerFrame
             << "(ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }
    frameCount++;
}
