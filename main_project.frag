#version 150

out vec4 outColor;
in vec3 pos_to_fragment;
in vec2 tex_to_fragment;

uniform sampler2D texUnit;
uniform sampler2D texUnit12;
uniform sampler2D texUnit13;
uniform sampler2D texUnit2;
uniform sampler2D texUnit22;
uniform sampler2D texUnit23;

uniform float textureTimer;
uniform float randomValue;
uniform float randomValue2;

uniform float numberSequence[64];
uniform float lightSequence[64];

uniform vec3 camPos;
void main(void)
{
	vec4 tex,tex2,tex3;
	vec2 texCoords, texCoords2, texCoords3;
	float speed = 0;
	if(mod(tex_to_fragment.s ,2) > 1){
		if(mod(tex_to_fragment.s/2,2) > 1.5){
			speed += textureTimer*randomValue;
		}else{
			speed += textureTimer*randomValue*2.3f;
		}
	}else{
		if(mod(tex_to_fragment.s/2,2) > 0.5){
			speed -= textureTimer*randomValue2;
		}else{
			speed -= textureTimer*randomValue2 * 2.3f;
		}
	}

	/*Different speeds and textures for the 3 different texture layers*/
	texCoords =  vec2(tex_to_fragment.s, tex_to_fragment.t+speed);
	texCoords2 = vec2(tex_to_fragment.s, tex_to_fragment.t+0.7*speed);
	texCoords3 = vec2(tex_to_fragment.s, tex_to_fragment.t+0.5*speed);
	int listIndex = 0;
	listIndex += int(floor(tex_to_fragment.t+speed) * 8);
	listIndex += int(floor(tex_to_fragment.s));
	if(numberSequence[int(mod(listIndex,64))] > 0){
		tex = texture(texUnit,texCoords);
		tex2 = texture(texUnit,texCoords2);
		tex3 = texture(texUnit,texCoords3);
	}else{
		tex = texture(texUnit2,texCoords);
		tex2 = texture(texUnit2,texCoords2);
		tex3 = texture(texUnit2,texCoords3);
	}


	float shade = 1.0f;

	/*Check distance from camera to fragment, creates "shadow/lightsource effect"
	 	with the camera as the light source*/
	vec3 camToWallVec = (camPos-pos_to_fragment);
	float distanceToWall = sqrt(pow(camToWallVec.x,2)+pow(camToWallVec.z,2));
	shade = shade - distanceToWall/4;

	if(lightSequence[int(mod(listIndex,64))] >= 1){
			shade *= 1.0f;
	}else{
			shade *= 0.5f;

	}

	tex = tex +0.2*tex3 + 0.6*tex2;
	outColor = vec4(0.0f,1.0f,0.0f,1.0f)*shade*tex;
}
