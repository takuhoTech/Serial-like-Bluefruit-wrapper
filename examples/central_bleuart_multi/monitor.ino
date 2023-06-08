#include <Adafruit_TinyUSB.h>
#include <Arduino_GFX_Library.h>
#include "StringSplitter.h"
#include "CentralBLE.h"
CentralBLE SerialBLE;

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_DataBus *bus = new Arduino_HWSPI(2 /* DC */, 3 /* CS */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 1 /* RST */, 0 /* rotation */, true /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
   End of Arduino_GFX setting
 ******************************************************************************/

/*int split(String data, char delimiter, String *dst) {
  int index = 0;
  int arraySize = (sizeof(data)) / sizeof((data[0]));
  int datalength = data.length();

  for (int i = 0; i < datalength; i++) {
    char tmp = data.charAt(i);
    if ( tmp == delimiter ) {
      index++;
      if ( index > (arraySize - 1)) return -1;
    }
    else dst[index] += tmp;
  }
  return (index + 1);
  }*/

/*void split(String str, double value[], char delim) {
  int k = 0;
  int j = 0;
  char text[8];

  for (int i = 0; i <= str.length(); i++) {
    char c = str.charAt(i);
    if ( c == delim || i == str.length() ) {
      text[k] = '\0';
      value[j] = atof(text);
      j++;
      k = 0;
    } else {
      text[k] = c;
      k++;
    }
  }
  }*/

void setup(void)
{
  //Serial.begin(115200);
  //while (!Serial); yield();
  gfx->begin();
  gfx->fillScreen(BLACK);
#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  SerialBLE.addPrphName("AirMeter");   //DEVICE_NUM = 0
  SerialBLE.addPrphName("PowerMeter"); //DEVICE_NUM = 1 //To add more than 2 peripherals, config MAX_PERIPHERAL definition in header file.
  SerialBLE.begin("Central BLE");
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
}
#define AIRMETER 0
#define POWERMETER 1 //The numbers defined here are the order addPrphName executed. this will make it useful!

void loop()
{
  typedef struct
  {
    double current = 0;
    double last = 0;
  } BAT;
  BAT AirMeterBat;
  BAT PowerMeterBat;

  typedef struct
  {
    double current = 0;
    double last = 0;
  } Flow;
  Flow AirSpeed;

  typedef struct
  {
    int current = 0;
    int last = 0;
  } Cadence;
  Cadence cadence;
  typedef struct
  {
    int current = 0;
    int last = 0;
  } Power;
  Power power;

  //gfx->drawLine(45, 120, 195, 120, WHITE);
  gfx->drawLine(0, 120, 240, 120, WHITE);
  /*gfx->setCursor(45, 104);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(2, 2, 0);
    gfx->println("90");*/

  gfx->setCursor(92, 40 + 47);
  gfx->setTextSize(3, 3, 0);
  gfx->println("RPM");
  gfx->setCursor(110, 135 + 63);
  gfx->println("W");

  delay(100);

  while (1) {
    if (SerialBLE.isOpen(AIRMETER)) {
      digitalWrite(LED_RED, HIGH);
      String tmp = SerialBLE.readStringUntil(getPeripheralID(AIRMETER), ',');
      StringSplitter *AirData = new StringSplitter(tmp, ' ', 2);
      AirSpeed.current = AirData->getItemAtIndex(0).toFloat();
      AirMeterBat.current = AirData->getItemAtIndex(1).toFloat();
    }
    else
    {
      digitalWrite(LED_RED, LOW);
    }
    if (SerialBLE.isOpen(POWERMETER)) {
      digitalWrite(LED_BLUE, HIGH);
      String tmp = SerialBLE.readStringUntil(getPeripheralID(POWERMETER), ',');
      StringSplitter *PowerData = new StringSplitter(tmp, ' ', 4);
      cadence.current = PowerData->getItemAtIndex(0).toInt();
      power.current = PowerData->getItemAtIndex(1).toInt();
      PowerMeterBat.current = PowerData->getItemAtIndex(3).toFloat();
    }
    else
    {
      digitalWrite(LED_BLUE, LOW);
    }
    gfx->setCursor(162, 98);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(1, 1, 0);
    gfx->println("Air:");
    gfx->setCursor(200, 98);
    gfx->setTextColor(BLACK);
    gfx->println(AirMeterBat.last, 2);
    gfx->setCursor(200, 98);
    gfx->setTextColor(WHITE);
    gfx->println(AirMeterBat.current, 2);
    gfx->setCursor(172 + 54, 98);
    gfx->println("V");

    gfx->setCursor(162, 108);
    gfx->setTextColor(WHITE);
    gfx->setTextSize(1, 1, 0);
    gfx->println("Power:");
    gfx->setCursor(200, 108);
    gfx->setTextColor(BLACK);
    gfx->println(PowerMeterBat.last, 2);
    gfx->setCursor(200, 108);
    gfx->setTextColor(WHITE);
    gfx->println(PowerMeterBat.current, 2);
    gfx->setCursor(172 + 54, 108);
    gfx->println("V");

    gfx->setCursor(7, 98);
    gfx->setTextSize(2, 2, 0);
    gfx->setTextColor(BLACK);
    gfx->println(AirSpeed.last, 2);
    gfx->setCursor(7, 98);
    gfx->setTextColor(WHITE);
    gfx->println(AirSpeed.current, 2);
    gfx->setCursor(56, 108);
    gfx->setTextSize(1, 1, 0);
    gfx->println("MPS");

    if (cadence.last >= 100) {
      gfx->setCursor(50, 25);
    }
    else {
      gfx->setCursor(74, 25);
    }
    gfx->setTextColor(BLACK);
    gfx->setTextSize(8, 8, 0);
    gfx->println(cadence.last);
    if (cadence.current >= 100) {
      gfx->setCursor(50, 25);
    }
    else {
      gfx->setCursor(74, 25);
    }
    if (cadence.current < 100) {
      gfx->setTextColor(WHITE);
    }
    else {
      gfx->setTextColor(RED);
    }
    gfx->println(cadence.current);

    gfx->setCursor(92, 40 + 47);
    gfx->setTextSize(3, 3, 0);
    gfx->println("RPM");

    if (power.last >= 100) {
      gfx->setCursor(50, 135);
    }
    else {
      gfx->setCursor(74, 135);
    }
    gfx->setTextColor(BLACK);
    gfx->setTextSize(8, 8, 0);
    gfx->println(power.last);

    if (power.current >= 100) {
      gfx->setCursor(50, 135);
    }
    else {
      gfx->setCursor(74, 135);
    }
    if (power.current < 300) {
      gfx->setTextColor(WHITE);
    }
    else {
      gfx->setTextColor(RED);
    }
    gfx->println(power.current);

    gfx->setCursor(110, 135 + 63);
    gfx->setTextSize(3, 3, 0);
    gfx->println("W");

    cadence.last = cadence.current;
    power.last = power.current;
    AirSpeed.last = AirSpeed.current;

    AirMeterBat.last = AirMeterBat.current;
    PowerMeterBat.last = PowerMeterBat.current;

    delay(100);
  }
}
