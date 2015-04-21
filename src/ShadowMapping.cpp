#include "ShadowMapping.h"

ShadowMapping::ShadowMapping(){
	parameters.add(depthValModifier.set("depthValModifier", 1.0, 0.0, 75.0));

	ofFbo::Settings settings;

	// settings.width = ofGetWidth()/4;					// width of images attached to fbo
	// settings.height = ofGetHeight()/4;					// height of images attached to fbo
	settings.width = ofGetWidth();					// width of images attached to fbo
	settings.height = ofGetHeight();		// height of images attached to fbo	settings.numColorbuffers = 0;		// how many color buffers to create
	// settings.numColorbuffers = 0;					
	//settings.colorFormats.push_back(GL_RGBA);		// format of the color attachments for MRT.
	settings.useDepth = true;				// whether to use depth buffer or not
	settings.useStencil = true;				// whether to use stencil buffer or not
	settings.depthStencilAsTexture = true;			// use a texture instead of a renderbuffer for depth (useful to draw it or use it in a shader later)
	// settings.textureTarget = GL_TEXTURE_2D;			// GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
	settings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;			// GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
	settings.internalformat = GL_RGBA;			// GL_RGBA, GL_RGBA16F_ARB, GL_RGBA32F_ARB, GL_LUMINANCE32F_ARB etc.
	settings.depthStencilInternalFormat = GL_DEPTH_COMPONENT32_ARB; 	// GL_DEPTH_COMPONENT(16/24/32)
	settings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
	settings.wrapModeVertical = GL_CLAMP_TO_EDGE;		// GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER etc.
	settings.minFilter = GL_LINEAR;				// GL_NEAREST, GL_LINEAR etc.
	settings.maxFilter = GL_LINEAR;				// GL_NEAREST, GL_LINEAR etc.
	settings.numSamples = 0;				// number of samples for multisampling (set 0 to disable)
	
	fbo.allocate(settings);

	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();

	blur.setup(settings);

	loadShaders();

	// fbo.getDepthTexture().bind();
	// 	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	// 	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	// fbo.getDepthTexture().unbind();
}

void ShadowMapping::loadShaders(){
	depthShader.load("shaders/depth_vert.glsl", "shaders/depth_frag.glsl");
	// shadowShader.load("shaders/shadow_vert.glsl", "shaders/shadow_frag.glsl");	
	shadowShader.load("shaders/shadow_advanced_vert.glsl", "shaders/shadow_advanced_frag.glsl");
}

void ShadowMapping::reloadShaders(){
	loadShaders();
}

void ShadowMapping::setLight(ofLight _light){
	light = _light;
	calculateLightSpace();
}

void ShadowMapping::setMaterial(ofMaterial _material){
	material = _material;
}

void ShadowMapping::calculateLightSpace(){
	ofMatrix4x4 lightModelMatrix;
	lightModelMatrix.makeIdentityMatrix();

	ofMatrix4x4 lightViewMatrix;
	lightViewMatrix.makeLookAtViewMatrix(light.getPosition(), ofVec3f(0), ofVec3f(0, -1, 0));
	
	ofMatrix4x4 lightProjectionMatrix;
	float ratio = float(ofGetWidth()) / float(ofGetHeight());
	float s = 200.0;
	// lightProjectionMatrix.makeOrthoMatrix(-s*ratio, s*ratio, -s, s, 0, 600);
	lightProjectionMatrix.makeOrthoMatrix(-s, s, -s, s, 0, 600);
	// lightProjectionMatrix.makePerspectiveMatrix(60.0, ratio, 1.5, 300.0);

	lightMVPMatrix = lightModelMatrix * lightViewMatrix * lightProjectionMatrix;

	// Bias matrix to transform [-1, 1] to [0, 1] for
	// texture (shadowMap) lookup coordinates
	ofMatrix4x4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	depthBiasMVPMatrix = lightMVPMatrix * biasMatrix;
}

void ShadowMapping::beginDepth(){
	calculateLightSpace();
	ofEnableDepthTest();

		// glEnable(GL_CULL_FACE);
		// glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE); // cull front faces - this helps with artifacts and shadows with exponential shadow mapping
    glCullFace(GL_FRONT);

	fbo.begin();
		ofClear(0, 0, 0, 255);
		depthShader.begin();
			depthShader.setUniformMatrix4f("lightMVPMatrix", lightMVPMatrix);				
			depthShader.setUniformMatrix4f("eyeModelViewMatrix_inv", ofGetCurrentViewMatrix().getInverse());
			depthShader.setUniform1f("depthValModifier", depthValModifier);

}

void ShadowMapping::endDepth(){
		depthShader.end();
	fbo.end();
	
	glDisable(GL_CULL_FACE);

}

void ShadowMapping::beginShadow(){
	// blur.apply(fbo, -1.0);	
	// fbo.getTexture().draw(fbo.getWidth()/4.0, ofGetHeight() - fbo.getHeight()/4.0, fbo.getWidth()/4.0, fbo.getHeight()/4.0);


	shadowShader.begin();
		shadowShader.setUniformTexture("shadowMap", fbo.getDepthTexture(), 0);
		// shadowShader.setUniformTexture("shadowMap", fbo.getTexture(), 0);
		shadowShader.setUniformMatrix4f("depthBiasMVPMatrix", depthBiasMVPMatrix);
		shadowShader.setUniformMatrix4f("eyeModelViewMatrix_inv", ofGetCurrentViewMatrix().getInverse());
		shadowShader.setUniform2f("res", ofGetWidth(), ofGetHeight());
		shadowShader.setUniform1f("depthValModifier", depthValModifier);
		shadowShader.setUniform3f("lightDirection_inv", -light.getLookAtDir());
		// Set light uniforms
		ofFloatColor c(light.getAmbientColor());
		shadowShader.setUniform3f("light.color", c.r, c.g, c.b);
		shadowShader.setUniform1f("light.power", 1.0);
		// Set material uniforms
		ofFloatColor ac(material.getAmbientColor());
		shadowShader.setUniform3f("material.ambient", ac.r, ac.g, ac.b);
		ofFloatColor dc(material.getDiffuseColor());
		shadowShader.setUniform3f("material.diffuse", dc.r, dc.g, dc.b);
		ofFloatColor sc(material.getSpecularColor());
		shadowShader.setUniform3f("material.specular", sc.r, sc.g, sc.b);
		shadowShader.setUniform1f("material.shininess", material.getShininess());
}

void ShadowMapping::endShadow(){
	shadowShader.end();
}
void ShadowMapping::update(){

}

void ShadowMapping::drawDepth(){
	ofTexture tex = fbo.getDepthTexture();
	// tex.draw(ofGetWidth() - tex.getWidth(), ofGetHeight() - tex.getHeight());
	tex.draw(ofGetWidth() - tex.getWidth()/4.0, ofGetHeight() - tex.getHeight()/4.0, tex.getWidth()/4.0, tex.getHeight()/4.0);
	
	// ofSetColor(ofColor::white);
	// fbo.draw(ofGetWidth() - tex.getWidth()/4.0, ofGetHeight() - tex.getHeight()/4.0, tex.getWidth()/4.0, tex.getHeight()/4.0);
}

void ShadowMapping::draw(){

}