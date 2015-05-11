#pragma once

#include "ofMain.h"
#include "demoParticle.h"
#include "Styling.h"
#include "ofxKinect.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		void resetParticles();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // Particle vars
		particleMode currentMode;
		string currentModeStr;
    
        Styling style;

		vector <demoParticle> p;
		vector <ofPoint> attractPoints;
		vector <ofPoint> attractPointsWithMovement;
    
        // Kinect + OpenCV vars
        ofxKinect kinect;
        ofxCv::ContourFinder contourFinder;
        
        ofImage colorImg;
        ofImage grayImage;         // grayscale depth image
        ofImage grayThreshNear;    // the near thresholded image
        ofImage grayThreshFar;     // the far thresholded image
        ofImage grayPreprocImage;  // grayscale pre-processed image
        // @ToDo : Scale polyline not image
        ofImage grayImageScaled; // for getting the ize of our screen
        int nearThreshold;
        int farThreshold;
        int angle;
    
        ofPoint attractPt;
};
