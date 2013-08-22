#include "SuperSonic.h"

ChannelData::ChannelData() {
#if 0
  data.push_back(0);
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);
#endif
}

void ChannelData::print() {
}

void ChannelData::scaleData() {
  int min = 30;
  int max = 4000;
  data[0] = ofMap(data[0],min, max, 0, 1024);
  data[1] = ofMap(data[1],min, max, 0, 1024);
  data[2] = ofMap(data[2],min, max, 0, 1024);
  data[3] = ofMap(data[3],min, max, 0, 1024);

}
// --------------------------------------------

SuperSonic::SuperSonic() 
  :win_w(0)
  ,win_h(0)
  ,cb_user(NULL)
  ,cb_change(NULL)
{

}

SuperSonic::~SuperSonic() {
}

void SuperSonic::setup(int winW, int winH) {

  if(!winW || !winH) {
    printf("error: invalid winW / winH, expect wrong drawing. win_w: %d, win_h: %d\n", winW, winH);
  }

  win_w = winW;
  win_h = winH;

  /*
  serial.listDevices();
  if(!serial.setup(0, 57600)) {
    printf("error: Cannot setup serial connection.\n");
    ::exit(EXIT_FAILURE);
  }
  */
}

void SuperSonic::generateTestData() {
  size_t num_els = 1; 
  uint16_t v = 0;
  uint8_t* vp = (uint8_t*)&v;
  for(size_t i = 0; i < num_els; ++i) {
    v = ofNoise(ofGetElapsedTimeMillis() * 0.001) * 1024;  buffer.push_back(vp[0]); buffer.push_back(vp[1]);    // printf("+ a: %d ", v);
    v = ofNoise(ofGetElapsedTimeMillis() * 0.0013) * 1024;  buffer.push_back(vp[0]); buffer.push_back(vp[1]);    // printf("b: %d ", v);
    v = ofNoise(ofGetElapsedTimeMillis() * 0.0015) * 1024;  buffer.push_back(vp[0]); buffer.push_back(vp[1]);    // printf("c: %d ", v);
    v = ofNoise(ofGetElapsedTimeMillis() * 0.0013) * 1024;  buffer.push_back(vp[0]); buffer.push_back(vp[1]);   // printf("d: %d\n", v);

    buffer.push_back(0x00);
    buffer.push_back(0x00);
    buffer.push_back(0x00);
    buffer.push_back(0x01);
  }
}

void SuperSonic::update() {
  //  generateTestData();
  parse();

  if(win_w > 0 && data.size() > win_w){
    data.erase(data.begin());
  }
}

void SuperSonic::draw() {
  int h = win_h / 4;
  ofEnableAlphaBlending();

  drawGraph(0, 0 * h, h);
  drawGraph(1, 1 * h, h);
  drawGraph(2, 2 * h, h);
  drawGraph(3, 3 * h, h);

  ofViewport(0,0, win_w, win_h);
  char buf[100];

  ofDrawBitmapString("Sensor input: 0", 10, 0.95 * h + 0 * h);
  ofDrawBitmapString("Sensor input: 1", 10, 0.95 * h + 1 * h);
  ofDrawBitmapString("Sensor input: 2", 10, 0.95 * h + 2 * h);
  ofDrawBitmapString("Sensor input: 3", 10, 0.95 * h + 3 * h);
}

void SuperSonic::drawGraph(int dx, int y, int height) {
  int x = 0;
  ofViewport(0,y, win_w, height);
  ofBackgroundGradient(ofColor(0,40,41), ofColor(0,30,20), OF_GRADIENT_LINEAR);

  // tiny horizontal lines
  {
    glBegin(GL_LINES);
    int num_lines = 40;
    float lh = float(win_h/num_lines);
    for(int i = 0; i < num_lines; ++i) {
      if(i % 2 == 0) {
        glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
        glVertex2f(0, i * lh);
        glVertex2f(5, i * lh);
      }
      else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
        glVertex2f(0, i * lh);
        glVertex2f(10, i * lh);
      }
    }
    glEnd();
  }

  // graph 
  {
    glColor4f(0.0, 0.28f, 0.37f, 0.9f);
    glBegin(GL_TRIANGLE_STRIP);
    x = 0;
    for(std::deque<ChannelData>::iterator it = data.begin();  it !=  data.end(); ++it) {  
      ChannelData& cd = *it;
      float yy = ofMap(cd.data[dx], 0, 1024, 0.0, 1.0);
      glVertex2f(x, win_h);
      glVertex2f(x,  yy * win_h + 50);
      ++x;
    }
    glEnd();
  }

  // gradient graph
  {
    glBegin(GL_TRIANGLE_STRIP);
    x = 0;
    for(std::deque<ChannelData>::iterator it = data.begin();  it !=  data.end(); ++it) {  
      ChannelData& cd = *it;
      float yy = ofMap(cd.data[dx], 0, 1024, 0.0, 1.0);
      glColor4f(0.0, 0.18f, 0.37f, 0.1f);
      glVertex2f(x, win_h);
      glColor4f(0.0, 0.18f, 0.37f, 0.9f);
      glVertex2f(x,  yy * win_h);
      ++x;
    }
    glEnd();
  }

  // another layer of graph
  {
    glColor4f(0.0, 0.28f, 0.47f, 0.9f);
    glBegin(GL_TRIANGLE_STRIP);
    x = 0;
    for(std::deque<ChannelData>::iterator it = data.begin();  it !=  data.end(); ++it) {  
      ChannelData& cd = *it;
      float yy = ofMap(cd.data[dx], 0, 1024, 0.0, 1.0);
      glVertex2f(x, win_h);
      glVertex2f(x,  yy * win_h + 100);
      ++x;
    }
    glEnd();
  }

  // colored single line
  {
    glBegin(GL_LINE_STRIP);
    glLineWidth(1.5f);
    x = 0;
    for(std::deque<ChannelData>::iterator it = data.begin();  it !=  data.end(); ++it) {  
      ChannelData& cd = *it;
      float yy = ofMap(cd.data[dx], 0, 1024, 0.0, 1.0);
      glColor3f(1.0, 0.31f, 0.28);
      glVertex2f(x,  yy * win_h);
      ++x;
    }
    glEnd();
  }

  // dots
  {
    int num_lines  = 100;
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    float lh = float(win_w/num_lines);
    for(int i = 0; i <= num_lines; ++i) {
      if(i % 5 == 0) {
        glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
      }
      else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
      }
      glVertex2f(i * lh, 40);
      glVertex2f(i * lh, 40);
    }
    glEnd();
  }

  // horizontal lines
  int num_lines  = 10;
  {
    glBegin(GL_LINES);
    float lh = float(win_h/num_lines);
    for(int i = 0; i < num_lines; ++i) {
      if(i % num_lines-2 == 0) {
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
      }
      else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
      }

      glVertex2f(0, i * lh);
      glVertex2f(win_w, i * lh);
    }
    glEnd();
  }

  // vertical lines
  {
    glBegin(GL_LINES);
    num_lines  = 100;
    float lw = float(win_w/num_lines);
    for(int i = 0; i <= num_lines; ++i) {
      if(i % 5 == 0) {
        glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
      }
      else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
      }
      glVertex2f(i * lw, win_h);
      glVertex2f(i * lw, 0);
    }
    glEnd();
  }

  // horizontal lines
  glLineWidth(1.0f);
  glBegin(GL_LINES);
  glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
  glVertex2f(0.0, win_h-1);
  glVertex2f(win_w, win_h-1);
  glEnd();
  glLineWidth(1.0f);
}

bool SuperSonic::parse() {

  if(buffer.size() < 4) {
    return false;
  }

#if 0
  for(size_t i = 0; i < buffer.size() - 4; ++i) {
    if(buffer.at(i) == 0x00 && buffer.at(i+1) == 0x00 
       && buffer.at(i+2) == 0x00 && buffer.at(i + 3) == 0x01) {
      printf("\n");
    }
    else {
      printf("%02X ", buffer[i]);
    }
  }

  buffer.clear();
  return true;
#endif

#if 1
  bool found = false;
  int num_sensors = 4;
  size_t min_buffer_size = sizeof(uint16_t) * num_sensors + 4;
  size_t sequence_start = 0;
  size_t sequence_end = 0;
  int state = 0;
  size_t start_dx = 0;
  size_t end_dx = 0;
  for(size_t i = 0; i < buffer.size() - 4; ++i) {
    if(!start_dx) {
      start_dx = i;
    }
    if(buffer.at(i) == 0x00 && buffer.at(i+1) == 0x00 
       && buffer.at(i+2) == 0x00 && buffer.at(i + 3) == 0x01) 
      {
        if(state == 0) {
          sequence_start = i;
          state = 1;
        }
        else if(state == 1) {
          sequence_end = i;
          state = 0;
          //    printf("Found a sequence: %ld to %ld\n", sequence_start, sequence_end);
        }
        end_dx = i;
      }

    if(sequence_end > 0) {
      start_dx = sequence_start + 4; // - (num_sensors * sizeof(uint16_t));
      buffer.erase(buffer.begin(), buffer.begin() + start_dx);
      //printf("Start looking at: %ld\n", start_dx);
      
      ChannelData entry;
      readU16(entry.data[0], i-8);
      readU16(entry.data[1], i-6);
      readU16(entry.data[2], i-4);
      readU16(entry.data[3], i-2);
      entry.scaleData();
      //      printf("%d, %d, %d, %d\n", entry.data[0], entry.data[1], entry.data[2], entry.data[3]);
      data.push_back(entry);
      
      sequence_start = 0;
      sequence_end = 0;
      found = true;
    }

    if(end_dx) {
      //      printf("found a sequence at: %ld\n", end_dx);
      //      buffer.erase(buffer.begin() + start_dx, buffer.begin() + end_dx);
      start_dx = 0;
      end_dx = 0;
             
    }
  }

  if(data.size() > 6 && found) {
    ChannelData prev = data[data.size()-5];
    ChannelData curr = data[data.size()-1];

    
    for(size_t i = 0; i < 4; ++i) {
      float val_a = prev.data[i];
      float val_b = curr.data[i];
      //      int32_t d = prev.data[i] - curr.data[i];
      float d = val_a - val_b;
      d = sqrt(d * d);
      if(d > 300 && d < 2000 && cb_change) {
        cb_change(cb_user);
        printf("%f (%f <> %f) , ", d, val_a, val_b);
      }
      //if(d = 0.0f) { 
      ///  continue;
      // }
      //   d = sqrtf(d);
      
      //d = sqrt(d);
      
    }
    printf("\n");
  }
  /*
  scaled.clear();
  scaled.push_back(0);
  scaled.push_back(1);
  scaled.push_back(2);
  scaled.push_back(3);
  
  for(std::deque<ChannelData>::iterator it = data.begin() ; it != data.end(); ++it) {
    ChannelData& cd = *it;
    for(int i = 0; i < 4; ++i) {
      scaled[i] += cd.data[i];
    }
  }
  for(size_t i = 0; i < scaled.size(); ++i) {
    scaled[i] /= scaled.size();
    printf(">> %ld\n", scaled[i]);
  }

  printf("---\n");
  */

#else   
  bool found_data = false;
  size_t dx_start = 0;
  for(size_t i = 0; i < buffer.size() - 4; ++i) {
    if(buffer.at(i) == 0x00 && buffer.at(i+1) == 0x00 
       && buffer.at(i+2) == 0x00 && buffer.at(i + 3) == 0x01)
      {
          //    if(buffer[i] == 0x00 && buffer[i+1] == 0x00 && buffer[i+2] == 0x00 && buffer[i + 3] == 0x01) {

        if(i < 8) {
          buffer.erase(buffer.begin() + i, buffer.begin() + i + 4);
        }
        else {
          dx_start = i - 8;
          ChannelData entry;
          readU16(entry.data[0], i-8);
          readU16(entry.data[1], i-6);
          readU16(entry.data[2], i-4);
          readU16(entry.data[3], i-2);
          printf("%d, %d, %d, %d\n", entry.data[0], entry.data[1], entry.data[2], entry.data[3]);
          data.push_back(entry);
          entry.print();
          buffer.erase(buffer.begin() + i, buffer.begin() + i + 4);
          found_data = true;
        }
    }
    if(buffer.size() < 12) {
      break;
    }

  }
  
  return found_data;
#endif
}

void SuperSonic::onResize(int winW, int winH) {
  if(!winW || !winH) {
    printf("error: invalid width/height");
    return;
  }
  win_w = winW;
  win_h = winH;
}
