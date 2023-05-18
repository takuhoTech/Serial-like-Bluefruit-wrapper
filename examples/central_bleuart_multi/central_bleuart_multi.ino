#include "CentralBLE.h"
CentralBLE SerialBLE;



void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while ( !Serial ) yield(); // for nrf52840 with native usb
  Serial.println("Central Multi BLEUART");
  SerialBLE.addPrphName("AirMeter");   //DEVICE_NUM = 0
  SerialBLE.addPrphName("PowerMeter"); //DEVICE_NUM = 1 //To add more than 2 peripherals, config MAX_PERIPHERAL definition in header file.
  SerialBLE.begin("Central BLE");
}
#define AIRMETER 0
#define POWERMETER 1 //The numbers defined here are the order addPrphName executed. this will make it useful!

void loop()
{
  if (SerialBLE.isOpen(AIRMETER))
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  while (SerialBLE.available(getPeripheralID(AIRMETER)) > 0)
  {
    Serial.print("Air:");
    while (SerialBLE.available(getPeripheralID(AIRMETER)) > 0)
    {
      Serial.print(SerialBLE.read(getPeripheralID(AIRMETER)));
    }
    Serial.println("");
  }

  while (SerialBLE.available(getPeripheralID(POWERMETER)) > 0)
  {
    Serial.print("Power:");
    while (SerialBLE.available(getPeripheralID(POWERMETER)) > 0)
    {
      Serial.print(SerialBLE.read(getPeripheralID(POWERMETER)));
    }
    Serial.println("");
  }
  //SerialBLE.read(getPeripheralID(AIRMETER));
  //SerialBLE.read(getPeripheralID(POWERMETER));
}
