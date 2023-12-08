#include "efeitos.h"
#include "delay.h"
#include "distortion.h"
#include "octave_daft_punk.h"
#include "clean.h"

Effect effects[NUM_EFFECTS] = {
  {setupDelay, processDelay},         // Índice 0 representa o delay
  {setupDistortion, processDistortion}, // Índice 1 representa a distorção
  {setupOctave, processOctave},      // Índice 2 representa o octave_daft_punk
  {setupClean, processClean}         // Indice 3 representa o clean
};

void setupEffects() {
  // Inicialização de efeitos
  // for (int i = 0; i < NUM_EFFECTS; ++i) {
  //   effects[i].setupEffect();
  // }
  effects[1].setupEffect();
}

void switchEffect(int effect) {
  if (effect >= 0 && effect < NUM_EFFECTS) {
    destroyDelay();
    //setupEffects(); // Reinicia todos os efeitos
    effects[effect].setupEffect(); // Configura o efeito selecionado
  }
}
