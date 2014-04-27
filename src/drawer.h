#pragma once

#include "ofMain.h"

namespace cerberus{
	class Drawer
			{
			public:
				Drawer();
				~Drawer();
				
				void setup();
				void update();
				void draw();
				
				ofEasyCam camera;
			};
}