#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

namespace cerberus{
	class Analyzer
	{
	public:
		Analyzer();
		~Analyzer();
		
		void setup();
		void update();
		void draw2D();
		void draw3D();
		
		
		// ofPoint GetPosCamX();
		// ofPoint GetPosCamY();
		// ofPoint GetPosCamZ();
		
	private:
		int sizeCamW;
		int sizeCamH;
		
		int sizeImage;
		int sizeFlame;
		
		bool isDebug;
		
		void ReceiveImage();
		ofVideoGrabber videoCam;
		ofxCvColorImage ofImgInput;
		
		void ConvertMonoqloImage();
		ofxCvGrayscaleImage ofImgInputGray;
		
		void SeparateImage();
		ofxCvGrayscaleImage ofImgX;//for Debugs
		ofxCvGrayscaleImage ofImgY;
		ofxCvGrayscaleImage ofImgZ;
		
		void LabelImage();
		ofxCvContourFinder contourFinderX; 
		ofxCvContourFinder contourFinderY; 
		ofxCvContourFinder contourFinderZ; 

		void LinerAnalyze();
		ofPoint posCamX,posCamY,posCamZ;
		double radZoomX,radZoomY,radZoomZ;
		vector<ofPoint> vecX,vecY,vecZ;
		
	};
}