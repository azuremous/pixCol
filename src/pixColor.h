/*-----------------------------------------------------------/
 pixColor.h
 github.com/azuremous
 Created by Jung un Kim a.k.a azuremous on 3/9/15
 /----------------------------------------------------------*/

#pragma once

#include "ofMain.h"
#include "info.h"

class pixColor : public ofBaseApp{
private:
    bool useImg;
    bool setIro;
    bool showHelp;
    bool showCursor;
    bool selectMode;
    bool isReadyWindowMove;
    
    int screenWidth, screenHeight;
    int paletteWidth, paletteHeight;
    int colRectWidth, colRectHeight;
    int statusBarHeight;
    int mainMargin;
    int barWidth;
    int cursorSize;
    int barY;
    int bri;
    int menuSize;
    int selectedColorID;
    
    ofPoint lastPos;
    ofImage refImg;
    
    ofFbo palette;
    ofFbo cursor;
    ofColor currentColor;
    
    ofTrueTypeFont infoFont;
    ofTrueTypeFont aboutFont;
    ofTrueTypeFont uiFont;
    
    statusBar bar;
    ofxIroCompress analyzer;
    ofxIro iroChecker;
    
    vector<ofColor>stockedColors;
    
protected:
    void resetSelectedColor();
    void setColor(int x, int y);
    string getColor(const ofColor &c, int n);
    ofColor setColor();
    string getRGB(const ofColor &c);
    string getHSB(const ofColor &c);
    string getFloat(const ofColor &c);
    
public:
    pixColor();
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
    
};
