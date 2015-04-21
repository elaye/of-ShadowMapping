#pragma once

#include "ofMain.h"

#include "ofxGui.h"

#include "ShadowMapping.h"

class ofApp : public ofBaseApp{

	ShadowMapping shadow;
	
	ofEasyCam cam;

	ofLight light;
	ofMaterial material;

	int lightLat, lightLon, lightDist;

	vector<ofBoxPrimitive> boxes;
	vector<ofSpherePrimitive> spheres;
	
	ofxToggle bBaseScene, bSpheresScene, bCubesScene, bDrawDepth, bDrawGrid;

	ofxPanel gui;

	public:
		void setup();
		void setupLight();
		void setupMaterial();
		void setupCubesScene();
		void setupSpheresScene();
		void update();
		void draw();
		void drawScene();
		void drawBaseScene();
		void drawCubesScene();
		void drawSpheresScene();
		void drawLight();
		void drawGrid();
		void keyPressed(int key);
};
