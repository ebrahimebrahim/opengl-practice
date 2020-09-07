#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <stdexcept>

class Shader {
    public:

        //The shader program's ID in OpenGL
        GLuint shader_program_id = 0;

        // Throws runtime errors when files not found or there are compiling/linking issues
        Shader(std::string vertex_shader_path, std::string fragment_shader_path);
        Shader() = default;
        Shader(const Shader&) = delete; // Do not copy. It manages a global OpenGL resource.
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&);
        Shader& operator=(Shader&&);
        ~Shader();


        // Tell OpenGL that we are now using this shader
        void use() const;

        // TODO Create family of overloaded "smart" uniform setters
        // void setUniform(std::string_view name,  std::vector<TYPE>  value) const;
};



#endif
