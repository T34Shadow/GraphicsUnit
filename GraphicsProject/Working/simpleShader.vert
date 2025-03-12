#version 460

layout(location = 0) in vec3 vertPos;

uniform float aspectRatio;


out vec3 colour;

void main()
{
	colour = 1.0 - vertPos;
	vec3 transformedPos = vertPos;
	transformedPos.x /= aspectRatio;
	gl_Position = vec4(transformedPos * -0.5, 1.0);
}