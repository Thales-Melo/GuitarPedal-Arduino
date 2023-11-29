#ifndef EFEITOS_H
#define EFEITOS_H

#include <Arduino.h>

#define NUM_EFFECTS 2  // Número total de efeitos (delay e distortion até então)

// Struct para armazenar informações sobre um efeito
struct Effect {
  void (*setupEffect)();
  void (*processEffect)();
};

extern Effect effects[NUM_EFFECTS];

void setupEffects();
void switchEffect(int effect);

#endif
