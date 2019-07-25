/*-----------------------------------------------------------/
 statusBar.h
 github.com/azuremous
 Created by Jung un Kim a.k.a azuremous on 4/15/19.
 /----------------------------------------------------------*/
#pragma once
#include "ofxSimpleButton.h"

class statusBar {
private:
    int size;
    int width;
    ofxSimpleButton closeBtn;
    
public:
    void setup(int size = 40){
        this->size = size;
        width = ofGetWidth();
        closeBtn.setup(5, 5, 30, 30, false, true);
        closeBtn.setColor(ofColor(255, 0, 0));
        closeBtn.show();
    }
    
    void render(bool isReadyWindowMove){
        ofSetColor(192 - (92 * isReadyWindowMove));
        ofDrawRectangle(0, 0, width, size);
        
        closeBtn.render();
    }
    
    bool checkClick(float x, float y){
        closeBtn.touchDown(x, y);
        return closeBtn.getIsSelect();
    }
    
    int getSize() const { return size; }
};
