#include "clean.h"

#define LED 13
#define TOGGLE 2
#define POTENTIOMETER A4

#define PWM_FREQ 0x00FF
#define PWM_MODE 0
#define PWM_QTY 2

//other variables
int clean_input, clean_vol_variable=512;
int clean_counter=0;
byte clean_ADC_low, clean_ADC_high;

void setupClean(){
 //setup IO
  pinMode(POTENTIOMETER, INPUT);
  pinMode(LED, OUTPUT);
  
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

void processClean() {
  // get ADC data
  clean_ADC_low = ADCL; // you need to fetch the low byte first
  clean_ADC_high = ADCH;
  //construct the input sumple summing the ADC low and high byte.
  clean_input = ((clean_ADC_high << 8) | clean_ADC_low) + 0x8000; // make a signed 16b value

  
  //write the PWM signal
  OCR1AL = ((clean_input + 0x8000) >> 8); // convert to unsigned, send out high byte
  OCR1BL = clean_input; // send out low byte

}
