#ifndef OPENGLMESHPART_HPP 
#define OPENGLMESHPART_HPP

#include "../map/Brush.hpp"

struct OpenGLMeshPart
{
    std::vector<GLfloat> vertex_buffer_data;
    std::vector<GLfloat> color_buffer_data;
    std::vector<GLuint> element_buffer_data;    
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint elementbuffer;
};

#endif
