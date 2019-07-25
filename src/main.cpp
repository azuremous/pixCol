/*-----------------------------------------------------------/
 main.cpp
 github.com/azuremous
 Created by Jung un Kim a.k.a azuremous on 3/9/15
 /----------------------------------------------------------*/
#include "ofMain.h"
#include "pixColor.h"
#include "ofAppGLFWWindow.h"

int main( ){
    
    ofGLFWWindowSettings settings;
    settings.decorated = false;
    ofAppGLFWWindow window;
    ofCreateWindow(settings);
    window.setup(settings);
    
    ofRunApp(new pixColor());

}
