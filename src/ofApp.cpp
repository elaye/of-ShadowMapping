#include "ofApp.h"

void ofApp::setup(){
	setupLight();
	setupCubesScene();
	setupSpheresScene();

	shadow.setLight(light);
	shadow.setMaterial(material);

	gui.setup();
	gui.add(bBaseScene.setup("Base", false));
	gui.add(bSpheresScene.setup("Spheres", false));
	gui.add(bCubesScene.setup("Cubes", true));
	gui.add(bDrawDepth.setup("Draw depth", true));
	gui.add(bDrawGrid.setup("Draw grid", false));
}

void ofApp::setupLight(){
	lightLat = 80;
	lightLon = -60;
	lightDist = 200;
	light.orbit(lightLat, lightLon, lightDist);
	light.lookAt(ofVec3f(0.0));
	light.setAmbientColor(ofFloatColor(1.0, 0.6, 0.5, 1.0));
}

void ofApp::setupMaterial(){
	material.setDiffuseColor(ofFloatColor(1.0, 1.0, 0.5, 1.0));
	material.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1, 1.0) * material.getDiffuseColor());
	material.setSpecularColor(ofFloatColor(1.0, 0.7, 0.7, 1.0));
	material.setShininess(20.0);
}

void ofApp::setupCubesScene(){
	for(int i = 0; i < 50; ++i){
		ofBoxPrimitive box;
		box.set(ofRandom(30, 80));
		box.setResolution(20);
		box.setPosition(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
		boxes.push_back(box);
	}
}

void ofApp::setupSpheresScene(){
	for(int i = 0; i < 50; ++i){
		ofSpherePrimitive sphere;
		sphere.setRadius(ofRandom(30, 80));
		sphere.setResolution(20);
		sphere.setPosition(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
		spheres.push_back(sphere);
	}
}

void ofApp::update(){
	shadow.beginDepth();
		drawScene();
	shadow.endDepth();
}

void ofApp::draw(){
	ofBackgroundGradient( ofColor(40,40,40), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);	

	if(bDrawGrid) drawGrid();
	if(bDrawDepth) shadow.drawDepth();	
		
	cam.begin();
		shadow.beginShadow();
			drawScene();
		shadow.endShadow();
		drawLight();
	cam.end();

	ofDisableDepthTest();
	gui.draw();
}

void ofApp::drawScene(){
	if(bBaseScene) drawBaseScene();
	if(bCubesScene) drawCubesScene();
	if(bSpheresScene) drawSpheresScene();
}

void ofApp::drawBaseScene(){
	float r = 100.0;
	float s = 60.0;
	ofSetColor(ofColor::white);

	ofDrawSphere(r, 0, 0, s);
	ofDrawBox(r * cos(2*M_PI/3.0), 0.0, r * sin(2*M_PI/3.0), s);
	ofDrawCone(r * cos(-2*M_PI/3.0), 0.0, r * sin(-2*M_PI/3.0), s, s);	
}

void ofApp::drawCubesScene(){
	for(auto& b : boxes){
		b.draw();
	}
}

void ofApp::drawSpheresScene(){
	for(auto& s : spheres){
		s.draw();
	}
}

void ofApp::drawLight(){
	ofPushMatrix();
		ofSetColor(light.getAmbientColor());
		ofVec3f p = light.getPosition();
		ofTranslate(p);
		ofDrawArrow(p, p-light.getLookAtDir(), 10);
	ofPopMatrix();
}

void ofApp::drawGrid(){
	ofSetColor(ofColor(60));
	ofPushMatrix();
		ofRotate(90, 0, 0, -1);
		ofDrawGridPlane( 10, 10, false );
	ofPopMatrix();	
}

void ofApp::keyPressed(int key){
	switch(key){
		case 'r':
			shadow.reloadShaders();
			break;
		case 356:
			lightLat += 3;
			light.orbit(lightLat, lightLon, lightDist);
			shadow.setLight(light);
			break;
		case 357:
			lightLon -= 3;
			lightLon = max(-90, lightLon);
			light.orbit(lightLat, lightLon, lightDist);
			shadow.setLight(light);
			break;
		case 358:
			lightLat -= 3;
			light.orbit(lightLat, lightLon, lightDist);
			shadow.setLight(light);
			break;
		case 359:
			lightLon += 3;
			lightLon = min(90, lightLon);
			light.orbit(lightLat, lightLon, lightDist);
			shadow.setLight(light);
			break;
		case 360:
			lightDist += 5;
			light.dolly(5.0);
			shadow.setLight(light);
			break;
		case 361:
			lightDist -= 5;
			light.dolly(-5.0);
			shadow.setLight(light);
			break;
		default:
			break;
	}
}