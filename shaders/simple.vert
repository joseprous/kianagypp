#version 420 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColorV;

uniform mat4 MVP;

void main(){

    //    gl_Position.xyz = vertexPosition_modelspace;
    //gl_Position.w = 1.0;

    // Output position of the vertex, in clip space : MVP * position
    vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
    gl_Position = MVP * v;

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fragmentColorV = vertexColor;
}
