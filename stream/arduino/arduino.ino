#include <FastLED.h>

#define LED_NUM_WIDE 10
#define LED_NUM_HIGH 10

#define LED_PIN A0

CRGB leds[LED_NUM_WIDE * LED_NUM_HIGH];
const int IDpin[8] = { 3, 5, 4, 2, 9, 7, 6, 8 };

void setup() {
  Serial.begin(500000);

  for (int i = 0; i < 8; i++) {
    pinMode(IDpin[i], INPUT_PULLUP);
  }
  
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, LED_NUM_WIDE * LED_NUM_HIGH);

  for (int i = 0; i < 100; i++) {
    leds[i].r = 0;
    leds[i].g = 255;
    leds[i].b = 0;
  }

  FastLED.show();
}

void loop() {
  if (Serial.available()) {
    
    String in = Serial.readStringUntil('\n');
    char mode = in[0];
    if (mode == 120) {
      FastLED.show();
    }
    switch(mode) {
      case 'i':
        Serial.print("ID : ");
        Serial.println((int)getID());
        break;
      case 'n':
        int number = in[1];
        char a = 0;
        if( number - 100 == getID() ){
          for (int i = 0; i < 100; i++) {
            int to_i = 9 - i / 10;
            int to_j = to_i % 2 ? i % 10 : 9 - i % 10;
            while (!Serial.available());
            leds[to_i * 10 + to_j].r = Serial.read();
            while (!Serial.available());
            leds[to_i * 10 + to_j].g = Serial.read();
            while (!Serial.available());
            leds[to_i * 10 + to_j].b = Serial.read();
          }
        } else {
          for (int i = 0; i < 300; i++) {
            while (!Serial.available());
            Serial.read();
          }
        }
        break;
    }
  }
}

uint8_t getID() {
  int out[2] = {0, 0};
  for (int i = 0; i < 8; i++) {
    out[i/4] *= 2;
    out[i/4] += !digitalRead(IDpin[i]);
  }
  return out[0] * 10 + out[1];
}
