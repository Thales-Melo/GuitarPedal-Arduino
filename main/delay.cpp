#include "delay.h"
#include "efeitos.h"

#define LED 13
#define FOOTSWITCH 12
#define POTENTIOMETER A0

#define PWM_FREQ 0x00FF
#define PWM_MODE 0
#define PWM_QTY 2

static int delay_input, delay_vol_variable = 512;
static int delay_counter = 0;
static unsigned int delay_ADC_low, delay_ADC_high;

#define MAX_DELAY 2000
byte* DelayBuffer = nullptr;
unsigned int DelayCounter = 0;
unsigned int Delay_Depth = 0;  // Inicializado no setupDelay

void setupDelay() {
  pinMode(FOOTSWITCH, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(POTENTIOMETER, INPUT_PULLUP);

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

  // Inicializa Delay_Depth
  Delay_Depth = MAX_DELAY;
  
  // Alocação dinâmica do DelayBuffer
  DelayBuffer = (byte*)malloc(Delay_Depth * sizeof(byte));
}

void destroyDelay() {
  // Libera a memória alocada para o DelayBuffer
  if (DelayBuffer != nullptr) {
    free(DelayBuffer);
    DelayBuffer = nullptr;
  }
}

void processDelay() {
  if (digitalRead(FOOTSWITCH)) {
    digitalWrite(LED, HIGH);

    delay_ADC_low = ADCL;
    delay_ADC_high = ADCH;

    // Verificar primeiro se DelayBuffer já foi alocado p nao dar ruim
    if (DelayBuffer != nullptr) {
      DelayBuffer[DelayCounter] = delay_ADC_high;

      delay_counter++;
      if (delay_counter == 100) {
        delay_counter = 0;
        int potValue = analogRead(POTENTIOMETER);
        Delay_Depth = map(potValue, 0, 1023, 0, MAX_DELAY);
        digitalWrite(LED, LOW);
      }

      DelayCounter++;
      if (DelayCounter >= Delay_Depth) DelayCounter = 0;

      delay_input = (((DelayBuffer[DelayCounter] << 8) | delay_ADC_low) + 0x8000) + (((delay_ADC_high << 8) | delay_ADC_low) + 0x8000);
      OCR1AL = ((delay_input + 0x8000) >> 8);
      OCR1BL = delay_input;
    }
  } else {
    digitalWrite(LED, LOW);
  }
}
