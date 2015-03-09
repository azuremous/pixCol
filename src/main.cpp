#include "ofMain.h"
#include "ofApp.h"
//retina mode
//========================================================================
int main( ){
	ofSetupOpenGL(1200, 600,OF_WINDOW);			// <-------- setup the GL context
    ofSetWindowPosition(960, 300);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
