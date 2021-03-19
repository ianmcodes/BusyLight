#include <Arduino.h>
#include "party_anim.h"

PartyAnim::PartyAnim() {
  numFrames = 1;
  fps = 10.0;
}

int* PartyAnim::nextFrame(int* frame) {
  for(int i = 0; i < NUM_LEDS; i++) {
    frame[i] = random(MIN_VAL, MAX_VAL);
  }
  return frame;
}
