#version 150

out vec4 outColor;
in vec3 normal_to_frag;
in vec2 tex_to_fragment;

uniform mat4 camMatrix;
uniform sampler2D texUnit;

void main(void)
{
	vec4 tex = texture(texUnit, tex_to_fragment);
	vec3 light = normalize(vec3(0,10,10));
	float shade = dot(normal_to_frag, light);
	shade  = clamp(shade, 0,1);
	vec3 shading =  vec3(shade);
	outColor = tex*shade;
}
