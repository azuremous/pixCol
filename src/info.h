/*-----------------------------------------------------------/
 info.h
 github.com/azuremous
 Created by Jung un Kim a.k.a azuremous on 7/25/15
 /----------------------------------------------------------*/

#include "statusBar.h"
#include "ofxIro.h"

const string menuList[] = {
    "copy color as RGB: c",
    "copy color as HSB: b",
    "copy color as float: f",
    "copy color as hex: x",
    "",
    "move up: up",
    "move down: down",
    "add color: a",
    "delete color: d",
    "export: e",
    "make palette from image: i",
};

const string colorType[] = {
    "H : ",
    "S : ",
    "B : ",
    "R : ",
    "G : ",
    "B : "
};

const string warnig = "Drag image into app";
const string aboutApp = "pixColor\nver 1.0\nby azuremous\nhttps://github.com/azuremous/pixColor";
