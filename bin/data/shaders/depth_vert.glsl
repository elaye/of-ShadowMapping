#version 330

in vec4 position;

uniform mat4 modelViewMatrix;

uniform mat4 eyeModelViewMatrix_inv;
uniform mat4 lightMVPMatrix;

void main(){
	mat4 modelMatrix = eyeModelViewMatrix_inv * modelViewMatrix;
	gl_Position = lightMVPMatrix * modelMatrix * position;
}