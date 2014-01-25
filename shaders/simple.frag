#version 430 core

// Ouput data
out vec3 color;
// Interpolated values from the vertex shaders
in vec3 fragmentColor;

void main()
{

	// Output color = red 
	//color = vec3(1,0,0);

    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
    color = fragmentColor;

}
