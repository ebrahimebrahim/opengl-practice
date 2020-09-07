#ifndef OPENGL_CONTEXT_SETUP_FOR_TEST
#define OPENGL_CONTEXT_SETUP_FOR_TEST

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

void glfw_error_callback(int error, const char* description){
    std::cerr << "GLFW Error: " << description << std::endl;
}

GLFWwindow* setupOpenGL(){
  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit()==GLFW_FALSE)
      throw std::runtime_error("GLFW failed to initialize.");

  // GLFW doesn't support creating contexts without an associated window.
  // However, contexts with hidden windows can be created with the GLFW_VISIBLE window hint.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  GLFWwindow* offscreen_context = glfwCreateWindow(640, 480, "", NULL, NULL);
  if (!offscreen_context)
      throw std::runtime_error("Failed to create GLFW window.");

  glfwMakeContextCurrent(offscreen_context);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
      throw std::runtime_error("Failed to initialize GLAD.");

  return offscreen_context;
}

void teardownOpenGL(GLFWwindow* window){
  glfwDestroyWindow(window);
  glfwTerminate();
}

#endif
