#ifndef ANIM_h
#define ANIM_h
#include "led_config.h"
//struct Anim {
//  int numFrames;
//  int fps;
//  int frames[][NUM_LEDS];
//};

class Anim {
  public:
    virtual int* nextFrame(int* frame) = 0;
    int numFrames;
    double fps;
};
#endif
