#include "distortion.h"

#define LED 13
#define TOGGLE 2
#define POTENTIOMETER A4

#define PWM_FREQ 0x00FF
#define PWM_MODE 0
#define PWM_QTY 2

#define MAX_DISTORTION 32768

int distortion_input, distortion_threshold = 6000; // Valor inicial ajustado por tentativa e erro (pedalshield test)
int distortion_counter = 0;
unsigned int distortion_ADC_low, distortion_ADC_high;

void setupDistortion() {
  
  pinMode(TOGGLE, INPUT);
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

void processDistortion() {
  // if (!digitalRead(TOGGLE)) {
    digitalWrite(LED, HIGH);
    // Serial.println("PROCESS_DISTORTION");
    // distortion_ADC_low = 0;
    distortion_ADC_low = ADCL;
    distortion_ADC_high = ADCH;

    distortion_counter++;

    if (distortion_counter == 1000) {

      distortion_counter = 0;
      int potValue = analogRead(POTENTIOMETER);
      Serial.print("pot_value = ");
      Serial.println(potValue);
      // delay(1000);
      distortion_threshold = map(potValue, 0, 1023, 0, MAX_DISTORTION);
      Serial.print("dist_threshold = ");
      Serial.println(distortion_threshold);
      digitalWrite(LED, LOW);
    }

    distortion_input = ((distortion_ADC_high << 8) | distortion_ADC_low) + 0x8000;

    if (distortion_input > distortion_threshold) {
      distortion_input = distortion_threshold;
    }

    OCR1AL = ((distortion_input + 0x8000) >> 8);
    OCR1BL = distortion_input;
  // }
}


