#include <Arduino.h>
#include "Adafruit_seesaw.h"

// todo need come back and orient the pins to the correct location
#define ON_INDICATOR_PIN 1
#define SM_DIRECTION_PIN 3
#define SM_PULSE_PIN 4
#define SM_ENABLE_PIN 5
#define HIGH_STOP_SWITCH_PING 6
#define LOW_STOP_SWITCH_PING 7
#define CTRL_STOP_BTN 8
#define CTRL_TOP_BTN 9
#define CTRL_BOTTOM_BTN 10
#define MICRO_STEP_SWITCH 24

#define SEESAW_ADDR 0x36
#define EXPECTED_SEESAW_VERSION 4991

Adafruit_seesaw ss;

boolean microStep = false;

// put function declarations here:
void setupSeesaw();

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  setupSeesaw();
}

void loop()
{
  if (!ss.digitalRead(MICRO_STEP_SWITCH))
  {
    microStep = !microStep;
    Serial.print("Micro step state changed: ");
    Serial.println(microStep);
  }

  int32_t new_position = ss.getEncoderPosition();
}

// put function definitions here:
void setupSeesaw()
{
  Serial.println("Looking for seesaw!");
  if (!ss.begin(SEESAW_ADDR))
  {
    Serial.println("Couldn't find seesaw on default address");
    while (1)
      delay(10);
  }

  Serial.println("seesaw started");

  uint32_t version = ((ss.getVersion() >> 16) & 0xFFFF);
  if (version != EXPECTED_SEESAW_VERSION)
  {
    Serial.print("Wrong firmware loaded? ");
    Serial.println(version);
    while (1)
      delay(10);
  }

  Serial.print("Found product ");
  Serial.println(EXPECTED_SEESAW_VERSION);

  ss.pinMode(MICRO_STEP_SWITCH, INPUT_PULLUP);
  Serial.println("Turning on interrupts");
  delay(10);
  ss.setGPIOInterrupts((uint32_t)1 << MICRO_STEP_SWITCH, 1);
  ss.enableEncoderInterrupt();
}