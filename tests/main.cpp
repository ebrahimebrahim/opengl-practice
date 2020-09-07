#define CATCH_CONFIG_MAIN
#include<catch.hpp>

// For OpenGL context setup
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// standard library
#include <utility>

// Individual units being tested
#include<Shader.h>


TEST_CASE("Shader default ctor works","[Shader]") {
  Shader s{};
  REQUIRE( s.shader_program_id == 0 );
}


// --- OpenGL context setup
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
// ------------------------


Shader return_a_shader() {return Shader("tests/shaders/t.vert","tests/shaders/t.frag");}
bool shader_is_live(const Shader & s) {return s.shader_program_id != 0;}
TEST_CASE("Shader move works","[Shader]") {
  GLFWwindow* offscreen_context = setupOpenGL();

  SECTION("move constructor ends up with live target"){
    Shader s{return_a_shader()}; // Move ctor
    REQUIRE( shader_is_live(s) );
  }

  SECTION("move assignement operator ends up with live target"){
    Shader s{};
    s = return_a_shader(); // Move assignement
    REQUIRE( shader_is_live(s) );
  }

  SECTION("moving takes resource away from source"){
    Shader s = return_a_shader();
    REQUIRE( shader_is_live(s) );
    Shader s2 = std::move(s); // move ctor
    REQUIRE( !shader_is_live(s) );
    REQUIRE( shader_is_live(s2) );

    s = return_a_shader();
    s2 = std::move(s); // move assignment
    REQUIRE( !shader_is_live(s) );
    REQUIRE( shader_is_live(s2) );
  }

  teardownOpenGL(offscreen_context);
}
