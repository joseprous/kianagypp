/*
kianagy++
Copyright (C) 2014 José Prous

This file is part of kianagy++.

kianagy++ is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

kianagy++ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with kianagy++.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BRUSH_HPP
#define BRUSH_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

struct line{
    glm::dvec3 dir;
    glm::dvec3 point;
};

//ax+by+cz+d=0
struct Plane{
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

struct Vertex
{
    glm::dvec3 pos;
    double ang;
    Vertex(){}
    Vertex(glm::dvec3 position):pos(position){}
};

struct poly
{
    std::vector<Vertex> vertexes;
    Plane plane;
    glm::dvec3 normal;
    std::string tex;
    glm::dvec3 center;
};

typedef std::vector<poly> Mesh;

class brush
{
public:
    
    void load(const rawbrush &rb, float scale);
    const Mesh& getMesh() const {return mesh;}

private:
   
    Mesh mesh;
    
    void order_vertexes();
    void remove_extra_vertexes();
    void add_vertexes_to_mesh();
    void create_planes_from_points(const rawbrush &b);
    void scale_vertexes(float scale);

};

#endif

