#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    camWidth = 320;
    camHeight = 240;
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
    
    
    snapShotSound.loadSound("snapshotSound.mp3");
	snapShotSound.setVolume(0.5f);
	font.loadFont("castaway.ttf", 32);
    
    img.allocate(320, 240, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0,0, 0);
    
    font.drawString("Amnesia", 440,40);
    
    vidGrabber.draw(352,50);
    
    if (snapshot == true){
        
        snapCounter++;
        snapShotSound.play();
        snapshot = false;
		cout << "Snapshot " + ofToString(snapCounter)<< endl;
        
        unsigned char* bytes = new unsigned char[camWidth*camHeight*3];
        bytes = vidGrabber.getPixels();
        
        imgToSave.setFromPixels(bytes,camWidth, camHeight, OF_IMAGE_COLOR);
        
        img.setFromPixels(bytes,camWidth, camHeight, OF_IMAGE_COLOR);
        
        int i = 0;
        while ( i < 10000 ) {
            int pos = ofRandom(3*img.getPixelsRef().size()/6,5*img.getPixelsRef().size()/6);
            
            cout << pos << endl;
            
            float color = abs(sin( float(pos) / 18.f)) * 255.f;
            img.getPixelsRef()[pos] = color;
            img.getPixelsRef()[pos+1] = color;
            img.getPixelsRef()[pos+2] = color;
            img.getPixelsRef()[pos+3] = color;
            img.getPixelsRef()[pos+4] = color;
            
            i++;
        }
        img.reloadTexture();
        
        img.saveImage("snapShot_" + ofToString(snapCounter) + ".png");
    }
    
    if (imgToSave.bAllocated()){
        imgToSave.draw(172,310);
        img.draw(510,310);
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
