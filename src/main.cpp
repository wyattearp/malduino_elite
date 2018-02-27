/* 
 * Malduino Elite rewrite by kripthor
 * 
 * original work by Seytonic and Spacehuhn,  see malduino.com
 *
 * Check Keyboard.cpp for correct keyboard map and go to https://github.com/Nurrl/LocaleKeyboard.js for other maps 
 *
 */

#include <SPI.h>
#include <SD.h>
#include "Debug.h"
#include "Keyboard.h"
#include "DipSwitch.h"
#include "ScriptHandler.h"

#define CSPIN 4 //Chip-Select of the SD-Card reader
#define ledPin 3
#define blinkInterval 50
#define MULTI_SCRIPT_RUN_SEQ 0xF  // dip = 1111 means we'll run everything script in order until we hit an error

void searchAndRunScript();

File payload;
File fromSD;
DipSwitch dip;
String lastLine;
ScriptHandler sc;

int defaultDelay = 5;
int defaultCharDelay = 5;
int lastDip = -1;
bool ledOn = true;

void searchAndRunScript()
{
  String scriptName = dip.getDipsString() + ".txt";

  payload = SD.open(scriptName);
  if (!payload)
  {
    DEBUG_PRINT("setup: not found '" + scriptName + "'");
    return;
  }
  else
  {
    DEBUG_PRINT("setup: found script '" + scriptName + "'");

    String line;
    Keyboard.begin();
    while (payload.available())
    {
      line = payload.readStringUntil('\n');
      line.trim();
      sc.ParseLine(line);
      lastLine = line;
    }
    payload.close();
    Keyboard.end();
  }
}

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

#ifdef DEBUG
  Serial.begin(115200);
  delay(3000);
  DEBUG_PRINT("DEBUG started, press any key to init.");
  while (!(Serial.available() > 0)) {
    delay(100);
  }
#endif

  if (!SD.begin(CSPIN))
  {
    DEBUG_PRINT("setup: couldn't access sd-card");
    return;
  }
}

void loop()
{
  //In the event that you can switch dips while connected to usb,
  //the malduino will execute the script of the new dip configuration
  if (lastDip != dip.getDips())
  {
    lastDip = dip.getDips();
    searchAndRunScript();
  }

  //Flash fast when script ends
  else
  {
    ledOn = !ledOn;
    digitalWrite(ledPin, ledOn);
    delay(blinkInterval);
  }
}
