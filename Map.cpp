#include "Map.hpp"

#include <iostream>
#include <string>

void print_vec3(std::string name, glm::dvec3 v)
{
    std::cout << name << ": " << v.x << "," << v.y << "," << v.z << std::endl;
}

entity::entity(){}

entity::entity(std::vector<header> headers, std::vector<rawbrush> rawbrushes)
{
    this->headers = headers;
    this->rawbrushes = rawbrushes;
}

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

/*int interlineplane(struct line line1,struct plane plane1, glm::dvec3 &point)
{
    glm::dvec3 p1,p2;
    double u,d;
	p1 = line1.point;
	p2 = line1.point + line1.dir;
	d = plane1.a*(p1.x-p2.x)+plane1.b*(p1.y-p2.y)+plane1.c*(p1.z-p2.z);
	if(d < -0.01 || d > 0.01){
		u=(plane1.a*p1.x+plane1.b*p1.y+plane1.c*p1.z+plane1.d)/d;
		point = p1 * (u * (p2 - p1));
		return 1;
	}
	return 0;
}*/
double dabs(double n)
{
    if(n<0) return n*(-1);
    return n;
}
    
int interlineplane(struct line line1,struct plane plane1, glm::dvec3 &point)
{
    //print_vec3("line1.dir",line1.dir);
    //print_vec3("plane1.normal",plane1.normal);
   
    // Check for (near) parallel line and plane
    double denominator = glm::dot(line1.dir, plane1.normal);
    //std::cout << "denominator: " << denominator << std::endl;    
    if (dabs(denominator) == 0) {
        //std::cout << "no intersection" << std::endl;
        return 0;
    }
    //std::cout << "intersection" << std::endl;

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
	//printf("res:%lf\n",res);		
}

int comppoints(glm::dvec3 p1, glm::dvec3 p2){
	if((p1.x > p2.x-1 && p1.x < p2.x+1)&&(p1.y > p2.y-1 && p1.y < p2.y+1)&&(p1.z > p2.z-1 && p1.z < p2.z+1))
		return 1;
	return 0;
}

constexpr double pi() { return std::atan(1)*4; }

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


brush load_brush(rawbrush b)	
{
    int num = b.rawplanes.size();
	brush aux;
    std::vector<plane> planes(num);
	line lineaux;
    glm::dvec3 pointaux;
	int maxvert;
	int i,j,k,c;
	
	maxvert=(int)comb(num-1,2)+1;

    aux.polys.resize(num);
    
	/**********Ini crear planos ***********/
	for(i=0;i<num;i++){
/*        print_vec3("point0",b.rawplanes[i].point0);
        print_vec3("point1",b.rawplanes[i].point1);
        print_vec3("point2",b.rawplanes[i].point2);*/
		planes[i]=plane3points(b.rawplanes[i].point0, b.rawplanes[i].point1, b.rawplanes[i].point2);
        //std::cout << "plane: " << planes[i].a << "x + " << planes[i].b << "y + " << planes[i].c << "z + " << planes[i].d << " = 0 ";
        //print_vec3("normal",planes[i].normal);
        
		aux.polys[i].normal=planes[i].normal;
		aux.polys[i].tex=b.rawplanes[i].texture_name;
        aux.polys[i].vertexes.resize(maxvert);
		aux.polys[i].num=0;
	}
	/**********Fin crear planos ***********/

    //std::cout << std::endl;
    //std::cout << std::endl;
    
	/**********Ini crear vertices ***********/	
	for(i=0;i<num;i++){
		for(j=i+1;j<num;j++){
			if(inter2planes(planes[i],planes[j],&lineaux)){
                //print_vec3("line point",lineaux.point);
                //print_vec3("line dir",lineaux.dir);

				for(k=0;k<num;k++){
					if(k!=i && k!=j){
						if(interlineplane(lineaux,planes[k],pointaux)){
                            //std::cout << "plane: " << planes[k].a << "x + " << planes[k].b << "y + " << planes[k].c << "z + " << planes[k].d << " = 0 ";
                            //print_vec3("normal",planes[k].normal);

                            
                            //std::cout << "inter line plane: " << pointaux.x << "," << pointaux.y << "," << pointaux.z << std::endl;
                            
							aux.polys[k].vertexes[aux.polys[k].num]=pointaux;
							aux.polys[k].num++;						
						}
					}	
				}
                //std::cout << std::endl;

			}
		}	
	}
	/**********Fin crear vertices ***********/

    // std::cout << std::endl;
	
	/**********Ini recortar vertices ***********/
	
	for(i=0;i<num;i++){
		for(j=0;j<aux.polys[i].num;j++){
			for(k=j+1;k<aux.polys[i].num;k++){
				if(comppoints(aux.polys[i].vertexes[j],aux.polys[i].vertexes[k])){
					for(c=j;c<aux.polys[i].num-1;c++){
						aux.polys[i].vertexes[c]=aux.polys[i].vertexes[c+1];
					}
					aux.polys[i].num--;
					j--;
					break;					
				}
			}			
		}
	}
	
	for(i=0;i<num;i++){
		for(j=0;j<aux.polys[i].num;j++){
			for(k=0;k<num;k++){
				if(k!=i && pointinplane(aux.polys[i].vertexes[j],planes[k])==1){
					for(c=j;c<aux.polys[i].num-1;c++){
						aux.polys[i].vertexes[c]=aux.polys[i].vertexes[c+1];
					}
					aux.polys[i].num--;
					j--;
					break;
				}
			}
		}
	}

	/**********Ini ordenar ***********/

	for(i=0;i<num;i++){
        ordervertexes(&(aux.polys[i]));
	}

    for(i=0;i<num;i++){
        //std::cout << "poly: " << i << std::endl;
        for(j=0;j<aux.polys[i].num;j++){
            glm::dvec3 v = aux.polys[i].vertexes[j];
            //std::cout << "vertex: " << v.x << "," << v.y << "," << v.z << std::endl;
            aux.polys[i].vertex_buffer_data.push_back(v.x);
            aux.polys[i].vertex_buffer_data.push_back(v.y);
            aux.polys[i].vertex_buffer_data.push_back(v.z);

            aux.polys[i].color_buffer_data.push_back(1);
            aux.polys[i].color_buffer_data.push_back(1);
            aux.polys[i].color_buffer_data.push_back(1);
            
        }
        
        /*std::cout << "vertex_buffer_data ";
        for(GLfloat v : aux.polys[i].vertex_buffer_data){
            std::cout << v << ", ";
        }
        std::cout << std::endl;
        std::cout << "color_buffer_data ";
        for(GLfloat v : aux.polys[i].color_buffer_data){
            std::cout << v << ", ";
        }
        std::cout << std::endl;
        */
        GLfloat *vertex_buffer_datap = aux.polys[i].vertex_buffer_data.data();
        GLfloat *color_buffer_datap = aux.polys[i].color_buffer_data.data();
            
        glGenBuffers(1, &(aux.polys[i].vertexbuffer));
        glBindBuffer(GL_ARRAY_BUFFER, aux.polys[i].vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*aux.polys[i].vertex_buffer_data.size(), vertex_buffer_datap, GL_STATIC_DRAW);

        glGenBuffers(1, &(aux.polys[i].colorbuffer));
        glBindBuffer(GL_ARRAY_BUFFER, aux.polys[i].colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*aux.polys[i].color_buffer_data.size(), color_buffer_datap, GL_STATIC_DRAW);
        /*std::cout << "vertex buffer: " << aux.polys[i].vertexbuffer << std::endl;
        std::cout << "color buffer: " << aux.polys[i].colorbuffer << std::endl;
        std::cout << std::endl;*/
        
    }
    
	return aux;
}


void entity::load_brushes()
{
    for(rawbrush b : this->rawbrushes)
    {
        this->brushes.push_back(load_brush(b));        
    }
}