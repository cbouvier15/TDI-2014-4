#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed (int key);
	void keyReleased (int key);
	void mouseDragged(int x, int y, int button);
	
	ofxMidiOut midiOut;
	int channel;
	
	int note, velocity;
    
    int ticks;
    bool pressed,pressed_w,pressed_a;
    int ticks_w;
    
    float r,g,b;
    
};
