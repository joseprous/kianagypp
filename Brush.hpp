#ifndef BRUSH_HPP
#define BRUSH_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct line{
    glm::dvec3 dir;
    glm::dvec3 point;
};

//ax+by+cz+d=0
struct plane{
	double a,b,c,d;
	glm::dvec3 normal;
};

struct rawplane
{
    glm::dvec3 point0;
    glm::dvec3 point1;
    glm::dvec3 point2;
    std::string texture_name;
    std::vector<double> texture_data;
};

struct rawbrush
{
    std::vector<rawplane> rawplanes;
};

struct poly
{
    std::vector<glm::dvec3> vertexes;

    glm::dvec3 normal;
    std::string tex;
    glm::dvec3 center;
    int num;
};

class brush
{
public:
    
    void load(rawbrush &rb);
    void draw(GLuint programID, glm::mat4 projection, glm::mat4 view);

private:
    
    std::vector<poly> polys;
    std::vector<GLfloat> vertex_buffer_data;
    std::vector<GLfloat> color_buffer_data;
    std::vector<GLuint> element_buffer_data;    
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint elementbuffer;
    int num;

    void create_buffers();
    void order_vertexes();
    void remove_extra_vertexes(std::vector<plane> &planes);
    void add_vertexes_to_polys(std::vector<plane> &planes);
    void create_planes_from_points(rawbrush &b, std::vector<plane> &planes);

};

#endif

