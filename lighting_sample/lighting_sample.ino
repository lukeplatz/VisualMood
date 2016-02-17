#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);
int currentColor = 0;
float voltage = 0.0;
int count = 0;

void setup() {
  Serial.begin(9600); //bits per second
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {
  count++;
  //----Pressure Sensor----
    // Read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    if (count >= 50){
      voltage = sensorValue * (5.0 / 1023.0);
      count = 0;
    }
    
    if(4.5 <= voltage && voltage < 4.9 && currentColor != 2){
      colorWipe(strip.Color(0, 255, 0), 15); // Green
      currentColor = 2;
    }else if(voltage >= 4.9 && currentColor != 1){
      colorWipe(strip.Color(0, 0, 255), 15); // Blue
      currentColor = 1;
    }else if (voltage < 4.5 && currentColor != 0) {
      colorWipe(strip.Color(255, 0, 0), 15); // Red
      currentColor = 0;
    }
  //----Lights----
  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 15); // Red
//  colorWipe(strip.Color(0, 255, 0), 15); // Green
//  colorWipe(strip.Color(0, 0, 255), 15); // Blue
//
//  // Send a theater pixel chase in...
//  theaterChase(strip.Color(127, 127, 127), 15); // White
//  theaterChase(strip.Color(127,   0,   0), 15); // Red
//  theaterChase(strip.Color(  0,   0, 127), 15); // Blue
//
//  rainbow(15);
//  rainbowCycle(15);
//  theaterChaseRainbow(25);
//  delay(1000);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
//      strip.show();
//      delay(wait);
  }
  strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();

        delay(wait);

        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}