#include <SPI.h>
#include <SD.h>
#include <FastLED.h>

#define LED_NUM_WIDE 10
#define LED_NUM_HIGH 10

#define LED_PIN A0
#define SD_PIN 10

CRGB leds[LED_NUM_WIDE * LED_NUM_HIGH];
const int IDpin[8] = { 3, 5, 4, 2, 9, 7, 6, 8 };

File runFilePtr;
unsigned char Height = 30;
unsigned char Width = 50;
unsigned int Frame = 0;
uint8_t runFileNum = 0;

unsigned long nextChangeFrame = 0;
unsigned int frame = 0;
bool isStart = false;

unsigned char Row = 0;
unsigned char Col = 0;

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 8; i++) {
    pinMode(IDpin[i], INPUT_PULLUP);
  }
  
  FastLED.addLeds<WS2811, LED_PIN, RGB>(leds, LED_NUM_WIDE * LED_NUM_HIGH);

  if (!SD.begin(SD_PIN)) {
    Serial.println("initialization failed!");
    while (1);
  }

  for (int i = 0; i < 100; i++) {
    leds[i].r = 0;
    leds[i].g = 255;
    leds[i].b = 0;
  }

  FastLED.show();
}

void loop() {
  if (Serial.available()) {
    String in = Serial.readString();
    char mode = in[0];
    switch(mode) {
      case 'r':
        runFileNum = in.substring(1, in.length()).toInt();
        if (runFilePtr) {
          runFilePtr.close();
        }
        getFileInfo();
        break;
      case 's':
        nextChangeFrame = millis();
        isStart = true;
        frame = 65535;
        break;
      case 'e':
        isStart = false;
        break;
      case 'b':

        break;
      case 'i':
        Serial.print("ID : ");
        Serial.println((int)getID());
        Serial.print("File Num : ");
        Serial.println(runFileNum);
        Serial.print("Height : ");
        Serial.println(Height);
        Serial.print("Width : ");
        Serial.println(Width);
        Serial.print("Frame : ");
        Serial.println(Frame);
        break;
    }
  }
  if (isStart && millis() >= nextChangeFrame) {
    if (frame >= Frame) {
      frame = 0;
      uint8_t id = getID();
      Row = id / 10;
      Col = id % 10;
      runFilePtr.seek((3 * Col * 10) + (Width * 3 * Row * 10));
    }
    
    unsigned char selectBoxColor[100][3];

    /// Get Color Data

    for (int i = 0; i < 10; i++) {
      runFilePtr.read(selectBoxColor[i * 10], 30);
      // Seek to next row
      runFilePtr.seek(runFilePtr.position() + (Width - 10) * 3);
    }

    /// Map pixel address

    for (int i = 0; i < 100; i++) {
        int to_i = 9 - i / 10;
        int to_j = to_i % 2 ? 9 - i % 10 : i % 10;
        leds[i].r = selectBoxColor[to_i * 10 + to_j][0];
        leds[i].g = selectBoxColor[to_i * 10 + to_j][1];
        leds[i].b = selectBoxColor[to_i * 10 + to_j][2];
    }

    // Seek to next frame
    runFilePtr.seek(runFilePtr.position() + (Width * (Height - 10)) * 3);

    FastLED.show();
    
    frame++;

    nextChangeFrame += 100;
  }
}

void getFileInfo() {
  String file = String(runFileNum);
  file += ".txt";
  runFilePtr = SD.open(file.c_str());
  if (runFilePtr) {
    /// Read file info
    unsigned long sz = runFilePtr.size();
    runFilePtr.seek((sz / 300 * 300) + 1);

    Height = runFilePtr.read();

    Frame = 0;
    unsigned char temp;
    int a = 0;
    while (runFilePtr.available()) {
      runFilePtr.readBytes(&temp, 1);
      Frame *= 256;
      Frame += temp;
    }
  
    Width = sz / 3 / Height / Frame;
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
