#ifndef MEETING_ANIM_h
#define MEETING_ANIM_h
#include "anim.h"
class MeetingAnim: public Anim {
  private:
    const int MAX_VAL = 0xff;
    const int MIN_VAL = 0x00;
    int red = MAX_VAL;
    int color = red << 16;
    
  public:
    MeetingAnim();
    int* nextFrame(int* frame);
};
#endif
