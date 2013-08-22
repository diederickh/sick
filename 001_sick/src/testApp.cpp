#include "testApp.h"

void on_sensor_change(void* user) {
  testApp* app = static_cast<testApp*>(user);
  app->audio.playRandomSound();
}

void on_level_changed(LedStrip& ls, void* user) {
  printf("Level changed of led strip channel: %d to value: %d\n", ls.channel, ls.getLevel());
  testApp* app = static_cast<testApp*>(user);
  app->dmx.setLevel(ls.channel, ls.getLevel());
}

void on_sick_trigger(void* user) {
  testApp* app = static_cast<testApp*>(user);
  app->audio.playRandomSound();
}

//--------------------------------------------------------------
testApp::testApp()
  :led_floor_tester(led_floor) 
  ,led_floor_fader(led_floor, sick)
{
}

void testApp::setup(){
  ofBackground(33);
  ofSetFrameRate(60);
  ofSetVerticalSync(false);
  ofSetCircleResolution(60);
  led_floor.setup();

  led_floor.setChannel(0, 0, 1);
  led_floor.setChannel(0, 1, 2);
  led_floor.setChannel(1, 0, 3);
  led_floor.setChannel(1, 0, 4);
  led_floor.setChannel(2, 0, 5);
  led_floor.setChannel(2, 1, 6);
  led_floor.setChannel(3, 0, 7);
  led_floor.setChannel(3, 1, 8);

  led_floor.setLevelChangedCallback(on_level_changed, this);

  int total = 8;
  dmx.connect("tty.usbserial-ENVWHRUO");
  sensor_serial.setup("tty.usbmodemfa131", 115200);
  sonic.setup(ofGetWidth(), ofGetHeight());
  sonic.cb_change = on_sensor_change;
  sonic.cb_user = this;

  audio.addSound("audio/Distance.wav");
  audio.addSound("audio/Payoff.wav");
  audio.addSound("audio/Percusion1.mp3");
  audio.addSound("audio/Percusion2.mp3");
  audio.addSound("audio/Percusion3.mp3");
  audio.addSound("audio/Percusion4.mp3");
  audio.addSound("audio/Percusion5.mp3");

  sick.setup();
  sick.cb_trigger = on_sick_trigger;
  sick.cb_user = this;
}


//--------------------------------------------------------------
void testApp::update(){
  ofSetWindowTitle(ofToString(ofGetFrameRate()));
#if 0
  if(sensor_serial.update()) {
    sonic.writeBytes(sensor_serial.buffer, sensor_serial.nread);
  }
  sonic.update();
#endif

  sick.update();
  
  led_floor_tester.update();
  led_floor.update();

  led_floor_fader.update();
  dmx.update();
}

//--------------------------------------------------------------
void testApp::draw(){
  led_floor.draw();
  //sonic.draw();
  
  sick.scale = ofMap(mouseX, 0, ofGetWidth(), 0.001, 1.0, true);
  sick.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  if(key == '1') {
    led_floor_tester.start();
  }
  else if(key == '2') {
    led_floor_tester.stop();
  }
  else if(key == ' ') {
    audio.playRandomSound();
  }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
