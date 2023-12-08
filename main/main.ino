#include "efeitos.h"

#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2
#define POTENTIOMETER A4
#define ENCODER_CLK 3
#define ENCODER_DT 4
#define ENCODER_SW 5

int selectedEffect = 0;
bool effectOn = false;

void setup() {
  pinMode(FOOTSWITCH, INPUT_PULLUP);
  pinMode(TOGGLE, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(POTENTIOMETER, INPUT_PULLUP);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_SW, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_SW), handleEncoderSwitch, FALLING);

  setupEffects();  // Inicialização de todos os efeitos
}

void loop() {
  if (digitalRead(FOOTSWITCH)) {
    digitalWrite(LED, HIGH);
    if (effectOn) {
      effects[selectedEffect].processEffect(); // Chama a função de processamento do efeito atual
    }
  }
  else {
    digitalWrite(LED, LOW);
    effectOn = false;
  }
}

void handleEncoderSwitch() {
  // Chamado quando o botão do encoder é pressionado
  selectedEffect = (selectedEffect + 1) % NUM_EFFECTS;  // Altera o efeito selecionado
  switchEffect(selectedEffect);                         // Atualiza o efeito selecionado
  effectOn = true;  // Liga o efeito quando o usuário muda para um novo efeito
}
