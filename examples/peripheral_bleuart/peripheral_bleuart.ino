#include "PeripheralBLE.h"
PeripheralBLE SerialBLE;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) yield(); //Blocking wait for connection when debug mode is enabled
  Serial.println("Peripheral BLEUART");
  SerialBLE.begin("AirMeter");
}

void loop()
{
  /*while (Serial.available()) //Forward data from HW Serial to BLEUART
    {
    delay(2); //Delay to wait for enough input, since we have a limited transmission buffer
    uint8_t buf[64];
    int count = Serial.readBytes(buf, sizeof(buf));
    SerialBLE.write( buf, count );
    }
    while ( SerialBLE.available() ) //Forward from BLEUART to HW Serial
    {
    uint8_t ch;
    ch = (uint8_t) SerialBLE.read();
    Serial.write(ch);
    }*/
  if (SerialBLE.isOpen())
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  String str = "RPM:";
  str += String(rand());
  str += " PWR:";
  str += String(128);
  //SerialBLE.write(str.c_str());
  SerialBLE.write("Hello from Air");
  delay(1000);
}
