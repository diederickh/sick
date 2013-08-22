#ifndef ROXLU_LED_FLOOR_H
#define ROXLU_LED_FLOOR_H

#include "ofMain.h"

struct LedStrip;
typedef void(*strip_callback)(LedStrip& strip, void* user); // led strip callback; called when e.g. the level changed

struct LedStrip {
  LedStrip();
  ~LedStrip();
  void setLevel(unsigned char level);
  unsigned char getLevel();

  float start_angle;
  float radius;
  float size;
  float x;
  float y;
  int channel; /* dmx channel */
  strip_callback cb_level_changed; // called when the level of a strip changed
  void* cb_user; // passed into cb_level_changed

private:
  unsigned char level;
};

struct LedStripPair {
  LedStrip a;
  LedStrip b;
  void setup(float startAngle, int x, int y, 
             int minRadius, int maxRadius, 
             float minSize, float maxSize);
  void setLevel(int num, unsigned char level);
  void setChannel(int num, int chan);
  void setLevelChangedCallback(strip_callback levelCB, void* user);
};

class LedFloor {
 public:
  LedFloor();
  ~LedFloor();
  void setup();
  void setLevelChangedCallback(strip_callback levelCB, void* user); 
  void update();
  void draw();
  void setLevel(int pair, int num, unsigned char level); // set individual level
  void setInnerLevel(unsigned char level); // set the level for the inner row led strips
  void setOuterLevel(unsigned char level); // set the level for the outer row led strips
  void setChannel(int pair, int num, int channel); // set the DMX channel
 public:

 private:
  void drawLedStripPair(LedStripPair& p);
  void drawLedStrip(LedStrip& strip);
  void drawArc(float startAngle, int x, int y, int radius, float size, unsigned char level);
  LedStripPair leds_top_left;
  LedStripPair leds_top_right;
  LedStripPair leds_bottom_right;
  LedStripPair leds_bottom_left;
};

class LedFloorTester {
 public:
  LedFloorTester(LedFloor& lf);
  void start();
  void stop();
  void update();
  uint64_t millis();
  
 private:
  LedFloor& lf;
  int state;
  uint64_t timeout;
};

inline uint64_t LedFloorTester::millis() {
  return ofGetElapsedTimeMillis();
}

inline unsigned char LedStrip::getLevel() {
  return level;
}

// ----------------------------------------

struct LedFloorFade {
  LedFloorFade();
  uint64_t timeout;
};

class SickInput;
class LedFloorFader {
 public:
  LedFloorFader(LedFloor& lf, SickInput& sick);
  void update();
 public:
  LedFloor& lf;
  SickInput& sick;
  std::vector<int> counts; // number of blobs per quadrant
  std::vector<int> counts_prev; // previous counts
  std::vector<LedFloorFade> timeouts;
};
#endif
