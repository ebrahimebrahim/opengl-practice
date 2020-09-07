#define CATCH_CONFIG_MAIN
#include<catch.hpp>

#include<Shader.h>


TEST_CASE("Shader default ctor works","[Shader]") {
    Shader s{};
    REQUIRE( s.shader_program_id == 0 );
}


Shader return_a_shader() {return Shader("tests/shaders/t.vert","tests/shaders/t.frag");}
TEST_CASE("Shader move works","[Shader]") {
    Shader s{return_a_shader()}; // Move ctor
    REQUIRE( s.shader_program_id != 0 );

    Shader s2{};
    s2 = return_a_shader(); // Move assignement
    REQUIRE( s2.shader_program_id != 0 );
}
