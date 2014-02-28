#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

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

struct brush
{
    std::vector<poly> polys;
    std::vector<GLfloat> vertex_buffer_data;
    std::vector<GLfloat> color_buffer_data;
    std::vector<GLuint> element_buffer_data;    
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint elementbuffer;
};

struct header
{
    std::string variable;
    std::string value;
};

class entity
{
private:
    
public:
    
    entity();
    entity(std::vector<header> headers, std::vector<rawbrush> rawbrushes);
    std::vector<header> headers;
    std::vector<rawbrush> rawbrushes;
    std::vector<brush> brushes;
    void load_brushes();
};

class Map
{
public:
    std::vector<entity> entities;    
};

struct line{
    glm::dvec3 dir;
    glm::dvec3 point;
};

//ax+by+cz+d=0
struct plane{
	double a,b,c,d;
	glm::dvec3 normal;
};

#endif
