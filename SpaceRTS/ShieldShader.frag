#version 450 core

//layout (early_fragment_tests) in;

//Atomic counter to allocate items in the linked list, 1D buffer to store the list, and 2D image to store the heads of the lists
layout (binding = 0, offset = 0) uniform atomic_uint indexCounter;
layout (binding = 0, rgba32ui) uniform uimageBuffer listBuffer;
layout (binding = 1, r32ui) uniform uimage2D headPointerImage;

uniform vec3 cameraPosition;

//Structure to contain the position and size of a hit
struct Hit
{
	vec3 position;
	float size;
};

//Make a buffer that will be a vector of these hits
layout (std430, binding = 0) buffer hitBuffer
{
	Hit hits[];
};

//World coordinate variables
in vec3 worldPosition;
in vec3 worldNormal;
in vec3 shipPosition;

out vec4 colour;

vec4 shadeFragment()
{
	vec3 fragmentToCamera = normalize(cameraPosition - worldPosition);
	vec3 normal = normalize(worldNormal);

	float cosTheta = dot(fragmentToCamera, normal);
	float visibility = 1.0 - abs(cosTheta);
	visibility = pow(visibility, 1.5) * 0.1;

	colour = vec4(0.5, 0.5, 1.0, visibility);

	uint i;
	for (i=0; i<hits.length(); i++)
	{
		Hit hit = hits[i];
		float distance = length(shipPosition - hit.position);
		float intensity = pow(hit.size / distance, 1.5);

		colour += vec4(intensity);
	}

	return colour;
}

void main()
{
	colour = shadeFragment();

	if (colour.a > 0.01)
	{
		//Get the next index in the linked list buffer
		uint newHead = atomicCounterIncrement(indexCounter);

		//Insert the fragment into the list
		uint oldHead = imageAtomicExchange(headPointerImage, ivec2(gl_FragCoord.xy), newHead);

		//Construct the item we'll insert into the buffer
		uvec4 item;
		item.x = oldHead;						  //The pointer to the next item in the list
		item.y = packUnorm4x8(colour);			  //Pack the 4 8-bit components of the colour into one 32-bit uint
		//item.z = floatBitsToUint(gl_FragCoord.z); //The depth of the fragment, so we can sort them later
		item.z = uint(double(gl_FragCoord.z) * 2147483647);
		item.w = 0;								  //Unused

		//Put the item into the list
		imageStore(listBuffer, int(newHead), item);
	}
	discard;
}