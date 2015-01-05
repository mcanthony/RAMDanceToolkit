//
//  dpCameraUnit_Controller.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_Controller.h"

dpCameraUnit_Controller::dpCameraUnit_Controller(){

	for (int i = 0;i < 4;i++){
		cvAnalysis[i].mGui.setPosition(-1000, 0);
		cvFXUnit[i].mGui.setPosition(-1000, 0);
	}

	cvAnalysis[0].hakoniwa_name = "prism";
	cvAnalysis[1].hakoniwa_name = "pendulum";
	cvAnalysis[2].hakoniwa_name = "struggle";
	cvAnalysis[3].hakoniwa_name = "hakoniwa_D";

	gui.setup();
	gui.addLabel("main Console");
	gui.addSpacer();
	gui.setTriggerWidgetsUponLoad(false);
	gui.addLabel("Load Preset");
	gui.addToggleMatrix("LoadPreset", 5, 8);
	gui.addLabel("Save Preset");
	gui.addToggleMatrix("SavePreset", 5, 8);
	gui.addLabel("PresetName");
	gui.addTextInput("namePreset", "PresetName");
	gui.autoSizeToFitWidgets();
	
	ofAddListener(gui.newGUIEvent, this, &dpCameraUnit_Controller::guiEvent);
}

dpCameraUnit_Controller::~dpCameraUnit_Controller(){

}

void dpCameraUnit_Controller::update(){
	
	inputUnit	.update();
	
	if (inputUnit.mFourSplit){
		for (int i = 0;i < 4;i++){
			cvFXUnit[i]  .update(inputUnit.mFinalSource_FourSplit[i], inputUnit.mIsFrameNew);
			cvAnalysis[i].update(cvFXUnit[i].mSource, cvFXUnit[i].mGraySource, inputUnit.mIsFrameNew);
		}

	}else{
		cvFXUnit[0]		.update(inputUnit.mFinalSource_Small, inputUnit.mIsFrameNew);
		cvAnalysis[0]	.update(cvFXUnit[0].mSource, cvFXUnit[0].mGraySource, inputUnit.mIsFrameNew);
	}
	

}

void dpCameraUnit_Controller::draw(){

	inputUnit.drawUI(0, 300);
	inputUnit.drawThumbnail(0, 410, 0.5);

	if (inputUnit.mFourSplit){
		for (int i = 0;i < 4;i++){

			cvFXUnit[i].drawUI			(240 + i*370, 0);
			cvFXUnit[i].drawThumbnail	(450 + i*370, 0, 0.5);

			cvAnalysis[i].drawUI		(240 + i * 370, 500);
			cvAnalysis[i].drawThumbnail	(450 + i * 370, 500, 0.5);

		}
	}else{

		cvFXUnit[0].draw(240,0);
		cvAnalysis[0].draw(240,500);

	}
}

void dpCameraUnit_Controller::simulator_initialize(){
	for (int i = 0;i < inputUnit.mCameraList.size();i++){
		if (inputUnit.mCameraList[i] == "Video") inputUnit.mSourceType = i;
	}
	inputUnit.mVideoPlayer.play();
	inputUnit.mFourSplit = true;

//	cvFXUnit_second		.mEnableFrameDiff = true;
//
//
//	cvAnalysis			.mEnableOptFlow = true;
//	cvAnalysis_second	.mEnableOptFlow = true;
//
//	cvAnalysis			.mEnableSendOSC = true;
//	cvAnalysis_second	.mEnableSendOSC = true;
}

void dpCameraUnit_Controller::guiEvent(ofxUIEventArgs &e){
	
	ofxUIWidget* w = e.widget;
	
	if (w->getName() == "Load Setting"){

	}
	
	if (w->getName() == "Save Setting"){
		inputUnit.mGui.saveSettings("inputUnit.xml");
		
		for (int i = 0;i < 4;i++){
			cvFXUnit[i]		.mGui.saveSettings("cvFxUnit_"+ofToString(i)+".xml");
			cvAnalysis[i]	.mGui.saveSettings("Analysis_"+ofToString(i)+".xml");
		}
	}
	
	cout << w->getName() << endl;
	
	if (w->getName().substr(0,10) == "LoadPreset"){
		ofxUIToggle* tm = (ofxUIToggle*)(e.widget);

		int i = ofToInt(w->getName().substr(11,1));
		int j = ofToInt(w->getName().substr(14,1));

		gui.loadSettings("Preset"+ofToString(i)+ofToString(j)+"/Controller.xml");
		inputUnit.mGui.loadSettings("Preset"+ofToString(i)+ofToString(j)+"/inputUnit.xml");
		inputUnit.sourceReflesh();

		for (int o = 0;o < 4;o++){
			cvFXUnit[o].mGui.loadSettings("Preset"+ofToString(i)+ofToString(j)+"/cvFxUnit_"+ofToString(o)+".xml");
			cvAnalysis[o].mGui.loadSettings("Preset"+ofToString(i)+ofToString(j)+"/Analysis_"+ofToString(o)+".xml");
		}
		
	}
	
	if (w->getName().substr(0,10) == "SavePreset"){
		ofxUIToggle* tm = (ofxUIToggle*)(e.widget);

		int i = ofToInt(w->getName().substr(11,1));
		int j = ofToInt(w->getName().substr(14,1));

		ofDirectory::createDirectory("Preset"+ofToString(i)+ofToString(j));
		
		gui.saveSettings("Preset"+ofToString(i)+ofToString(j)+"/Controller.xml");
		inputUnit.mGui.saveSettings("Preset"+ofToString(i)+ofToString(j)+"/inputUnit.xml");
		
		for (int o = 0;o < 4;o++){
			cvFXUnit[o].mGui.saveSettings("Preset"+ofToString(i)+ofToString(j)+"/cvFxUnit_"+ofToString(o)+".xml");
			cvAnalysis[o].mGui.saveSettings("Preset"+ofToString(i)+ofToString(j)+"/Analysis_"+ofToString(o)+".xml");
		}
		
	}
	
	if (w->getName() == "namePreset"){
		ofxUITextInput* ti = (ofxUITextInput*)(e.widget);
		ti->setTextString(ti->getTextString());
	}
}