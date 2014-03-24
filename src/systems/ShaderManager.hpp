#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <GL/glew.h>
#include "parts/OpenGLShadersPart.hpp"
#include <map>

class ShaderManager
{
private:
   
    std::map<std::string, GLuint> mGLPrograms;

    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path,const char * geometry_file_path);

public:
    
    GLuint GetShaderProgram(const OpenGLShadersPart &shaders);


};

#endif
