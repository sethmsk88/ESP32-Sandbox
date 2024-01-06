#include <Arduino.h>
#include "OTA.h"

#define LED_PIN 9
#define BTN_PIN 10

int ledState = LOW; // tracks the current state of LED
int lastButtonState; // the previous state of button
int currentButtonState; // the current state of button

void setup() {
  Serial.begin(115200);

  setupOTA();

  pinMode(BTN_PIN, INPUT);  // Set button pin to input mode
  pinMode(LED_PIN, OUTPUT); // Set LED pin to output mode

  currentButtonState = digitalRead(BTN_PIN);
}

void blinkLED() {
  delay(1000);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  ElegantOTA.loop();

  //blinkLED();

  lastButtonState    = currentButtonState;                // save the last state
  currentButtonState = digitalRead(BTN_PIN); // read new state
  
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.print("The button is pressed: ");
  
    // toggle state of LED
    if(ledState == LOW) {
       ledState = HIGH;  
       Serial.println("Turning LED on");
    }
    else {
      ledState = LOW;
      Serial.println("Turning LED off");
    }
 
    // control LED arccoding to the toggled state
    digitalWrite(LED_PIN, ledState);  //turns the LED on or off based on the variable

    
  }
  
}

