#include "AudioManager.h"

AudioManager::AudioManager() {
}

void AudioManager::addSound(std::string file) {
  ofSoundPlayer* p = new ofSoundPlayer();
  if(!p->loadSound(file)) {
    printf("ERROR: Cannot load: %s\n", file.c_str());
    return;
  }
  p->setMultiPlay(true);
  p->setVolume(1.0f);
  players.push_back(p);
}

void AudioManager::playRandomSound() {
  size_t dx = 0;
  do {
    dx = ofRandom(0, players.size());
  } while(dx >= players.size());

  players[dx]->play();
  
}

