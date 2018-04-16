#version 150

out vec4 outColor;
in vec3 normal_to_frag;
in vec2 tex_to_fragment;

uniform sampler2D texUnit;

void main(void)
{
  vec4 tex;
  tex = texture(texUnit,tex_to_fragment);
	outColor = tex;
}
