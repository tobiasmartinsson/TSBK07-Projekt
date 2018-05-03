#version 150

in vec3 inPosition;
uniform mat4 camMatrix;
// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
out vec3 pos_to_fragment;


void main(void)
{
	gl_Position = projMatrix * camMatrix * mdlMatrix * vec4(inPosition, 1.0);
	pos_to_fragment = vec3(mdlMatrix * vec4(inPosition, 1.0));
}
