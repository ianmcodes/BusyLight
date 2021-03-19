#ifndef PARTY_ANIM_h
#define PARTY_ANIM_h
#include "anim.h"
class PartyAnim: public Anim {
  private:
    const int MAX_VAL = 0xf0f0f0;
    const int MIN_VAL = 0x000000;
    
  public:
    PartyAnim();
    int* nextFrame(int* frame);
};
#endif
