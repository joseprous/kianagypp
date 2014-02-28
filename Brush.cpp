#include "Brush.hpp"

/*void print_vec3(std::string name, glm::dvec3 v)
{
    std::cout << name << ": " << v.x << "," << v.y << "," << v.z << std::endl;
    }*/

unsigned int comb(unsigned int m, unsigned int n) {
    unsigned r = 1;
    unsigned d;
    if (n > m) return 0;
    for (d=1; d <= n; d++) {
        r *= m--;
        r /= d;
    }
    return r;
}

struct plane planepnormal(glm::dvec3 point, glm::dvec3 normal)
{
	struct plane aux;
	aux.normal=normal;
	aux.a=normal.x;
	aux.b=normal.y;
	aux.c=normal.z;
	aux.d=-glm::dot(normal,point);
	return aux;
}
struct plane plane3points(glm::dvec3 p1, glm::dvec3 p2, glm::dvec3 p3)
{
    glm::dvec3 aux;
	aux = glm::normalize(glm::cross(p3 - p1, p2 - p1));
	return planepnormal(p1,aux);
}

int inter2planes(plane p1, plane p2, line *l1)
{
    double s1, s2, a, b;
	double n1n2dot,n1normsqr,n2normsqr;
    glm::dvec3 d = glm::cross(p1.normal,p2.normal);
    if (glm::length(d) == 0) {
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
    
int interlineplane(struct line line1,struct plane plane1, glm::dvec3 &point)
{  
    // Check for (near) parallel line and plane
    double denominator = glm::dot(line1.dir, plane1.normal);
    if (dabs(denominator) == 0) {
        return 0;
    }

// Nonparallel, so compute intersection
    double t = -(plane1.a * line1.point.x + plane1.b * line1.point.y + plane1.c * line1.point.z + plane1.d);
    t = t / denominator;
    point = line1.point + t * line1.dir;
    return 1;
}



int pointinplane(glm::dvec3 point,struct plane p)
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


void brush::create_planes_from_points(rawbrush &b, std::vector<plane> &planes)
{
	int maxvert=(int)comb(num-1,2)+1;
	for(int i=0;i<num;i++){
		planes[i]=plane3points(b.rawplanes[i].point0, b.rawplanes[i].point1, b.rawplanes[i].point2);
        
		polys[i].normal=planes[i].normal;
		polys[i].tex=b.rawplanes[i].texture_name;
        polys[i].vertexes.resize(maxvert);
		polys[i].num=0;
	}
}

void brush::add_vertexes_to_polys(std::vector<plane> &planes)
{
    glm::dvec3 pointaux;
	line lineaux;
    for(int i=0;i<num;i++){
		for(int j=i+1;j<num;j++){
			if(inter2planes(planes[i],planes[j],&lineaux)){
				for(int k=0;k<num;k++){
					if(k!=i && k!=j){
						if(interlineplane(lineaux,planes[k],pointaux)){
							polys[k].vertexes[polys[k].num]=pointaux;
							polys[k].num++;						
						}
					}	
				}
			}
		}
    }
}

void brush::remove_extra_vertexes(std::vector<plane> &planes)
{
	for(int i=0;i<num;i++){
		for(int j=0;j<polys[i].num;j++){
			for(int k=j+1;k<polys[i].num;k++){
				if(comppoints(polys[i].vertexes[j],polys[i].vertexes[k])){
					for(int c=j;c<polys[i].num-1;c++){
						polys[i].vertexes[c]=polys[i].vertexes[c+1];
					}
					polys[i].num--;
					j--;
					break;					
				}
			}			
		}
	}
	
	for(int i=0;i<num;i++){
		for(int j=0;j<polys[i].num;j++){
			for(int k=0;k<num;k++){
				if(k!=i && pointinplane(polys[i].vertexes[j],planes[k])==1){
					for(int c=j;c<polys[i].num-1;c++){
						polys[i].vertexes[c]=polys[i].vertexes[c+1];
					}
					polys[i].num--;
					j--;
					break;
				}
			}
		}
	}    
}

void ordervertexes(struct poly *p)
{
    glm::dvec3 centro,p1,p2,pointaux;
    int i,ban;
    float *angs;
    float gdot,ga;
  
    angs=(float *)malloc(sizeof(float)*p->num);
    centro.x=0;
    centro.y=0;
    centro.z=0;
    for(i=0;i<p->num;i++){
        centro.x+=p->vertexes[i].x;
        centro.y+=p->vertexes[i].y;
        centro.z+=p->vertexes[i].z;
    }
    centro.x/=p->num;
    centro.y/=p->num;
    centro.z/=p->num;

    p->center=centro;
		
    p1 = p->vertexes[0] - centro;
    for(i=1;i<p->num;i++){			
        p2 = p->vertexes[i] - centro;
        angs[i] = acos(glm::dot(p1,p2)/(glm::length(p1)*glm::length(p2)));
        pointaux = glm::cross(p1,p2);
        gdot = glm::dot(pointaux,p->normal);
			
        if(gdot>-0.01 && gdot< 0.01){
            angs[i]=pi();					
        }else{
            if(gdot<0){
                angs[i]=2*pi()-angs[i];					
            }
        }
    }
    do{
        ban=0;
        for(i=1;i<p->num-1;i++){
            if(angs[i]>angs[i+1]){
                ga=angs[i];
                angs[i]=angs[i+1];
                angs[i+1]=ga;
                pointaux=p->vertexes[i];
                p->vertexes[i]=p->vertexes[i+1];
                p->vertexes[i+1]=pointaux;
                ban=1;
            }	
        }
    }while(ban);
    free(angs);			  
}


void brush::order_vertexes()
{
	for(int i=0;i<num;i++){
        ordervertexes(&(polys[i]));
	}    
}

void brush::create_buffers()
{
    GLuint elem=0;
    for(int i=0;i<num;i++){
        glm::dvec3 v;
        
        for(int j=2;j<polys[i].num;j++){
            v = polys[i].vertexes[0];
            vertex_buffer_data.push_back(v.x);
            vertex_buffer_data.push_back(v.y);
            vertex_buffer_data.push_back(v.z);
            element_buffer_data.push_back(elem++);

            v = polys[i].vertexes[j-1];
            vertex_buffer_data.push_back(v.x);
            vertex_buffer_data.push_back(v.y);
            vertex_buffer_data.push_back(v.z);
            element_buffer_data.push_back(elem++);
            
            v = polys[i].vertexes[j];
            vertex_buffer_data.push_back(v.x);
            vertex_buffer_data.push_back(v.y);
            vertex_buffer_data.push_back(v.z);
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

void brush::load(rawbrush &rb)
{
    num = rb.rawplanes.size();
	//brush aux;
    std::vector<plane> planes(num);
	
    polys.resize(num);
    
    create_planes_from_points(rb,planes);
    
    add_vertexes_to_polys(planes);

    remove_extra_vertexes(planes);

    order_vertexes();

    create_buffers();
}

void brush::draw(GLuint programID, glm::mat4 projection, glm::mat4 view)
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
        element_buffer_data.size(),    // count
        GL_UNSIGNED_INT,   // type
        (void*)0           // element array buffer offset
        );
            
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
