o #include <Adafruit_MCP4725.h>
#include <Keypad.h> 
#include <Wire.h>

Adafruit_MCP4725 dac;
#define DAC_RESOLUTION    (12)
// voltage * (4096/5) <-- this is the equation used for converting the voltage out to 12bit scale
  
//  2.000 1638.4
//  2.083 1706.4
//  2.167 1775.2
//  2.205 1806.3
//  2.333 1911.2
//  2.417 1980.0
//  2.500 2048.0
//  2.583 2116.0
//  2.667 2184.8
//  2.750 2252.8
//  2.833 2320.8
//  2.917 2389.6
//  3.000 2457.6
//  3.083 2525.6
//  3.167 2594.4
//  3.205 2625.5
//  3.333 2730.4
//  3.417 2799.2
//  3.500 2867.2
//  3.583 2935.2
//  3.667 3004.0
//  3.750 3072.0
//  3.833 3140.0
//  3.917 3208.8
//  4.000 3276.8

const byte ROWS = 8;
const byte COLS = 8; 
char keys[ROWS][COLS] = {
{0,1,0,9 ,0,17,0,25},
{0,2,0,10,0,18,0,0},
{0,3,0,11,0,19,0,0},
{0,4,0,12,0,20,0,0},
{0,5,0,13,0,21,0,0},
{0,6,0,14,0,22,0,0},
{0,7,0,15,0,23,0,0},
{0,8,0,16,0,24,0,0}
};

double voltOct[26] = {0, // 0 here because I'm bad at counting
2.000,2.083,2.167,2.205,2.333,2.417,2.500,2.583,2.667,2.750,2.833,2.917,
3.000,3.083,3.167,3.205,3.333,3.417,3.500,3.583,3.667,3.750,3.833,3.917,
4.000}; // 4 here because 25 keys (Extra C)

byte rowPins[ROWS] = {37, 39, 41, 43, 45, 47, 49, 51}; 
byte colPins[COLS] = {36, 38, 40, 42, 44, 46, 48, 50}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
const byte GATE = 54;

const int plusPin = 30;    
const int minusPin = 31; 
const int plusLed = 53; 
const int minusLed = 52;
int switchState;   

void setup(){
  dac.begin(0x62);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GATE, OUTPUT);
  keypad.addEventListener(keypadEvent);
  pinMode(plusPin, INPUT_PULLUP);
  pinMode(minusPin, INPUT_PULLUP);
  pinMode(plusLed, OUTPUT);
  pinMode(minusLed, OUTPUT);
}

void loop(){
  if (digitalRead(plusPin) == HIGH && digitalRead(minusPin) == LOW) {
    // turn LED on:
    digitalWrite(plusLed, HIGH);
    digitalWrite(minusLed, LOW);
    switchState=1;
  } else if (digitalRead(plusPin) == LOW && digitalRead(minusPin) == HIGH) {
    // turn LED on:
    digitalWrite(plusLed, LOW);
    digitalWrite(minusLed, HIGH);
    switchState=-1;
  } else  {
    // turn LED on:
    digitalWrite(plusLed, LOW);
    digitalWrite(minusLed, LOW);
    switchState=0;
  }
  char key = keypad.getKey();
  if (key != NO_KEY){
    Serial.print("Octave wrote:");
    Serial.println(switchState);
    Serial.print("Voltage wrote:");
    Serial.println(voltOct[key], 3);
    Serial.print("12bit Scale:");
    Serial.println((voltOct[key]*(4096/5)+switchState));
    dac.setVoltage((voltOct[key]*(4096/5)+switchState), false);
  }
}

void keypadEvent(KeypadEvent key){
    switch (keypad.getState()){
    case PRESSED:
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(GATE, HIGH);
    break;

    case RELEASED:
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(GATE, LOW);
    break;
    }
}
