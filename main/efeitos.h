#ifndef EFEITOS_H
#define EFEITOS_H

#include <Arduino.h>
#include <RotaryEncoder.h>

#define NUM_EFFECTS 2  // Número total de efeitos
enum {CLEAN, CHORUS, BELL};

#define PWM_FREQ 0x00FF
#define PWM_MODE 0
#define PWM_QTY 2

// Struct para armazenar um ponteiro de função para a função de processamento de cada efeito
struct Effect {
  void (*processEffect)();
};

extern Effect effects[NUM_EFFECTS];

void setupEffects();
void switchEffect(int *selectedEffect);

#endif
