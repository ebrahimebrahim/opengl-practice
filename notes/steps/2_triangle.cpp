#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

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

std::string fileToString(const char* filename){
            std::ifstream is(filename);
            if (!is.is_open()){
                std::string error_msg("Could not open file ");
                error_msg.append(filename);
                error_msg.append(".");
                throw std::runtime_error(error_msg);
            }
            std::stringstream ss;
            ss << is.rdbuf();
            return ss.str();
}


class Application {
public:
    void run() {
        initWindow();

        setupShaders();
        setupTriangleVBOandVAO();

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

    std::string vertexShaderSource;
    std::string fragmentShaderSource;
    GLuint shaderProgram = 0;
    GLuint VBO = 0;
    GLuint VAO = 0;



    void setupShaders(){
        //load and compile vertex shader
        vertexShaderSource = fileToString("shader.vert");
        const char * vertexShaderSource_cstring = vertexShaderSource.c_str();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        if (!vertexShader) throw std::runtime_error("OpenGL failed to create vertex shader.");
        glShaderSource(vertexShader,1,&vertexShaderSource_cstring,nullptr);
        glCompileShader(vertexShader);
        GLint compileSuccess;
        constexpr short infoLogSize = 512;
        char infoLog[infoLogSize];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileSuccess);
        if (compileSuccess == GL_FALSE){
            glGetShaderInfoLog(vertexShader,infoLogSize,nullptr,infoLog);
            std::string error_msg("Vertex shader compiler error:\n");
            error_msg.append(infoLog);
            throw std::runtime_error(error_msg);
        }

        //load and compile fragment shader
        fragmentShaderSource = fileToString("shader.frag");
        const char * fragmentShaderSource_cstring = fragmentShaderSource.c_str();
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        if (!fragmentShader) throw std::runtime_error("OpenGL failed to create fragment shader.");
        glShaderSource(fragmentShader,1,&fragmentShaderSource_cstring,nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileSuccess);
        if (compileSuccess == GL_FALSE){
            glGetShaderInfoLog(fragmentShader,infoLogSize,nullptr,infoLog);
            std::string error_msg("Fragment shader compiler error:\n");
            error_msg.append(infoLog);
            throw std::runtime_error(error_msg);
        }

        //link shaders
        shaderProgram = glCreateProgram();
        if (!shaderProgram) throw std::runtime_error("OpenGL failed to create a program object in which to link shaders.");
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        GLint linkSuccess;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess == GL_FALSE){
            glGetProgramInfoLog(shaderProgram,infoLogSize,nullptr,infoLog);
            std::string error_msg("Shader program linking error:\n");
            error_msg.append(infoLog);
            throw std::runtime_error(error_msg);
        }
        
        //clean up, these are already linked in shaderProgram
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        //use this shaderProgram for future drawing commands
        glUseProgram(shaderProgram);
    }


    void setupTriangleVBOandVAO(){
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
       
        // We could unbind the VAO and only bind it when we want to draw the triangle, inside the render loop.
        // This might be safer so we dont overwrite the VAO accidentally.
        // But here the triangle is the only object we want to draw, so it doesn't matter.
        // glBindVertexArray(0);
    }


    void render() {
        glClearColor(0.3,0.2,0.2,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES,0,3);
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
