#include "drawer.h"
namespace cerberus{
	Drawer::Drawer(){};
	Drawer::~Drawer(){};
	
	void Drawer::setup(){
		ofEnableDepthTest();
		ofEnableAntiAliasing();
	};
	void Drawer::update(){};
	void Drawer::draw(){
		// camera.begin();
		// 	ofSetColor(0, 0, 0);
		// 	ofDrawGrid(100,10,true,false,true,false);
		// camera.end();
	};
}