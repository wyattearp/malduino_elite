#ifndef SCRIPTHANDLER_H
#define SCRIPTHANDLER_H

#include "Keyboard.h"

class ScriptHandler {
 public:
  ScriptHandler(void);
  void SetKeyboard(Keyboard_ *keyboard);
  void SetKeypressDelay(unsigned long delay_ms);
  void SetCommandDelay(unsigned long delay_ms);
  inline void WriteCharacter(uint8_t c);
  void WriteString(String s);
  void EchoFileHex(String sdFileName);
  void RepeatLastLine(int times);
  void ParseLine(String line);
  void ParseKeys(String keys);
  inline void ParseKey(String key);

 private:
  Keyboard_ *ourKeyboard;
  unsigned long keypressDelay;
  unsigned long commandDelay;
  String lastLine;
};

#endif