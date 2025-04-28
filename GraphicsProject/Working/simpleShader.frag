#version 460

// a simple flat colour shader

in vec4 vPosition;
in vec3 vNormal;
in vec2 uvs;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

uniform vec3 Ka; // ambient material colour;
uniform vec3 Kd; // diffuse material colour;
uniform vec3 Ks; // specular material colour;
uniform float specularPower; // material specualr power;

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

	//cal view vector and reflection vector 
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L,N);

	//cal specularTerm
	float specularTerm = pow(max(0,dot(R,V)),specularPower);

	//cal diffuse 
	vec3 diffuse = LightColour * Kd * lambertTerm;
	vec3 ambient = AmbientColour * Ka;
	vec3 specular = LightColour * Ks * specularTerm;

	//output lambert as grayscale 
	FragColour = vec4( ambient + diffuse + specular, 1);
}