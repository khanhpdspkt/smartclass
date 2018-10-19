#include "Ticker.h"

#define GPIO_LED  2

/* Ticker for blinking led */
Ticker blinkTicker;

void blinkLed(void) {
  Serial.println("Ticker occurs");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Ticker ESP32 example ");
  pinMode(GPIO_LED, OUTPUT);

  /* Start calling of blinkLed function every 10 seconds */
  blinkTicker.attach(10, blinkLed);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(GPIO_LED, LOW);
  delay(200);
  digitalWrite(GPIO_LED, HIGH);
  delay(200);
}
