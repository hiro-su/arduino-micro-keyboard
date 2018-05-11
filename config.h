#ifndef INCLUDED_CONFIG_H_

  #define INCLUDED_CONFIG_H_

  const byte buttonON = LOW;
  const byte buttonOFF = HIGH;

  const byte rowNum = 4;
  const byte colNum = 6;
  const byte layerNum = 3;
  
  const byte rowPin[rowNum] = { 8, 9, 10, 11 };
  const byte colPin[colNum] = { 2, 3, 4, 5, 6, 7 };
  
  const byte ledPin = 13;
  const byte layerLedPin[layerNum - 1] = { 0, 0 }; 
  
#endif
