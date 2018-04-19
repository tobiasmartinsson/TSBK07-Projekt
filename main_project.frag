#version 150

out vec4 outColor;
in vec3 normal_to_frag;
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
void main(void)
{
	vec4 tex,tex2,tex3;
	vec2 texCoords, texCoords2, texCoords3;
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

	vec4 color = vec4(0.0f,1.0f,0.0f,1.0f);
	//vec3 light = normalize(vec3(0.0f,0.0f,10.0f));
	//float shade = dot(normal_to_frag, light);
	//shade  = clamp(shade, 0,1);
	float shade = 1.0f;
	if(lightSequence[int(mod(listIndex,64))] >= 1){
			shade = 1.0f;
	}else{
			shade = 0.5f;
			/*if(mod(int(mod(listIndex,64)),8) < 7){
				if(lightSequence[int(mod(listIndex,63))+1] == 1.0f){
					shade = 0.5f;
					color = vec4(1.0f,0,0,1.0f);
				}
			}
			if(mod(int(mod(listIndex,64)),8) > 0){
				if(lightSequence[int(mod(listIndex,63))-1] == 1.0f){
					shade = 0.5f;
					color = vec4(0,0,1.0f,1.0f);
				}
			}*/
	}

	/*if(mod(int(mod(listIndex,64)),8) == 7){
		shade = 0.5f;
		color = vec4(1,1,1,1);
	}*/


	//vec3 shading =  vec3(shade);
	shade  = clamp(shade, 0,1);
	tex = tex +0.2*tex3 + 0.6*tex2;
	outColor = color*shade*tex;
}
