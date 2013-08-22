#include "SickInput.h"

SickInput::SickInput()
  :cb_trigger(NULL)
  ,cb_user(NULL)
  ,scale(1.0f)
  ,radius(1000) // mm
{
}

void SickInput::setup() {
  osc.setup(8888);
}


void SickInput::update() {

  if(osc.hasWaitingMessages()) {
    found_points.clear();
    num_inside_prev = num_inside;
    num_outside_prev = num_outside;
    num_inside = 0;
    num_outside = 0;
  }
  ofVec2f center(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

  while(osc.hasWaitingMessages()){
    // get the next message
    ofxOscMessage m;
    osc.getNextMessage(&m);
    std::string address = m.getAddress();

    uint32_t ntrackers = m.getArgAsInt32(0);

    for(uint32_t i = 0; i < ntrackers; ++i) {
      float tx = m.getArgAsFloat(1 + i * 2 + 0);
      float ty = m.getArgAsFloat(1 + i * 2 + 1);
      float d = sqrtf(tx * tx + ty * ty);

      SickPoint p;
      p.x = tx;
      p.y = ty;
      p.dist = d;
      p.is_inside = p.dist < radius;

      // In which quadrant is the point
      ofVec2f pv(center.x + p.x * scale, center.y + p.y * scale);
      ofVec2f dir = pv - center;
      float a = atan2(dir.y, dir.x);
      if(a < 0) {
        a += TWO_PI;
      }
      a = a * RAD_TO_DEG;
      if(a >= 270) {
        p.quadrant = 1;
      }
      else if(a >= 180) {
        p.quadrant = 0;
      }
      else if(a >= 90) {
        p.quadrant = 3;
      }
      else {
        p.quadrant = 2;
      }

      // inside or outside the threshold area
      if(p.is_inside) {
        num_inside++;
      }
      else {
        num_outside++;
      }

      found_points.push_back(p);
    }
  }
}

void SickInput::draw() {

  if(!found_points.size()){
    return;
  }

  float cx = ofGetWidth() * 0.5;
  float cy = ofGetHeight() * 0.5;
  ofVec2f center(cx, cy);

  glLineWidth(1.0f);
  ofNoFill();
  ofSetColor(ofColor::magenta);
  ofCircle(cx, cy, radius * scale);

  for(size_t i = 0; i < found_points.size(); ++i) {
    SickPoint p = found_points[i];
    ofVec2f pv(cx + p.x * scale, cy + p.y * scale);
    ofVec2f dir = pv - center;

    ofSetColor(255,0,0);
    ofLine(center.x, center.y, pv.x, pv.y);

    if(p.is_inside) { 
      ofSetColor(255,255,0);
    }
    else {
      ofSetColor(0,255,0);
    }
    if(p.quadrant == 0) {
      ofSetColor(255,0,0);
    }
    else if(p.quadrant == 1) {
      ofSetColor(0,244,0);
    }
    else if(p.quadrant == 2) {
      ofSetColor(0,0,255);
    }
    else  {
      ofSetColor(0,255,255);
    }

    ofFill();
    ofCircle(pv.x, pv.y, 10);
    
    ofSetColor(ofColor::green);
    ofNoFill();
    ofCircle(pv.x, pv.y, 12);
  }

  // trigger the audio when there is a change in number of people tracked
  if(num_inside != num_inside_prev || num_outside != num_outside_prev) {
    if(cb_trigger) {
      cb_trigger(cb_user);
    }
  }

}
