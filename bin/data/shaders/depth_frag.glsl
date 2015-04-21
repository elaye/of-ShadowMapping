#version 330

// uniform float depthValModifier;

// Ouput data
// out float fragmentDepth;
out vec4 depth;

// float LinearizeDepth(float zoverw){  
// 	// float n = 1.0; // camera z near  
// 	// float f = 20000.0; // camera z far
// 	float n = 15.0; // camera z near  
// 	float f = 1500.0; // camera z far  
// 	return (2.0 * n) / (f + n - zoverw * (f - n));
// }

void main(){
	// float z = gl_FragCoord.z;
	// fragmentDepth = LinearizeDepth(z)/depthValModifier;
	// fragmentDepth = z;

	// Optional ?
	// fragmentDepth = gl_FragCoord.z;
	// depth = vec4(vec3(gl_FragCoord.z), 1.0);
	// depth = vec4(0.0, 1.0, 0.0, 1.0);
}