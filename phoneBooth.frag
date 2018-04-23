#version 150

out vec4 outColor;
in vec3 normal_to_frag;
in vec2 tex_to_fragment;
in vec3 pos_to_fragment;

uniform sampler2D texUnit;
uniform vec3 camPos;
void main(void)
{
  vec3 camToWallVec = (camPos-pos_to_fragment);
  float distanceToBooth = sqrt(pow(camToWallVec.x,2)+pow(camToWallVec.z,2));
  /*if( distanceToBooth < 2.0f){
    shade = 1.0f;
  }else if(distanceToBooth < 1.5f){
    shade = 0.7f;
  }
  else if(distanceToBooth < 1f){
    shade = 0.7f;
  }*/
  float shade = 1.0f - distanceToBooth/4;


  vec4 tex;
  tex = texture(texUnit,tex_to_fragment);
	outColor = shade * tex;
}
