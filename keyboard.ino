#include <Keyboard.h>
#include <Adafruit_MCP23017.h>
#include <RingBuf.h>

#include "keymap.h"
#include "config.h"
#include "i2c.h"

Adafruit_MCP23017 mcp_right;
Adafruit_MCP23017 mcp_left;

// Create a RinBuf object designed to hold a 6 of keycodes
RingBuf *buf = RingBuf_new(sizeof(byte), 6);

volatile unsigned long time_prev = 0, time_now;
unsigned long time_chat = 1;

bool currentState[rowNum * 2][colNum];
bool beforeState[rowNum * 2][colNum];

int layer = 0;
int beforeLayer = 0;
bool layerHoldState = false;
bool layerChangeState = false;
bool layerShift = false;
 
void setup() {
  Serial.begin(9600);
  Keyboard.begin();

  // use default address 0
  mcp_right.begin(MCP_ADDR_RIGHT);
  mcp_left.begin(MCP_ADDR_LEFT);

  for (byte i = 0; i < rowNum; i++) {
    mcp_right.pinMode(rowPin[i], OUTPUT);
    mcp_left.pinMode(rowPin[i], OUTPUT);
  }

  for (byte i = 0; i < colNum; i++) {
    mcp_right.pinMode(colPin[i], INPUT);
    mcp_left.pinMode(colPin[i], INPUT);

    mcp_right.pullUp(colPin[i], HIGH);
    mcp_left.pullUp(colPin[i], HIGH);
  }

  for (byte i = 0; i < rowNum * 2; i++) {
    for (byte j = 0; j < colNum; j++) {
      currentState[i][j] = buttonOFF;
      beforeState[i][j] = buttonOFF;
    }
    mcp_right.digitalWrite(rowPin[i], buttonOFF);
    mcp_left.digitalWrite(rowPin[i], buttonOFF);
  }

  // use the p13 LED as debugging
  pinMode(ledPin, OUTPUT); 

  // check layer LED
  mcp_right.pinMode(layerLedPin[0], OUTPUT);
  mcp_left.pinMode(layerLedPin[1], OUTPUT);

  mcp_right.digitalWrite(layerLedPin[0], LOW);
  mcp_left.digitalWrite(layerLedPin[1], LOW);

  delay(200);
}

void loop() {
  for (byte i = 0; i < rowNum * 2; i++) {
    if (i < 4) {
      mcp_left.digitalWrite(rowPin[i], buttonON);
    } else {
      mcp_right.digitalWrite(rowPin[i - 4], buttonON);
    }

    for (byte j = 0; j < colNum; j++) {
      if (i < 4) {
        currentState[i][j] = mcp_left.digitalRead(colPin[j]);
      } else {
        currentState[i][j] = mcp_right.digitalRead(colPin[j]);
      }

      if (currentState[i][j] != beforeState[i][j]) {
        time_now = millis();

        // チャタリング対策
        if (time_now - time_prev > time_chat) {
          if (currentState[i][j] == buttonON) {
            printKeyMatrix(i, j);
            pressKey(keyMap[layer][i][j]);
          } else {
            if (!layerHoldState) printKeyMatrix(i, j);
            releaseKey(keyMap[layer][i][j]);
          }
          
          setLayerLed();
          beforeState[i][j] = currentState[i][j];
          time_prev = time_now;
        } else {
          Serial.println("chattering...");
        }
      }
    }

    if (i < 4) {
      mcp_left.digitalWrite(rowPin[i], buttonOFF);
    } else {
      mcp_right.digitalWrite(rowPin[i - 4], buttonOFF);
    }
  }
}

void pressKey(byte _keyCode) {
  byte bufKeyCode;

  digitalWrite(ledPin, HIGH);
  printKey(_keyCode, "Push!");

  switch (_keyCode) {
    case OSL:
      if (!layerHoldState) {
        beforeLayer = toggleLayer(layerShift);
        layerHoldState = true;
        layerChangeState = true;
      }
      break;
      
    case TGL:
      toggleLayer(layerShift);
      break;

    case KC_LSFT:
    case KC_RSFT:
      layerShift = true;
      Keyboard.press(_keyCode);
      break;

    case KC_LCTL:
    case KC_LALT:
    case KC_LGUI:
    case KC_RCTL:
    case KC_RALT:
    case KC_RGUI:
      Keyboard.press(_keyCode);
      break;

    default:
      if (buf->isFull(buf)) {
        Serial.print("buf isFull ");
        Serial.print(bufKeyCode);
        Serial.println(" Release...");
        buf->pull(buf, &bufKeyCode);
        Keyboard.release(bufKeyCode);
      }

      buf->add(buf, &_keyCode);
      Keyboard.press(_keyCode);

      if (layerHoldState) {
        layerHoldState = false;
        Serial.print("Release Hold Layer... ");
        Serial.println(OSL);
        releaseKey(OSL);
      }
  }
}

void releaseKey(byte _keyCode) {
  switch (_keyCode) {
    case KC_LSFT:
    case KC_RSFT:
      layerShift = false;
      break;
  }

  if (!layerHoldState) {
    digitalWrite(ledPin, LOW);

    switch (_keyCode) {
      case OSL:
        break;
      default:
        printKey(_keyCode, "Release!");
        Serial.println("");
    }

    buf->pull(buf, NULL);
    Keyboard.release(_keyCode);

    // レイヤー変更でリリースターゲットキーがずれる場合の対処
    if (layerChangeState && _keyCode != OSL) {
      layerChangeState = false;
      layer = beforeLayer;

      Keyboard.releaseAll(); 
    }
  }
}

byte toggleLayer(bool _layerShift) {
  // レイヤーキーの同時押しは許可しない
  Keyboard.releaseAll();

  int _beforeLayer = _layerShift ? layer-- : layer++;
  if (layer >= layerNum) {
    layer = 0;
  } else if (layer < 0) {
    layer = layerNum - 1;
  }

  Serial.print("Change Layer: ");
  Serial.print(_beforeLayer);
  Serial.print(" => ");
  Serial.println(layer);

  return _beforeLayer;
}

void setLayerLed() {
  byte layerLed[layerNum - 1];

  switch (layer) {
    case 0:
      layerLed[0] = LOW;
      layerLed[1] = LOW;
      break;
    case 1:
      layerLed[0] = HIGH;
      layerLed[1] = LOW;
      break;
    case 2:
      layerLed[0] = HIGH;
      layerLed[1] = HIGH;
      break;
  }

  mcp_right.digitalWrite(layerLedPin[0], layerLed[0]);
  mcp_left.digitalWrite(layerLedPin[1], layerLed[1]);
}

void printKey(byte _keyCode, String _msg) {
  Serial.print(_keyCode);
  Serial.print(" ");
  Serial.println(_msg);
}

void printKeyMatrix(byte _i, byte _j) {
  Serial.print("key(");
  Serial.print(layer);
  Serial.print(",");
  Serial.print(_i);
  Serial.print(",");
  Serial.print(_j);
  Serial.print(") ");
}
