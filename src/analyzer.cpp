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
			LineMatch();
			
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
		// cout<<"vecY.size : "<<vecY.size()<<"\n";
		ofSetColor(255, 127, 127);
		for (int i = 0; i < vecX.size(); i++){
			// ofLine(vecX[i]*2000+posCamX,posCamX);
			ofLine(vecX[i]*2000,ofPoint(0,0,0));
		}
		
		ofSetColor(127, 255, 127);
		for (int i = 0; i < vecY.size(); i++){
			ofLine(vecY[i]*2000+posCamY,posCamY);
		}
		
		ofSetColor(127, 127, 255);
		for (int i = 0; i < vecZ.size(); i++){
			ofLine(vecZ[i]*2000+posCamZ,posCamZ);
		}
		
		ofSetColor(0, 0, 0);
		for (int i = 0; i < posTargets.size(); i++){
			ofCircle(posTargets[i], 4);
		}
		ofLine(ofPoint(0,2,0),ofPoint(0,2,0)+ofPoint(4,0,6).normalize());
		ofLine(ofPoint(0,3,0),ofPoint(0,3,0)+ofPoint(5,0,6).normalize());
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
	
	bool Analyzer::Calc2LineNearestDistAndPos(
			ofPoint pp1,
			ofPoint pv1,
			ofPoint pp2,
			ofPoint pv2,
			float* pOut_dist,
			ofPoint* pOut_pos1,
			ofPoint* pOut_pos2
	){
		ofPoint v1, v2;
		ofPoint p1 = pp1, p2 = pp2;
		
		// // D3DXVec3Normalize( &v1, pv1 );
		// // D3DXVec3Normalize( &v2, pv2 );
		
		v1 = pv1.normalize();
		v2 = pv2.normalize();
		
		// // float D1 = D3DXVec3Dot( &(p2 - p1), &v1 );
		// // float D2 = D3DXVec3Dot( &(p2 - p1), &v2 );
		
		float D1 = (p2 - p1).dot(v1);
		float D2 = (p2 - p1).dot(v2);
		
		ofPoint cross ;
		// float Dv = D3DXVec3LengthSq( D3DXVec3Cross( &cross, &v1, &v2 ) );
		cross = v1.cross(v2);
		float Dv = cross.lengthSquared();
		
		if (Dv < 0.000001f) {
			if ( pOut_dist ) {
				ofPoint v;
				// *pOut_dist = D3DXVec3Length( D3DXVec3Cross( &v, &(p2 - p1), &v1 ) );
				v = (p2 - p1).cross(v1);
				*pOut_dist = v.length();
			}
			return false;
		}
		// float Dv = D3DXVec3Dot( &v1, &v2 );
		// float t1 = ( D1 - D2 * Dv ) / ( 1.0f - Dv * Dv );
		// float t2 = ( D2 - D1 * Dv ) / ( Dv * Dv - 1.0f );
		Dv = v1.dot(v2);
		float t1 = ( D1 - D2 * Dv ) / ( 1.0f - Dv * Dv );
		float t2 = ( D2 - D1 * Dv ) / ( Dv * Dv - 1.0f );

		ofPoint
		Q1 = p1 + t1 * v1,
		Q2 = p2 + t2 * v2;

		if ( pOut_dist )
			*pOut_dist = (Q2 - Q1).length();

		if ( pOut_pos1 )
			*pOut_pos1 = Q1;

		if ( pOut_pos2 )
			*pOut_pos2 = Q2;

		return true;
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
			
			vecX.push_back(ofPoint(x, y, z).normalize());
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
			
			vecY.push_back(ofPoint(x, y, z).normalize());
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
			
			vecZ.push_back(ofPoint(x, y, z).normalize());
		}
	};
	
	void Analyzer::LineMatch(){
		maxDist = 2;
		posTargets.clear();
		
		float dist;
		ofPoint pos1;
		ofPoint pos2;
		bool isSuccess;
		
		for (int i = 0; i < vecX.size(); ++i)
		{
			bool flag = false;
			for (int j = 0; j < vecY.size(); ++j)
			{
				isSuccess = Calc2LineNearestDistAndPos(
					posCamX,
					vecX[i],
					posCamY,
					vecY[j],
					&dist,
					&pos1,
					&pos2
				);
				cout<<"dist : "<<dist<<" i : "<<i<<"\n";
				// cout<<"x:"<<pos1.x<<" y:"<<pos1.y<<" z:"<<pos1.z<<"\n";
				if(isSuccess&&(dist<maxDist)){
					for (int k = 0; k < vecZ.size(); ++k)
					{
						// cout<<"2match! : "<<i<<" & "<<j<<"\n";
						isSuccess = Calc2LineNearestDistAndPos(
							posCamY,
							vecY[j],
							posCamZ,
							vecZ[k],
							&dist,
							&pos1,
							&pos2
						);
						if(isSuccess&&(dist<maxDist)){
							posTargets.push_back((pos1+pos2)/2);
							flag = true;
							break;
						}
					}
				}
				if (flag)
				{
					flag = false;
					break;
				}
			}
		}
		
		Calc2LineNearestDistAndPos(
			ofPoint(0,2,0),
			ofPoint(4,0,6),
			ofPoint(0,3,0),
			ofPoint(5,0,6),
			&dist,
			&pos1,
			&pos2
		);
		// cout<<"dist : "<<dist<<"\n";
		
	};
}