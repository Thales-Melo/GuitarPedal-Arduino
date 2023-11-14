// Com base nos trabalhos anteriores da ElectroSmash.
// pedalshield_uno_delay.ino lê o ADC, salva-o no array DelayBuffer[] e reproduz na saída PWM.
// Pressionar o footswitch aumenta ou diminui o atraso usando um potenciômetro.
// Definição dos recursos de hardware.

#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2
#define POTENTIOMETER A0

// Definição dos parâmetros de saída PWM
#define PWM_FREQ 0x00FF // Frequência PWM - 31.3KHz
#define PWM_MODE 0 // Rápido (1) ou Correção de Fase (0)
#define PWM_QTY 2 // 2 PWMs em paralelo

// Outras variáveis
int input, vol_variable = 512;
int counter = 0;
unsigned int ADC_low, ADC_high;

#define MAX_DELAY 2000
byte DelayBuffer[MAX_DELAY];
unsigned int DelayCounter = 0;
unsigned int Delay_Depth = MAX_DELAY;

void setup() {
  // Configuração de E/S
  pinMode(FOOTSWITCH, INPUT_PULLUP);
  pinMode(TOGGLE, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // Configuração do ADC
  ADMUX = 0x60; // Ajuste à esquerda, adc0, VCC interno
  ADCSRA = 0xe5; // Liga o ADC, ck/32, acionamento automático
  ADCSRB = 0x07; // Captura t1 para acionamento
  DIDR0 = 0x01; // Desliga entradas digitais para adc0

  // Configuração do PWM
  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1)); //
  TCCR1B = ((PWM_MODE << 3) | 0x11); // ck/1
  TIMSK1 = 0x20; // Interrupção na captura
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02); // Liga as saídas
  sei(); // Liga interrupções
}

void loop() {
  // Liga o LED se o efeito estiver ligado.
  if (digitalRead(FOOTSWITCH)) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);

  // Nada aqui, tudo acontece na interrupção do Timer 1.
}

ISR(TIMER1_CAPT_vect) {
  // Obtemos os dados do ADC
  ADC_low = 0; // ADC_low sempre 0 para economizar espaço
  ADC_high = ADCH;

  // Armazenamos apenas o bit alto
  DelayBuffer[DelayCounter] = ADC_high;

  counter++;
  if (counter == 100) {
    counter = 0;
    // Leitura do potenciômetro
    int potValue = analogRead(POTENTIOMETER);

    // Mapeia o valor do potenciômetro para o intervalo desejado para Delay_Depth
    Delay_Depth = map(potValue, 0, 1023, 0, MAX_DELAY);

    // Pisca o LED
    digitalWrite(LED, LOW);
  }

  // Incrementa/reset do contador de delay
  DelayCounter++;
  if (DelayCounter >= Delay_Depth) DelayCounter = 0;

  input = (((DelayBuffer[DelayCounter] << 8) | ADC_low) + 0x8000) + (((ADC_high << 8) | ADC_low) + 0x8000);

  // Escreve o sinal PWM
  OCR1AL = ((input + 0x8000) >> 8);
  OCR1BL = input;
}
