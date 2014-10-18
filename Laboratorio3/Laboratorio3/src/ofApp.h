#pragma once

#include "ofMain.h"
#include "ofx3DModelLoader.h"
#include "ofxUI.h"
#include "ofxAssimpModelLoader.h"

typedef struct butaca{
    bool used;
    int value;
    ofx3DModelLoader butaca3D;
}butaca;

typedef struct question_node{
    int qNumber;
    char* question;
    int yesAnswer;
    int noAnswer;
    char* iconPath;
    char* modelPath;
}question_node;

class ofApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();
        void setupGUI();
        void setupLights();
        void setupCamera();
        void setupData();
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void guiEvent(ofxUIEventArgs &e);
        void keyPressed(int key);
        void exit();
    
        ofxXmlSettings xml_file;
    
        question_node* survey;
        int questionCount;
    
        ofEasyCam cam;
        float cam_distance;
        ofVec3f target_pos;
    
        butaca cine[10][10];
    
        ofPlanePrimitive piso;
        ofImage textura_esfera;
        ofImage textura_cono;
    
        ofxUICanvas *gui;
    
        int ticks,angle;
        bool rotate;
        bool drawBall;
        bool drawCone;
    
        ofxUIDropDownList *ddl;
    
        ofShader shader;
    
        ofMaterial material;
    
        ofLight pointLight;
    
        ofxAssimpModelLoader model;
};

