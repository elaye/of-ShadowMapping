#version 330

in vec4 position;
in vec2 texcoord;

out vec2 texCoordVarying;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 textureMatrix;

void main(){
	gl_Position = modelViewProjectionMatrix * position;
	texCoordVarying = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
	// texCoordVarying = texcoord;
}