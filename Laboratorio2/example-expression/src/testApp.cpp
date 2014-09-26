#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	
	tracker.setup();
	tracker.setRescale(.5);
    
    // UDP connection for commands - Indicate actiones.
    udpConnection_cmd.Create();
    udpConnection_cmd.Bind(11888);
    udpConnection_cmd.SetNonBlocking(true);
    
    message = "NonExpression";
    
    classifier.load("expressions");
}

void testApp::update() {
    char signal;
	cam.update();
	if(cam.isFrameNew()) {
		if(tracker.update(toCv(cam))) {
			classifier.classify(tracker);
		}		
	}
    udpConnection_cmd.Receive(&signal, 1);
    if(signal == 'a'){
        getAndSendActualExpression();
    }
}

static OSStatus setTextToPasteboard(char* byteArrayIndex) {
    
	OSStatus                err = noErr;
	static PasteboardRef	pasteboard = NULL;
	PasteboardCreate( kPasteboardClipboard, &pasteboard );
    
	err = PasteboardClear( pasteboard );
    require_noerr( err, PasteboardClear_FAILED );
    
	CFDataRef  data;
    
	data = CFDataCreate(kCFAllocatorDefault, (uchar*) byteArrayIndex, strlen(byteArrayIndex)+1);
	err = PasteboardPutItemFlavor( pasteboard, (PasteboardItemID)1, kUTTypeUTF8PlainText, data, 0);
	require_noerr( err, PasteboardPutItemFlavor_FAILED );
    
PasteboardPutItemFlavor_FAILED:
PasteboardClear_FAILED:
    return err;
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	tracker.draw();
	
	int w = 100, h = 12;
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(5, 10);
	int n = classifier.size();
	int primary = classifier.getPrimaryExpression();    
    
  for(int i = 0; i < n; i++){
		ofSetColor(i == primary ? ofColor::red : ofColor::black);
		ofRect(0, 0, w * classifier.getProbability(i) + .5, h);
		ofSetColor(255);
		ofDrawBitmapString(classifier.getDescription(i), 5, 9);
		ofTranslate(0, h + 5);
  }
	ofPopMatrix();
	ofPopStyle();
	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
	drawHighlightString(
		string() +
		"r - reset\n" +
		"e - add expression\n" +
		"a - add sample\n" +
		"s - save expressions\n"
		"l - load expressions",
		14, ofGetHeight() - 7 * 12);
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
		classifier.reset();
	}
	if(key == 'e') {
		classifier.addExpression();
	}
	if(key == 'a') {
		classifier.addSample(tracker);
	}
	if(key == 's') {
		classifier.save("expressions");
	}
}


void testApp::getAndSendActualExpression(){
    int primary = classifier.getPrimaryExpression();
    message = classifier.getDescription(primary);
    cout << "Voy a enviar: " << message << endl;
    setTextToPasteboard((char*)message.c_str());
}