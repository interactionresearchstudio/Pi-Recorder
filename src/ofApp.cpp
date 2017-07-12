#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
#ifdef      PI_VERSION
    gpio18.setup("18");
    gpio18.export_gpio();
    gpio18.setdir_gpio("out");
    gpio17.setup("17");
    gpio17.export_gpio();
    gpio17.setdir_gpio("in");
#endif
    
	isPlaying = false;
	isRecording = false;
	buttonFlag = false;
  newAnswer = false;
    state_button = "";

    currentQuestion = 0;

    getJSONConfig();


    sampleRate 	= 44100; /* Sampling Rate */
    bufferSize	= 512; /* Buffer Size. you have to fill this buffer with sound using the for loop in the audioOut method */

    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    audioInput = new float[bufferSize];
#ifdef      PI_VERSION
   	questionDir.listDir(ofFilePath::getUserHomeDir() + "/../boot/questions/");
#else
	questionDir.listDir(ofFilePath::getUserHomeDir() + "/Desktop/questions/");
#endif
	questionDir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

	numOfQuestions = (int)questionDir.size();
	if(numOfQuestions){
		questions.assign(numOfQuestions, maxiSample());
	}

	// you can now iterate through the files and load them into the ofImage vector
	for(int i = 0; i < numOfQuestions; i++){
		questions[i].load(questionDir.getPath(i));
	}
	

	ofBackground(ofColor::white);
    
    soundStream.printDeviceList();

	soundStream.setDeviceID(2);
	
	//if you want to set the device id to be different than the default
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only 

	soundStream.setup(this, 2, 1, sampleRate, bufferSize, 4);

	// on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
	// use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
	soundStream.start();

}

//--------------------------------------------------------------
void ofApp::update(){
    //cout << ofToString(recorder.isRecording()) << endl;
#ifdef      PI_VERSION
	gpio17.getval_gpio(state_button);
#endif
		    if(state_button == "1" && buttonFlag == false){
        //  cout << state_button << endl;
		    	buttonFlag = true;
		    	if(isPlaying == false && isRecording == false){
		    		isPlaying = true;
		    	}
		    	//gpio18.setval_gpio("1");
		    }
		    if(state_button == "0" && buttonFlag == true){
		    	buttonFlag = false;
		    	//gpio18.setval_gpio("0");
		    }
	if(isRecording){
    if(newAnswer == false){
        loaded = false;
        newAnswer = true;
        //answers.erase( answers.begin() );
        answers.clear();
        answers.resize(answers.size()+1);
        loaded = true;
       // cout << "START RECORD" << endl;
#ifdef      PI_VERSION
    answers[0].setup(ofToDataPath(ofFilePath::getUserHomeDir() + "/../boot/recordings/"+ofToString(currentQuestion)+".wav"));
#else
    answers[0].setup(ofToDataPath(ofFilePath::getUserHomeDir() + "/Desktop/recordings/"+ofToString(currentQuestion)+".wav"));
#endif
    answers[0].startRecording();
#ifdef      PI_VERSION
        gpio18.setval_gpio("1");
#endif
    }
		elapsedTime = ofGetElapsedTimeMillis() - startTime;

		if(elapsedTime >= recordLength*1000){
           // cout << "END RECORD" << endl;
			isRecording = false;
			answers[0].stopRecording();
            answers[0].saveToWav();
            currentQuestion++;
#ifdef      PI_VERSION
            gpio18.setval_gpio("0");
#endif
            if(currentQuestion == numOfQuestions){
                currentQuestion = 0;
            }
        }
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    ofSetHexColor(0x00FF00);    
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    double wave;

    
    for (int i = 0; i < bufferSize; i++){
   
    	if(isPlaying){
    	  	wave = questions[currentQuestion].playOnce();
    		if(questions[currentQuestion].getPosition() == questions[currentQuestion].getLength()){
                startTime = ofGetElapsedTimeMillis();
                questions[currentQuestion].setPosition(0);
           		isRecording = true;
                isPlaying = false;
           		newAnswer = false;
       	 	}
            output[i*nChannels    ] = wave*0.5;
        	output[i*nChannels + 1] = wave*0.5;
   		}

    }
    
}
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	if(isRecording && loaded){
  		for (int i = 0; i < bufferSize; i++){
            audioInput[i] = input[i];
		}
		answers[0].passData(audioInput, bufferSize);
	}
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   
    if(key == 'a'){
            state_button = "0";
       }
    
    if(key == 's'){
             state_button = "1";
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

void ofApp::exit(){
 // gpio17.unexport_gpio();
 // gpio18.unexport_gpio();

}

void ofApp::getJSONConfig(){
    // Now parse the JSON
#ifdef PI_VERSION
   string file = ofFilePath::getUserHomeDir() + "/../boot/config/conf.json";
#else   
 string file = ofFilePath::getUserHomeDir() + "/Desktop/config/conf.JSON";
#endif
   // string file = ofFilePath::getUserHomeDir() + "/../boot/config/conf.json";
    bool parsingSuccessful = result.open(file);
    
    if (parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << result.getRawString();
    }
    else
    {
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
    
   recordLength =  result["recordLength"].asDouble();
   voiceModulation =  result["voiceModulation"].asDouble();
   randomQuestionOrder = result["randomQuestionOrder"].asInt();
    cout << recordLength << endl;
    
}
