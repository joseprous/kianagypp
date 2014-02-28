#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Brush.hpp"

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
    
    void draw(GLuint programID, glm::mat4 projection, glm::mat4 view);
    
    std::vector<entity> entities;    
};

Map load_map(std::string map_file);

#endif
