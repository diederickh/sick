#ifndef ROXLU_AUDIO_MANAGER_H
#define ROXLU_AUDIO_MANAGER_H

#include "ofMain.h"

class AudioManager {
 public:
  AudioManager();
  void update();
  void addSound(std::string file);
  void playRandomSound();
 public:
  std::vector<ofSoundPlayer*> players;
};
#endif
