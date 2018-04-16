#version 450 core

layout (binding = 0, rgba32ui) uniform uimageBuffer listBuffer;
layout (binding = 1, r32ui) uniform uimage2D headPointerImage;
//layout (binding = 2, r32ui) uniform uimage2D opaqueImage;
layout (binding = 2, r32ui) uniform uimage2D opaqueDepthImage;
layout(binding = 3, rgba32f) uniform image2D opaqueImage;

#define MAX_FRAGMENTS 40

uvec2 fragments[MAX_FRAGMENTS];

out vec4 colour;

void main()
{	
	ivec2 screenCoordinates = ivec2(gl_FragCoord.xy);

	uint opaqueDepth = imageLoad(opaqueDepthImage, screenCoordinates).x;
	uint currentIndex = imageLoad(headPointerImage, screenCoordinates).x;
	uint fragmentCount = 0;

	while (currentIndex != 0 && fragmentCount < MAX_FRAGMENTS)
	{
		uvec4 fragment = imageLoad(listBuffer, int(currentIndex));
		currentIndex = fragment.x;
		if (fragment.z < opaqueDepth)
		{
			fragments[fragmentCount] = fragment.yz;
			fragmentCount++;
		}
	}

	uint i, j;
	//If there's more than one fragment in the list, then sort them in depth order
	if (fragmentCount > 1)
	{
		for (i=0; i<fragmentCount - 1; i++)
		{
			for (j=i+1; j<fragmentCount; j++)
			{
				uvec2 fragment1 = fragments[i];
				uvec2 fragment2 = fragments[j];

				if (fragment1.y < fragment2.y)
				{
					fragments[i] = fragment2;
					fragments[j] = fragment1;
				}
			}
		}
	}

	vec3 opaqueImageColour = imageLoad(opaqueImage, screenCoordinates).xyz;
	colour = vec4(opaqueImageColour, 1.0);
	for (i=0; i<fragmentCount; i++)
	{
		vec4 fragmentColour = unpackUnorm4x8(fragments[i].x);	
		colour = vec4(fragmentColour.a * fragmentColour.xyz, fragmentColour.a) + (1 - fragmentColour.a) * colour;
	}
}