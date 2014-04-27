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
		camera.begin();
			ofDrawGridPlane(50,50,true);
		camera.end();
	};
}