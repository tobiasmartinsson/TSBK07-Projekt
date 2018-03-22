#version 150

out vec4 outColor;
in vec3 normal_to_frag;

uniform mat4 camMatrix;

void main(void)
{
	vec3 light = mat3(camMatrix)*normalize(vec3(0,10,10));
	float shade = dot(normal_to_frag, light);
	shade  = clamp(shade, 0,1);
	vec3 shading =  vec3(shade);
	outColor = vec4(shading, 1);
}
