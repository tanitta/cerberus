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
		void draw();
		
		ofVideoGrabber videoCam;
	private:
		int sizeCamW;
		int sizeCamH;
		bool isDebug;
		
		void ReceiveImage();
		void ConvertMonoqloImage();
		void SeparateImage();
		void LabelImage();
		void Debug();
		ofxCvColorImage ofImgInput;
		ofxCvGrayscaleImage ofImgInputGray;
		
		ofxCvContourFinder contourFinderX; 
		ofxCvContourFinder contourFinderY; 
		ofxCvContourFinder contourFinderZ; 
		
		
		//oC関係の宣言
		IplImage *frameImage;//中間処理用のoCの画像
		
		IplImage *ocImgInput;
		IplImage *ocImgInputGray;

		IplImage *ocImgX;
		IplImage *ocImgY;
		IplImage *ocImgZ;

		IplImage *ocImgXDebug;
		IplImage *ocImgYDebug;
		IplImage *ocImgZDebug;
		
		ofxCvGrayscaleImage ofImgX;//for Debugs
		ofxCvGrayscaleImage ofImgY;
		ofxCvGrayscaleImage ofImgZ;
		
		
		
	};
}