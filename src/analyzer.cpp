#include "analyzer.h"
namespace cerberus{
	Analyzer::Analyzer(){};
	Analyzer::~Analyzer(){};
	
	void Analyzer::setup(){
		isDebug = false;
		
		sizeCamW = 1200;
		sizeCamH = 960;
		sizeImage = sizeCamH/2;
		sizeFlame = (sizeCamW-sizeCamH)/4;
		
		videoCam.setDeviceID(0);
		videoCam.initGrabber(sizeCamW,sizeCamH);
		
		ofImgX.allocate(sizeImage,sizeImage);
		ofImgY.allocate(sizeImage,sizeImage);
		ofImgZ.allocate(sizeImage,sizeImage);
		
		posCamX.set(2000,0,0);
		posCamY.set(0,2000,0);
		posCamZ.set(0,0,2000);
		radZoomX = 10.0*PI/180.0;
		radZoomY = 10.0*PI/180.0;
		radZoomZ = 10.0*PI/180.0;
	};
	void Analyzer::update(){
		videoCam.update();
		if (videoCam.isFrameNew()){
			ReceiveImage();
			ConvertMonoqloImage();
			SeparateImage();
			LabelImage();
			
			LinerAnalyze();
			
			if (isDebug)
			{
			}
		}
	};
	void Analyzer::draw2D(){
		if (videoCam.isFrameNew()){
			if (isDebug)
			{
				ofSetColor(255, 255, 255);
				ofImgX.draw(0,0);
				for (int i = 0; i < contourFinderY.nBlobs; i++){
					ofPushMatrix();
					contourFinderY.blobs[i].draw(sizeImage,0);
					ofFill();
					ofSetColor(500, 0, 0);
					ofEllipse(contourFinderY.blobs[i].centroid.x+sizeImage, contourFinderY.blobs[i].centroid.y, 4, 4);
					ofPopMatrix();
				}
			}
		}
		
	};
	void Analyzer::draw3D(){
		//lineY
		cout<<"vecY.size : "<<vecY.size()<<"\n";
		ofSetColor(255, 127, 127);
		for (int i = 0; i < vecX.size(); i++){
			ofLine(vecX[i],posCamX);
		}
		
		ofSetColor(127, 255, 127);
		for (int i = 0; i < vecY.size(); i++){
			ofLine(vecY[i],posCamY);
		}
		
		ofSetColor(127, 127, 255);
		for (int i = 0; i < vecZ.size(); i++){
			ofLine(vecZ[i],posCamZ);
		}
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
	void Analyzer::LinerAnalyze(){
		vecX.clear();
		for (int i = 0; i < contourFinderX.nBlobs; i++){
			double xh, yh, l;
			xh = ((double)contourFinderX.blobs[i].centroid.x - (double)sizeImage*0.5)/(double)sizeImage*2;
			yh = ((double)contourFinderX.blobs[i].centroid.y - (double)sizeImage*0.5)/(double)sizeImage*2;
			l = 1.0/tan(radZoomX*0.5);
			
			double x,y,z;
			x = -1;
			
			y = yh/l*(x);
			z = -xh/l*(x);
			
			vecX.push_back(ofPoint(x*4000+posCamX.x, y*4000, z*4000));
		}
		
		vecY.clear();
		for (int i = 0; i < contourFinderY.nBlobs; i++){
			double xh, yh, l;
			xh = ((double)contourFinderY.blobs[i].centroid.x - (double)sizeImage*0.5)/(double)sizeImage*2;
			yh = ((double)contourFinderY.blobs[i].centroid.y - (double)sizeImage*0.5)/(double)sizeImage*2;
			l = 1.0/tan(radZoomY*0.5);
			
			double x,y,z;
			y = -1;
			
			x = xh/l*(y);
			z = -yh/l*(y);
			
			vecY.push_back(ofPoint(x*4000, y*4000+posCamY.y, z*4000));
		}
		
		vecZ.clear();
		for (int i = 0; i < contourFinderZ.nBlobs; i++){
			double xh, yh, l;
			xh = ((double)contourFinderZ.blobs[i].centroid.x - (double)sizeImage*0.5)/(double)sizeImage*2;
			yh = ((double)contourFinderZ.blobs[i].centroid.y - (double)sizeImage*0.5)/(double)sizeImage*2;
			l = 1.0/tan(radZoomZ*0.5);
			
			double x,y,z;
			z = -1;
			
			x = xh/l*(z);
			y = yh/l*(z);
			
			vecZ.push_back(ofPoint(x*4000, y*4000, z*4000+posCamZ.z));
		}
	};
}