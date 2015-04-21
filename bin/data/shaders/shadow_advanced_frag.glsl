#version 330 core

struct Material {
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;	
};

struct Light {
	vec3 color;
	float power;
};

// Interpolated values from the vertex shaders
// in vec2 UV;
// in vec3 Position_worldspace;
// in vec3 Normal_cameraspace;
// in vec3 EyeDirection_cameraspace;
// in vec3 LightDirection_cameraspace;
// in vec4 ShadowCoord;

in vec3 varyingNormal;
in vec3 eyeDirection;
in vec3 lightDirection;
in vec4 shadowCoord;


// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
// uniform sampler2D myTextureSampler;
// uniform mat4 MV;
// uniform vec3 LightPosition_worldspace;
// uniform sampler2DShadow shadowMap;

uniform mat4 modelViewMatrix;
// uniform vec3 LightPosition_worldspace;
// uniform sampler2DShadow shadowMap;
uniform sampler2DRect shadowMap;
uniform vec2 res;

uniform Material material;
uniform Light light;

// vec2 poissonDisk[16] = vec2[]( 
//    vec2( -0.94201624, -0.39906216 ), 
//    vec2( 0.94558609, -0.76890725 ), 
//    vec2( -0.094184101, -0.92938870 ), 
//    vec2( 0.34495938, 0.29387760 ), 
//    vec2( -0.91588581, 0.45771432 ), 
//    vec2( -0.81544232, -0.87912464 ), 
//    vec2( -0.38277543, 0.27676845 ), 
//    vec2( 0.97484398, 0.75648379 ), 
//    vec2( 0.44323325, -0.97511554 ), 
//    vec2( 0.53742981, -0.47373420 ), 
//    vec2( -0.26496911, -0.41893023 ), 
//    vec2( 0.79197514, 0.19090188 ), 
//    vec2( -0.24188840, 0.99706507 ), 
//    vec2( -0.81409955, 0.91437590 ), 
//    vec2( 0.19984126, 0.78641367 ), 
//    vec2( 0.14383161, -0.14100790 ) 
// );

// Returns a random number based on a vec3 and an int.
// float random(vec3 seed, int i){
// 	vec4 seed4 = vec4(seed,i);
// 	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
// 	return fract(sin(dot_product) * 43758.5453);
// }

void main(){

	// Light emission properties
	// vec3 LightColor = vec3(1,1,1);
	// float LightPower = 5.0f;
	
	// Material properties
	// vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;
	// vec3 MaterialDiffuseColor = vec3(1.0, 1.0, 1.0);
	// vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	// vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);
	// vec3 MaterialSpecularColor = vec3(0.7,0.7,0.7);

	// Distance to the light
	//float distance = length( LightPosition_worldspace - Position_worldspace );

	// // Normal of the computed fragment, in camera space
	vec3 n = normalize(varyingNormal);
	// // Direction of the light (from the fragment to the light)
	vec3 l = normalize(lightDirection);
	// // Cosine of the angle between the normal and the light direction, 
	// // clamped above 0
	// //  - light is at the vertical of the triangle -> 1
	// //  - light is perpendiular to the triangle -> 0
	// //  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0.0, 1.0);
	
	// Eye vector (towards the camera)
	vec3 E = normalize(eyeDirection);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
	
	float visibility = 1.0;

	// Fixed bias, or...
	// float bias = 0.005;

	// ...variable bias
	// float bias = 0.005*tan(acos(cosTheta));
	// bias = clamp(bias, 0,0.01);

	// Sample the shadow map 4 times
	// for (int i=0;i<4;i++){
	// 	// use either :
	// 	//  - Always the same samples.
	// 	//    Gives a fixed pattern in the shadow, but no noise
	// 	int index = i;
	// 	//  - A random sample, based on the pixel's screen location. 
	// 	//    No banding, but the shadow moves with the camera, which looks weird.
	// 	// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
	// 	//  - A random sample, based on the pixel's position in world space.
	// 	//    The position is rounded to the millimeter to avoid too much aliasing
	// 	// int index = int(16.0*random(floor(Position_worldspace.xyz*1000.0), i))%16;
		
	// 	// being fully in the shadow will eat up 4*0.2 = 0.8
	// 	// 0.2 potentially remain, which is quite dark.
	// 	// visibility -= 0.2*(1.0-texture( shadowMap, vec3((shadowCoord.xy + poissonDisk[index]/700.0)*res,  (shadowCoord.z-bias)/shadowCoord.w) ));
	// 	visibility -= 0.2*(1.0-texture( shadowMap, (shadowCoord.xy + poissonDisk[index]/700.0)*res));
	
	// }

	float fragDepth = shadowCoord.z;
	float nearestOccluderDepth = texture(shadowMap, shadowCoord.xy * res).x;

	if(fragDepth > 0){
		// float s = 50.0;
		float s = 100.0;
		// 	// visibility -= 0.2*(1.0-texture( shadowMap, vec3((shadowCoord.xy + poissonDisk[index]/700.0)*res,  (shadowCoord.z-bias)/shadowCoord.w) ));
		visibility = clamp( exp( s * (nearestOccluderDepth - fragDepth)), 0.0, 1.0 );
	}
	// For spot lights, use either one of these lines instead.
	// if ( texture( shadowMap, (ShadowCoord.xy/ShadowCoord.w) ).z  <  (ShadowCoord.z-bias)/ShadowCoord.w )
	// if ( textureProj( shadowMap, ShadowCoord.xyw ).z  <  (ShadowCoord.z-bias)/ShadowCoord.w )

	// Ambient
	vec3 a = material.ambient;
	// Diffuse
	vec3 d = visibility * material.diffuse * light.color * light.power * cosTheta;
	// Specular
	vec3 s = visibility * material.specular * light.color * light.power * pow(cosAlpha, material.shininess);
	
	color = vec4(a + d + s, 1.0);
}