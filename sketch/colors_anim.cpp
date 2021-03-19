#include "colors_anim.h"

ColorsAnim::ColorsAnim() {
  numFrames = 1;
  fps = 100.0;
}

int* ColorsAnim::nextFrame(int* frame) {
  int color;
  red += incRed;
  green += incGreen;
  blue += incBlue;
  if(currPhase == addBlue && blue >= MAX_VAL) {
    currPhase = subRed;
    incBlue = 0;
    incRed = -stepSize;
    blue = MAX_VAL;
  } else if(currPhase == subRed && red <= MIN_VAL) {
    currPhase = addGreen;
    incRed = 0;
    incGreen = stepSize;
    red = MIN_VAL;
  } else if(currPhase == addGreen && green >= MAX_VAL) {
    currPhase = subBlue;
    incGreen = 0;
    incBlue = -stepSize;
    green = MAX_VAL;
  } else if(currPhase == subBlue && blue <= MIN_VAL) {
    currPhase = addRed;
    incBlue = 0;
    incRed = stepSize;
    blue = MIN_VAL;
  } else if(currPhase == addRed && red >= MAX_VAL){
    currPhase = subGreen;
    incRed = 0;
    incGreen = -stepSize;
    red = MAX_VAL;
  } else if(currPhase == subGreen && green <= MIN_VAL) {
    currPhase = addBlue;
    incGreen = 0;
    incBlue = stepSize;
    green = MIN_VAL;
  }
  color = (red << 16) + (green << 8) + blue;
  for(int i = 0; i < NUM_LEDS; i++) {
    frame[i] = color;
  }
  return frame;
}
