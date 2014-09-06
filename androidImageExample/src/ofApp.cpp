#include "ofApp.h"
#include "ofxAndroidVibrator.h"

//--------------------------------------------------------------
void ofApp::setup(){
	presentation = true;
	presentationCounter = 0;
    shock = glitch = false;

    counter = 0;

    r = g = b = 255;

	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	ofxAccelerometer.setup();

	selectImage();

	image.loadImage(imagePath);
	presentationImage.loadImage("presentation.png");

	ofBackground(255,255,255);
	ofSetColor(255,255,255);
}

//--------------------------------------------------------------
void ofApp::selectImage(){
	imagesDir.listDir("images");
	int randomImage = ofRandom(0,imagesDir.size()-1);
	imagePath = imagesDir.getPath(randomImage);

	image.loadImage(imagePath);
}

//--------------------------------------------------------------
void ofApp::update(){
	accel = ofxAccelerometer.getForce();
	if((accel.x > 1.3) || (accel.y > 1.3)){
		shock = true;
	}

    // Obtengo los pixeles del video en el momento
    ofPixels pixels = image.getPixelsRef();

    if(shock){
        r = ofRandom(0, 255);
        g = ofRandom(0, 255);
        b = ofRandom(0, 255);
        counter++;
        if(counter == 45){
            glitch = true;
            counter = 0;
        }
    }

    if(glitch){
        int glitchNum = ofRandom(1,4);
        if(glitchNum == 1 ){
            // Aplico el glitch
            explosionGlitch(pixels);
        }else if (glitchNum == 2){
            // Aplico el glitch
            stretchGlitch(pixels);
        }else if(glitchNum == 3){
            // Aplico el glitch
            colorGlitch(pixels);
        }
        glitch = false;
        shock = false;
        r = 255;
        g = 255;
        b = 255;
        ofxAndroidVibrator::vibrate(50);
    }
    presentationCounter++;
    if (presentationCounter == 200)
    	presentation = false;
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(presentation)
    	presentationImage.draw(0,0);
    else {
        ofBackground(r, g, b);
        ofSetColor(r, g, b);
        image.draw(0,0);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::explosionGlitch(ofPixels pixels){
	int explosionQty = 0;

	while (explosionQty < 15){
		// Inicializo las matrices para el intercambio de colores.
		int limitI = ofRandom(16,128);
		int limitJ = ofRandom(16,128);

		ofColor** matrix0 = new ofColor*[limitI];
		ofColor** matrix1 = new ofColor*[limitI];

		for (int i = 0; i < limitI; ++i){
			matrix0[i] = new ofColor[limitJ];
			matrix1[i] = new ofColor[limitJ];
		}

		// Obtengo dos posiciones aleatorais
		int posX0 = ofRandom(0,pixels.getWidth()-128);
		int posY0 = ofRandom(0,pixels.getHeight()-128);

		//Scan all the pixels
		for(int y=posY0,i=0;y<pixels.getHeight(),i<limitI;y++,i++){
			for(int x=posX0,j=0;x<pixels.getWidth(),j<limitJ;x++,j++){
				matrix0[i][j] = pixels.getColor(x, y);
			}
		}

		// Obtengo dos posiciones aleatorais
		int posX1 = ofRandom(0,pixels.getWidth()-128);
		int posY1 = ofRandom(0,pixels.getHeight()-128);

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
}

//--------------------------------------------------------------
void ofApp::stretchGlitch(ofPixels pixels){
    int explosionQty = 0;

    while (explosionQty < 15){

        // Obtengo dos posiciones aleatorais
		int posX1 = ofRandom(0,pixels.getWidth()-128);
		int posY1 = ofRandom(0,pixels.getHeight()-128);

        for(int x=posX1,j=0;x<pixels.getWidth(),j<25;x++,j++){

            ofColor color = pixels.getColor(x,posY1);

            for(int y=posY1;y<pixels.getHeight();y++){
                pixels.setColor(x, y, color);
            }
        }
        explosionQty++;
    }
    // Actualizo pixels
    image.setFromPixels(pixels);
}

//--------------------------------------------------------------
void ofApp::colorGlitch(ofPixels pixels){

    // Cargo la tabla de colores random
    int table[16];
    for (int i=0;i<16;i++)
        table[i] = ofRandom(0, 255);

    //Scan all the pixels
    for(int y=0;y<pixels.getHeight();y++){
        for(int x=0;x<pixels.getWidth();x++){

            ofColor col = pixels.getColor(x, y);
            col.r = table[col.r/16];
            col.g = table[col.g/16];
            col.b = table[col.b/16];
            pixels.setColor(x, y, col);
        }
    }
    // Actualizo pixels
    image.setFromPixels(pixels);
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
	image.saveImage(imagePath);
}

//--------------------------------------------------------------
void ofApp::stop(){
	image.saveImage(imagePath);
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
