#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
private:
    ofColor pixColor;
    float width, height;
    float colRectW, colRectH;
    float briBarH;
    
    int bri;
    int selectedColorID;
    bool selectMode;
    
    vector<ofColor>stockedColors;
    ofTrueTypeFont uiFont;
    ofFbo screen;
    ofFbo cursor;
    
protected:
    void resetSelectedColor();
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void setBriBar(float y);
    
};
