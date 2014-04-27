#include "analyzer.h"
namespace cerberus{
	Analyzer::Analyzer(){};
	Analyzer::~Analyzer(){};
	
	void Analyzer::setup(){
		sizeCamW = 1200;
		sizeCamH = 960;
		
		videoCam.setDeviceID(0);
		videoCam.initGrabber(sizeCamW,sizeCamH);
	};
	void Analyzer::update(){
		// ReceiveImage();
		// SeparateImage();
		// LabelImage();
		videoCam.update();
	};
	void Analyzer::draw(){
		// call this once per update
		videoCam.draw(0, 0);
		if (videoCam.isFrameNew()){
		// do computer vision / process the pixels
			cout<<"hoge\n";
		}
		
	};
	
	void Analyzer::ReceiveImage(){
		videoCam.update();  // call this once per update
		if (videoCam.isFrameNew()){
		// do computer vision / process the pixels
			
		}
	};
	void Analyzer::SeparateImage(){};
	void Analyzer::LabelImage(){};
	
}