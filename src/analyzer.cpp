#include "analyzer.h"
namespace cerberus{
	Analyzer::Analyzer(){};
	Analyzer::~Analyzer(){};
	
	void Analyzer::setup(){
		isDebug = true;
		
		sizeCamW = 1200;
		sizeCamH = 960;
		sizeImage = sizeCamH/2;
		sizeFlame = (sizeCamW-sizeCamH)/4;
		
		videoCam.setDeviceID(0);
		videoCam.initGrabber(sizeCamW,sizeCamH);
		
		ofImgX.allocate(sizeImage,sizeImage);
		ofImgY.allocate(sizeImage,sizeImage);
		ofImgZ.allocate(sizeImage,sizeImage);
		
		// ocImgInput = cvCreateImage(cvSize(sizeCamW, sizeCamH), IPL_DEPTH_8U, 3);
		// ocImgInputGray = cvCreateImage(cvSize(sizeCamW, sizeCamH), IPL_DEPTH_8U, 1);

		// ocImgX = cvCreateImage(cvSize(sizeImage, sizeImage), IPL_DEPTH_8U, 1);
		// ocImgY = cvCreateImage(cvSize(sizeImage, sizeImage), IPL_DEPTH_8U, 1);
		// ocImgZ = cvCreateImage(cvSize(sizeImage, sizeImage), IPL_DEPTH_8U, 1);

		// ocImgXDebug = cvCreateImage(cvSize(sizeImage, sizeImage), IPL_DEPTH_8U, 3);
		// ocImgYDebug = cvCreateImage(cvSize(sizeImage, sizeImage), IPL_DEPTH_8U, 3);
		// ocImgZDebug = cvCreateImage(cvSize(sizeImage, sizeImage), IPL_DEPTH_8U, 3);
	};
	void Analyzer::update(){
		videoCam.update();
		if (videoCam.isFrameNew()){
			ReceiveImage();
			ConvertMonoqloImage();
			SeparateImage();
			LabelImage();
			
			// for (int i = 0; i < contourFinderX.nBlobs; i++){
			// 	//contourFinderX.blobs[i].centroid.x;
			// }
			if (isDebug)
			{
			}
		}
	};
	void Analyzer::draw(){
		if (videoCam.isFrameNew()){
			if (isDebug)
			{
			}
		}
		// for (int i = 0; i < contourFinderX.nBlobs; i++){
		// 	ofPushMatrix();
		// 	contourFinderX.blobs[i].draw(0,0);
		// 	ofFill();
		// 	ofSetColor(500, 0, 0);
		// 	ofEllipse(contourFinderX.blobs[i].centroid.x, contourFinderX.blobs[i].centroid.y, 4, 4);
		// 	ofPopMatrix();
		// }
		
	};
	
	void Analyzer::ReceiveImage(){
		ofImgInput.setFromPixels(videoCam.getPixels(), sizeCamW,sizeCamH);
	};
	
	void Analyzer::ConvertMonoqloImage(){
		ofImgInputGray = ofImgInput;
		ofImgInputGray.invert();
		ofImgInputGray.threshold(5);
	};
	void Analyzer::SeparateImage(){
		//X
		ofImgInputGray.setROI(sizeFlame, 0, sizeImage, sizeImage);
			ofImgX = ofImgInputGray;
		ofImgInputGray.resetROI();
		
		//Y
		ofImgInputGray.setROI(sizeFlame*3+sizeImage, 0, sizeImage, sizeImage);
			ofImgY = ofImgInputGray;
		ofImgInputGray.resetROI();
		
		//Z
		ofImgInputGray.setROI(sizeFlame, sizeImage, sizeImage, sizeImage);
			ofImgZ = ofImgInputGray;
		ofImgInputGray.resetROI();
	};
	void Analyzer::LabelImage(){
		ofImgX.dilate();
		ofImgY.dilate();
		ofImgZ.dilate();
		contourFinderX.findContours(ofImgX, 1, ofImgX.width * ofImgX.height, 50, false, true);
		contourFinderY.findContours(ofImgY, 1, ofImgY.width * ofImgY.height, 50, false, true);
		contourFinderZ.findContours(ofImgZ, 1, ofImgZ.width * ofImgZ.height, 50, false, true);
	};
}