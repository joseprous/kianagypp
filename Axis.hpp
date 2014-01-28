#ifndef AXIS_HPP
#define AXIS_HPP

#include "Entity.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

class Axis : public Entity
{
private:
    
    GLfloat g_vertex_buffer_data[3*3*2] = {
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f
    };
    
    GLfloat g_color_buffer_data[3*3*2] = {
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f
    };
    
public:

    GLuint vertexbuffer;
    GLuint colorbuffer;
    float size;
    GLuint programID;

    Axis(GLuint programID, float size);
    void Draw(glm::mat4 projection, glm::mat4 view, glm::mat4 translate, glm::mat4 orientation);
    void Draw();
};

#endif
