#pragma once

#include "ofMain.h"

#include "ShadowMapping.h"
#include "Blur.h"

class ShadowMapping {

	ofShader depthShader, shadowShader;

	ofFbo fbo;

	ofLight light;
	ofMaterial material;

	ofMatrix4x4 lightMVPMatrix, lightModelViewMatrix, lightProjectionMatrix, depthBiasMVPMatrix;

	Blur blur;

	ofParameter<float> depthValModifier;

	public:
		ofParameterGroup parameters;
		ShadowMapping();
		void update();
		void draw();

		void loadShaders();
		void reloadShaders();

		void beginDepth();
		void endDepth();
		
		void beginShadow();
		void endShadow();
		
		void drawDepth();
		void setLight(ofLight light);
		void setMaterial(ofMaterial material);
		void calculateLightSpace();
};