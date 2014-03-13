#version 420 core

in vec3 fragmentColor;
in vec3 dist;
const vec4 WIRE_COL = vec4(0.0,0.0,0.0,1);

out vec4 out_Color;

void main(void)
{
	float d = min(dist[0],min(dist[1],dist[2]));
 	float I = exp2(-2*d*d);

 	out_Color = I*WIRE_COL + (1.0 - I)* vec4(fragmentColor,1);
}
