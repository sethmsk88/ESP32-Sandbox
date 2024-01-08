#include <Arduino.h>
#include <FastLED.h>
#include "OTA.h"

#define DATA_PIN 9
#define BTN_PIN 10
#define NUM_LEDS 47

int ledState = LOW; // tracks the current state of LED
int lastButtonState; // the previous state of button
int currentButtonState; // the current state of button

CRGB leds[NUM_LEDS];
CRGB color;

void setup() {
  Serial.begin(115200);

  setupOTA();

  pinMode(BTN_PIN, INPUT);  // Set button pin to input mode

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 800);
  FastLED.setBrightness(20);

  color = CRGB::DarkRed;

  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();

  currentButtonState = digitalRead(BTN_PIN);
}

void loop() {
  ElegantOTA.loop();

  lastButtonState    = currentButtonState;                // save the last state
  currentButtonState = digitalRead(BTN_PIN); // read new state
  
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.print("The button is pressed: ");
  
    // toggle state of LED
    if(ledState == LOW) {
      ledState = HIGH;
      fill_solid(leds, NUM_LEDS, color);
      Serial.println("Turning LED on");
    }
    else {
      ledState = LOW;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      Serial.println("Turning LED off");
    }

    FastLED.show();
  }
}

