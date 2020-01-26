#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN A0

#define SERIAL_RX_BUFFER_SIZE 600

const int IDpin[8] = { 3, 5, 4, 2, 9, 7, 6, 8 };

Adafruit_NeoPixel pixels(100, LED_PIN, NEO_RGB + NEO_KHZ800);

uint8_t *Pixels;

uint8_t id;

void setup() {
  Serial.begin(500000);

  for (int i = 0; i < 8; i++) {
    pinMode(IDpin[i], INPUT_PULLUP);
  }
  
  pixels.begin();

  for (int i = 0; i < 100; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  }

  Pixels = pixels.getPixels();

  pixels.show();

  id = getID();
}

char in[5];
uint8_t x;
int i;

char mode;

uint8_t to_i;
uint8_t to_j;

uint8_t *p;

char a;

unsigned char USART_Receive( void )
{
  int rx = -1;
  while ((rx = Serial.read()) == -1);
  return rx;
}

void loop() {
  x = 0;
  do {
    if (x >= 5) x = 0;
    in[x] = USART_Receive();
  } while (in[x++] != '\n');
  
  mode = in[0];
  if (mode == 'n') {
    if( in[1] - 100 == id ){
      for (i = 0; i < 100; i++) {
        to_i = 9 - i / 10;
        to_j = to_i % 2 ? i % 10 : 9 - i % 10;
        
        p = &Pixels[(to_i * 10 + to_j) * 3];
        
        p[0] = USART_Receive();
        p[1] = USART_Receive();
        p[2] = USART_Receive();
      }
    } else {
      for (i = 0; i < 300; i++) {
        USART_Receive();
      }
    }
  }
  else if (mode == 'x') {
    pixels.show();

    id = getID();
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
