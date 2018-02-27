#include "ScriptHandler.h"
#include "Debug.h"

ScriptHandler::ScriptHandler(void) {}

void ScriptHandler::SetKeyboard(Keyboard_ *keyboard) {
  this->ourKeyboard = keyboard;
}

void ScriptHandler::SetKeypressDelay(unsigned long delay_ms) {
  this->keypressDelay = delay_ms;
}

inline void ScriptHandler::WriteCharacter(uint8_t c) {
  ourKeyboard->press(c);
  delay(keypressDelay);
  ourKeyboard->release(c);
}

void ScriptHandler::WriteString(String s) {
  for (unsigned int i = 0; i < s.length(); i++) {
    WriteCharacter(s.charAt(i));
  }
}

// Simple function to output the content of a file in the sdcard as escape chars
// For example, in linux you can use ' echo -n -e "\xAA\xBB\XCC" > /tmp/file '
void ScriptHandler::EchoFileHex(String sdFileName) {
  return;
  /*
DEBUG_PRINT("echoFileHex: Opening file '" + sdFileName + "'");
fromSD = SD.open(sdFileName);
if (!fromSD)
{
  DEBUG_PRINT("echoFileHex: Couldn't find file: '" + sdFileName + "'");
  return;
}
else
{
  while (fromSD.available())
  {
    int c = fromSD.read();
    String hex = String(c, HEX);
    if (hex.length() > 1)
    {
      WriteCharacterString("\\x" + hex);
    }
    else
    {
      WriteCharacterString("\\x0" + hex);
    }
  }
  fromSD.close();
}
*/
}

void ScriptHandler::RepeatLastLine(int times) {
  // Won't repeat a REPEAT
  if (lastLine.startsWith("REPEAT ")) {
    return;
  }
  for (int i = 0; i < times; i++) {
    ParseLine(lastLine);
  }
}

void ScriptHandler::ParseLine(String line) {
  if (line.startsWith("REM "))
    return;
  else if (line.startsWith("DEFAULTDELAY "))
    commandDelay = line.substring(12).toInt();
  else if (line.startsWith("DEFAULT_DELAY "))
    commandDelay = line.substring(13).toInt();
  else if (line.startsWith("DEFAULTCHARDELAY "))
    keypressDelay = line.substring(16).toInt();
  else if (line.startsWith("DEFAULT_CHAR_DELAY "))
    keypressDelay = line.substring(18).toInt();
  else if (line.startsWith("DELAY "))
    delay(line.substring(6).toInt());
  else if (line.startsWith("ECHOFILEHEX "))
    EchoFileHex(line.substring(12));
  else if (line.startsWith("STRING "))
    WriteString(line.substring(7));
  else if (line.startsWith("REPEAT "))
    RepeatLastLine(line.substring(7).toInt());
  else
    ParseKeys(line);
  ourKeyboard->releaseAll();
  delay(commandDelay);
  // preserve the last line for potential repeats
  lastLine = line;
}

void ScriptHandler::ParseKeys(String keys) {
  String key;
  String nextKeys = keys;
  int p;

  DEBUG_PRINT("ParseKeys: '" + nextKeys + "'");

  nextKeys.trim();
  p = nextKeys.indexOf(" ");
  while (p > 0) {
    key = nextKeys.substring(0, p);
    nextKeys = nextKeys.substring(p + 1);
    key.trim();
    DEBUG_PRINT("Key: '" + key + "' Next:'" + nextKeys + "' p:" + String(p));
    ParseKey(key);
    p = nextKeys.indexOf(" ");
  }
  nextKeys.trim();
  ParseKey(nextKeys);
}

void ScriptHandler::ParseKey(String key) {
  DEBUG_PRINT("ParseKey: '" + key + "'");

  if (key.length() == 1)
    ourKeyboard->press(key.charAt(0));
  else if (key.equals("ENTER"))
    ourKeyboard->press(KEY_RETURN);
  else if (key.equals("GUI") || key.equals("WINDOWS"))
    ourKeyboard->press(KEY_LEFT_GUI);
  else if (key.equals("SHIFT"))
    ourKeyboard->press(KEY_LEFT_SHIFT);
  else if (key.equals("ALT"))
    ourKeyboard->press(KEY_LEFT_ALT);
  else if (key.equals("CTRL") || key.equals("CONTROL"))
    ourKeyboard->press(KEY_LEFT_CTRL);
  else if (key.equals("CAPSLOCK"))
    ourKeyboard->press(KEY_CAPS_LOCK);
  else if (key.equals("DELETE"))
    ourKeyboard->press(KEY_DELETE);
  else if (key.equals("END"))
    ourKeyboard->press(KEY_END);
  else if (key.equals("ESC") || key.equals("ESCAPE"))
    ourKeyboard->press(KEY_ESC);
  else if (key.equals("HOME"))
    ourKeyboard->press(KEY_HOME);
  else if (key.equals("INSERT"))
    ourKeyboard->press(KEY_INSERT);
  else if (key.equals("PAGEUP"))
    ourKeyboard->press(KEY_PAGE_UP);
  else if (key.equals("PAGEDOWN"))
    ourKeyboard->press(KEY_PAGE_DOWN);
  else if (key.equals("SPACE"))
    ourKeyboard->press(' ');
  else if (key.equals("TAB"))
    ourKeyboard->press(KEY_TAB);
  else if (key.equals("UP") || key.equals("UPARROW"))
    ourKeyboard->press(KEY_UP_ARROW);
  else if (key.equals("DOWN") || key.equals("DOWNARROW"))
    ourKeyboard->press(KEY_DOWN_ARROW);
  else if (key.equals("LEFT") || key.equals("LEFTARROW"))
    ourKeyboard->press(KEY_LEFT_ARROW);
  else if (key.equals("RIGHT") || key.equals("RIGHTARROW"))
    ourKeyboard->press(KEY_RIGHT_ARROW);
  else if (key.equals("PRINTSCREEN"))
    ourKeyboard->press(PRINTSCREEN);
  else if (key.equals("F1"))
    ourKeyboard->press(KEY_F1);
  else if (key.equals("F2"))
    ourKeyboard->press(KEY_F2);
  else if (key.equals("F3"))
    ourKeyboard->press(KEY_F3);
  else if (key.equals("F4"))
    ourKeyboard->press(KEY_F4);
  else if (key.equals("F5"))
    ourKeyboard->press(KEY_F5);
  else if (key.equals("F6"))
    ourKeyboard->press(KEY_F6);
  else if (key.equals("F7"))
    ourKeyboard->press(KEY_F7);
  else if (key.equals("F8"))
    ourKeyboard->press(KEY_F8);
  else if (key.equals("F9"))
    ourKeyboard->press(KEY_F9);
  else if (key.equals("F10"))
    ourKeyboard->press(KEY_F10);
  else if (key.equals("F11"))
    ourKeyboard->press(KEY_F11);
  else if (key.equals("F12"))
    ourKeyboard->press(KEY_F12);
  else if (key.equals("NUM_0"))
    WriteCharacter(KEYPAD_0);
  else if (key.equals("NUM_1"))
    WriteCharacter(KEYPAD_1);
  else if (key.equals("NUM_2"))
    WriteCharacter(KEYPAD_2);
  else if (key.equals("NUM_3"))
    WriteCharacter(KEYPAD_3);
  else if (key.equals("NUM_4"))
    WriteCharacter(KEYPAD_4);
  else if (key.equals("NUM_5"))
    WriteCharacter(KEYPAD_5);
  else if (key.equals("NUM_6"))
    WriteCharacter(KEYPAD_6);
  else if (key.equals("NUM_7"))
    WriteCharacter(KEYPAD_7);
  else if (key.equals("NUM_8"))
    WriteCharacter(KEYPAD_8);
  else if (key.equals("NUM_9"))
    WriteCharacter(KEYPAD_9);
  else if (key.equals("NUM_ASTERIX"))
    WriteCharacter(KEYPAD_ASTERIX);
  else if (key.equals("NUM_ENTER"))
    WriteCharacter(KEYPAD_ENTER);
  else if (key.equals("NUM_Minus"))
    WriteCharacter(KEYPAD_MINUS);
  else if (key.equals("NUM_PERIOD"))
    WriteCharacter(KEYPAD_PERIOD);
  else if (key.equals("NUM_PLUS"))
    WriteCharacter(KEYPAD_PLUS);
  else
    DEBUG_PRINT("ParseKey: failed");
}