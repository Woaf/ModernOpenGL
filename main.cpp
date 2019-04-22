#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const std::string TITLE = "Modern OpenGL";
static const unsigned int WIDTH = 1280;
static const unsigned int HEIGHT = 720;

static bool fullscreen = false;
static bool hasToSwitch = false;

void handleKeyboard(GLFWwindow* window, int key, int code, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if(fullscreen)
        {
            fullscreen = !fullscreen;
            hasToSwitch = true;
        }
        else
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    if(key == GLFW_KEY_F && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
    {
        fullscreen = !fullscreen;
        hasToSwitch = true;
    }
}

void checkForFullscreen(GLFWwindow*& window, bool& windowInit)
{
    if(hasToSwitch || windowInit)
    {
        hasToSwitch = false;
        glfwDestroyWindow(window);
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

int main()
{
    GLFWwindow* window = nullptr;
    bool windowInit = true;
    if(!initOpenGL(window, windowInit))
        return -1;

    while(!glfwWindowShouldClose(window))
    {
        showFPS(window);
        glfwPollEvents();

        checkForFullscreen(window, windowInit);

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
