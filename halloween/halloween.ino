#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define NUM_LEDS 150

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(210, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int red   = 204;
int grn   = 0;
int blu   = 0;
int space = 5;
int cnt   = 0;

void setup() {
   int i;

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}


void HalloweenEyes(byte red, byte green, byte blue, 
                   int EyeWidth, int EyeSpace, 
                   boolean Fade, int Steps, int FadeDelay,
                   int EndPause){
  randomSeed(analogRead(0));
  
  int i;
  int StartPoint  = random( 0, NUM_LEDS - (2*EyeWidth) - EyeSpace );
  int Start2ndEye = StartPoint + EyeWidth + EyeSpace;
  
  for(i = 0; i < EyeWidth; i++) {
    strip.setPixelColor(StartPoint + i, red, green, blue);
    strip.setPixelColor(Start2ndEye + i, red, green, blue);
  }
  
  strip.show();
  
  if(Fade==true) {
    float r, g, b;
  
    for(int j = Steps; j >= 0; j--) {
      r = j*(red/Steps);
      g = j*(green/Steps);
      b = j*(blue/Steps);
      
      for(i = 0; i < EyeWidth; i++) {
        strip.setPixelColor(StartPoint + i, r, g, b);
        strip.setPixelColor(Start2ndEye + i, r, g, b);
      }
      
      strip.show();
      delay(FadeDelay);
    }
  }
  
  for(uint16_t i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0,0,0);
 
  delay(EndPause);
}

void loop() {
   int i;
   int dir[2];
   int d;

  // Fixed:
  // HalloweenEyes(0xff, 0x00, 0x00, 1,4, true, 10, 80, 3000);
  // or Random:

#if 0
  HalloweenEyes(0xff, 0x00, 0x00, 
                1, 2, 
                true, random(5,50), random(50,150), 
                random(1000, 10000));

#else

   for (i=0; i < NUM_LEDS/2; i++) {
      dir[0]  = 74-i;
      dir[1]  = 75+i;

      for (d=0; d < 2; d++) {
         if ((i % space) == cnt) {
            strip.setPixelColor(dir[d], 100, 100, 80 );
         }
         else {
            strip.setPixelColor(dir[d], 128, 25, 0 );
         }
      }
   }
   
   if ( ++cnt == space ) cnt = 0; 
   strip.show();
   delay(250);

#endif
}

