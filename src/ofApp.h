#pragma once

#define PI_VERSION

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxJSON.h"
#include "maxiGrains.h"
#ifdef      PI_VERSION
#include "ofxGPIO.h"
#endif

typedef hannWinFunctor grainPlayerWin;

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
        void processAnswer();

		void exit();
    
        void audioOut(float * output, int bufferSize, int nChannels);
        void audioIn(float * input, int bufferSize, int nChannels);
    
        void getJSONConfig();
		
		// we will have a dynamic number of images, based on the content of a directory:
		ofDirectory questionDir, configDir;
        vector<maxiSample> questions;
        vector<maxiRecorder>answers;
    
		ofSoundStream soundStream;

        //Audio stuff
        int		initialBufferSize; /* buffer size */
        int		sampleRate;
        int		bufferSize;

        float* audioInput;
    
        //Config file settings
        ofxJSONElement result;

        double recordLength;
        double voiceModulation;
        int randomQuestionOrder;


        //Recorder
        maxiRecorder recorder;
     

        //Recorder time
       	double startTime;
		double elapsedTime;

#ifdef      PI_VERSION
        GPIO gpio18;
        GPIO gpio17;
#endif
		string state_button;
		bool buttonFlag;

		bool newAnswer;
        bool isPlaying;
        bool isRecording;
        bool loaded;
        int currentQuestion;
        int numOfQuestions;
    
    //processing
        float* record;

		
};

