#ifndef MY_DEBUG_H
#define MY_DEBUG_H

#define DEBUG true  // <-- uncomment to turn serial output on
#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif

#endif