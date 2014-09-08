#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxAccelerometer.h"

class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void selectImage();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		ofImage image, presentationImage;
		ofVec3f accel;
		ofDirectory imagesDir;
		string imagePath;

        void explosionGlitch(ofPixels pixels);
        void stretchGlitch(ofPixels pixels);
        void colorGlitch(ofPixels pixels);
        void mergeGlitch(ofPixels pixels);

        bool shock_soft,shock_hard,glitch, presentation;

        int counter, presentationCounter;

        int r,g,b;
};
