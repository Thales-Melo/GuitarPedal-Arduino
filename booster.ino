// Baseado em trabalhos anteriores da ElectroSmash.
// pedalshield_uno_booster.ino lê o ADC e reproduz na saída PWM.
// Pressionar o potenciômetro aumenta ou diminui o volume.


// Definição de recursos de hardware.
#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2
#define POTENTIOMETER A0  // Potenciômetro substitui pushbuttons
#define PWM_FREQ 0x00FF   // Frequência PWM - 31.3KHz
#define PWM_MODE 0        // Rápido (1) ou Correção de Fase (0)
#define PWM_QTY 2         // 2 PWMs em paralelo

// Outras variáveis
int input, vol_variable = 10000;
int counter = 0;
unsigned int ADC_low, ADC_high;

void setup() {
  // Configuração de IO
  pinMode(FOOTSWITCH, INPUT_PULLUP);
  pinMode(TOGGLE, INPUT_PULLUP);
  pinMode(POTENTIOMETER, INPUT);  // Potenciômetro como entrada analógica
  pinMode(LED, OUTPUT);

  // Configuração do ADC
  ADMUX = 0x60;  // Ajuste à esquerda, adc0, VCC interno
  ADCSRA = 0xe5;  // Liga o ADC, ck/32, disparo automático
  ADCSRB = 0x07;  // Captura t1 para o disparo
  DIDR0 = 0x01;   // Desativa entradas digitais para adc0

  // Configuração do PWM
  TCCR1A = (((PWM_QTY - 1) << 5) | 0x80 | (PWM_MODE << 1));
  TCCR1B = ((PWM_MODE << 3) | 0x11);  // ck/1
  TIMSK1 = 0x20;  // Interrupção na captura
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= ((PWM_QTY << 1) | 0x02);  // Liga as saídas
  sei();  // Liga interrupções - não é realmente necessário com Arduino
}

void loop() {
  // Liga o LED se o efeito estiver ligado.
  if (digitalRead(FOOTSWITCH))
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);
  // Nada aqui, tudo acontece na interrupção do Timer 1.
}

ISR(TIMER1_CAPT_vect) {
  // Obtém dados do ADC
  ADC_low = ADCL;  // Precisa buscar o byte baixo primeiro
  ADC_high = ADCH;
  // Constrói a amostra de entrada somando os bytes baixo e alto do ADC.
  input = ((ADC_high << 8) | ADC_low) + 0x8000;  // Cria um valor assinado de 16 bits

  // Todo o processamento de sinal digital acontece aqui.

  counter++;  // Para economizar recursos, os potenciômetros são verificados a cada 100 vezes.
  if (counter == 100) {
    counter = 0;
    // Atualiza o volume com base no valor do potenciômetro
    vol_variable = analogRead(POTENTIOMETER);
  }

  // A amplitude do sinal de entrada é modificada com base no valor do potenciômetro.
  input = map(input, -32768, +32768, -vol_variable, vol_variable);

  // Escreve o sinal PWM
  OCR1AL = ((input + 0x8000) >> 8);  // Converte para sem sinal, envia o byte alto
  OCR1BL = input;  // Envia o byte baixo
}
