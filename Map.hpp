#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

struct rawplane
{
    glm::vec3 point0;
    glm::vec3 point1;
    glm::vec3 point2;
    std::string texture_name;
    std::vector<float> texture_data;
};

struct rawbrush
{
    std::vector<rawplane> rawplanes;
};

struct poly
{
    std::vector<glm::vec3> vertexes;
    glm::vec3 normal;
    std::string tex;
    glm::vec3 center;
    int num;
};

struct brush
{
    std::vector<poly> polys;
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
    glm::vec3 dir;
    glm::vec3 point;
};

//ax+by+cz+d=0
struct plane{
	float a,b,c,d;
	glm::vec3 normal;
};

#endif
