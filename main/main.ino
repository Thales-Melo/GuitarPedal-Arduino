#include "efeitos.h"

#define LED 13
#define TOGGLE 2
#define POTENTIOMETER A5

int selectedEffect = CLEAN;
int oldEffect = 0;
int flag = 0;

void setup() {

  pinMode(TOGGLE, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(POTENTIOMETER, INPUT);

  setupEffects();  // Inicialização de todos os efeitos

  Serial.begin(9600);
}

void loop() {

  // Ao desligar e ligar novamente o pedal, o efeito selecionado é o mesmo que estava selecionado antes de desligar

  // Se o toggle estiver ligado, o efeito selecionado é ativado
  // Led acende para indicar que o efeito está ativado
  if (!digitalRead(TOGGLE)) {
    digitalWrite(LED, HIGH);
    if(flag){
      selectedEffect = oldEffect;
      flag = 0;
    }
  }

  // Se o toggle estiver desligado, o efeito é desativado e o clean (efeito padrão) é ativado
  // Led apaga para indicar que o efeito está desativado
  else {
    digitalWrite(LED, LOW);
    if(!flag){
      oldEffect = selectedEffect;
      selectedEffect = CLEAN;
      flag = 1;
    }
  }

  switchEffect(&selectedEffect);
}


// Interrupção que chama a função de processamento do efeito selecionado
ISR(TIMER1_CAPT_vect) {
    effects[selectedEffect].processEffect(); // Chama a função de processamento do efeito atual
}
