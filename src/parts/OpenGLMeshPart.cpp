#include "OpenGLMeshPart.hpp"

#include "Logger.hpp"

OpenGLMeshPart create_glmesh(const Mesh &mesh)
{
    OpenGLMeshPart glMesh;

    GLuint elem=0;
    for(const poly &p : mesh){
        glm::dvec3 v0 = p.vertexes[0].pos;
        glm::dvec3 v;
        for (auto it = p.vertexes.begin()+2 ; it != p.vertexes.end(); ++it){
            glMesh.vertex_buffer_data.push_back((GLfloat)v0.x);
            glMesh.vertex_buffer_data.push_back((GLfloat)v0.y);
            glMesh.vertex_buffer_data.push_back((GLfloat)v0.z);
            glMesh.element_buffer_data.push_back(elem++);

            v = (it-1)->pos;
            glMesh.vertex_buffer_data.push_back((GLfloat)v.x);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.y);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.z);
            glMesh.element_buffer_data.push_back(elem++);

            v = it->pos;
            glMesh.vertex_buffer_data.push_back((GLfloat)v.x);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.y);
            glMesh.vertex_buffer_data.push_back((GLfloat)v.z);
            glMesh.element_buffer_data.push_back(elem++);

            for(int k=0;k<9;k++){
                glMesh.color_buffer_data.push_back(1);
            }
        }
    }

    /*for(auto v : glMesh.vertex_buffer_data){
        Log(v);
        }*/
    
    GLfloat *vertex_buffer_datap = glMesh.vertex_buffer_data.data();
    GLfloat *color_buffer_datap = glMesh.color_buffer_data.data();
    GLuint *element_buffer_datap = glMesh.element_buffer_data.data();

    glGenBuffers(1, &(glMesh.vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, glMesh.vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*glMesh.vertex_buffer_data.size(), vertex_buffer_datap, GL_STATIC_DRAW);

    glGenBuffers(1, &(glMesh.colorbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, glMesh.colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*glMesh.color_buffer_data.size(), color_buffer_datap, GL_STATIC_DRAW);

    glGenBuffers(1, &(glMesh.elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*glMesh.element_buffer_data.size(), element_buffer_datap, GL_STATIC_DRAW);
    return glMesh;
}
