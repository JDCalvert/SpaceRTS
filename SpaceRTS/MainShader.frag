#version 450 core

in vec3 worldPosition;
in vec2 textureCoordinates;
in vec3 worldNormal;
in vec3 worldTangent;
in vec3 worldBitangent;

layout(location = 0) out vec4 colour;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
	vec4 lightColour = vec4(1.0, 1.0, 1.0, 1.0);
	int lightPower = 100;

	//Get the colour of texture. This is the colour of the fragment in perfect lighting
	vec4 textureColour = texture(diffuseMap, textureCoordinates);
	float textureR = textureColour.r;
	float textureG = textureColour.g;
	float textureB = textureColour.b;
	float textureA = textureColour.a;
	
	//Get the colour of the fragment under the ambient and diffuse lights
	vec4 ambientColour = textureColour * ambient;
	vec4 diffuseColour = textureColour * diffuse;

	vec3 worldLightToFragment = worldPosition - lightPosition;
	vec3 worldFragmentToCamera = cameraPosition - worldPosition;

	mat3 worldTangentBitangentNormal = mat3(worldTangent, worldBitangent, worldNormal);

	vec3 textureNormal = texture(normalMap, textureCoordinates).xyz;
	textureNormal = normalize(textureNormal * 2.0 - 1.0);

	//Work out how directly the light is shining onto the surface
	vec3 n = worldTangentBitangentNormal * textureNormal;
	vec3 l = normalize(worldLightToFragment);
	float cosTheta = clamp(dot(n,-l), 0, 1);

	float distance = length(worldLightToFragment);
	float distanceSquared = distance * distance;

	//The RGB values of the texture should change depending on the light, but the alpha shouldn't depend on that
	vec3 diffuseColourLighting = vec3(diffuseColour * lightColour * lightPower * cosTheta);// / distanceSquared);
	colour = ambientColour + vec4(diffuseColourLighting, diffuseColour.a);

	//Don't let the colour be brighter than the actual texture
	colour.r = clamp(colour.r, 0, textureR);
	colour.g = clamp(colour.g, 0, textureG);
	colour.b = clamp(colour.b, 0, textureB);
	colour.a = clamp(colour.a, 0, textureA);

	//Work out how directly the reflection from the fragment is shining at the camera
	vec3 E = normalize(worldFragmentToCamera);
	vec3 R = reflect(l,n);
	float cosAlpha = clamp(dot(E,R), 0, 1);

	vec4 specularColour = texture(specularMap, textureCoordinates);

	//We can add on specular, as it's added shine, and we don't want to clamp it because it can be really bright
	colour += specularColour * lightColour * lightPower * pow(cosAlpha, 200) / distance;
}