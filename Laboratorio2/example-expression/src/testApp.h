#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxNetwork.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void getAndSendActualExpression();
	void keyPressed(int key);
	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
	ExpressionClassifier classifier;
    ofxUDPManager udpConnection_cmd;
    string message;
};
