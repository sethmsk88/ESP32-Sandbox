#include <Arduino.h>
#include <FastLED.h>
#include "OTA.h"

#include <Battery18650Stats.h>

#define DATA_PIN 9
#define BTN_PIN 10
#define NUM_LEDS 47

#define ADC_PIN 16

Battery18650Stats battery(ADC_PIN);

int ledState = LOW; // tracks the current state of LED
int lastButtonState; // the previous state of button
int currentButtonState; // the current state of button

CRGB leds[NUM_LEDS];
CRGB color;

void setup() {
  Serial.begin(115200);
  
  setupOTAUpdateAndSerialMonitor();

  pinMode(BTN_PIN, INPUT);  // Set button pin to input mode

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 800);
  FastLED.setBrightness(20);

  color = CRGB::DarkRed;

  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();

  currentButtonState = digitalRead(BTN_PIN);
}

void handleButton() {
  lastButtonState    = currentButtonState;                // save the last state
  currentButtonState = digitalRead(BTN_PIN); // read new state
  
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    WebSerial.print("The button is pressed: ");
  
    // toggle state of LED
    if(ledState == LOW) {
      ledState = HIGH;
      fill_solid(leds, NUM_LEDS, color);
      WebSerial.println("Turning LED on");
    }
    else {
      ledState = LOW;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      WebSerial.println("Turning LED off");
    }

    FastLED.show();
  }
}

void printBatteryStats() {
  WebSerial.print("Volts: ");
  WebSerial.println(battery.getBatteryVolts());

  WebSerial.print("Charge level: ");
  WebSerial.println(battery.getBatteryChargeLevel());

  WebSerial.print("Charge level (using the reference table): ");
  WebSerial.println(battery.getBatteryChargeLevel(true));
}

void loop() {
  ElegantOTA.loop();
  //webSerialLoop(); // Write some test info to serial monitor
  //handleButton();
  FastLED.show();

  EVERY_N_SECONDS(30) {
    //printBatteryStats();
    int sensorValue = analogRead(ADC_PIN);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    
    // To get the conversion factor, I took the measuredVoltage without the conversion factor,
    // and then divided it by the voltage of the battery as measured by a multimeter.
    float conversionFactor = 2.1328;

    float batteryVoltage = 3.7;
    float measuredVoltage = (sensorValue * (batteryVoltage / 1023.0)) / conversionFactor;
    // print out the value you read:
    WebSerial.println(measuredVoltage);
  } 
}







