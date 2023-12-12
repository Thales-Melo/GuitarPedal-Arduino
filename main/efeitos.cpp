#include "efeitos.h"
#include "clean.h"
#include "chorus.h"
// #include "bell_shifter.h"

Effect effects[NUM_EFFECTS] = {
  {processClean},
  {processChorus}
  // {processBellShifter}
};

void setupEffects() {
  
  // setup ADC
  ADMUX = 0x60; // left adjust, adc0, internal vcc
  ADCSRA = 0xe5; // turn on adc, ck/32, auto trigger
  ADCSRB = 0x07; // t1 capture for trigger
  DIDR0 = 0x01; // turn off digital inputs for adc0

  // setup PWM
  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
  TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK1 = 0x20; // interrupt on capture interrupt
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02); // turn on outputs
  sei(); // turn on interrupts - not really necessary with arduino
}

RotaryEncoder encoder(A2, A3);
int valor = 0;
int newPos = 0;

void switchEffect(int *selectedEffect) {

  static int pos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();

  if (pos != newPos) {

    //Seleciona novo efeito na ordem
    //Checando tambem se ultrapassa o indice de efeitos, caso sim, joga a selecao pra extremidade oposta
    *selectedEffect = (newPos) % NUM_EFFECTS;
    if(*selectedEffect < 0){
      *selectedEffect *= -1;
    }

    //debug
    // Serial.println(newPos);
    // Serial.println(*selectedEffect);
    // Serial.println();

    pos = newPos;
    
    //Funcoes de desalocacao
    destroy_chorus();
    // destroy_bell_shifter();
  }
  
}
