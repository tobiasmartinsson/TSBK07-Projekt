#version 150

out vec4 outColor;
in vec3 normal_to_frag;
in vec2 tex_to_fragment;

uniform mat4 camMatrix;
uniform sampler2D texUnit;
uniform sampler2D texUnit2;


uniform float textureTimer;
uniform float randomValue;
uniform float randomValue2;

uniform float numberSequence[64];
void main(void)
{
	vec4 tex;
	vec2 texCoords;
	float speed = 0;
	//change to *X for number of vertical lines
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

	texCoords =  vec2(tex_to_fragment.s, tex_to_fragment.t+speed);
	int listIndex = 0;
	listIndex += int(floor(tex_to_fragment.t+speed) * 8);
	listIndex += int(floor(tex_to_fragment.s));
	if(numberSequence[int(mod(listIndex,63))] > 0){
		tex = texture(texUnit,texCoords);
	}else{
		tex = texture(texUnit2,texCoords);
	}

	/*if(tex_to_fragment.s < 1.0){
	 	if(tex_to_fragment.t  < 1.0){
		 	tex = texture(texUnit2, texCoords);
		}else{
			tex = texture(texUnit, texCoords);
		}
	}
	else{
	 tex = texture(texUnit, texCoords);
	}*/

	vec3 light = normalize(vec3(0,10,10));
	float shade = dot(normal_to_frag, light);
	shade  = clamp(shade, 0,1);
	vec3 shading =  vec3(shade);
	outColor = vec4(0,1,0,1)*tex;
}
