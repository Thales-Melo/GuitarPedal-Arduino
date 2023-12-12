#include "mult16x16.h"
#include "bell_shifter.h"

//defining harware resources.
#define LED 13
#define FOOTSWITCH 12
#define TOGGLE 2
#define PUSHBUTTON_1 A5
#define PUSHBUTTON_2 A4

//defining the output PWM parameters
#define PWM_FREQ 0x00FF // pwm frequency - 31.3KHz
#define PWM_MODE 0 // Fast (1) or Phase Correct (0)
#define PWM_QTY 2 // 2 PWMs in parallel
#define SIZE 1000 // make this smaller if it clicks

int* buffer = nullptr; // sample buffer
unsigned int location = 0; // current sample input position
unsigned int offset = 0; // playback sample offset from input
byte shift = 0x80;
unsigned int fractional = 0x80; // fractional sample rate
int data_buffer = 0x80;
int bell_counter=0;
int toggle_position=0;

void destroy_bell_shifter() {
  if (buffer != nullptr) {
    free(buffer);
    buffer = nullptr;
  }
}

void processBellShifter(){

  if(buffer == nullptr){
    buffer = (int*)malloc(sizeof(int)*SIZE);
  }
  
  // output data
  OCR1AL = ((data_buffer >> 8) + 0x80); 
  OCR1BL = data_buffer; // output the bottom byte
  
  // get ADC data
  byte temp1 = ADCL; // you need to fetch the low byte first
  byte temp2 = ADCH; // yes it needs to be done this way
  int input = ((temp2 << 8) | temp1) + 0x8000; // make a signed 16b value
  
 //BUTTONS
  bell_counter++; //the pushbuttons are checked every 2500 times.
  if(bell_counter==2500)
{ 

bell_counter=0;
if (!digitalRead(PUSHBUTTON_2)) {
    shift++;
        if (shift >= 0xfe) shift = 0xfe;
      }
 if (!digitalRead(PUSHBUTTON_1)) {
    shift--;
        if (shift <= 1) shift = 1;
      }
    }

//Depending on the Toggle switch position, the effect is reseted)
  if(digitalRead(TOGGLE)!=toggle_position)
    {
    toggle_position = digitalRead(TOGGLE); //update the new value
    shift=0x80;
   }
  
  buffer[location] = input; // store incoming data
  location++; // increment storage location
  if (location >= SIZE) location = 0;  // boundary wrap
  unsigned int temp = location + offset; // find next sample
  if (temp >= SIZE) temp -= SIZE; // boundary wrap
  int output = buffer[temp];  // fetch sample
  temp += (SIZE >> 1);  // find sample on opposite side of buffer
  if (temp >= SIZE) temp -= SIZE; // boundary wrap
  int output2 = buffer[temp]; // fetch sample
  unsigned int distance; // find distance to buffer boundary
  if (offset > (SIZE >> 1)) distance = SIZE - offset;
  else distance = offset;
  int result; // average the 2 samples based on distance to boundary
  MultiSU16X16toH16(result, output, (distance << 7));
  MultiSU16X16toH16(output, output2, (((SIZE >> 1) - distance) << 7));
  output += result;
  fractional += shift; // increment offset
  if (fractional >= 0x0080) {
    offset += (fractional >> 7);
    fractional &= 0x007f;
  }
  if (offset >= SIZE) offset -= SIZE; // boundary wrap

  // save data
  data_buffer = output;
}
