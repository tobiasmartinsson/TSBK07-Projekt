#version 150

out vec4 outColor;
in vec2 tex_to_fragment;
in vec3 pos_to_fragment;

uniform sampler2D texUnit;
uniform vec3 camPos;
void main(void)
{
  vec3 camToWallVec = (camPos-pos_to_fragment);
  float distanceToBooth = sqrt(pow(camToWallVec.x,2)+pow(camToWallVec.z,2));
  float shade = 1.0f - distanceToBooth/4;

  vec4 tex = texture(texUnit,tex_to_fragment);
	outColor = shade * tex;
}
