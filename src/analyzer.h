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
		
		void LineMatch();
		double maxDist;
		vector<ofPoint> posTargets;
		
		
		bool Calc2LineNearestDistAndPos(
			ofPoint pp1,
			ofPoint pv1,
			ofPoint pp2,
			ofPoint pv2,
			float* pOut_dist,
			ofPoint* pOut_pos1,
			ofPoint* pOut_pos2
		);
		
	};
}