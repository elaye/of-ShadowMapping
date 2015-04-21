#include "Blur.h"

Blur::Blur(){
	vShader.load("shaders/basic_vert.glsl", "shaders/gaussblur_v_frag.glsl");
	hShader.load("shaders/basic_vert.glsl", "shaders/gaussblur_h_frag.glsl");
}

void Blur::setup(ofFbo::Settings& settings){
	blurFbo.allocate(settings);
	blurFbo.begin();
		ofClear(0, 0, 0, 255);
	blurFbo.end();
}

void Blur::apply(ofFbo& fbo, float amount){
	blurFbo.begin();
		ofClear(0, 0, 0, 255);
		vShader.begin();
			vShader.setUniformTexture("blurSampler", fbo.getDepthTexture(), 0);
			vShader.setUniform1f("sigma", 4.0);
			vShader.setUniform1f("blurSize", 1.0/fbo.getHeight());
			// vShader.setUniform1f("blurSize", 5.0);
			vShader.setUniform2f("res", ofGetWidth(), ofGetHeight());
			ofDrawPlane(fbo.getWidth()/2.0, fbo.getHeight()/2.0, fbo.getWidth(), fbo.getHeight());	
		vShader.end();
	blurFbo.end();

	fbo.begin();
		ofClear(0, 0, 0, 255);
		hShader.begin();
			hShader.setUniformTexture("blurSampler", blurFbo.getTexture(), 0);
			hShader.setUniform1f("sigma", 4.0);
			hShader.setUniform1f("blurSize", 1.0/blurFbo.getWidth());
			// hShader.setUniform1f("blurSize", 5.0);
			hShader.setUniform2f("res", ofGetWidth(), ofGetHeight());
			ofDrawPlane(blurFbo.getWidth()/2.0, blurFbo.getHeight()/2.0, blurFbo.getWidth(), blurFbo.getHeight());	
		hShader.end();
	fbo.end();	

	fbo.draw(0, ofGetHeight() - fbo.getHeight()/4.0, fbo.getWidth()/4.0, fbo.getHeight()/4.0);
}