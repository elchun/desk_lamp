#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 27
#define SIDE_LENGTH 9
#define MAX_PULSES 3

struct Pulse {
  int pixel;
  uint8_t r, g, b;
  float brightness;
  float step;
  bool fadingIn;
  bool active;
  unsigned long lastUpdate;
};


Pulse pulses[MAX_PULSES];

unsigned long lastPulseAdd = 0;



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
// uint32_t color = strip.Color(70, 100, 90);
// uint32_t color = strip.Color(5, 5, 5);
// uint32_t yellow = strip.Color(60, 60, 0);
uint32_t color = strip.Color(60, 10, 0);




void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  for (int i = 0; i < MAX_PULSES; i++) {
    pulses[i].active = false;
  }

}

void pulse(int wait){
  for (int row_idx = 0; row_idx < SIDE_LENGTH; row_idx++) {
    
    strip.clear();
    for (int pixel_multiplier = 0; pixel_multiplier < 3; pixel_multiplier++) {
      int pixel_idx;
      if (pixel_multiplier == 1) {
        pixel_idx = SIDE_LENGTH * (pixel_multiplier + 1) - 1 - row_idx;
      } else {
        pixel_idx = row_idx + SIDE_LENGTH * pixel_multiplier;
      }
      strip.setPixelColor(pixel_idx, color);
    }
    int sensorValue = analogRead(A0);

    if (sensorValue > 200) {
      strip.show();
      delay((sensorValue - 200) / 4);
    } else {
      strip.clear();
      strip.show();
      delay(10);
    }

  }
}

// void loop() {
//   // put your main code here, to run repeatedly:
//   strip.fill(color, 0, 12);
//   strip.show();

// }

void loop() {
//  strip.fill(color);
//   strip.show();
  // pulse(100);
  //  rainbow_basic(10);
   random_pulse();

  // // read the input on analog pin 0:
  // int sensorValue = analogRead(A0);
  // // print out the value you read:
  // Serial.println(sensorValue);
  // delay(1);  // delay in between reads for stability
}


void rainbow_basic(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { 
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    int sensorValue = analogRead(A0);

    if (sensorValue > 200) {
      strip.show();
      delay((sensorValue - 200) / 100);
    } else {
      strip.clear();
      strip.show();
      delay(10);
    }
  }
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for (int row_idx = 0; row_idx < SIDE_LENGTH; row_idx++) {
      for (int pixel_multiplier = 0; pixel_multiplier < 3; pixel_multiplier++) {
        int pixel_idx;
        if (pixel_multiplier == 1) {
          pixel_idx = SIDE_LENGTH * (pixel_multiplier + 1) - 1 - row_idx;
        } else {
          pixel_idx = row_idx + SIDE_LENGTH * pixel_multiplier;
        }
        int pixelHue = firstPixelHue + (row_idx * 65536L / SIDE_LENGTH);
        strip.setPixelColor(pixel_idx, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }
        int sensorValue = analogRead(A0);

    if (sensorValue > 200) {
      strip.show();
      delay((sensorValue - 200) / 100);
    } else {
      strip.clear();
      strip.show();
      delay(10);
    }
    // strip.show();
    // delay(wait);
  }
}

void random_pulse() {
  unsigned long currentTime = millis();

  // Read potentiometer value and map to fade speed
  int sensorValue = analogRead(A0);
  float speedFactor = map(sensorValue, 0, 1023, 5, 20) / 1000.0; // 0.001 - 0.02 per update

  // Possibly add a new pulse every 100ms
  if (currentTime - lastPulseAdd > 100) {
    lastPulseAdd = currentTime;

    if (random(0, 100) > 90) { // ~40% chance to add
      for (int i = 0; i < MAX_PULSES; i++) {
        if (!pulses[i].active) {
          pulses[i].pixel = random(LED_COUNT);
          pulses[i].r = random(50, 255);
          pulses[i].g = random(20, 100);
          pulses[i].b = random(20, 100);
          // pulses[i].r = random(20, 50);
          // pulses[i].g = random(5, 10);
          // pulses[i].b = random(5, 10);
          pulses[i].brightness = 0.0;
          pulses[i].step = speedFactor;
          pulses[i].fadingIn = true;
          pulses[i].active = true;
          pulses[i].lastUpdate = currentTime;
          break;
        }
      }
    }
  }

  // Clear LEDs before redrawing
  strip.clear();

  // Update all pulses
  for (int i = 0; i < MAX_PULSES; i++) {
    if (pulses[i].active) {
      if (currentTime - pulses[i].lastUpdate >= 20) { // update every 20ms
        pulses[i].lastUpdate = currentTime;

        // Update brightness
        if (pulses[i].fadingIn) {
          pulses[i].brightness += pulses[i].step;
          if (pulses[i].brightness >= 1.0) {
            pulses[i].brightness = 1.0;
            pulses[i].fadingIn = false;
          }
        } else {
          pulses[i].brightness -= pulses[i].step;
          if (pulses[i].brightness <= 0.0) {
            pulses[i].brightness = 0.0;
            pulses[i].active = false;
          }
        }
      }

      // Apply color
      float b = pulses[i].brightness;
      uint8_t r = (uint8_t)(pulses[i].r * b);
      uint8_t g = (uint8_t)(pulses[i].g * b);
      uint8_t bl = (uint8_t)(pulses[i].b * b);
      strip.setPixelColor(pulses[i].pixel, r, g, bl);
    }
  }

  strip.show();
}

