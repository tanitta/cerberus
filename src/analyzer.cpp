#include "analyzer.h"
namespace cerberus{
	Analyzer::Analyzer(){};
	Analyzer::~Analyzer(){};
	
	void Analyzer::setup(){
		isDebug = true;
		sizeCamW = 1200;
		sizeCamH = 960;
		
		videoCam.setDeviceID(0);
		videoCam.initGrabber(sizeCamW,sizeCamH);
		
		ofImgX.allocate(480,480);
		ofImgY.allocate(480,480);
		ofImgZ.allocate(480,480);
		
		ocImgInput = cvCreateImage(cvSize(sizeCamW, sizeCamH), IPL_DEPTH_8U, 3);
		ocImgInputGray = cvCreateImage(cvSize(sizeCamW, sizeCamH), IPL_DEPTH_8U, 1);

		ocImgX = cvCreateImage(cvSize(480, 480), IPL_DEPTH_8U, 1);
		ocImgY = cvCreateImage(cvSize(480, 480), IPL_DEPTH_8U, 1);
		ocImgZ = cvCreateImage(cvSize(480, 480), IPL_DEPTH_8U, 1);

		ocImgXDebug = cvCreateImage(cvSize(480, 480), IPL_DEPTH_8U, 3);
		ocImgYDebug = cvCreateImage(cvSize(480, 480), IPL_DEPTH_8U, 3);
		ocImgZDebug = cvCreateImage(cvSize(480, 480), IPL_DEPTH_8U, 3);
	};
	void Analyzer::update(){
		videoCam.update();
		if (videoCam.isFrameNew()){
			ReceiveImage();
			ConvertMonoqloImage();
			SeparateImage();
			
			LabelImage();
			
			if (isDebug)
			{
				Debug();
			}
		}
	};
	void Analyzer::draw(){
		if (videoCam.isFrameNew()){
			if (isDebug)
			{
				
				// ofImgX.draw(0,0);
			}
		}
		for (int i = 0; i < contourFinderX.nBlobs; i++){
			ofPushMatrix();
			//画面サイズいっぱいに表示されるようリスケール
			// glScalef((float)ofGetWidth() / (float)ofImgY.width, 
			//          (float)ofGetHeight() / (float)ofImgY.height, 
			//          1.0f);
			contourFinderX.blobs[i].draw(0,0);
			ofFill();
			ofSetColor(500, 0, 0);
			ofEllipse(contourFinderX.blobs[i].centroid.x, contourFinderX.blobs[i].centroid.y, 4, 4);
			ofPopMatrix();
		}
		
	};
	
	void Analyzer::ReceiveImage(){
		ofImgInput.setFromPixels(videoCam.getPixels(), sizeCamW,sizeCamH);

		// ocImgInput = ofImgInput.getCvImage();
	};
	
	void Analyzer::ConvertMonoqloImage(){
		ofImgInputGray = ofImgInput;
		ofImgInputGray.invert();
		ofImgInputGray.threshold(50);
		// cvNot(ocImgInput, ocImgInput);
		// cvCvtColor(ocImgInput, ocImgInputGray, CV_BGR2GRAY);
		// cvThreshold(ocImgInputGray, ocImgInputGray, 5, 255, CV_THRESH_BINARY);
	};
	void Analyzer::SeparateImage(){
		//X
		ofImgInputGray.setROI(60, 0, 480, 480);
			ofImgX = ofImgInputGray;
		ofImgInputGray.resetROI();
		
		//Y
		ofImgInputGray.setROI(60+600, 0, 480, 480);
			ofImgY = ofImgInputGray;
		ofImgInputGray.resetROI();
		
		//Z
		ofImgInputGray.setROI(60, 480, 480, 480);
			ofImgZ = ofImgInputGray;
		ofImgInputGray.resetROI();
		
		// cvSetImageROI(ocImgInputGray, cvRect(60, 0, 480, 480));//X
		// 	cvCopy(ocImgInputGray, ocImgX);
		// cvResetImageROI(ocImgInputGray);

		// cvSetImageROI(ocImgInputGray, cvRect(60+600, 0, 480, 480));//Y
		// 	cvCopy(ocImgInputGray, ocImgY);
		// cvResetImageROI(ocImgInputGray);

		// cvSetImageROI(ocImgInputGray, cvRect(60, 480, 480, 480));//Z
		// 	cvCopy(ocImgInputGray, ocImgZ);
		// cvResetImageROI(ocImgInputGray);
	};
	void Analyzer::LabelImage(){
		// cvCvtColor(ocImgX, ocImgXDebug, CV_GRAY2BGR);
		// cvCvtColor(ocImgY, ocImgYDebug, CV_GRAY2BGR);
		// cvCvtColor(ocImgZ, ocImgZDebug, CV_GRAY2BGR);

		// ofImgX = ocImgXDebug;
		// ofImgY = ocImgYDebug;
		// ofImgZ = ocImgZDebug;
		
		contourFinderX.findContours(ofImgX, 1, ofImgX.width * ofImgX.height, 10, false, false);
		contourFinderY.findContours(ofImgY, 1, ofImgY.width * ofImgY.height, 10, false, false);
		contourFinderZ.findContours(ofImgZ, 1, ofImgZ.width * ofImgZ.height, 10, false, false);
	};
	void Analyzer::Debug(){
		//debug
		// cvCvtColor(ocImgX, ocImgXDebug, CV_GRAY2BGR);
		// cvCvtColor(ocImgY, ocImgYDebug, CV_GRAY2BGR);
		// cvCvtColor(ocImgZ, ocImgZDebug, CV_GRAY2BGR);

		// ofImgX = ocImgXDebug;
		// ofImgY = ocImgYDebug;
		// ofImgZ = ocImgZDebug;
	};
}