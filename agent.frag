#version 150

out vec4 outColor;
in vec3 pos_to_fragment;
uniform vec3 camPos;

void main(void)
{
  vec3 camToWallVec = (camPos-pos_to_fragment);
  float distanceToAgent = sqrt(pow(camToWallVec.x,2)+pow(camToWallVec.z,2));
  float shade = 1.0f - distanceToAgent/4;
	outColor =  shade * vec4(0,1,0,1);
}
