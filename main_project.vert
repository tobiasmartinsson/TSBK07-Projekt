#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 tex_to_fragment;
out vec3 pos_to_fragment;

uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 camMatrix;

void main(void)
{
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);

	tex_to_fragment = inTexCoord;
	pos_to_fragment = vec3(mdlMatrix * vec4(inPosition, 1.0));
}
