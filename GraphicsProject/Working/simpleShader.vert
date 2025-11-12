#version 460

//simple shader

layout( location = 0) in vec4 pos;
layout( location = 1) in vec4 Normal;
layout( location = 2) in vec2 UVs;
layout( location = 3) in vec3 tangent;
layout( location = 4) in vec3 biTangent;

//we need this for camera projection.
uniform mat4 ProjectionViewModel;

//we need this matrix transform the normal
uniform mat4 ModelMatrix;

out vec4 vPosition;
out vec3 vNormal;
out vec2 uvs;
out vec3 tangent;
out vec3 biTangent;

void main()
{
	uvs = UVs;
	vPosition = ModelMatrix * pos;
	vNormal = (ModelMatrix * Normal).xyz; 
	gl_Position = ProjectionViewModel * pos;
}