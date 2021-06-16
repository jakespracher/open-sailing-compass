/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED 7-Segment backpacks 
  ----> http://www.adafruit.com/products/881
  ----> http://www.adafruit.com/products/880
  ----> http://www.adafruit.com/products/879
  ----> http://www.adafruit.com/products/878

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>

Adafruit_7segment matrix = Adafruit_7segment();

/* Assign a unique ID to this sensor at the same time */
Adafruit_LIS3MDL lis3mdl;
#define LIS3MDL_CLK 13
#define LIS3MDL_MISO 12
#define LIS3MDL_MOSI 11
#define LIS3MDL_CS 10
#define PI 3.14159265389

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  matrix.begin(0x70);
  matrix.setBrightness(3);

  Serial.println("**** Open Laser Compass ****");
  Serial.println("");

  /* Enable auto-gain */
  lis3mdl.enableAutoRange(true);

  /* Initialise the sensor */
  if (! lis3mdl.begin_I2C()) { 
    /* There was a problem detecting the LIS2MDL ... check your connections */
    Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!");
    while (1) delay(10);
  }

  /* Display some basic information on this sensor */
  lis3mdl.printSensorDetails();
}

void loop(void) {
  /* Get a new sensor event */
  sensors_event_t event;
  lis3mdl.getEvent(&event);

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: ");
  Serial.print(event.magnetic.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(event.magnetic.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(event.magnetic.z);
  Serial.print("  ");
  Serial.println("uT");
  Serial.print("  ");

  int heading = calculateHeading(event.magnetic.y, event.magnetic.x);
  Serial.print("  ");
  Serial.print("Heading: ");
  Serial.print(heading, DEC);
  matrix.print(heading, DEC);
  matrix.writeDisplay();

  /* Delay before the next sample */
  delay(200);
}

int calculateHeading(float magX, float magY) {
  if (magY > 0.0) {
    return 90 - atan(magX / magY) * 180.0 / PI;
  } else if (magY < 0.0) {
    return 270 - atan(magX / magY) * 180.0 / PI;
  } else if (magY == 0.0 && magX < 0.0) {
    return 180;
  } else if (magY == 0.0 && magX > 0.0) {
    return 0;
  }
}
