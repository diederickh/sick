#include "LedFloor.h"
#include "SickInput.h"

LedStrip::LedStrip()
  :x(0.0f)
  ,y(0.0f)
  ,radius(0.0f)
  ,size(0.0f)
  ,start_angle(0.0f)
  ,level(255)
  ,channel(0)
  ,cb_user(NULL)
  ,cb_level_changed(NULL)
{
}

LedStrip::~LedStrip() {
  x = 0.0f;
  y = 0.0f;
  radius = 0.0f;
  size = 0.0f;
  start_angle = 0.0f;
}

void LedStrip::setLevel(unsigned char l) {
  level = l;
  if(cb_level_changed) {
    cb_level_changed(*this, cb_user);
  }
}

// ------------------------------------------------------------

void LedStripPair::setup(float startAngle, int x, int y, 
                         int minRadius, int maxRadius, 
                         float minSize, float maxSize)
{
  a.start_angle = startAngle;
  a.radius = minRadius;
  a.size = minSize;
  a.x = x;
  a.y = y;

  b.start_angle = startAngle;
  b.radius = maxRadius;
  b.size = maxSize;
  b.x = x;
  b.y = y;
}

void LedStripPair::setLevel(int num, unsigned char level)  {
  if(num == 0) {
    a.setLevel(level);
  }
  else {
    b.setLevel(level);
  }
}

void LedStripPair::setChannel(int num, int chan) {
  if(num == 0) {
    a.channel = chan;
  }
  else {
    b.channel = chan;
  }
}

void LedStripPair::setLevelChangedCallback(strip_callback levelCB, void* user) {
  a.cb_level_changed = levelCB;
  b.cb_level_changed = levelCB;
  a.cb_user = user;
  b.cb_user = user;
}

// ------------------------------------------------------------
LedFloor::LedFloor() {
}

LedFloor::~LedFloor() {
}

void LedFloor::setInnerLevel(unsigned char level) {
  setLevel(0, 0, level);
  setLevel(1, 0, level);
  setLevel(2, 0, level);
  setLevel(3, 0, level);
}

void LedFloor::setOuterLevel(unsigned char level) {
  setLevel(0, 1, level);
  setLevel(1, 1, level);
  setLevel(2, 1, level);
  setLevel(3, 1, level);
}

// pair 0 = top left, 1 = top right, 2 = bottom right, 3 = bottom left
void LedFloor::setLevel(int ledStripPair, int num, unsigned char level) {
  switch(ledStripPair) {
    // top left
    case 0: {
      leds_top_left.setLevel(num, level);
      break;
    }
    case 1: {
      leds_top_right.setLevel(num, level);
      break;
    }
    case 2: {
      leds_bottom_right.setLevel(num, level);

      break;
    }
    case 3: {
      leds_bottom_left.setLevel(num, level);
      break;
    }
    default: {
      printf("Error: error while setting the led strip!!!\n");
    };
  };
}

void LedFloor::setChannel(int ledStripPair, int num, int channel) {
 switch(ledStripPair) {
    case 0: {
      leds_top_left.setChannel(num, channel);
      break;
    }
    case 1: {
      leds_top_right.setChannel(num, channel);
      break;
    }
    case 2: {
      leds_bottom_right.setChannel(num, channel);

      break;
    }
    case 3: {
      leds_bottom_left.setChannel(num, channel);
      break;
    }
    default: {
      printf("Error: error while setting the led strip!!!\n");
    };
  };
}

void LedFloor::setup() {
  float cx = ofGetWidth() * 0.5;
  float cy = ofGetHeight() * 0.5;
  float w = 0;

  leds_top_left.setup(    PI,            cx - w, cy,     150, 190, 8.0f, 13.0f);
  leds_top_right.setup(   PI + HALF_PI,  cx + w, cy,     150, 190, 8.0f, 13.0f);
  leds_bottom_right.setup(0,             cx + w, cy + w, 150, 190, 8.0f, 13.0f);
  leds_bottom_left.setup(HALF_PI,        cx - w, cy + w, 150, 190, 8.0f, 13.0f);
}

void LedFloor::setLevelChangedCallback(strip_callback levelCB, void* user) {
  leds_top_left.setLevelChangedCallback(levelCB, user);
  leds_top_right.setLevelChangedCallback(levelCB, user);
  leds_bottom_left.setLevelChangedCallback(levelCB, user);
  leds_bottom_right.setLevelChangedCallback(levelCB, user);
}

void LedFloor::update() {
}

void LedFloor::draw() {
  drawLedStripPair(leds_top_left);
  drawLedStripPair(leds_top_right);
  drawLedStripPair(leds_bottom_right);
  drawLedStripPair(leds_bottom_left);
}

void LedFloor::drawLedStripPair(LedStripPair& p) {
  drawLedStrip(p.a);
  drawLedStrip(p.b);
};

void LedFloor::drawLedStrip(LedStrip& ls) {
  drawArc(ls.start_angle, ls.x, ls.y, ls.radius, ls.size, ls.getLevel());
}

void LedFloor::drawArc(float startAngle, int x, int y, int radius, float size, unsigned char level) {
  int res = 20;
  float angle = startAngle;

  float as = HALF_PI / float(res);

  
  float lp = float(level)/255.0f;
  glColor3f(lp, lp, lp);
  glPointSize(size);
  glBegin(GL_POINTS);

  for(int i = 0; i < res; ++i) {
    float ca = cos(angle);
    float sa = sin(angle);
    float xx = x + ca * radius;
    float yy = y + sa * radius;
    glVertex2f(xx, yy);
    angle += as;
  }

  glEnd();
}

// ------------------------------------------------------------------

LedFloorTester::LedFloorTester(LedFloor& floor)
  :lf(floor)
  ,state(0)
{
}

void LedFloorTester::start() {
  state = 1;
  timeout = millis() + 1000;
}

void LedFloorTester::stop() {
  state = 0;
}

void LedFloorTester::update() {
  if(!state) {
    return;
  }

  uint64_t now = millis();
  if(now < timeout) {
    return;
  }
  
  int time = 250;
  switch(state) {
    case 1: {
      lf.setInnerLevel(100);
      
      timeout = now + time;
      state = 2;
      break;
    }
    case 2: {
      lf.setOuterLevel(100);
      timeout = now + time;
      state = 3;
      break;
    }
    case 3: { 
      lf.setInnerLevel(255);
      timeout = now + time;
      state = 4;
      break;
    }
    case 4: {
      lf.setOuterLevel(255);
      timeout = now + time;
      state = 1;
      break;
    }
    default: {
      break;
    }
  }

}

// ------------------------------------------------------------
LedFloorFade::LedFloorFade()
  :timeout(0)
{
}

LedFloorFader::LedFloorFader(LedFloor& lf, SickInput& sick)
  :lf(lf)
  ,sick(sick)
{
  counts.assign(4, 0);
  counts_prev.assign(4,0);
  timeouts.assign(4, LedFloorFade());
}

void LedFloorFader::update() {
  // fill current counts.
  for(size_t i = 0; i < sick.found_points.size(); ++i) {
    if(sick.found_points[i].quadrant < 4) {
      counts[sick.found_points[i].quadrant]++;
    }
  }
  
  // check which quadrant changed
  for(size_t i = 0; i < counts.size(); ++i) {
    if(counts[i] != counts_prev[i]) {
      // printf("QUADRANT CHANGE >>>>>>>>>>> q=%d, %d <> %d\n",i, counts[i], counts_prev[i]);
      if(!timeouts[i].timeout) {
        timeouts[i].timeout = ofGetElapsedTimeMillis() + 5000;
      }
    }
  }

  // and do the fades
  uint64_t now = ofGetElapsedTimeMillis();
  for(size_t i = 0; i < timeouts.size(); ++i) {
    LedFloorFade& f = timeouts[i];
    if(f.timeout <= now) {
      f.timeout = 0;
      lf.setLevel(i,0,0);
    }
    else {
      int64_t delta = (timeouts[i].timeout - now);
      float perc = delta / 5000.0f;
      int value = (0.5 + 0.5 * sin(delta * 0.003) * 255);
      //printf("%d - %d - %f\n", value, delta, perc);
      lf.setLevel(i, 0, value);
      lf.setLevel(i, 1, 255 - value);
    }
  }

  counts_prev = counts;
  counts.clear();
  counts.assign(4,0);
}
