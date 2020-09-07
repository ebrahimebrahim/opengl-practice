#define CATCH_CONFIG_MAIN
#include<catch.hpp>

#include<Shader.h>


TEST_CASE( "Factorials are computed", "[factorial]" ) {
    Shader s{};
    REQUIRE( s.shader_program_id == 0 );
}
