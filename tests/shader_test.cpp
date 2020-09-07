#include<catch.hpp>

#include"opengl_context_setup.h"
#include <utility>

#include<Shader.h>



TEST_CASE("Shader default ctor works","[Shader]") {
  Shader s{};
  REQUIRE( s.shader_program_id == 0 );
}

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
