#include "Map.hpp"
#include "Map_Driver.hpp"

#include <iostream>
#include <string>

entity::entity(){}

entity::entity(std::vector<header> headers, std::vector<rawbrush> rawbrushes)
{
    this->headers = headers;
    this->rawbrushes = rawbrushes;
}

void entity::load_brushes()
{
    for(rawbrush b : this->rawbrushes)
    {
        brush aux;
        aux.load(b);
        this->brushes.push_back(aux);        
    }
}
void Map::draw(GLuint programID, glm::mat4 projection, glm::mat4 view)
{
    glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !

    glm::mat4 MVP        = projection * view * Model; // Remember, matrix multiplication is the other way around


    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


    GLuint WinScaleID = glGetUniformLocation(programID, "WIN_SCALE");

    glUniform2f(WinScaleID, 640, 480);

    for(brush b : entities[0].brushes){
        b.draw(programID, projection, view);
    }    
}

Map load_map(std::string map_file)
{
    Map_Driver driver;
    driver.parse (map_file.c_str());                

    return driver.map;
}
