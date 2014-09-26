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
	
    // VideoGrabber para capturar la imagen de la camara web.
	ofVideoGrabber cam;
	
    // Addon FaceTracker permite tracker el rostro en los
    // frames de la camara.
    ofxFaceTracker tracker;
    
    // Classifier sirve para determinar una expresion del rostro
    // capturado basandose en un entrenamiento previo persistido
    // como matrices de OpenCV.
	ExpressionClassifier classifier;
    
    // Puerto UDP que recibe la señal de Adium para generar el
    // emoticon que se quiere insertar.
    ofxUDPManager udpConnection_cmd;
    
    string message;
};
