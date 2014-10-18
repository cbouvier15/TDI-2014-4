#include "ofApp.h"

void ofApp::setupGUI(){
    // GUI
    ofEnableSmoothing();
    
    gui = new ofxUICanvas();
    
    ofxUIColor gui_color(1,0,0,1);
    gui->setColorBack(gui_color);
    
    gui->setGlobalButtonDimension(64);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->addMultiImageToggle("TOGGLE1", "icons/Libros.png", false);
    gui->addMultiImageToggle("TOGGLE2", "icons/JazzBlues.png", false);
    gui->addMultiImageToggle("TOGGLE3", "icons/Diario.png", false);
    
    vector<string> items;
    items.push_back("FIRST ITEM"); items.push_back("SECOND ITEM"); items.push_back("THIRD ITEM");
    ddl = gui->addDropDownList("DROP DOWN LIST", items);
    
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    
    gui->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void ofApp::setupLights(){
    //some model / light stuff
    ofEnableDepthTest();
    glShadeModel (GL_SMOOTH);
    
    // turn on smooth lighting //
    ofSetSmoothLighting(true);
    
    // Point lights emit light in all directions //
    // set the diffuse color, color reflected from the light source //
    pointLight.setSpotConcentration(100.f);
    pointLight.setDiffuseColor(ofColor(255.f, 255.f, 200.f));
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 200.f));
    pointLight.setPosition(0, 0, 1000);
    pointLight.setPointLight();
}

//--------------------------------------------------------------
void ofApp::setupCamera(){
    //cam.setFarClip(3000.f);
    //cam.setNearClip(100.f);
    //cam.setOrientation(ofVec3f(1,0,0));
    //cam.lookAt(ofVec3f(0,0,0),ofVec3f(0,0,1));
    //cam.setPosition(0, 300, 0);
    //cam.setTarget(ofVec3f(500,0,0));
    
    cam_distance = 1300.f;
    target_pos = ofVec3f(0,10,-5);
    
    // Asi es una buena posicion para comenzar
    cam.setTarget(target_pos);
    cam.setDistance(cam_distance);
    cam.disableMouseInput();
}

//--------------------------------------------------------------
void ofApp::setupData(){
    xml_file.loadFile("culturalSurvey.xml");
    
    survey = new question_node[questionCount];
    
    cout << "****** START XML PARSING *******" << endl;
    xml_file.pushTag("culturalSurvey");
    questionCount = xml_file.getNumTags("questionNode");
    cout << "Cantidad question node: " << questionCount << endl;
    for(int i = 0; i < questionCount; i++){
        xml_file.pushTag("questionNode",i);
        cout << "Inicio nodo" << i << endl;
        survey[i].question = (char*)(xml_file.getValue("question", "").c_str());
        cout << survey[i].question << endl;
        survey[i].yesAnswer = xml_file.getValue("yesAnswer", 0);
        cout << survey[i].yesAnswer << endl;
        survey[i].noAnswer = xml_file.getValue("noAnswer", 0);
        cout << survey[i].noAnswer << endl;
        survey[i].iconPath = (char*)(xml_file.getValue("iconPath", "").c_str());
        cout << survey[i].iconPath << endl;
        survey[i].modelPath = (char*)(xml_file.getValue("modelPath", "").c_str());
        cout << survey[i].modelPath << endl;
        xml_file.popTag();
        cout << "Fin nodo" << i << endl;
    }
    xml_file.popTag();
    cout << "****** END XML PARSING *******" << endl;
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLogLevel(OF_LOG_VERBOSE);

    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofBackground(10, 10, 10);
    ofSetVerticalSync(true);
    
    piso.setPosition(0.0, 0.0, -160);
    piso.set(2000.0,2000.0);
    
    ofDisableArbTex();
    textura_esfera.loadImage("carpet.jpg");
    textura_esfera.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    textura_cono.loadImage("Leather.jpg");
    textura_cono.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    // Cargo las butacas.
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            //cine[i][j].butaca3D.loadModel("butaca/butaca.3ds", 1.5);
            cine[i][j].butaca3D.loadModel("Butaca - texturada.3DS", 1.5);
            cine[i][j].butaca3D.setPosition((j-5)*100,(i-5)*100,(i-5)*30);
            cine[i][j].used = true;
        }
    }
    
    // GUI
    setupGUI();
    
    // Lights
    setupLights();
    
    // Camera
    setupCamera();
    
    // Data
    //setupData();
    
    ticks = 0;
    rotate = false;
    angle = 36;

    //shader.load("shaders/toon.vert", "shaders/toon.frag");
    
    drawBall = false;
    drawCone = false;
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(name == "TOGGLE1"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value TOGGLE1: " << toggle->getValue() << endl;
        
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                if(toggle->getValue()){
                    drawCone = true;
                }else{
                    drawCone = false;
                }
            }
        }
        
    } else if(name == "TOGGLE3"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value TOGGLE3: " << toggle->getValue() << endl;
        
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                if(toggle->getValue()){
                    drawBall=true;
                }else{
                    drawBall=false;
                }
            }
        }
    }else if(name == "TOGGLE2"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        cout << "value TOGGLE2: " << toggle->getValue() << endl;
        rotate = true;
    }else if(name == "DROP DOWN LIST"){
        ofxUIDropDownList *dropDown = (ofxUIDropDownList *) e.widget;
        
        if(dropDown->getSelectedNames().size() > 0){
            cout << "value DROP DOWN: " << dropDown->getSelectedNames()[0] << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Se determina si hay que generar la animacion de rotacion
    // Y se aplica la rotacion.
    if (rotate && ticks <= 10){
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                if(((i==5) || (i==6) || (i==7) )){
                    cine[i][j].butaca3D.setRotation(1, angle*ticks, 0, 0, 1);
                }
            }
        }
        ticks++;
    }
    
    if(ticks>10){
        rotate = false;
        ticks = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //ofSetColor(255.f,255.f,255.f);
    
    // enable lighting //
    ofEnableLighting();
    pointLight.enable();

    cam.begin();
        ofDrawAxis(100.0);
        ofDrawGrid();
    
        piso.draw();
    
        // Se renderizan las butacas.
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                if(cine[i][j].used){
                    //ofSetColor(107.f,52.f,73.f);
                    cine[i][j].butaca3D.draw();
                    //ofSetColor(255.f,255.f,255.f);
                }
                if (drawBall){
                    textura_esfera.getTextureReference().bind();
                    ofDrawSphere(cine[i][j].butaca3D.pos[0],cine[i][j].butaca3D.pos[1] ,cine[i][j].butaca3D.pos[2]+75, 25);
                    textura_esfera.getTextureReference().unbind();
                }
                
                if(drawCone){
                    textura_cono.getTextureReference().bind();
                    ofDrawCone(cine[i][j].butaca3D.pos[0],cine[i][j].butaca3D.pos[1] ,cine[i][j].butaca3D.pos[2]+100, 25,25);
                    textura_cono.getTextureReference().unbind();
                }
            }
        }
	cam.end();
    ofDisableLighting();
    
    gui->draw();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    pointLight.setPosition(x, y, 1000);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'c') {
        if(cam.getMouseInputEnabled()) cam.disableMouseInput();
        else cam.enableMouseInput();
    }
    
    if (key == OF_KEY_UP) {
        cam_distance -= 10;
        cam.setDistance(cam_distance);
    }
    
    if (key == OF_KEY_DOWN) {
        cam_distance += 10;
        cam.setDistance(cam_distance);
    }
    
    if (key == OF_KEY_RIGHT){
        target_pos = ofVec3f(target_pos.x+5.0f,target_pos.y+5.0f,target_pos.z);
        cam.setTarget(target_pos);
    }
    
    if (key == OF_KEY_LEFT){
        target_pos = ofVec3f(target_pos.x-5.0f,target_pos.y-5.0f,target_pos.z);
        cam.setTarget(target_pos);
    }
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    delete gui;
}
