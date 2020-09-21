#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);


class Application {
  public:

    Application() {
      glfwSetErrorCallback(glfw_error_callback);
      if (glfwInit()==GLFW_FALSE)
          throw std::runtime_error("GLFW failed to initialize.");

      // We don't plan to use compatibility features
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      // Need at least OpenGL version 3.3
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

      window = glfwCreateWindow(WIDTH, HEIGHT, "Peup", nullptr, nullptr);
      if (!window)
          throw std::runtime_error("Failed to create GLFW window.");

      glfwSetKeyCallback(window, glfw_key_callback);
      glfwSetFramebufferSizeCallback(window, glfw_resize_callback);

      glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
      glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
      double initial_cursor_xpos, initial_cursor_ypos;
      glfwGetCursorPos(window, &initial_cursor_xpos, &initial_cursor_ypos);
      cursor = glm::vec2(initial_cursor_xpos,initial_cursor_ypos);


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

    
    ~Application() {
      glfwDestroyWindow(window);
      glfwTerminate();
    }


    void run() {
        Shader shader{"src/shaders/shader.vert","src/shaders/shader.frag"};
        shader.use();
        setupTriangleVAO();

        stbi_set_flip_vertically_on_load(true);
        int width, height, n_channels;
        unsigned char * texture_data = stbi_load("assets/tex_test.png",&width,&height,&n_channels,0);
        if (!texture_data){
          std::stringstream error_msg;
          error_msg << "Error loading texture file. stb_image error message: " << stbi_failure_reason();
          throw std::runtime_error(error_msg.str());
        }
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,  GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texture_data);


        glm::mat4 model = glm::mat4(1.0f);

        // glm::mat4 view = glm::mat4(1.0f);
        // view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
        // Moving everything FORWARD (-z dir) by 3 is effectively us moving BACK by 4

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),float(WIDTH)/float(HEIGHT),0.1f,100.0f);


        glEnable(GL_DEPTH_TEST);


        // -- Setting up a camera

        auto cameraPos = glm::vec3(0.0f,0.0f,3.0f); // cam origin
        auto cameraTarget = glm::vec3(0.0f,0.0f,0.0f);
        auto cameraNegDirection = glm::normalize(cameraPos - cameraTarget); // cam z axis
        auto cameraDir = - cameraNegDirection;
        auto cameraRight = glm::normalize(glm::cross(glm::vec3(0.0,1.0,0.0),cameraNegDirection)); // cam x axis
        auto cameraUp = glm::normalize(glm::cross(cameraNegDirection,cameraRight)); // cam y axis
        std::cout
          << "Right: " << glm::to_string(cameraRight) << "\n"
          << "Up: "    << glm::to_string(cameraUp) << "\n"
          << "Back: "  << glm::to_string(cameraNegDirection) << "\n"
          << "Pos: "   << glm::to_string(cameraPos) << "\n\n";

        //But here is an easier way to jump right to view matrix using cameraPos and cameraTarget:
        glm::mat4 view = glm::lookAt(cameraPos,cameraTarget,glm::vec3(0.0,1.0,0.0));
        std::cout << "And here is glm::lookAt matrix straight from initial data:\n"
          << glm::to_string(view) << "\n";
        std::cout << "Note that when we print this it lists the *columns*\n";
        // Note: now we are actually just going to recompute view matrix at each frame


        // -- End setting up camera


        // I guess this one wont change much so let's set it once here
        shader.setUniform("projection",projection);


        float time_at_previous_frame = 0.0f;
        glm::vec2 cursor_at_previous_frame = cursor;
        while (!glfwWindowShouldClose(window)) {
          float current_time = float(glfwGetTime());
          float delta = current_time-time_at_previous_frame;
          time_at_previous_frame = current_time;
          // std::cout << "FPS: " << 1.0f/delta << "\n";
          glm::vec2 mouse_delta = cursor - cursor_at_previous_frame;
          cursor_at_previous_frame = cursor;



          glClearColor(0.3,0.2,0.2,1.0);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          float greenValue = sin(glfwGetTime()) / 2.0f + 0.5f;
          shader.setUniform("colorFromApplication",{0.0f, greenValue, 0.0f, 1.0f});

          // If we had multiple VAOs, we would bind the one we want before drawing here
          // If we had multiple textures, we would also bind the one we want here
          // (we would do this for each texture unit, if what we are drawing using multiple texture units)
          // If we had multiple shader programs, we would *use* one here
          // If our uniform variables needed to change, we would update them here.
          model = glm::mat4(1.0f);
          model = glm::rotate(model,glm::radians(50.0f),glm::vec3(0.0f,1.0f,0.0f));
          model = glm::rotate(model, float(glfwGetTime()) * glm::radians(20.0f),glm::vec3(0.0f,1.0f,0.0f));

          moveCamera(delta,mouse_delta,cameraPos,cameraDir);
          view = glm::lookAt(cameraPos,cameraPos+cameraDir,glm::vec3(0.0,1.0,0.0));

          shader.setUniform("model",model);
          shader.setUniform("view",view);
          glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);

          glfwSwapBuffers(window);
          glfwPollEvents();
        }


        glDeleteTextures(1,&texture);
        deleteTriangleBufferData();
    }

    void set_cursor_pos(const double& xpos, const double& ypos){
      cursor = glm::vec2(xpos,ypos);
    }

  private:
    GLFWwindow* window = nullptr;
    const uint32_t WIDTH  = 800;
    const uint32_t HEIGHT = 600;

    glm::vec2 cursor;

    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;
    GLuint texture;





    void setupTriangleVAO(){
      glGenVertexArrays(1,&VAO);
      glBindVertexArray(VAO);

      glGenBuffers(1,&VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      float vertices[] = {
        -0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
         0.7f,  0.7f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f
      };
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glGenBuffers(1,&EBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      GLuint indices[] = {0,1,2,2,3,1};
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
      glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
      glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
      glEnableVertexAttribArray(2);

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

    // move camera position and camera direction one frame's worth
    // based on the current glfw input state in the window
    void moveCamera(float delta, glm::vec2 mouse_delta, glm::vec3 & pos, glm::vec3 & dir) {
      float walkSpeed = 2.0f;
      float mouseSensitivity = 0.0015f;
      float walkDist = delta * walkSpeed;
      glm::vec3 camRight = glm::normalize(glm::cross(dir,glm::vec3(0.0,1.0,0.0)));
      glm::vec3 worldUp  = glm::vec3(0.0f,1.0f,0.0f);
      if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        pos += walkDist * dir;
      if (glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        pos -= walkDist * dir;
      if (glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        glm::vec3 camRight = glm::normalize(glm::cross(dir,worldUp));
        pos -= walkDist * camRight;
      }
      if (glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        pos += walkDist * camRight;
      }

      // set yaw
      dir = glm::rotate(dir,-mouseSensitivity*mouse_delta[0],worldUp);

      //set pitch
      glm::vec3 new_dir = glm::rotate(dir,-mouseSensitivity*mouse_delta[1],camRight);
      if (new_dir[0]*dir[0] + new_dir[2]*dir[2] > 0.1f) // don't allow pitch beyond zenith/nadir
        dir = new_dir;
    }


};

// application is now a global :(
Application app;

void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  // std::cout << "Cursor position: (" << xpos << " , " << ypos << ")\n";
  app.set_cursor_pos(xpos,ypos);
}


int main() {

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
