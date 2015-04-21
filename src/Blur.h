#pragma once

#include "ofMain.h"

class Blur {

	ofFbo blurFbo;
	ofShader vShader, hShader;

	public:
		Blur();
		void setup(ofFbo::Settings& settings);
		void apply(ofFbo& fbo, float amount);
};