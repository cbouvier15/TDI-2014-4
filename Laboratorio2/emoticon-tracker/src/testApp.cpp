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
    udpConnection_cmd.Bind(11999);
    udpConnection_cmd.SetNonBlocking(true);
    
    message = "NonExpression";
    
    // Se cargan las expresiones antes entrenadas.
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

    // Al recibir una señal en este puerto, se captura la
    // expresion y se guarda en el portapepeles el codigo
    // de emoticon para obtenerlo desde Adium.
    cout << udpConnection_cmd.Receive(&signal, 1);
    if(signal == 'a'){
        getAndSendActualExpression();
    }
}


// Metodo auxiliar para poner informacion en el Portapapeles de Mac OS.
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


// Funcion auxiliar que determina utilizando el classifier
// la expresion con mayor probabilidad y la almacena
// en el portapapeles.

void testApp::getAndSendActualExpression(){
    int primary = classifier.getPrimaryExpression();
    message = classifier.getDescription(primary);
    setTextToPasteboard((char*)message.c_str());
}
