#version 330 core

// Input vertex data, different for all executions of this shader.
// layout(location = 0) in vec3 vertexPosition_modelspace;
// layout(location = 1) in vec2 vertexUV;
// layout(location = 2) in vec3 vertexNormal_modelspace;

in vec4 position;
// in vec2 vertexUV;
in vec4 normal;


// Output data ; will be interpolated for each fragment.
// out vec2 UV;
// out vec3 Position_worldspace;
// out vec3 Normal_cameraspace;
// out vec3 EyeDirection_cameraspace;
// out vec3 LightDirection_cameraspace;
// out vec4 ShadowCoord;

// out vec3 Position_worldspace;
out vec3 varyingNormal;
out vec3 eyeDirection;
out vec3 lightDirection;
out vec4 shadowCoord;

// Values that stay constant for the whole mesh.
// uniform mat4 MVP;
// uniform mat4 V;
// uniform mat4 M;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;

// uniform vec3 LightInvDirection_worldspace;
uniform vec3 lightDirection_inv;
uniform mat4 eyeModelViewMatrix_inv;
uniform mat4 depthBiasMVPMatrix;


void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  modelViewProjectionMatrix * position;
	
	mat4 modelMatrix = eyeModelViewMatrix_inv * modelViewMatrix;
	shadowCoord = depthBiasMVPMatrix * modelMatrix * position;
	// shadowCoord = depthBiasMVP * position;
	
	// Position of the vertex, in worldspace : M * position
	// Position_worldspace = (M * position).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	eyeDirection = vec3(0,0,0) - (modelViewMatrix * position).xyz;

	// Vector that goes from the vertex to the light, in camera space
	lightDirection = (viewMatrix * vec4(lightDirection_inv,0)).xyz;
	
	// Normal of the the vertex, in camera space
	varyingNormal = (modelViewMatrix * vec4(normal.xyz,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	// UV = vertexUV;
}
