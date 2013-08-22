#pragma once

#include "ofxDMX.h"
#include "ofMain.h"
#include "LedFloor.h"
#include "SuperSonic.h"
#include "SensorSerial.h"
#include "AudioManager.h"
#include "SickInput.h"


void on_sensor_change(void* user); // gets called when the sensor trigger a change in distance above a certain treshold (ultra sonic sensors)
void on_level_changed(LedStrip& ls, void* user); // gets called when we need to chnage the level of a ledstrip 
void on_sick_trigger(float dist, void* user); // gets called when the sick input lets us know we need to make a sound

class testApp : public ofBaseApp{
 public:
  testApp();
  void setup();
  void update();
  void draw();
		
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  SickInput sick;
  LedFloor led_floor;
  LedFloorTester led_floor_tester;
  LedFloorFader led_floor_fader;
  ofxDmx dmx;
  SensorSerial sensor_serial;
  SuperSonic sonic;
  AudioManager audio;

};
