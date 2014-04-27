#pragma once
#include "ofMain.h"
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
		
		void ReceiveImage();
		void SeparateImage();
		void LabelImage();
	};
}