#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
out vec2 tex_to_fragment;
out vec3 normal_to_frag;
// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	normal_to_frag = normalMatrix1 * inNormal;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);

	tex_to_fragment = inTexCoord;
}
