#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	int num = 5000;
    for(int i = 0; i < num;i++){
     p.push_back(demoParticle(style.getForgroundColor(), ofRandom(1, 5), (int)ofRandom(100, 255)));
    }
//	p.assign(num, demoParticle());
	currentMode = PARTICLE_MODE_REPEL;

	currentModeStr = "2 - PARTICLE_MODE_REPEL"; 

	resetParticles();
    
    // Kinect setup
	kinect.init();
	//kinect.init(true);                // shows infrared instead of RGB video image
	//kinect.init(false, false);        // disable video image (faster fps)
	
    // Open Kinect stream
	kinect.open();                      // opens first available kinect
	//kinect.open(1);                   // open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// Print the intrinsic IR sensor values
	if (kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
    // Thresholds
    nearThreshold = 188; //230;
	farThreshold = 102; //70;
    
	// Zero the tilt on startup
	angle = 14;
	kinect.setCameraTiltAngle(angle);
    
    // Allocate images
    colorImg.allocate(kinect.width, kinect.height, OF_IMAGE_COLOR);
	grayImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    grayThreshNear.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
	grayThreshFar.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    grayPreprocImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    grayImageScaled.allocate(1920, 1080, OF_IMAGE_GRAYSCALE);
    
    // Configure contour finder
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
    contourFinder.setFindHoles(false);
    contourFinder.setSortBySize(true);
}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used in the forth demo 
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
	}
	
	attractPointsWithMovement = attractPoints;
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    
	// There is a new frame and we are connected
	if (kinect.isFrameNew()) {
        
        // Load grayscale depth image from the kinect source
        
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
        // THIS IS DIRTY
        // WE NEED TO DI THIS THE RIGHT WAY
        // @Todo
        //        grayImage.resize(1920, 1080);
        
        // Threshold image
        threshold(grayImage, grayThreshNear, nearThreshold, true);
        threshold(grayImage, grayThreshFar, farThreshold);
        
        // Convert to CV to perform AND operation
        Mat grayThreshNearMat = toCv(grayThreshNear);
        Mat grayThreshFarMat = toCv(grayThreshFar);
        Mat grayImageMat = toCv(grayImage);
        
        // cvAnd to get the pixels which are a union of the two thresholds
        bitwise_and(grayThreshNearMat, grayThreshFarMat, grayImageMat);
        
        // Save pre-processed image for drawing it
        grayPreprocImage = grayImage;
		// Process image
        
        grayImage.mirror(false, true);
        dilate(grayImage);
        dilate(grayImage);
        blur(grayImage, 5);
        
        
        // Mark image as changed
        grayImage.update();
        
        // Find contours
        //contourFinder.setThreshold(ofMap(mouseX, 0, ofGetWidth(), 0, 255));
        //        grayImageScaled = grayImage;
        //        grayImageScaled.resize(1920, 1080);
        contourFinder.findContours(grayImage);
        
        
        // Update biggest contour -------------------------------------------------------------
        
        attractPt.set(ofGetMouseX(), ofGetMouseY());

        int n = contourFinder.size();
        if (n > 0) {
            
            ofPolyline biggestPolyline = contourFinder.getPolyline(0); // The biggest one
            
            // Scale polyline and add vertices
            for(int i=0;i< (int)biggestPolyline.size();i++){
                biggestPolyline[i].x*= (ofGetWidth()/ 640);
                biggestPolyline[i].y*= (ofGetHeight()/360);
                //drawing.addVertex(biggestPolyline[i]);
                
                attractPt = biggestPolyline.getCentroid2D();
            }
        }
	}
    
    // Update particles
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].update(attractPt);
	}
	
	//lets add a bit of movement to the attract points
	for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(style.getGradientColorOne(), style.getGradientColorTwo(), OF_GRADIENT_CIRCULAR);

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	
	ofSetColor(190);
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(unsigned int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);
    
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.", 10, 20);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 30);
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == '1'){
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 		
	}
	if( key == '2'){
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse"; 				
	}
	if( key == '3'){
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force"; 						
	}
	if( key == '4'){
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
		resetParticles();
	}	
		
	if( key == ' ' ){
		resetParticles();
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
