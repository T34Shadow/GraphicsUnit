#version 460

//simple shader



layout( location = 0) in vec4 pos;

uniform mat4 ProjectionViewModel;

void main()
{
	gl_Position = ProjectionViewModel * pos;
}