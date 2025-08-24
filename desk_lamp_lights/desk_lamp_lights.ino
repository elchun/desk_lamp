#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 94

#define NUM_LED 12
#define START_IDX 82

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// uint32_t magenta = strip.Color(255, 0, 255);
uint32_t magenta = strip.Color(50, 50, 100);
uint32_t red = strip.Color(255, 0, 0);
// uint32_t color = strip.Color(100, 10, 30);
uint32_t color = strip.Color(100, 10, 90);




void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// void loop() {
//   // put your main code here, to run repeatedly:
//   strip.fill(color, 0, 12);
//   strip.show();

// }

void loop() {
 // strip.fill(color);
  //strip.show();
   rainbow(100);
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=START_IDX; i<START_IDX + NUM_LED; i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}