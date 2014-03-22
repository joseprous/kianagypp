#ifndef OPENGLSHADERSPART_HPP
#define OPENGLSHADERSPART_HPP

#include <GL/gl.h>
#include <string>

struct OpenGLShadersPart
{
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::string geometryShaderPath;
    std::string key;   
};

#endif
