/*

  # SuperSonic

  Parses data from the serial port for the Nissan Leaf project. We implmeent a 
  simple protocol where we receive uint16_t from 4 devices followed by a 
  end sequence of 0x00 0x00 0x00 0x01.

     CF FF   13 AA    CA 33    DE 31  00 00 00 01
       ^      ^         ^        ^        ^
       |      |         |        |        |_  0x00000001 <-- 8 bytes end of sequence
       |      |         |        |___________ 0xDE31     <-- data from sensor 4
       |      |         |____________________ 0xCA33     <-- data from sensor 3
       |      |______________________________ 0x13AA     <-- data from sensor 2
       |_____________________________________ 0xCFFF     <-- data from sensor 1

       

 */
#ifndef ROXLU_SUPERSONIC_H
#define ROXLU_SUPERSONIC_H

#include "ofMain.h"
#include <vector>
#include <algorithm>
#include <iterator>

typedef void(*supersonic_callback)(void* user);

struct ChannelData {
  ChannelData();
  void scaleData();
  void print();
  uint16_t data[4];
  //std::vector<uint16_t> data;
  //std::vector<uint16_t> scaled;
};

class SuperSonic {
 public:
  SuperSonic();
  ~SuperSonic();
  void setup(int winW, int winH);
  void onResize(int winW, int winH);
  void update();
  void draw();
  void drawGraph(int dx, int y, int height);
  bool parse();
  void generateTestData();
  void readU16(uint16_t& result, size_t offset);
  void writeBytes(uint8_t* buf, size_t nbytes);
 public:
  supersonic_callback cb_change; // called when there is a big change and audio should be triggered
  void* cb_user; // gets passed into cb_change;
 private:
  int win_w;
  int win_h;
  ofSerial serial;
  std::vector<uint8_t> buffer;
  std::deque<ChannelData> data;
  std::deque<uint32_t> scaled;
};

inline void SuperSonic::writeBytes(uint8_t* buf, size_t nbytes) {
  if(!buf) { 
    printf("Invalid buffer pointer!\n");
    return;
  }
  if(!nbytes) {
    printf("Invalid nbytes\n");
    return;
  }
  std::copy(buf, buf + nbytes, std::back_inserter(buffer));
}

inline void SuperSonic::readU16(uint16_t& result, size_t offset) {
  if(buffer.size() < offset+2) {
    result = 0;
    printf("error: Trying to read an uint16_t but the buffer is too small to contain a uint16_t\n");
    return;
  }
  memcpy((char*)&result, (char*)&buffer[offset], 2);
}

#endif
