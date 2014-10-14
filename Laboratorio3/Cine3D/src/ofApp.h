#pragma once

#include "ofMain.h"
#include "ofx3DModelLoader.h"
#include "ofxUI.h"

typedef struct butaca{
    bool used;
    int value;
    ofx3DModelLoader butaca3D;
}butaca;

class ofApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();
        void guiEvent(ofxUIEventArgs &e);
        void exit();
    
        ofEasyCam cam; 
        butaca cine[10][10];
    
        ofPlanePrimitive piso;
        ofImage textura_piso;
    
        ofPlanePrimitive pared_izq;
        ofImage textura_pared_izq;
    
        ofPlanePrimitive pared_der;
        ofImage textura_pared_der;
    
        ofPlanePrimitive screen;
        ofImage textura_screen;
    
        ofxUICanvas *gui;
    
        int ticks,angle;
        bool rotate;
};

