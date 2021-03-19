#include "chase_anim.h"

ChaseAnim::ChaseAnim() {
  numFrames = 1;
  fps = 10.0;
}

int* ChaseAnim::nextFrame(int *frame) {
  for(int i = 0; i < NUM_LEDS; i++) {
    frame[i] = 0;
  }
  frame[redPos] = red;
  redPos++;
  frame[greenPos] = green;
  greenPos++;
  frame[bluePos] = blue;
  bluePos++;
  if(redPos >= NUM_LEDS) {
    redPos = 0;
  }
  if(greenPos >= NUM_LEDS) {
    greenPos = 0;
  }
  if(bluePos >= NUM_LEDS) {
    bluePos = 0;
  }
  return frame;
}
