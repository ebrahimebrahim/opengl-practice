#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

void glfw_error_callback(int error, const char* description){
    std::cerr << "GLFW Error: " << description << std::endl;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key==GLFW_KEY_ESCAPE && action==GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void glfw_resize_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}


class Application {
public:
    void run() {
        initWindow();
        mainLoop();
        cleanup();
    }

private:
    void initWindow() {
        glfwSetErrorCallback(glfw_error_callback);
        if (glfwInit()==GLFW_FALSE)
            throw std::runtime_error("GLFW failed to initialize.");

        // We don't plan to use compatibility features; core is sufficient.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Need at least OpenGL version 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Peup", nullptr, nullptr);
        if (!window) 
            throw std::runtime_error("Failed to create GLFW window.");

        glfwSetKeyCallback(window, glfw_key_callback);
        glfwSetFramebufferSizeCallback(window, glfw_resize_callback);

        // make opengl context current in the window
        glfwMakeContextCurrent(window);

        // glad knows how to during-runtime find the right OpenGL functions in a particular environment
        // somehow this line brings in the right OpenGL functions for us to use...
        // but I really don't see what is the connection to GLFW here and who is doing what exactly...
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            throw std::runtime_error("Failed to initialize GLAD.");

        // vsync
        glfwSwapInterval(1);

        glViewport(0,0,WIDTH,HEIGHT);
    }


    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {

            render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* window = nullptr;
    const uint32_t WIDTH  = 800;
    const uint32_t HEIGHT = 600;

    void render() {
        glClearColor(0.3,0.2,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

};

int main() {
    Application app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
