#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>

#include "Shader.h"


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

        Shader shader{"shader.vert","shader.frag"};
        setupTriangleVAO();

        shader.use();
        while (!glfwWindowShouldClose(window)) {

          glClearColor(0.3,0.2,0.2,1.0);
          glClear(GL_COLOR_BUFFER_BIT);

          float greenValue = sin(glfwGetTime()) / 2.0f + 0.5f;
          // TODO Setting a uniform, as done below, should really be done via a Shader member function
          GLint vertexColorLocation = glGetUniformLocation(shader.shader_program_id,"colorFromApplication");
          if (vertexColorLocation == -1) throw std::runtime_error("Application has peuped while looking up a uniform variable.");
          glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

          glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);

          glfwSwapBuffers(window);
          glfwPollEvents();
        }

        deleteTriangleBufferData();

        cleanupGLFW();
    }

  private:
    GLFWwindow* window = nullptr;
    const uint32_t WIDTH  = 800;
    const uint32_t HEIGHT = 600;

    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;

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

    void cleanupGLFW() {
      glfwDestroyWindow(window);
      glfwTerminate();
    }




    void setupTriangleVAO(){
      glGenVertexArrays(1,&VAO);
      glBindVertexArray(VAO);

      glGenBuffers(1,&VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.7f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glGenBuffers(1,&EBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      GLuint indices[] = {0,1,2,2,3,1};
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
      glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);

      // We could unbind the VAO and only bind it when we want to draw the triangle, inside the render loop.
      // This might be safer so we dont overwrite the VAO accidentally.
      // But here the triangle is the only object we want to draw, so it doesn't matter.
      // glBindVertexArray(0);
    }

    void deleteTriangleBufferData(){
      glDeleteVertexArrays(1,&VAO);
      glDeleteBuffers(1,&VBO);
      glDeleteBuffers(1,&EBO);
      VAO=0;
      VBO=0;
      EBO=0;
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
