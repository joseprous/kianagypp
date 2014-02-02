#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

struct rawplane
{
    glm::vec3 point1;
    glm::vec3 point2;
    glm::vec3 point3;
    std::string texture_name;
    std::vector<float> texture_data;
};

struct rawbrush{
    std::vector<rawplane> planes;
};

struct header{
    std::string variable;
    std::string value;
};

struct entity{
    std::vector<header> headers;
    std::vector<rawbrush> brushes;
};

struct Map{
    std::vector<entity> entities;    
};


#endif
