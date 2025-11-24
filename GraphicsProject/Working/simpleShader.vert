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

out mat3 TBN;

void main()
{
	uvs = UVs;
	vPosition = ModelMatrix * pos;
	vNormal = (ModelMatrix * Normal).xyz; 

	vec3 T = normalize(vec3(ModelMatrix * vec4(tangent,0.0)).xyz);
	vec3 N = normalize(vec3(ModelMatrix * vec4(vNormal,0.0)).xyz);

	T = normalize(T - dot(T, N) * N);

	vec3 B = normalize(vec3(cross(N,T)));

	gl_Position = ProjectionViewModel * pos;

	TBN = mat3(T, B, N);
}