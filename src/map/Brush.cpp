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

#include "Brush.hpp"
#include <algorithm>

struct Plane planepnormal(glm::dvec3 point, glm::dvec3 normal)
{
	Plane aux;
	aux.normal=normal;
	aux.a=normal.x;
	aux.b=normal.y;
	aux.c=normal.z;
	aux.d=-glm::dot(normal,point);
	return aux;
}
struct Plane plane3points(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3)
{
    glm::dvec3 aux;
	aux = glm::normalize(glm::cross(p3 - p1, p2 - p1));
	return planepnormal(p1,aux);
}

int inter2planes(Plane p1, Plane p2, line *l1)
{
    double s1, s2, a, b;
	double n1n2dot,n1normsqr,n2normsqr;
    glm::dvec3 d = glm::cross(p1.normal,p2.normal);
    if (glm::length(d) < 0.0001f) {
        return 0;
    }

    l1->dir = d;
    s1 = p1.d; // d from the plane equation
    s2 = p2.d;
    n1n2dot = glm::dot(p1.normal, p2.normal);
    n1normsqr = glm::dot(p1.normal, p1.normal);
    n2normsqr = glm::dot(p2.normal, p2.normal);
    a = (s2 * n1n2dot - s1 * n2normsqr) / (n1n2dot*n1n2dot - n1normsqr * n2normsqr);
    b = (s1 * n1n2dot - s2 * n1normsqr) / (n1n2dot*n1n2dot - n1normsqr * n2normsqr);

    l1->point = a * p1.normal + b * p2.normal;

    l1->point = (-1.0) * l1->point;

    return 1;
}

double dabs(double n)
{
    if(n<0) return n*(-1);
    return n;
}

int interlineplane(struct line line1,struct Plane plane1, Vertex &vertex)
{
    // Check for (near) parallel line and plane
    double denominator = glm::dot(line1.dir, plane1.normal);
    if (dabs(denominator) < 0.0001f) {
        return 0;
    }

// Nonparallel, so compute intersection
    double t = -(plane1.a * line1.point.x + plane1.b * line1.point.y + plane1.c * line1.point.z + plane1.d);
    t = t / denominator;
    vertex.pos = line1.point + t * line1.dir;
    return 1;
}



int pointinplane(glm::dvec3 point,struct Plane p)
{
	double res;
	res=p.a * point.x + p.b * point.y + p.c * point.z + p.d;
	if(res<1 && res>-1){
		return 0;
	}else{
		if(res<0){
			return -1;
		}else{
			return 1;
		}
	}
}

int comppoints(glm::dvec3 p1, glm::dvec3 p2){
	if((p1.x > p2.x-1 && p1.x < p2.x+1)&&(p1.y > p2.y-1 && p1.y < p2.y+1)&&(p1.z > p2.z-1 && p1.z < p2.z+1))
		return 1;
	return 0;
}

constexpr double pi() { return std::atan(1)*4; }


void brush::create_planes_from_points(rawbrush &b)
{
    for(rawplane &rp : b.rawplanes){
        poly p;
        p.plane = plane3points(rp.point0, rp.point1, rp.point2);
		p.normal = p.plane.normal;
		p.tex = rp.texture_name;
        polys.push_back(p);
    }
    
}

void brush::add_vertexes_to_polys()
{
    Vertex pointaux;
	line lineaux;
    for (auto it = polys.begin() ; it != polys.end(); ++it){
        for (auto it2 = it+1 ; it2 != polys.end(); ++it2){
			if(inter2planes(it->plane,it2->plane,&lineaux)){
                for (auto it3 = polys.begin() ; it3 != polys.end(); ++it3){
                    if(it3!=it && it3!=it2){
						if(interlineplane(lineaux,it3->plane,pointaux)){
                            it3->vertexes.push_back(pointaux);
						}                        
                    }
                }                
            }            
        }        
    }
}

void brush::remove_extra_vertexes()
{
    for(poly &p : polys){
        for (auto it = p.vertexes.begin() ; it != p.vertexes.end(); ++it){
            for (auto it2 = it+1 ; it2 != p.vertexes.end(); ++it2){
				if(comppoints(it->pos,it2->pos)){
                    p.vertexes.erase(it);
                    it--;
					break;
				}
			}
		}
	}

    for (auto it = polys.begin() ; it != polys.end(); ++it){
        for (auto it2 = it->vertexes.begin() ; it2 != it->vertexes.end(); ++it2){
            for (auto it3 = polys.begin() ; it3 != polys.end(); ++it3){
				if(it3!=it && pointinplane(it2->pos,it3->plane)==1){
                    it->vertexes.erase(it2);
					it2--;
					break;
				}
			}
		}
	}
}

void ordervertexes(struct poly &p)
{
    glm::dvec3 centro,p1,p2;
    Vertex vertexAux;
    double gdot;

    centro.x = 0;
    centro.y = 0;
    centro.z = 0;
    for(Vertex v : p.vertexes){
        centro.x += v.pos.x;
        centro.y += v.pos.y;
        centro.z += v.pos.z;
    }
    centro.x /= p.vertexes.size();
    centro.y /= p.vertexes.size();
    centro.z /= p.vertexes.size();

    p.center = centro;

    p1 = p.vertexes[0].pos - centro;

    p.vertexes[0].ang = 0;
    for(Vertex &v : p.vertexes){
        if(&v == &p.vertexes[0]) continue;

        p2 = v.pos - centro;
        v.ang = acos(glm::dot(p1,p2)/(glm::length(p1)*glm::length(p2)));
        glm::dvec3 pointaux = glm::cross(p1,p2);
        gdot = glm::dot(pointaux,p.normal);

        if(gdot>-0.01 && gdot< 0.01){
            v.ang = pi();
        }else{
            if(gdot<0){
                v.ang = 2*pi() - v.ang;
            }
        }
    }

    std::sort(p.vertexes.begin(),p.vertexes.end(),
              [](const Vertex &a, const Vertex &b) -> bool
              {
                  return a.ang > b.ang; 
              });

}

void brush::order_vertexes()
{
    for(poly &p : polys){
        ordervertexes(p);
	}
}

void brush::create_buffers()
{
    GLuint elem=0;
    for(poly &p : polys){
        glm::dvec3 v0 = p.vertexes[0].pos;
        glm::dvec3 v;
        for (auto it = p.vertexes.begin()+2 ; it != p.vertexes.end(); ++it){
            vertex_buffer_data.push_back((GLfloat)v0.x);
            vertex_buffer_data.push_back((GLfloat)v0.y);
            vertex_buffer_data.push_back((GLfloat)v0.z);
            element_buffer_data.push_back(elem++);

            v = (it-1)->pos;
            vertex_buffer_data.push_back((GLfloat)v.x);
            vertex_buffer_data.push_back((GLfloat)v.y);
            vertex_buffer_data.push_back((GLfloat)v.z);
            element_buffer_data.push_back(elem++);

            v = it->pos;
            vertex_buffer_data.push_back((GLfloat)v.x);
            vertex_buffer_data.push_back((GLfloat)v.y);
            vertex_buffer_data.push_back((GLfloat)v.z);
            element_buffer_data.push_back(elem++);

            for(int k=0;k<9;k++){
                color_buffer_data.push_back(1);
            }
        }

        GLfloat *vertex_buffer_datap = vertex_buffer_data.data();
        GLfloat *color_buffer_datap = color_buffer_data.data();
        GLuint *element_buffer_datap = element_buffer_data.data();

        glGenBuffers(1, &(vertexbuffer));
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex_buffer_data.size(), vertex_buffer_datap, GL_STATIC_DRAW);

        glGenBuffers(1, &(colorbuffer));
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*color_buffer_data.size(), color_buffer_datap, GL_STATIC_DRAW);

        glGenBuffers(1, &(elementbuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*element_buffer_data.size(), element_buffer_datap, GL_STATIC_DRAW);
    }
}
void brush::scale_vertexes(float scale)
{
    for(poly &p : polys){
        for(Vertex &v : p.vertexes){
            v.pos *= scale;
        }
    }
}

void brush::load(dynamicsWorldSP dynamicsWorld, rawbrush &rb, float scale)
{
    create_planes_from_points(rb);

    add_vertexes_to_polys();

    remove_extra_vertexes();

    order_vertexes();

    scale_vertexes(scale);

    create_buffers();

    convexHullShape = std::make_shared<btConvexHullShape>();
    for(poly &p : polys){
        for(Vertex &v : p.vertexes){
            convexHullShape->addPoint(btVector3((GLfloat)v.pos.x,(GLfloat)v.pos.y,(GLfloat)v.pos.z));
        }
    }
    groundMotionState = std::make_shared<btDefaultMotionState>();

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState.get(),convexHullShape.get(),btVector3(0,0,0));

    groundRigidBody = std::make_shared<btRigidBody>(groundRigidBodyCI);
    dynamicsWorld->addRigidBody(groundRigidBody.get());

}

void brush::draw()
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );


    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
        );
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        (GLsizei)element_buffer_data.size(),    // count
        GL_UNSIGNED_INT,   // type
        (void*)0           // element array buffer offset
        );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
