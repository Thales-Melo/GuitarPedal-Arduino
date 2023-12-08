#include "efeitos.h"

#define LED 13
#define TOGGLE 2
#define POTENTIOMETER A4
#define ENCODER_CLK 3
#define ENCODER_DT 4

int flag = 0;

int selectedEffect = 1;

void setup() {

  pinMode(TOGGLE, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(POTENTIOMETER, INPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);

  // attachInterrupt(digitalPinToInterrupt(ENCODER_SW), handleEncoderSwitch, FALLING);

  setupEffects();  // Inicialização de todos os efeitos
  Serial.begin(9600);
}

void loop() {

  if (!digitalRead(TOGGLE)) {

    digitalWrite(LED, HIGH);
  // effects[selectedEffect].setupEffect();
    // if () {
      // effects[selectedEffect].setupEffect();
      // effects[selectedEffect].processEffect(); // Chama a função de processamento do efeito atual
    // }
  }
  else {
    digitalWrite(LED, LOW);
  }
}

void handleEncoderSwitch() {

  // Chamado quando o botão do encoder é pressionado
  Serial.print("HandleEncoderSwitch: ");
  flag++;
  Serial.println(flag);
  selectedEffect = (selectedEffect + 1) % NUM_EFFECTS;  // Altera o efeito selecionado
  switchEffect(selectedEffect);                         // Atualiza o efeito selecionado
}

ISR(TIMER1_CAPT_vect) {
  // Serial.println("ISR");
  effects[selectedEffect].processEffect(); // Chama a função de processamento do efeito atual
}
