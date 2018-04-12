#version 150

out vec4 outColor;
in vec3 normal_to_frag;
in vec2 tex_to_fragment;

uniform mat4 camMatrix;
uniform sampler2D texUnit;
uniform float textureTimer;
uniform float randomValue;
uniform float randomValue2;
void main(void)
{
	vec4 tex;
	//change to *X for number of vertical lines
	if(mod(tex_to_fragment.s*8,2) > 1){
		if(mod(tex_to_fragment.s*4,2) > 1.5){
			tex = texture(texUnit, vec2(tex_to_fragment.s, tex_to_fragment.t+textureTimer*randomValue));
		}else{
			tex = texture(texUnit, vec2(tex_to_fragment.s, tex_to_fragment.t+textureTimer*randomValue*2.3f));
		}
	}else{
		if(mod(tex_to_fragment.s*4,2) > 0.5){
			tex = texture(texUnit, vec2(tex_to_fragment.s, tex_to_fragment.t-textureTimer*randomValue2));
		}else{
			tex = texture(texUnit, vec2(tex_to_fragment.s, tex_to_fragment.t-textureTimer*randomValue2 * 2.3f));
		}
	}
	vec3 light = normalize(vec3(0,10,10));
	float shade = dot(normal_to_frag, light);
	shade  = clamp(shade, 0,1);
	vec3 shading =  vec3(shade);
	outColor = tex*shade;
}
