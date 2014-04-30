#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("cerberus");
	drawer.setup();
	analyzer.setup();
	
	ofEnableDepthTest();
	ofEnableAntiAliasing();
	
}

//--------------------------------------------------------------
void ofApp::update(){
	analyzer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// drawer.draw();
	analyzer.draw2D();
	camera.begin();
			ofSetColor(0, 0, 0);
			ofDrawGrid(100,10,true,false,true,false);
			analyzer.draw3D();
	camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
