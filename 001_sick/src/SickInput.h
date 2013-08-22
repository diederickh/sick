#ifndef ROXLU_SICKINPUT_H
#define ROXLU_SICKINPUT_H

#include "ofMain.h"
#include "ofxOsc.h"

typedef void(*sick_callback)(void* user);

struct SickPoint {
  float x;
  float y;
  float dist;
  bool is_inside; // is inside or outside the threshold radius
  int quadrant; // 0 = top left, 1 = top right, 2 = bottom right, 3 = bottom left
};

class SickInput {
 public:
  SickInput();
  void setup();
  void update();
  void draw();
 public:
  ofxOscReceiver osc;
  sick_callback cb_trigger; // gets called when the distance is less then a specific threshold
  void* cb_user;
  std::vector<SickPoint> found_points;
  float scale;
  int num_inside;
  int num_outside;
  int num_inside_prev;
  int num_outside_prev;
  float radius;
};

#endif
