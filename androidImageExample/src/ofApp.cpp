#include "ofApp.h"
#include "ofxAndroidVibrator.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	ofxAccelerometer.setup();
	image.loadImage("images/bikers.jpg");
	ofBackground(255,255,255);
	ofSetColor(255,255,255);

	snapShot = false;
}

//--------------------------------------------------------------
void ofApp::update(){
	accel = ofxAccelerometer.getForce();
	if((accel.x > 1.3) || (accel.y > 1.3)){
		snapShot = true;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	image.draw(0,0);

	if(snapShot == true){

		// Reseteo el booleano de captura
		snapShot = false;

		// Obtengo los pixeles del video en el momento
		ofPixels pixels = image.getPixelsRef();

		int explosionQty = 0;

		while (explosionQty < 15){
		// --------------------------------------------------------
		// Inicializo las matrices para el intercambio de colores.
		int limitI = ofRandom(16,128);
		int limitJ = ofRandom(16,128);

		ofColor** matrix0 = new ofColor*[limitI];
		ofColor** matrix1 = new ofColor*[limitI];

		for (int i = 0; i < limitI; ++i){
			matrix0[i] = new ofColor[limitJ];
			matrix1[i] = new ofColor[limitJ];
		}

		// --------------------------------------------------------
		// Obtengo dos posiciones aleatorais
		int posX0 = ofRandom(0,pixels.getWidth()-128);
		int posY0 = ofRandom(0,pixels.getHeight()-128);

		//Scan all the pixels
		for(int y=posY0,i=0;y<pixels.getHeight(),i<limitI;y++,i++){
			for(int x=posX0,j=0;x<pixels.getWidth(),j<limitJ;x++,j++){
				matrix0[i][j] = pixels.getColor(x, y);
			}
		}

		// --------------------------------------------------------
		// Obtengo dos posiciones aleatorais
		int posX1 = ofRandom(20,300);
		int posY1 = ofRandom(40,200);

		//Scan all the pixels
		for(int y=posY1,i=0;y<pixels.getHeight(),i<limitI;y++,i++){
			for(int x=posX1,j=0;x<pixels.getWidth(),j<limitJ;x++,j++){
				matrix1[i][j] = pixels.getColor(x, y);
				pixels.setColor(x, y, matrix0[i][j]);
			}
		}

		// --------------------------------------------------------
		for(int y=posY0,i=0;y<pixels.getHeight(),i<limitI;y++,i++){
			for(int x=posX0,j=0;x<pixels.getWidth(),j<limitJ;x++,j++){
				pixels.setColor(x, y, matrix1[i][j]);
			}
		}

		// Libero la memoria
		for (int i = 0; i < limitI; ++i){
			delete [] matrix0[i];
			delete [] matrix1[i];
		}
		delete [] matrix0;
		delete [] matrix1;

		explosionQty++;
		}

		// Actualizo pixels
		image.setFromPixels(pixels);

		ofxAndroidVibrator::vibrate(50);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){
}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){
	image.saveImage("images/image.jpg");
}

//--------------------------------------------------------------
void ofApp::stop(){
	image.saveImage("images/image.jpg");
}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
