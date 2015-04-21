#version 330

precision highp float;

uniform float depthValModifier;
uniform vec2 res;

// uniform sampler2DRect shadowMap;
uniform sampler2DShadow shadowMap;

in vec4 shadowCoord;
in float dotNormalLight;

out vec4 color;

void main(){

	float nearestOccluderDist = texture(shadowMap, shadowCoord.xy * res).x;
	float fragDist = shadowCoord.z;

	// Bias to limit surface acne
	// float bias = 0.005*tan(acos(dotNormalLight));
	float bias = 0.005;
	float visibility = 1.0;
	if(nearestOccluderDist < fragDist-bias){
		visibility = 0.5;
		// visibility = clamp( exp( depthValModifier * (nearestOccluderDist - fragDist)), 0.0, 1.0 );
	}
	color = vec4(visibility * vec3(1.0), 1.0);
}