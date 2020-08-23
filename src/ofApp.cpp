#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    box2d.init();
    box2d.setFPS(60.0);
    box2d.setGravity(0, 10);
    
    mikuSvg.load("snowmiku.svg");
    outline = true;
    
    dir.listDir("snowflakes/");
    dir.allowExt("svg");
    dir.sort();
    
    if (dir.size()) {
        snowSvg.assign(dir.size(), ofxSVG());
    }
    
    for (int i = 0; i < (int)dir.size(); i++) {
        snowSvg[i].load(dir.getPath(i));
    }
    
    paths.resize(snowSvg.size());
    for (int i = 0; i < snowSvg.size(); i++) {
        paths[i] = snowSvg[i].getPaths()[0];
    }
    
    colors = {ofColor(242, 242, 242), //white
              ofColor(0, 165, 195), //teal
              ofColor(78, 201, 220), //turquoise
              ofColor(156, 237, 243), //lightcyan
              ofColor(252, 139, 174) //pink
    };
    
    // get every distinct shapes from the SVG file one by one
    for (int i = 0; i < mikuSvg.getNumPath(); i++) {
        
        // grab a shape
        ofPath shape = mikuSvg.getPathAt(i);
        
        // store its color
        int cr = (int)ofRandom(colors.size());
        shapeColors.push_back(colors[cr]);
        
        // grab the outline from the shape
        for (int j = 0; j < shape.getOutline().size(); j++) {
            ofPolyline line = shape.getOutline().at(j);
            
            // if this is a closed shape, join the first and last points
            // to close the contour
            if(line.isClosed()){
                line.addVertex(line.getVertices()[0]);
            }
            
            // create a box2dEdge from the shape to make it interact with
            // other objects into the box2d world
            shared_ptr<ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
            edge.get()->addVertexes(line);
            edge.get()->setPhysics(0.0, 0.1, 0.7);
            edge->create(box2d.getWorld());
            edges.push_back(edge);
        }
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor colorOne(255, 205, 219); // pink
    ofColor colorTwo(153, 198, 228); // light blue
    ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_CIRCULAR);
    
    // draw shapes outline
    if(outline){
    for (int i = 0; i < mikuSvg.getNumPath(); i++) {
        ofSetColor(shapeColors[i]);
        if (i == 0) {
            ofSetLineWidth(3);
        } else {
        ofSetLineWidth(2);
        }
        edges[i].get()->draw();
    }
    }
    
    for (int i = 0; i < circles.size(); i++) {
        ofPoint pos = circles[i].get()->getPosition();
        float size = circles[i].get()->getRadius();
        
        if (size > 20) {
        
        ofPushMatrix();
        ofTranslate(pos.x - size*2/2, pos.y - size*2/2);
        ofScale(.025*size*2, .025*size*2);
            colors[sc[i]].set(colors[sc[i]].r, colors[sc[i]].g, colors[sc[i]].b);
            paths[ss[i]].setFillColor(colors[sc[i]]);
            paths[ss[i]].draw();
            
        ofPopMatrix();
        
        } else {
            ofSetColor(255, 255, 255);
            if (ss[i] == 0) {
                ofDrawStar(pos.x, pos.y, size*0.6);
            } else {
            ofDrawEllipse(pos.x, pos.y, size*0.6, size*0.6);
            }
        }
    }
    
    myImage.grabScreen(0, 0, ofGetWidth(),  ofGetHeight());

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') { // create a new circle
            ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        //  set attributes to this circle (density, bounce, friction)
        circle.get()->setPhysics(1.0, 0.5, 0.1);
        circle.get()->setup(box2d.getWorld(), mouseX, mouseY, ofRandom(50, 70));
        //  add this circle to "circles" vector
        circles.push_back(circle);
        
        int r = (int)ofRandom(snowSvg.size()); //random snow shape
        ss.push_back(r);
        sc.push_back(0); //snow color (white)
    }
    
    if (key == 'm' || key == 'M') { // create a new circle (small)
            ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(1.0, 0.5, 0.1);
        circle.get()->setup(box2d.getWorld(), mouseX, mouseY, ofRandom(20, 40));
        circles.push_back(circle);
        
        int r = (int)ofRandom(snowSvg.size()); //random snow shape
        ss.push_back(r);
        int cr = (int)ofRandom(colors.size()); //random snow color
        sc.push_back(cr);
    }
    
    if (key == 'n' || key == 'N') { // create a new small ellipse
            ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(1.0, 0.5, 0.1);
        circle.get()->setup(box2d.getWorld(), mouseX, mouseY, ofRandom(10, 20));
        circles.push_back(circle);
        
        int r = (int)ofRandom(3);
        ss.push_back(r);
        sc.push_back(0); //snow color (white)
    }
    
    if (key == 'o' || key == 'O') {
    
            outline=!outline; // draw shape outline
    }
    
    if (key == 'p' || key == 'P') { //change outline color
           shapeColors.clear();
       for (int i = 0; i < mikuSvg.getNumPath(); i++) {
           int cr = (int)ofRandom(colors.size());
           shapeColors.push_back(colors[cr]);
       }
    }

    if (key == 'c' || key =='C') {
        circles.clear();
        ss.clear();
        sc.clear();
    
    }
    
    if (key == 'S' || key == 's') {
        myImage.save(ofGetTimestampString("%Y%m%d%H%M%S")+"##.png", OF_IMAGE_QUALITY_HIGH);
    }
}

//--------------------------------------------------------------
void ofApp::ofDrawStar(int x, int y, int r){
    int vertexNum = 5 * 2;
    int R;
    int outR = r;
    int inR = outR/2;
    
    ofPushMatrix();
    ofTranslate(x, y);
    ofRotateDeg(-90);
    ofBeginShape();
    for (int i = 0; i < vertexNum; i++) {
        if (i%2 == 0) {
            R = outR;
        } else {
            R = inR;
        }
        ofVertex(R*cos(DEG_TO_RAD*(360*i/vertexNum)), R*sin(DEG_TO_RAD*(360*i/vertexNum)));
    }
    ofEndShape();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
