#ifndef INCLUDED_KEYMAP_H_

  #define INCLUDED_KEYMAP_H_

  #include "keydefine.h"
  #include "config.h"

  const byte keyMap[layerNum][rowNum * 2][colNum] = {
    { // Layer0
      // left side
      { KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T },
      { KC_LSFT, KC_A, KC_S, KC_D, KC_F, KC_G },
      { KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B },
      { KC_GRV, KC_LALT, KC_DEL, OSL, KC_SPC, KC_LGUI },
      // right side
      { KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS },
      { KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_EQL },
      { KC_N, KC_M, KC_COMM, KC_DOT, KC_QUOT, KC_SLSH },
      { KC_BSPC, KC_ENT, KC_RGUI, TGL, KC_TAB, KC_BSLS },
    },
    { // Layer1
      // left side
      { KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5 },
      { KC_LSFT, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC },
      { KC_LCTL, KC_SLSH, KC_ASTR, KC_MINS, KC_PLUS, KC_EQL },
      { NONE, KC_LALT, KC_DEL, OSL, KC_SPC, KC_LGUI },
      // right side
      { KC_6, KC_7, KC_8, KC_9, KC_0, NONE },
      { KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_RSFT },
      { KC_DOT, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, KC_RCTL },
      { KC_BSPC, KC_ENT, KC_RGUI, TGL, KC_TAB, NONE },
    },
    { // Layer2
      // left side
      { KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6 },
      { KC_LSFT, NONE, KC_END, KC_PGUP, KC_PGDN, KC_HOME },
      { KC_LCTL, NONE, NONE, NONE, NONE, NONE },
      { KC_LBRC, KC_LALT, KC_DEL, OSL, KC_SPC, KC_LGUI },
      // right side
      { KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12 },
      { KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, NONE, KC_EQL },
      { NONE, NONE, NONE, NONE, NONE, KC_SLSH },
      { KC_BSPC, KC_ENT, KC_RGUI, TGL, KC_TAB, KC_BSLS },
    }
  };

#endif
