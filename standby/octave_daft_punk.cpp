#include "octave_daft_punk.h"

#define LED 13
#define FOOTSWITCH 12
#define POTENTIOMETER A4

#define PWM_FREQ 0x00FF
#define PWM_MODE 0
#define PWM_QTY 2

#define MAX_OCTAVE 500

static int octave_input;
static int octave_counter;
static unsigned int octave_ADC_low, octave_ADC_high;
static int octave_dist_variable;

void setupOctave() {
  pinMode(FOOTSWITCH, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(POTENTIOMETER, INPUT);

  ADMUX = 0x60;
  ADCSRA = 0xe5;
  ADCSRB = 0x07;
  DIDR0 = 0x01;

  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1));
  TCCR1B = ((PWM_MODE << 3) | 0x11);
  TIMSK1 = 0x20;
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02);
  sei();
}

void processOctave() {
  if (digitalRead(FOOTSWITCH)) {
    digitalWrite(LED, HIGH);

    octave_ADC_low = ADCL;
    octave_ADC_high = ADCH;

    int potValue = analogRead(POTENTIOMETER);
    octave_dist_variable = map(potValue, 0, 1023, 0, MAX_OCTAVE);

    octave_counter++;
    if (octave_counter == 2000) {
      octave_counter = 0;
      digitalWrite(LED, LOW);
    }

    octave_input = ((octave_ADC_high << 8) | octave_ADC_low) + 0x8000;

    if (octave_input > octave_dist_variable) {
      octave_input = octave_dist_variable;
    }

    OCR1AL = ((octave_input + 0x8000) >> 8); // Converte para unsigned, manda o high byte
    OCR1BL = octave_input; // Manda o low byte
  } 
  
  else {
    digitalWrite(LED, LOW);
  }
}
