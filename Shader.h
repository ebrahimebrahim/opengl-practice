#ifndef SHADER_H
#define SHADER_H

#include "glad/include/glad/glad.h"

#include <string>
#include <stdexcept>

class Shader {
    public:

        //The shader program's ID in OpenGL
        GLuint shader_program_id = 0;

        // Throws runtime errors when files not found or there are compiling/linking issues
        Shader(std::string vertex_shader_path, std::string fragment_shader_path);
        ~Shader();
        // TODO: Create default ctor, copy ctor, move ctor, copy ass, mov asst

        // Tell OpenGL that we are now using this shader
        void use() const;

        // TODO Create family of overloaded "smart" uniform setters
        // void setUniform(std::string_view name,  std::vector<TYPE>  value) const;
};



#endif
