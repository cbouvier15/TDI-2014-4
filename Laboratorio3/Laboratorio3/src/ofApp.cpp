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
    
    xml_file.pushTag("culturalSurvey");
    questionCount = xml_file.getNumTags("questionNode");
    
    // Inicializo Información de estado las preguntas
    selectedQuestion = new bool[questionCount];
    for(int i=0; i<questionCount; i++) {selectedQuestion[i]=0;}
    
    // Inicializo Información de estado de las butacas por preguntas
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            cineControl[i][j].question = new bool[questionCount];
            for(int k=0; k<questionCount; k++) {cineControl[i][j].question[k]=false;}
        }
    }

    for(int i = 0; i < questionCount; i++){
        xml_file.pushTag("questionNode",i);
//        survey[i].question = (char*)(xml_file.getValue("question", "").c_str());
//        cout << survey[i].question << endl;
        survey[i].yesAnswer = xml_file.getValue("yesAnswer", 0);
        survey[i].noAnswer = xml_file.getValue("noAnswer", 0);
//        survey[i].iconPath = (char*)(xml_file.getValue("iconPath", "").c_str());
//        cout << survey[i].iconPath << endl;
//        survey[i].modelPath = (char*)(xml_file.getValue("modelPath", "").c_str());
//        cout << survey[i].modelPath << endl;
        xml_file.popTag();
    }
    xml_file.popTag();
}
//--------------------------------------------------------------

// Se encarga de marcar que butacas dibujar según la cantidad
// De preguntas seleccionadas
void ofApp::markToDraw(int current){
    int count=0;
    
    cout << "ENTRA A markToDraw con current: " << current << endl;
    
    for(int i=0; i<questionCount;i++){
        if(selectedQuestion[i]){count++;}
    }
    
    cout << "La cantidad de question seleccionadas: " << count << endl;
    
    if (count == 0){
        for(int i = 0; i<10; i++){
            for(int j = 0; j<10; j++){
                cineControl[i][j].question[current] = false;
            }
        }
    }
    // Solo una question seleccionada
    else if(count==1){
        int toDraw;
        bool drawValue;
        // Chequeo si hay solo una porque habian 0
        // o porque habian 2
        if(selectedQuestion[current]){
            toDraw = survey[current].yesAnswer;
            drawValue = true;
        }else{
            drawValue = false;
            toDraw = 100;
        }
        // Marco butacas a dibujar o quitar modelo pregunta
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                if(toDraw>0){
                    cineControl[i][j].question[current] = drawValue;
                    toDraw--;
                }
            }
        }
        
    // Dos question seleccionadas
    }else if (count==2){
        int ready = false;
        int index = 0;
        int intersection = 0;
        int toDrawCurrent = 0;
        // Busco cual es la que esta dibujada
        while(!ready){
            if ((current != index) && (selectedQuestion[index])) {
                ready = true;
            }else {index++;}
        }
        
        // Calculo la cantidad de butacas de interseccion a dibujar
        float probIndex = ((float)(survey[index].yesAnswer))/100.0;
        float probCurrent = ((float)(survey[current].yesAnswer))/100.0;
        intersection = (int)((probIndex*probCurrent)*100);
        
        // Calculo la cantidad de butacas sin interseccion a dibujar
        toDrawCurrent = survey[current].yesAnswer - intersection;

        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                // Si ya tiene dibujada una question, en caso de quedar intersecciones marco
                // una para dibujar
                if((cineControl[i][j].question[index] == true) && (intersection>0)){
                    cineControl[i][j].question[current] = true;
                    intersection--;
                // Si no tiene dibujada una question, en caso de quedar sin interseccion marco
                // una para dibujar
                }else if ((cineControl[i][j].question[index] == false) && (toDrawCurrent>0)){
                    cineControl[i][j].question[current] = true;
                    toDrawCurrent--;
                }
            }
        }
    }
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
    setupData();
    
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
        selectedQuestion[0] = toggle->getValue();
        // Se marco/desmarco pregunta 0
        markToDraw(0);
    } else if(name == "TOGGLE3"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        selectedQuestion[1] = toggle->getValue();
        // Se marco/desmarco pregunta 1
        markToDraw(1);
    }else if(name == "TOGGLE2"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        selectedQuestion[2] = toggle->getValue();
        rotate = true;
    }else if(name == "DROP DOWN LIST"){
        ofxUIDropDownList *dropDown = (ofxUIDropDownList *) e.widget;
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
                // ESTA SOLO PARA DOS PREGUNTAS - HAY QUE ACTUALIZARLO A MAS PREGUNTAS LUEGO

                // Se dibujan/quitan modelos de pregunta 0
                if (cineControl[i][j].question[0]){
                    textura_esfera.getTextureReference().bind();
                    ofDrawSphere(cine[i][j].butaca3D.pos[0],cine[i][j].butaca3D.pos[1] ,cine[i][j].butaca3D.pos[2]+75, 25);
                    textura_esfera.getTextureReference().unbind();
                }
                // Se dibujan/quitan modelos de pregunta 1
                if(cineControl[i][j].question[1]){
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
