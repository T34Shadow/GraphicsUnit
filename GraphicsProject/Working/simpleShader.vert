#version 460

//simple shader

layout( location = 0) in vec4 pos;
layout( location = 1) in vec4 Normal;

out vec3 vNormal;

uniform mat4 ProjectionViewModel;

//we need this matrix transform the normal
uniform mat4 ModelMatrix;

void main()
{
	vNormal = (ModelMatrix * Normal).xyz; 
	gl_Position = ProjectionViewModel * pos;
}