#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxSvg.h"

class ofApp : public ofBaseApp{

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
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    
    ofxBox2d box2d;
    
    vector<shared_ptr<ofxBox2dEdge> > edges;
    vector <ofPtr<ofxBox2dCircle> > circles;
    
    vector<ofColor> shapeColors;
    
    ofImage myImage;
    ofxSVG mikuSvg;
    vector<ofxSVG> snowSvg;
    bool outline;
    ofDirectory dir;
    vector<int> ss;
    vector<int> sc;
    vector<ofColor> colors;
    vector<ofPolyline> outlines;
    vector<ofPath> paths;
    
    void ofDrawStar(int x, int y, int r);
};
