#version 450 core

//Turn on early fragment tests so we don't process fragments we shouldn't
//layout (early_fragment_tests) in;

layout (binding = 0, offset = 0) uniform atomic_uint indexCounter; //Atomic counter to allocate items in the linked list
layout (binding = 0, rgba32ui) uniform uimageBuffer listBuffer; //1D buffer to store fragments
layout (binding = 1, r32ui) uniform uimage2D headPointerImage; //2D image store the heads of the fragments for each pixel
layout (binding = 2, r32ui) uniform uimage2D opaqueDepthImage; //2D image to store the closest opaque fragment's depth
//layout (binding = 2, r32ui) uniform uimage2D opaqueImage; //2D image to store the closest opaque fragment

//Samplers to define how the surface will be lit
layout (binding = 0) uniform sampler2D diffuseMap;
layout (binding = 1) uniform sampler2D normalMap;
layout (binding = 2) uniform sampler2D specularMap;

//World coordinates variables and texture coordinates for shading the fragment
in vec3 worldPosition;
in vec3 worldNormal;
in vec3 worldTangent;
in vec3 worldBitangent;
in vec2 textureCoordinates;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

out vec4 colour;

vec3 calculateNormal()
{
	//Find the normal of the surface from the normal map, and normalise it
	vec3 textureNormal = texture(normalMap, textureCoordinates).xyz;
	textureNormal = normalize(textureNormal * 2.0 - 1.0);

	//Create a matrix representing the surface normal in world space. Normalise each of the components to be safe
	mat3 worldTangentBitangentNormal = mat3(normalize(worldTangent), normalize(worldBitangent), normalize(worldNormal));

	//Multiply the texture normal by this matrix to find the actual world space normal we'll use to calculate lighting
	//Again, normalise this to be safe
	return normalize(worldTangentBitangentNormal * textureNormal);
}

vec4 shadeFragment()
{
	//Default values for the light colour and brightness until I can be bothered to pass them in
	vec4 lightColour = vec4(1.0, 1.0, 1.0, 1.0);
	int lightPower = 50;

	//Get the colour of texture. This is the colour of the fragment in perfect lighting
	vec4 textureColour = texture(diffuseMap, textureCoordinates);
	
	//Get the colour of the fragment under the ambient and diffuse lights
	vec4 ambientColour = textureColour * ambient;
	vec4 diffuseColour = textureColour * diffuse;

	//Calculate the directions from the light to the fragment, and from the fragment to the camera
	vec3 worldLightToFragment = worldPosition - lightPosition;
	vec3 worldFragmentToCamera = cameraPosition - worldPosition;

	//Get the surface normal, and the direction from the light to the fragment
	vec3 n = calculateNormal();
	vec3 l = normalize(worldLightToFragment);

	//Calculate how directly the light is shining onto the surface. Take the dot
	//product of the surface normal and the direction from the fragment to the light.
	float cosTheta = clamp(dot(n,-l), 0, 1);

	//The brightness of the light decreases with the square of the distance from the light
	float distance = length(worldLightToFragment);
	float distanceSquared = distance * distance;

	//The RGB values of fragment are dependent on the light, but alpha isn't. Also add the ambient light.
	vec3 diffuseColourLighting = vec3(diffuseColour * lightColour * lightPower * cosTheta);
	vec4 colour = vec4(vec3(ambientColour) + diffuseColourLighting, diffuseColour.a);

	//Don't let the colour be brighter than the actual texture
	colour = clamp(colour, vec4(0), textureColour);

	//Work out how directly the reflection from the fragment is shining at the camera
	vec3 e = normalize(worldFragmentToCamera);
	vec3 r = reflect(l,n);
	float cosAlpha = clamp(dot(e,r), 0, 1);

	//Find how much the fragment should reflect the light
	vec4 specularColour = texture(specularMap, textureCoordinates);

	//Add on specular to the colour. Don't clamp it because it can be really bright
	colour += specularColour * lightColour * lightPower * pow(cosAlpha, 200) / distance;

	return colour;
}

void main()
{
	//Calculate the colour of the fragment
	colour = shadeFragment();

	//Convert our depth to a uint by multiplying it by max uint
	uint depth = uint(double(gl_FragCoord.z) * 2147483647);
	ivec2 screenPosition = ivec2(gl_FragCoord.xy);

	if (colour.a > 0.99)
	{
		//Fragment is opaque, just update the depth buffer
		uint originalAtomicDepth = imageAtomicMin(opaqueDepthImage, screenPosition, depth);
	}
	else
	{
		//Fragment is transparent. Only render it if it is visible at all.
		if (colour.a > 0.00)
		{
			uint opaqueDepth = imageLoad(opaqueDepthImage, screenPosition).x;
			if (depth < opaqueDepth)
			{
				//Get the next index in the linked list buffer
				uint newHead = atomicCounterIncrement(indexCounter);

				//Insert the fragment into the list
				uint oldHead = imageAtomicExchange(headPointerImage, screenPosition, newHead);

				//Construct the item we'll insert into the buffer
				uvec4 item;
				item.x = oldHead;				//The pointer to the next item in the list
				item.y = packUnorm4x8(colour);	//Pack the 4 8-bit components of the colour into one 32-bit uint
				item.z = depth;					//The depth of the fragment, so we can sort them later
				item.w = 0;						//Unused

				//Put the item into the list
				imageStore(listBuffer, int(newHead), item);
			}
		}
		discard;
	}
}