#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const std::string TITLE = "Modern OpenGL";
static const unsigned int WIDTH = 1280;
static const unsigned int HEIGHT = 720;

static bool fullscreen = false;

void handleKeyboard(GLFWwindow* window, int key, int code, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
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
             << "  "
             << "FPS: "
             << fps
             << "  "
             << "Frame time: "
             << msPerFrame
             << "(ms)";
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }
    frameCount++;
}

int main()
{
    if(!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = nullptr;
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

    if(window == nullptr)
    {
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, handleKeyboard);
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }

    while(!glfwWindowShouldClose(window))
    {
        showFPS(window);
        glfwPollEvents();

        glClearColor(0.23f, 0.23f, 0.65f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
