#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {

	ofSetVerticalSync(true);
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
    // Se obtienen los puertos MIDI
	midiOut.listPorts();
    
	// Se realiza la conexion con el puerto creaedo - TDI
	midiOut.openPort("IAC Driver TDI");
    
	channel = 0;
	note = 0;
	velocity = 0;
    ticks = 0;
    
    r = 255.0;
    g = 255.0;
    b = 255.0;
}

//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(r,g,b);
    ofFill();
    ofRect( 0, 0, 1024,1024 );
}

//--------------------------------------------------------------
void testApp::exit() {
	// clean up
	midiOut.closePort();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    switch(key) {
        // Se mapea la tecla presionada con un canal.
        // Cada canal se mapea con un canal MIDI y es tratado
        // independientemente en Live.
        case 'w':
            channel = 1;
            note = 50;
            r=251;g=38;b=12;
            break;
        case 'a':
            channel = 2;
            note = 60;
            r=217;g=68;b=246;
            break;
        case 's':
            channel = 3;
            note = 70;
            r=111;g=0;b=176;
            break;
        case 'd':
            channel = 4;
            note = 80;
            r=45;g=0;b=212;
            break;
        case 'f':
            channel = 5;
            note = 90;
            r=205;g=86;b=208;
            break;
        case 'g':
            channel = 6;
            r=250;g=86;b=48;
            break;
        case OF_KEY_RIGHT:
            r=250;g=86;b=48;
            channel = 7;
            break;
        case OF_KEY_LEFT:
            r=200;g=86;b=60;
            channel = 8;
            break;
            
    }
    
    velocity = 127;
    note = ofMap(key, 48, 122, 0, 127);
    midiOut.sendNoteOn(channel, note,  velocity);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}