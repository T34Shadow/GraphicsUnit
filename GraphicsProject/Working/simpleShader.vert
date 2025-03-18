#version 460

//simple shader

layout(location = 0) in vec3 vertPos;
//layout(location = 1) in vec3 vertColour;
//
//uniform float aspectRatio;
//
//
//out vec3 colour;
//
//void main()
//{
//	colour = vertColour;
//	vec3 transformedPos = vertPos;
//	transformedPos.x /= aspectRatio;
//	gl_Position = vec4(transformedPos * -0.5, 1.0);
//}

layout( location = 0) in vec4 pos;

uniform mat4 ProjectionViewModel;

void main()
{
	gl_pos = ProjectionViewModel * pos;
}