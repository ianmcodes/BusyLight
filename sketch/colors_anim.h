#ifndef COLORS_ANIM_h
#define COLORS_ANIM_h
#include "anim.h"
class ColorsAnim: public Anim {
  private:
    const int MAX_VAL = 0xff;
    const int MIN_VAL = 0x00;
    const int stepSize = 5;
    int red = MAX_VAL;
    int incRed = 0;
    int green = MIN_VAL;
    int incGreen = 0;
    int blue = MIN_VAL;
    int incBlue = stepSize;
    enum Phase {addBlue, subRed, addGreen, subBlue, addRed, subGreen};
    Phase currPhase = addBlue;
    
  public:
    ColorsAnim();
    int* nextFrame(int* frame);
};
#endif
