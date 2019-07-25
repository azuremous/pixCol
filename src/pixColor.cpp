#include "pixColor.h"

pixColor::pixColor():
useImg(false),
setIro(false),
showHelp(false),
showCursor(false),
selectMode(false),
isReadyWindowMove(false),
statusBarHeight(40),
mainMargin(20),
cursorSize(20),
barWidth(20),
bri(255),
selectedColorID(-1)
{
    menuSize = sizeof(menuList)/sizeof(menuList[0]);
}

//--------------------------------------------------------------
void pixColor::setup(){
    ofSetDataPathRoot("../Resources/");
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetWindowShape(1200, 600 + statusBarHeight);
    bar.setup(statusBarHeight);
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
    int height = screenHeight - statusBarHeight;
    paletteWidth = paletteHeight = height - (mainMargin * 2);
    colRectWidth = colRectHeight = height/2 - (mainMargin * 2);
    barY = statusBarHeight + mainMargin;
    currentColor.setHsb(bri, bri, bri);
    stockedColors.clear();
    
    palette.allocate(paletteWidth + mainMargin + barWidth, paletteHeight);
    palette.begin();
    ofClear(0);
    for (int h = 0; h < paletteHeight; h++) {
        for (int w = 0; w < paletteWidth; w++) {
            int hue = (int)ofMap(w, 0, paletteWidth, 0, 255);
            int sat = (int)ofMap(h, 0, paletteHeight, 0, 255, false);
            
            ofColor c;
            c.setHsb(hue, sat, bri);
            ofSetColor(c);
            ofDrawRectangle(w, h, 1, 1);
        }
    }
    ofTranslate(paletteWidth + mainMargin, 0);
    //draw bright rect
    for (int h = 0; h < paletteHeight; h++) {
        int b = (int)ofMap(h, 0, paletteHeight, 255, 0, true);
        ofColor c;
        c.set(255);
        c.setBrightness(b);
        ofSetColor(c);
        ofDrawRectangle(0, h, barWidth, 1);
    }
    palette.end();
    
    cursor.allocate(cursorSize, cursorSize);
    cursor.begin();
    ofClear(0);
    ofSetLineWidth(2);
    ofSetColor(0);
    ofDrawLine(0, cursorSize/2, cursorSize, cursorSize/2);
    ofDrawLine(cursorSize/2, 0, cursorSize/2, cursorSize);
    cursor.end();
    uiFont.load(ofFilePath::getCurrentWorkingDirectory()+"/SF-Pro-Display-Light.otf", 24);
    infoFont.load(ofFilePath::getCurrentWorkingDirectory()+"/SF-Pro-Display-Regular.otf",24);
    
    aboutFont.load(ofFilePath::getCurrentWorkingDirectory()+"/SF-Pro-Display-Regular.otf",18);
    
    iroChecker.setup(28., 22.);
    iroChecker.setBalance(0.8, 0.7);
    
}

//--------------------------------------------------------------
void pixColor::update(){
    ofSetWindowTitle("pixColor");
    if(useImg){
        ofPixels p = refImg.getPixels();
        if(refImg.getTexture().isAllocated() && p.size() > 0){
            analyzer.update(p);
            if (analyzer.isFrameNew()) {
                vector<ofColor>_col = analyzer.getColor();
                if(_col.size() > 0){
                    iroChecker.update(_col, CIE76);
                    if (iroChecker.isFrameNew() && setIro) {
                        stockedColors = iroChecker.getColor(NOWHITE);
                        setIro = false;
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void pixColor::draw(){
    
    ofPushMatrix();
    ofTranslate(mainMargin, mainMargin + statusBarHeight);
    ofSetColor(255);
    if (useImg) { refImg.draw(0, 0); }
    else{
        palette.draw(0, 0);
        //draw saturate rect
        int b = (int)ofMap(barY - (mainMargin + statusBarHeight), 0, paletteHeight, 0, 255, true);
        ofSetColor(b);
        ofDrawRectangle(paletteWidth + mainMargin, barY - (mainMargin + statusBarHeight), barWidth, 4);
    }
    
    ofTranslate(palette.getWidth() + mainMargin, 0);
    
    ofSetColor(currentColor);//pic color rect
    ofDrawRectangle(0, 0, colRectWidth, colRectHeight);
    
    //back color
    ofSetColor(40);
    ofDrawRectangle(0, colRectHeight + mainMargin, colRectWidth , screenHeight - ((mainMargin * 3) + colRectHeight + statusBarHeight));
    
    ofColor displayColor;
    if (selectMode) { displayColor = stockedColors[selectedColorID]; }
    else { displayColor = currentColor; }
    float h = uiFont.getStringBoundingBox("A", 0, 0).getHeight();
    
    ofPushMatrix();
    ofTranslate(mainMargin, colRectHeight + mainMargin * 2);
    ofSetColor(255);
    for(int i = 0; i < 6; i ++){
        int n = (i % 3);
        int _h =
        150 * (i / 3); uiFont.drawString(colorType[i]+getColor(displayColor, i), 0, h*(n + 1) + n*10 + _h);
    }
    ofPopMatrix();
    
    //draw stocked color rect
    ofTranslate(colRectWidth + mainMargin, 0);
    for (int i = 0; i < stockedColors.size(); i++) {
        ofSetColor(stockedColors[i]);
        ofDrawRectangle(0, mainMargin*i + 10*i, colRectWidth, mainMargin);
    }
    
    if (stockedColors.size() > 0 && selectMode) {
        ofPushStyle();
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(255);
        ofDrawRectangle(0, mainMargin*selectedColorID + 10*selectedColorID, colRectWidth, mainMargin);
        ofPopStyle();
    }
    ofPopMatrix();
    
    if(showCursor){
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(255);
        cursor.draw(ofGetMouseX() - cursorSize/2, ofGetMouseY() - cursorSize/2);
        ofPopStyle();
        ofPopMatrix();
    }
    
    bar.render(isReadyWindowMove);
    
    ofSetColor(0);
    infoFont.drawString("Help: h", 55, statusBarHeight/2 + uiFont.stringHeight("Q")/2 - 2);
    
    if(showHelp){
        int gap = uiFont.stringHeight("Q") + 15;
        int helpGap = 20;
        int helpWidth = screenWidth - helpGap * 2;
        int helpheight = screenHeight - helpGap - statusBarHeight;
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2 - helpWidth/2, statusBarHeight);
        ofSetColor(6, 45, 53, 200);
        ofDrawRectangle(0, 0, helpWidth, helpheight);
        ofTranslate(0, gap);
        ofSetColor(255, 248, 213);
        for(int i = 0; i < menuSize; i++){
            infoFont.drawString(menuList[i], 20, i*gap);
        }
        aboutFont.drawString(aboutApp, helpWidth/2, 0);
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
void pixColor::keyPressed(int key){
    
    switch (key) {
        case OF_KEY_UP:{
            if(stockedColors.size() > 0){
                if (!selectMode && selectedColorID == -1) {
                    selectMode = true;
                    selectedColorID = stockedColors.size()-1;
                }else if (selectMode){
                    selectedColorID--;
                    if (selectedColorID < 0) { selectedColorID = stockedColors.size() -1; }
                }
            }
        }break;
        case OF_KEY_DOWN:{
            if(stockedColors.size() > 0){
                if (!selectMode && selectedColorID == -1) {
                    selectMode = true;
                    selectedColorID = 0;
                }else if (selectMode){
                    selectedColorID++;
                    if (selectedColorID > stockedColors.size() -1) { selectedColorID = 0; }
                }
            }
        }break;
        case 'a':{
            if(stockedColors.size() < 9*2+1 && stockedColors.size() >= 0){
                stockedColors.push_back(currentColor);
            }
        }break;
        case 'd':{
            if(stockedColors.size() > 0){
                stockedColors.erase(stockedColors.begin()+selectedColorID);
                selectedColorID --;
                if (selectedColorID < 0) { selectedColorID++; }
            }
        }break;
        case 'c':{ //rgb
            ofGetWindowPtr()->setClipboardString(getRGB(setColor()));
        }break;
        case 'b':{ //hsb
            ofGetWindowPtr()->setClipboardString(getHSB(setColor()));
        }break;
        case 'f':{ //float
            ofGetWindowPtr()->setClipboardString(getFloat(setColor()));
        }break;
        case 'x':{ //hex
            ofGetWindowPtr()->setClipboardString(ofToString(setColor().getHex()));
        }break;
        case 'e':{ //export file
            if(stockedColors.size() > 0){
                string output = "";
                ofXml xml;
                auto colors = xml.appendChild("colors");
                for(int i = 0; i < stockedColors.size(); i++){
                    auto col = colors.appendChild("color");
                    col.appendChild("RGB").set(getRGB(stockedColors[i]));
                    col.appendChild("HSB").set(getHSB(stockedColors[i]));
                    output += "[rgb]:"+ getRGB(stockedColors[i]) +" [hsb]:" + getHSB(stockedColors[i]) + "\n";
                }
                string route = ofFilePath::getUserHomeDir()+"/Documents/";
                auto b =  ofBuffer(output.c_str(), output.length());
                xml.save(route + "colors.xml");
                ofBufferToFile(route + "colors.txt", b);
            }
        }break;
        case 'h':{
            showHelp = !showHelp;
        }break;
        case 'i':{
            if(useImg){
                stockedColors.clear();
                setIro = true;
            }
        }break;
            
    }
}

//--------------------------------------------------------------
void pixColor::keyReleased(int key){
    
}

//--------------------------------------------------------------
void pixColor::mouseMoved(int x, int y ){
    if(x >= mainMargin && x <= mainMargin + paletteWidth && y>= mainMargin + statusBarHeight && y <= mainMargin + statusBarHeight + paletteHeight){
        if(!showCursor){
            showCursor = true;
            ofHideCursor();
        }
    }else{
        if(showCursor){
            showCursor = false;
            ofShowCursor();
        }
    }
}

//--------------------------------------------------------------
void pixColor::mouseDragged(int x, int y, int button){
    if (isReadyWindowMove) {
        int nx = (x - lastPos.x) + ofGetWindowPositionX();
        int ny = (y - lastPos.y) + ofGetWindowPositionY();
        ofSetWindowPosition(nx, ny);
    }else{
        setColor(x, y);
    }
}

//--------------------------------------------------------------
void pixColor::mousePressed(int x, int y, int button){
    cout<<"current mouse cursor x:"<<x<<", y:"<<y<<endl;
    if(bar.checkClick(x, y)){
        ofExit();
    }else if(y > 0 && y < statusBarHeight){
        isReadyWindowMove = true;
        lastPos.set(x, y);
    }else{
        setColor(x, y);
    }
}

//--------------------------------------------------------------
void pixColor::mouseReleased(int x, int y, int button){
    isReadyWindowMove = false;
}

//--------------------------------------------------------------
void pixColor::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void pixColor::dragEvent(ofDragInfo dragInfo){
    if(dragInfo.files.size() == 1){
        string route = dragInfo.files[0];
        refImg.load(route);
        refImg.resize(paletteWidth, paletteHeight);
        useImg = true;
    }
}

//--------------------------------------------------------------
void pixColor::resetSelectedColor(){
    selectedColorID = -1;
    selectMode = false;
}

//--------------------------------------------------------------
void pixColor::setColor(int x, int y){
    if (x >= mainMargin && x <= mainMargin + paletteWidth && y >= mainMargin + statusBarHeight && y <= mainMargin + statusBarHeight + paletteHeight) {
        if(useImg){
            int rx = x - mainMargin;
            int ry = y - mainMargin;
            currentColor = refImg.getColor(rx, ry);
            resetSelectedColor();
        }else{
            int h = (int)ofMap(x, mainMargin, paletteWidth, 0, 255);
            int s = (int)ofMap(y, mainMargin, paletteHeight, 0, 255);
            currentColor.setHue(h);
            currentColor.setSaturation(s);
            resetSelectedColor();
        }
    }else if (x >= paletteWidth + mainMargin * 2 && x <= paletteWidth + mainMargin * 2 + barWidth && y >= mainMargin + statusBarHeight && y <= paletteHeight + mainMargin + statusBarHeight) {
        if(!useImg){
            int b = (int)ofMap(y, mainMargin, paletteWidth, 255, 0, true);
            bri = b;
            currentColor.setBrightness(bri);
            resetSelectedColor();
            barY = y;
        }
    }
}

//--------------------------------------------------------------
string pixColor::getColor(const ofColor &c, int n){
    string result;
    switch(n){
        case 0:{
            result =  ofToString((int)c.getHue());
            }break;
        case 1:{
           result =  ofToString((int)c.getSaturation());
        }break;
        case 2:{
            result =  ofToString((int)c.getBrightness());
        }break;
        case 3:{
            result =  ofToString((int)c.r);
        }break;
        case 4:{
            result =  ofToString((int)c.g);
        }break;
        case 5:{
            result =  ofToString((int)c.b);
        }break;
            
    }
    return result;
}

//--------------------------------------------------------------
ofColor pixColor::setColor(){
    ofColor c = currentColor;
    if(stockedColors.size() > 0){
        c = stockedColors[selectedColorID];
    }
    return c;
}

//--------------------------------------------------------------
string pixColor::getRGB(const ofColor &c){
    string s = ofToString((int)c.r)
    +","+ofToString((int)c.g)
    +","+ofToString((int)c.b);
    return s;
}

//--------------------------------------------------------------
string pixColor::getHSB(const ofColor &c){
    string s = ofToString((int)c.getHue())
    +","+ofToString((int)c.getSaturation())
    +","+ofToString((int)c.getBrightness());
    return s;
}

//--------------------------------------------------------------
string pixColor::getFloat(const ofColor &c){
    string s = ofToString((float)c.r / 255.)
    +","+ofToString((float)c.g / 255.)
    +","+ofToString((float)c.b / 255.);
    return s;
}
