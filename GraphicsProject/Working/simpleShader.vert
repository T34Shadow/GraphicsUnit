#version 460

//simple shader

layout( location = 0) in vec4 pos;
layout( location = 1) in vec4 Normal;
layout( location = 2) in vec2 UVs;

//we need this for camera projection.
uniform mat4 ProjectionViewModel;

//we need this matrix transform the normal
uniform mat4 ModelMatrix;

out vec3 vNormal;
out vec2 uvs;


void main()
{
	uvs = UVs;
	vNormal = (ModelMatrix * Normal).xyz; 
	gl_Position = ProjectionViewModel * pos;
}