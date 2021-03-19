#ifndef CHASE_ANIM_h
#define CHASE_ANIM_h
#include "anim.h"
class ChaseAnim: public Anim {
  private:
    const int MAX_VAL = 0xff;
    const int MIN_VAL = 0x00;
    int red = MAX_VAL << 16;
    int redPos = 2;
    int green = MAX_VAL << 8;
    int greenPos = 1;
    int blue = MAX_VAL;
    int bluePos = 0;
    
  public:
    ChaseAnim();
    int* nextFrame(int* frame);
};
#endif
