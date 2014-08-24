#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    camWidth = 640;
    camHeight = 480;
    snapCounter = 0;
    
    
    //we can now get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
	}
    
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    vidGrabber.draw(192,20);
    
    if (snapshot == true){
        
        snapCounter++;
        snapshot = false;
		cout << "Snapshot N¼ " + ofToString(snapCounter)<< endl;
        
        imgToSave.setFromPixels(vidGrabber.getPixels(),camWidth, camHeight, OF_IMAGE_COLOR);
        imgToSave.saveImage("snapShot_" + ofToString(snapCounter) + ".png");
    }
    
    if (imgToSave.bAllocated()){
        imgToSave.draw(192,520);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // If spaceKey pressed capture a snapshot
    if (key == ' '){
		snapshot = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
