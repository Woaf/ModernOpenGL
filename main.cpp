#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const std::string vPath = "../ModernOpenGL/shaders/shader.vert";
static const std::string fPath = "../ModernOpenGL/shaders/shader.frag";

static const std::string TITLE = "Modern OpenGL";
static const unsigned int WIDTH = 1280;
static const unsigned int HEIGHT = 720;

static bool fullscreen = false;
static bool hasToSwitch = false;

std::string readShader(const std::string path);
bool initOpenGL(GLFWwindow*& window, bool& windowInit);
void showFPS(GLFWwindow* window);
void handleKeyboard(GLFWwindow* window, int key, int code, int action, int mode);
void checkForFullscreen(GLFWwindow*& window, bool& windowInit);
GLuint createProgram();

int main()
{
    GLFWwindow* window = nullptr;
    bool windowInit = true;
    if(!initOpenGL(window, windowInit))
        return -1;

    GLfloat vertices[] = {
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLuint VAO, VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);


    GLuint program = createProgram();

    while(!glfwWindowShouldClose(window))
    {
        showFPS(window);
        glfwPollEvents();

        checkForFullscreen(window, windowInit);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

std::string readShader(const std::string path)
{
    std::ifstream source(path);
    std::string content((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());

    return content;
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

GLuint createProgram()
{
    GLint result;
    GLchar info[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vShader = readShader(vPath);
    const char* v = vShader.c_str();
    glShaderSource(vertexShader, 1, &v, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(vertexShader, sizeof(info), nullptr, info);
        std::cout << "VERTEX SHADER ERROR:" << std::endl << info << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fShader = readShader(fPath);
    const char* f = fShader.c_str();
    glShaderSource(fragmentShader, 1, &f, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if(!result)
    {
        glGetShaderInfoLog(fragmentShader, sizeof (info), nullptr, info);
        std::cout << "FRAGMENT SHADER ERROR:" << std::endl << info << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if(!result)
    {
        glGetProgramInfoLog(program, sizeof(info), nullptr, info);
        std::cout << "PROGRAM LINK ERROR:" << std::endl << info << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

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
