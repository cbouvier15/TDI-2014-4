#include "ofApp.h"

GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};

//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(255,255,255);
		
	ofSetVerticalSync(true);

    //some model / light stuff
    ofEnableDepthTest();
    glShadeModel (GL_SMOOTH);

    /* initialize lighting */
    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
    
    // Cargo las butacas.
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cine[i][j].butaca3D.loadModel("squirrel/NewSquirrel.3ds", 5);
            cine[i][j].butaca3D.setPosition(j*50,i*50,i*10);
            cine[i][j].used = true;
        }
    }
    
	// this sets the camera's distance from the object
	cam.setDistance(1000);
    
    // Piso
    piso.set(600, 600);
    piso.setPosition(250, 250, 0);
    
    ofDisableArbTex();
    textura_piso.loadImage("carpet.jpg");
    textura_piso.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    // Pared Izq
    pared_izq.set(600, 350);
    pared_izq.setPosition(-30, 250, 175);
    pared_izq.rotate(90, 0, 1, 0);
    pared_izq.rotate(90, 1, 0, 0);
    
    ofDisableArbTex();
    textura_pared_izq.loadImage("curtain.jpg");
    textura_pared_izq.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    // Pared Der
    pared_der.set(600, 350);
    pared_der.setPosition(530, 250, 175);
    pared_der.rotate(90, 0, 1, 0);
    pared_der.rotate(90, 1, 0, 0);
    pared_der.rotate(180, 0, 0, 1);
    
    ofDisableArbTex();
    textura_pared_der.loadImage("curtain.jpg");
    textura_pared_der.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    // Pantalla
    screen.set(600, 350);
    screen.setPosition(250, -30, 175);
    screen.rotate(90, 0, 1, 0);
    screen.rotate(90, 1, 0, 0);
    screen.rotate(180, 0, 1, 0);
    screen.rotate(270, 0, 0, 1);

    
    ofDisableArbTex();
    textura_screen.loadImage("screen.jpg");
    textura_screen.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	
    ofBackground(255,255,255);
    
    cam.begin();
    
    ofDrawAxis(50.0);
    ofDrawGrid();

    // Piso
    textura_piso.getTextureReference().bind();
    piso.draw();
    
    // Izq
    textura_pared_izq.getTextureReference().bind();
    pared_izq.draw();
    
    // Der
    textura_pared_der.getTextureReference().bind();
    pared_der.draw();
    
    // Pantalla
    textura_screen.getTextureReference().bind();
    screen.draw();
    
    ofSetColor(255, 255, 255);
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(cine[i][j].used){
                cine[i][j].butaca3D.draw();
            }
        }
    }
    
	cam.end();

}
