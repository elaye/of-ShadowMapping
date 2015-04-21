#version 330

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform mat4 normalMatrix;

uniform mat4 eyeModelViewMatrix_inv;
uniform mat4 depthBiasMVPMatrix;

uniform vec3 lightDir;

in vec4 position;
in vec4 normal;

out vec3 varyingNormal;
out vec4 shadowCoord;
// out float dotNormalLight;

void main(){
	gl_Position = modelViewProjectionMatrix * position;

	vec4 tempNormal = normalMatrix * normal;
	varyingNormal = normalize(tempNormal.xyz);

	// dotNormalLight = clamp(dot(normal.xyz, (modelViewMatrix * vec4(lightDir, 1.0)).xyz), 0.0, 1.0);

	mat4 modelMatrix = eyeModelViewMatrix_inv * modelViewMatrix;
	shadowCoord = depthBiasMVPMatrix * modelMatrix * position;
}