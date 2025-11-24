#version 460

// a simple flat colour shader

in vec4 vPosition;
in vec3 vNormal;
in vec2 uvs;

in mat3 TBN;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightColour02;
uniform vec3 LightDirection;
uniform vec3 LightDirection02;

uniform vec3 Ka; // ambient material colour;
uniform vec3 Kd; // diffuse material colour;
uniform vec3 Ks; // specular material colour;
uniform float specularPower; // material specualr power;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

out vec4 FragColour;

void main()
{

	//Normal mapping cal
	vec3 normal = vNormal;

	normal = texture(normalMap, uvs).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(TBN * normal); 

	//Ensure normal and light direction are normalised 
	vec3 N = normalize(normal);
	vec3 L = normalize(LightDirection);
	vec3 L2 = normalize(LightDirection02);
	//vec3 L = TBN * normalize(LightDirection - vPosition.xyz);
	//vec3 L2 = TBN * normalize(LightDirection02 - vPosition.xyz);

	vec3 albedo = texture(albedoMap, uvs).rgb;

	//calculate lambert term (negate light direction)
	float lambertTerm = max(0, min(1,dot(N,-L)));
	float lambertTerm02 = max(0, min(1,dot(N,-L2)));

	//cal view vector and reflection vector 
	vec3 V = normalize(cameraPosition);
	//vec3 V = TBN * normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L,N);
	vec3 R2 = reflect(L2,N);

	//cal specularTerm
	float specularTerm = pow( max( 0, dot( R + R2, V ) ), specularPower );

	//cal diffuse 
	vec3 ambient = AmbientColour * Ka * albedo;
	vec3 diffuse = (LightColour + LightColour02) * Kd * lambertTerm * albedo;
	vec3 specular = (LightColour + LightColour02) * Ks * specularTerm;

	//output lambert as grayscale 
	FragColour = vec4( ambient + diffuse + specular, 1);

	//TBN 0 = tangent
	//TBN 1 = bitangent 
	//TBN 2 = normal

	//FragColour = vec4(TBN[2], 1);//bitangent
	
	
}