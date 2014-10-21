#include "ofApp.h"

void ofApp::setupGUI(){
    // GUI
    ofEnableSmoothing();
    
    gui = new ofxUICanvas();
    
    ofxUIColor gui_color(1,0,0,1);
    gui->setColorBack(gui_color);
    
    gui->setGlobalButtonDimension(105);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
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
    
    xml_file.pushTag("culturalSurvey");
    questionCount = xml_file.getNumTags("questionNode");
    
    survey = new question_node[questionCount];
    
    // Inicializo array modelos de preguntas
    questionModels = new ofx3DModelLoader[questionCount];
    
    // Inicializo array colores modelos
    questionModelColors = new questionModelColor[questionCount];
    
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
        survey[i].question = (char*)(xml_file.getValue("question", "").c_str());
        survey[i].yesAnswer = xml_file.getValue("yesAnswer", 0);
        survey[i].noAnswer = xml_file.getValue("noAnswer", 0);
        
        survey[i].toggleName = (char*)(xml_file.getValue("iconName", "").c_str());
        string togglePath = (char*)(xml_file.getValue("iconPath", "").c_str());
        gui->addMultiImageToggle(survey[i].toggleName, togglePath, false);
        
        float scale = xml_file.getValue("modelScale", 0.0);
        questionModels[i].loadModel((char*)(xml_file.getValue("modelPath", "").c_str()), scale);
        
        questionModelColors[i].r = xml_file.getValue("modelColorR", 0);
        questionModelColors[i].g = xml_file.getValue("modelColorG", 0);
        questionModelColors[i].b = xml_file.getValue("modelColorB", 0);
        
        xml_file.popTag();
    }
    xml_file.popTag();
    
    gui->autoSizeToFitWidgets();
}
//--------------------------------------------------------------

// Se encarga de marcar que butacas dibujar según la cantidad
// De preguntas seleccionadas
void ofApp::markToDraw(int current){
    int count=0;
    
    for(int i=0; i<questionCount;i++){
        if(selectedQuestion[i]){count++;}
    }
    
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
        // Si seleccione current
        if(selectedQuestion[current]){
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
          // Si deseleccione current
        }else{
            // Marco butacas a quitar modelo pregunta
            for(int i=0; i<10; i++){
                for(int j=0; j<10; j++){
                    cineControl[i][j].question[current] = false;
                }
            }
        }
    }else if (count ==3){
        // Si seleccione current
        if(selectedQuestion[current]){
            int ready = false;
            int index1, index2;
            index1 = index2 = -1;
            for(int i=0; i<questionCount; i++){
                if(i != current && selectedQuestion[i]){
                    if(index1 == -1)
                        index1 = i;
                    else
                        index2 = i;
                }
            }
            // Calculo la cantidad de butacas de interseccion a dibujar
            float probIndex1 = ((float)(survey[index1].yesAnswer))/100.0;
            float probCurrent = ((float)(survey[current].yesAnswer))/100.0;
            int intersection1 = (int)((probIndex1*probCurrent)*100);
            
            float probIndex2 = ((float)(survey[index2].yesAnswer))/100.0;
            int intersection2 = (int)((probIndex2*probCurrent)*100);
            
            int intersectionTriple = int(probIndex2*probCurrent*probIndex1*100);
            
            intersection1 = intersection1 - intersectionTriple;
            intersection2 = intersection2 - intersectionTriple;
            
            int toDrawCurrent = survey[current].yesAnswer - intersection1 - intersection2 - intersectionTriple;
            
            for(int i=0; i<10; i++){
                for(int j=0; j<10; j++){
                    if(cineControl[i][j].question[index1] && cineControl[i][j].question[index2] && (intersectionTriple>0)){
                        cineControl[i][j].question[current] = true;
                        intersectionTriple--;
                    }else if(cineControl[i][j].question[index1] && !cineControl[i][j].question[index2] && (intersection1>0)){
                        cineControl[i][j].question[current] = true;
                        intersection1--;
                    }else if(!cineControl[i][j].question[index1] && cineControl[i][j].question[index2] && (intersection2>0)){
                        cineControl[i][j].question[current] = true;
                        intersection2--;
                    }else if(!cineControl[i][j].question[index1] && !cineControl[i][j].question[index2] && (toDrawCurrent>0)){
                        cineControl[i][j].question[current] = true;
                        toDrawCurrent--;
                    }
                }
            }
        // Si deseleccione current
        }else{
            // Marco butacas a quitar modelo pregunta
            for(int i=0; i<10; i++){
                for(int j=0; j<10; j++){
                    cineControl[i][j].question[current] = false;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::setupFonts(){
    // Fonts
    ofTrueTypeFont::setGlobalDpi(72);
    
    porcentaje_text.loadFont("fonts/Lato-Bla.ttf",65, true, true);
    porcentaje_text.setLineHeight(30.0f);
    porcentaje_text.setLetterSpacing(1.037);
    
    porcentaje_symbol.loadFont("fonts/Lato-Lig.ttf",50, true, true);
    porcentaje_text.setLineHeight(30.0f);
    porcentaje_text.setLetterSpacing(1.037);
    
    mensaje_fijo.loadFont("fonts/Lato-Lig.ttf",30, true, true);
    mensaje_fijo.setLineHeight(30.0f);
    mensaje_fijo.setLetterSpacing(1.037);
    
    mensaje_especifico.loadFont("fonts/Lato-Bol.ttf",40, true, true);
    mensaje_especifico.setLineHeight(30.0f);
    mensaje_especifico.setLetterSpacing(1.037);
    
    mensaje_error.loadFont("fonts/Lato-Lig.ttf",30, true, true);
    mensaje_error.setLineHeight(30.0f);
    mensaje_error.setLetterSpacing(1.037);
    
    ticks_text = 0;
    ticks_error = 0;
    
    mostrar_error = false;
    mostrar_mensaje = false;
}

//--------------------------------------------------------------
void ofApp::showError(){
    ticks_error++;
    ofSetColor(255.f,255.f,255.f);
    if (mostrar_error && ticks_error < 90){
        mensaje_error.drawString("Esta version solo soporta cruzamiento de 3 datos.", 200,465);
    } else if (ticks_error >= 90){
        ticks_error = 0;
        mostrar_error = false;
    }
}

//--------------------------------------------------------------
void ofApp::showText(){
    
    ticks_text++;
    ofSetColor(255.f,255.f,255.f);
    if (mostrar_mensaje && ticks_text < 120){
        porcentaje_text.drawString(ofToString(porcentaje), 200, 400);
        porcentaje_text.drawString("%", 280, 400);
        mensaje_fijo.drawString("de las personas que te rodean", 200, 425);
        mensaje_especifico.drawString(mensaje, 200,465);
    } else if (ticks_text >= 120){
        ticks_text = 0;
        mostrar_mensaje = false;
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    
//    ofLogLevel(OF_LOG_VERBOSE);

    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofBackground(10, 10, 10);
    ofSetVerticalSync(true);
    
    piso.setPosition(0.0, 0.0, -160);
    piso.set(2000.0,2000.0);
    
    ofDisableArbTex();
    
    // Cargo las butacas.
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cine[i][j].butaca3D.loadModel("objects/butaca.3DS", 1.5);
            cine[i][j].butaca3D.setPosition((j-5)*100,(i-5)*100,(i-5)*30);
            cine[i][j].used = true;
        }
    }
    
    // Lights
    setupLights();
    
    // Camera
    setupCamera();
    
    // Fonts
    setupFonts();
    
    presentacion.loadImage("textures/inicio.jpg");
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    int index = 0;
    bool done = false;
    
    int count=0;
    
    while(!done && (index<= questionCount)){
        if(name == survey[index].toggleName){
            ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
            selectedQuestion[index] = toggle->getValue();
            for(int i=0; i<questionCount;i++){
                if(selectedQuestion[i]){count++;}
            }
            // Se marco/desmarco pregunta 0
            markToDraw(index);
            if (toggle->getValue() && (count < 4)){
                mostrar_mensaje = true;
                mensaje = survey[index].question;
                porcentaje = survey[index].yesAnswer;
            }else if(count >= 4 ){
                showError();
            }else{
                mostrar_mensaje = false;
            }
            done = true;
        }else {index++;}
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofSetColor(255.f,255.f,255.f);
    
    // Estado inicial, se muestra imagen de presentacion.
    if (state == 0){
        ticks++;
        
        if (ticks > 0 && ticks <= 80){
            ofEnableAlphaBlending();
            presentacion.draw(ofPoint(0,0,0), 1024, 768);
            ofDisableAlphaBlending();
        }
        
        if (ticks > 80 && ticks <= 90){
            animation_state+=120;
            presentacion.draw(ofPoint(animation_state,0,0), 1024, 768);
        }
        
        if(ticks>90){
            state = 1;
            ticks = 0;
            
            // GUI
            setupGUI();
            // Data
            setupData();
        }
        
        // Estado de visualizacion de datos.
    } else if(state == 1){
    
        ofSetColor(255,255,255);
        // enable lighting //
        ofEnableLighting();
        pointLight.enable();

        cam.begin();
        piso.draw();
    
        // Se renderizan las butacas.
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                if(cine[i][j].used){
                    ofSetColor(70.f,95.f,56.f);
                    cine[i][j].butaca3D.draw();
                }
                // ESTA SOLO PARA DOS PREGUNTAS - HAY QUE ACTUALIZARLO A MAS PREGUNTAS LUEGO
                for(int k=0; k<questionCount; k++){
                    int x,y,z;
                    x = cine[i][j].butaca3D.pos[0];
                    y = cine[i][j].butaca3D.pos[1];
                    z = cine[i][j].butaca3D.pos[2]+50;
                    if(cineControl[i][j].question[k]){
                        if((k%questionCount)==0){
                            x+=20;
                            y-=38;
                            questionModels[k].setRotation(1,-35,0,0,1);
                        }else if((k%questionCount)==1){
                            y+=19;
                            z+=42;
                        }else if((k%questionCount)==2){
                            questionModels[k].setRotation(1,87,0,0,1);
                            z+=18;
                            x-=24;
                            y-=25;
                        }else if((k%questionCount)==3){
                            x+=9;
                            questionModels[k].setRotation(1,45,0,1,0);
                        }else if((k%questionCount)==4){
                            x-=70;
                            y-=50;
                        }
                        else if((k%questionCount)==5){
                            x+=0;
                            y-=12;
                            z+=0;
                        }
                        
                        ofSetColor(questionModelColors[k].r, questionModelColors[k].g, questionModelColors[k].b);
                        questionModels[k].setPosition(x, y,z);
                        questionModels[k].draw();
                    }
                }
            }
        }
        cam.end();
        ofDisableLighting();
    
        showText();
        
        gui->draw();
    }
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
