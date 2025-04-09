#version 460

// a simple flat colour shader

in vec3 vNormal;
in vec2 uvs;

uniform vec3 LightDirection;

uniform sampler2D albedoMap;

out vec4 FragColour;

void main()
{
	//Ensure normal and light direction are normalised 
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	vec3 albedo = texture(albedoMap, uvs).rgb;

	//calculate lambert term (negate light direction)
	float lambertTerm = max(0, min(1,dot(N,-L)));

	//output lambert as grayscale 
	FragColour = vec4(lambertTerm, lambertTerm, lambertTerm, 1);
}