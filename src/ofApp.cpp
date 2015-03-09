#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    selectedColorID = -1;
    selectMode = false;
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    width = height = ofGetHeight()/2 -40;
    colRectW = colRectH = ofGetHeight() - 40;
    bri = 255;
    pixColor.setHsb(255, 255, 255);
    
    stockedColors.clear();
    screen.allocate(ofGetWidth(), ofGetHeight());
    screen.begin();
    ofClear(0);
    for (int h = 0; h < colRectH; h++) {
        for (int w = 0; w < colRectW; w++) {
            int hue = (int)ofMap(w, 0, colRectW, 0, 255);
            int sat = (int)ofMap(h, 0, colRectH, 0, 255, false);
            
            ofColor c;
            c.setHsb(hue, sat, bri);
            ofSetColor(c);
            ofDrawRectangle(w, h, 1, 1);
        }
    }
    ofTranslate(colRectW + 5, 0);
    //draw bright rect
    for (int h = 0; h < colRectH; h++) {
        int b = (int)ofMap(h, 0, colRectH, 255, 0, true);
        ofColor c;
        c.set(255);
        c.setBrightness(b);
        ofSetColor(c);
        ofDrawRectangle(0, h, 20, 1);
    }
    screen.end();
    
    cursor.allocate(16, 16);
    cursor.begin();
    ofClear(0);
    ofSetLineWidth(2);
    ofSetColor(0);
    ofDrawLine(0, 8, 16, 8);
    ofDrawLine(8, 0, 8, 16);
    cursor.end();
    ofHideCursor();
    briBarH = 0;
    uiFont.load(ofFilePath::getCurrentWorkingDirectory()+"/Fibel Vienna.ttf", 30);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("pixCol");
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPushMatrix();
    ofTranslate(20, 20);
    ofPushMatrix();//draw color rect
    ofSetColor(255);
    screen.draw(0, 0);
    ofSetColor(0);
    ofDrawRectangle(colRectW + 5, briBarH - 20, 20, 2);
    ofPopMatrix();
    
    ofTranslate(40, 0);
    
    ofPushMatrix();
    //pic color rect
    ofTranslate(colRectW + 5, 0);
    ofSetColor(pixColor);
    ofDrawRectangle(0, 0, width, height);
    
    //back color
    ofSetColor(40);
    ofDrawRectangle(0, height + 20, width , ofGetHeight() - (height + 60));
    
    ofColor displayColor;
    if (selectMode) { displayColor = stockedColors[selectedColorID]; }
    else { displayColor = pixColor; }
    float h = uiFont.getStringBoundingBox("A", 0, 0).getHeight();
    ofTranslate(20, height + 35);
    ofSetColor(255);
    uiFont.drawString("H : "+ofToString((int)displayColor.getHue()), 0, h);
    //ofDrawBitmapString("h:"+ofToString((int)displayColor.getHue()), 0, 0);
    uiFont.drawString("S : "+ofToString((int)displayColor.getSaturation()), 0, h*2 + 10);
    uiFont.drawString("B : "+ofToString((int)displayColor.getBrightness()), 0, h*3 + 20);
    
    ofTranslate(0, 150);
    uiFont.drawString("R : "+ofToString((int)displayColor.r), 0, h);
    uiFont.drawString("G : "+ofToString((int)displayColor.g), 0, h*2 + 10);
    uiFont.drawString("B : "+ofToString((int)displayColor.b), 0, h*3 + 20);
    
    ofPopMatrix();
    
    //draw stocked color rect
    ofTranslate(colRectW + 5, 0);
    ofTranslate(width + 20, 0);
    for (int i = 0; i < stockedColors.size(); i++) {
        ofSetColor(stockedColors[i]);
        ofDrawRectangle(0, 20*i + 10*i, width, 20);
    }
    
    if (stockedColors.size() > 0 && selectMode) {
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(255);
        ofDrawRectangle(0, 20*selectedColorID + 10*selectedColorID, width, 20);
        ofPopStyle();
    }
    
    ofPopMatrix();
    
    ofPushMatrix();
    ofSetColor(255);
    cursor.draw(ofGetMouseX() - 8, ofGetMouseY() - 8);
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (stockedColors.size() > 0) {
        if (key == OF_KEY_UP) {
            if (!selectMode && selectedColorID == -1) {
                selectMode = true;
                selectedColorID = stockedColors.size()-1;
            }else if (selectMode){
                selectedColorID--;
                if (selectedColorID < 0) { selectedColorID = stockedColors.size() -1; }
            }
        }else if (key == OF_KEY_DOWN){
            if (!selectMode && selectedColorID == -1) {
                selectMode = true;
                selectedColorID = 0;
            }else if (selectMode){
                selectedColorID++;
                if (selectedColorID > stockedColors.size() -1) { selectedColorID = 0; }
            }
        }else if (key == 'a' && stockedColors.size() < 9*2+1) {
            stockedColors.push_back(pixColor);
        }else if (key == 'd' && stockedColors.size() > 0){
            stockedColors.erase(stockedColors.begin()+selectedColorID);
            selectedColorID --;
            if (selectedColorID < 0) selectedColorID++;
        }
        if(key == 'c'){
            string color = ofToString((int)stockedColors[selectedColorID].r)
            +", "+ofToString((int)stockedColors[selectedColorID].g)
            +","+ofToString((int)stockedColors[selectedColorID].b);
            ofGetWindowPtr()->setClipboardString(color);
        }
        if(key == 'h'){
            string color = ofToString((int)stockedColors[selectedColorID].getHue())
            +", "+ofToString((int)stockedColors[selectedColorID].getSaturation())
            +","+ofToString((int)stockedColors[selectedColorID].getBrightness());
            ofGetWindowPtr()->setClipboardString(color);
        }
        if(key == 'x'){
            string color = ofToString(pixColor.getHex());
            ofGetWindowPtr()->setClipboardString(color);
        }
    }else{
        if (key == 'a') {
            stockedColors.push_back(pixColor);
        }
        if(key == 'c'){
            string color = ofToString((int)pixColor.r)
            +", "+ofToString((int)pixColor.g)
            +","+ofToString((int)pixColor.b);
            ofGetWindowPtr()->setClipboardString(color);
        }
        if(key == 'h'){
            string color = ofToString((int)pixColor.getHue())
            +", "+ofToString((int)pixColor.getSaturation())
            +","+ofToString((int)pixColor.getBrightness());
            ofGetWindowPtr()->setClipboardString(color);
        }
        if(key == 'x'){
            string color = ofToString(pixColor.getHex());
            ofGetWindowPtr()->setClipboardString(color);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if (x >= 20 && x <= colRectW+20 && y >= 20 && y <= colRectH+20) {
        int h = (int)ofMap(x, 20, colRectW, 0, 255);
        int s = (int)ofMap(y, 20, colRectH, 0, 255);
        pixColor.setHue(h);
        pixColor.setSaturation(s);
        resetSelectedColor();
    }else if (x >= colRectW + 5 + 20 && x <= colRectH + 5+ 20 + 20 && y >= 20 && y <= colRectH + 20) {
        int b = (int)ofMap(y, 20, colRectH, 255, 0, true);
        bri = b;
        pixColor.setBrightness(bri);
        resetSelectedColor();
        setBriBar(y);
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    if (x >= 20 && x <= colRectW+20 && y >= 20 && y <= colRectH+20) {
        int h = (int)ofMap(x, 20, colRectW, 0, 255);
        int s = (int)ofMap(y, 20, colRectH, 0, 255);
        pixColor.setHue(h);
        pixColor.setSaturation(s);
        resetSelectedColor();
    }else if (x >= colRectW + 5 + 20 && x <= colRectH + 5+ 20 + 20 && y >= 20 && y <= colRectH + 20) {
        int b = (int)ofMap(y, 20, colRectH, 255, 0, true);
        bri = b;
        pixColor.setBrightness(bri);
        resetSelectedColor();
        setBriBar(y);
    }
    
}

//--------------------------------------------------------------
void ofApp::setBriBar(float y){
    briBarH = y;
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

//--------------------------------------------------------------
void ofApp::resetSelectedColor(){
    selectedColorID = -1;
    selectMode = false;
}
