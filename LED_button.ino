/****************************************************
 * HRONK Robotics Button Firmware - Version A
 *
 * Features:
 *  - Main menu (Timer Mode, Wack Mode)
 *  - 60s countdown timer displayed as big digits
 *      - 10–60: two digits (left=blue, right=red)
 *      - 0–9 : single centered digit (red)
 *  - Wack Mode:
 *      - Color = Red/Blue
 *      - Corner/edge button combos determine spread origin
 *      - Each press flips color (Red <-> Blue) with animation
 *  - Buzzer feedback (timer finish, color change)
 *  - OLED status displays for all modes
 *
 * Pin assignments:
 *  - CTRL_BTN (menu/select): GPIO 23
 *  - TL button: GPIO 27
 *  - BL button: GPIO 26
 *  - TR button: GPIO 33
 *  - BR button: GPIO 32
 *  - LED matrix DIN: GPIO 14
 *  - Buzzer: GPIO 25
 *  - OLED (software I2C): SCL=21, SDA=22
 ****************************************************/

#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include <Wire.h>

//////////////////// OLED (SW I2C) ////////////////////////////

U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0,
  21,   // SCL
  22,   // SDA
  U8X8_PIN_NONE
);

//////////////////// CONTROL BUTTON (MENU) ////////////////////////////

#define CTRL_BTN 23
const unsigned long HOLD_MS     = 600;   // hold to select mode
const unsigned long DOUBLE_MS   = 300;   // double-tap to exit
const unsigned long DEBOUNCE_MS = 30;

unsigned long ctrlDownTime    = 0;
unsigned long lastCtrlTapTime = 0;
unsigned long lastDebounce    = 0;
bool ctrlWasDown              = false;
bool ctrlHoldTriggered        = false;
bool needRedraw               = true;

//////////////////// MODES ////////////////////////////

enum ScreenMode {
  MENU,
  MODE_TIMER,
  MODE_WACK
};

ScreenMode screen = MENU;
int menuIndex = 0;   // 0=Timer, 1=Wack mode

//////////////////// LED MATRIX & BUZZER ////////////////////////////

#define LED_PIN     14
#define LED_COUNT   64
#define BUZZER_PIN  25

Adafruit_NeoPixel matrix(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Colors
uint32_t RED()   { return matrix.Color(255, 0, 0); }
uint32_t BLUE()  { return matrix.Color(0, 80, 255); }  // boosted blue
uint32_t OFF()   { return matrix.Color(0, 0, 0); }

void fillMatrixColor(uint32_t c) {
  for (int i = 0; i < LED_COUNT; i++)
    matrix.setPixelColor(i, c);
  matrix.show();
}

//////////////////// MATRIX MAPPING (SERPENTINE) ////////////////////////////
// Matches your physical panel wiring

void setMatrixXY(int x, int y, uint32_t color) {
  if (x < 0 || x > 7 || y < 0 || y > 7) return;

  int idx;
  if (y % 2 == 0) {
    // even row: reversed (right → left)
    idx = y * 8 + (7 - x);
  } else {
    // odd row: normal (left → right)
    idx = y * 8 + x;
  }

  matrix.setPixelColor(idx, color);
}

//////////////////// BUZZER TONES ////////////////////////////

void redUpTone() {
  tone(BUZZER_PIN, 700, 90);  delay(100);
  tone(BUZZER_PIN, 900, 90);  delay(100);
  tone(BUZZER_PIN, 1200,120); delay(130);
}

void blueDownTone() {
  tone(BUZZER_PIN, 1200, 90); delay(100);
  tone(BUZZER_PIN, 900, 90);  delay(100);
  tone(BUZZER_PIN, 700,120);  delay(130);
}

void resetTone() {
  tone(BUZZER_PIN, 1000,80); delay(100);
  tone(BUZZER_PIN, 600,120); delay(130);
}

void timerFinishEffect() {
  for (int i = 0; i < 3; i++) {
    fillMatrixColor(RED());
    tone(BUZZER_PIN, 1200, 150);
    delay(200);
    fillMatrixColor(OFF());
    delay(200);
  }
}

//////////////////// WACK MODE BUTTONS ////////////////////////////
// Corners mapped as you specified:
// TL = 27, BL = 26, TR = 33, BR = 32

#define PIN_TL 27
#define PIN_BL 26
#define PIN_TR 33
#define PIN_BR 32

// Wack color state: 0=off, 1=red, 2=blue
int wackColorState = 0;

// Debouncing for wack buttons (mask-based)
const unsigned long WACK_DEBOUNCE_MS = 40;
uint8_t wackLastRawMask      = 0;
uint8_t wackDebouncedMask    = 0;
unsigned long wackLastChange = 0;

// Read corners into a 4-bit mask: bit0=TL, bit1=TR, bit2=BL, bit3=BR
uint8_t readWackButtonMaskRaw() {
  bool tl = (digitalRead(PIN_TL) == LOW);
  bool tr = (digitalRead(PIN_TR) == LOW);
  bool bl = (digitalRead(PIN_BL) == LOW);
  bool br = (digitalRead(PIN_BR) == LOW);

  uint8_t m = 0;
  if (tl) m |= 0x01;  // TL
  if (tr) m |= 0x02;  // TR
  if (bl) m |= 0x04;  // BL
  if (br) m |= 0x08;  // BR
  return m;
}

// Spread animation (diamond wave) from origin, painting newColor
void spreadFrom(int ox, int oy, uint32_t newColor) {
  // We DO NOT clear first; we progressively overwrite with newColor
  for (int r = 0; r <= 14; r++) {
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        int dist = abs(x - ox) + abs(y - oy);
        if (dist <= r) {
          setMatrixXY(x, y, newColor);
        }
      }
    }
    matrix.show();
    delay(25);
  }
}

// Decide origin based on mask + paint next color
void handleWackSpread(uint8_t mask) {
  if (mask == 0) return; // no buttons pressed

  // Count how many pressed
  int count = 0;
  for (int i = 0; i < 4; i++) {
    if (mask & (1 << i)) count++;
  }

  int ox = 3;  // default origin = center
  int oy = 3;

  if (count == 1) {
    // Single corner
    if (mask & 0x01) { ox = 0; oy = 0; } // TL
    else if (mask & 0x02) { ox = 7; oy = 0; } // TR
    else if (mask & 0x04) { ox = 0; oy = 7; } // BL
    else if (mask & 0x08) { ox = 7; oy = 7; } // BR
  } else if (count == 2) {
    // Adjacent edges
    // TL + TR => top edge center
    if ((mask & 0x03) == 0x03) { ox = 3; oy = 0; }
    // BL + BR => bottom edge center
    else if ((mask & 0x0C) == 0x0C) { ox = 3; oy = 7; }
    // TL + BL => left edge center
    else if ((mask & 0x05) == 0x05) { ox = 0; oy = 3; }
    // TR + BR => right edge center
    else if ((mask & 0x0A) == 0x0A) { ox = 7; oy = 3; }
    else {
      // Diagonal pair => center
      ox = 3; oy = 3;
    }
  } else {
    // 3 or 4 buttons => center
    ox = 3; oy = 3;
  }

  // Decide next color (flip between red & blue)
  int newState;
  if (wackColorState == 0)      newState = 1; // start with red
  else                          newState = 3 - wackColorState; // 1<->2

  uint32_t newColor = (newState == 1) ? RED() : BLUE();

  // Run spread animation
  spreadFrom(ox, oy, newColor);

  // After animation, ensure entire matrix is that color
  fillMatrixColor(newColor);

  // Update global state + play tone
  wackColorState = newState;
  if (newState == 1) redUpTone();
  else               blueDownTone();
}

// Update Wack mode with debouncing
void updateWackMode() {
  uint8_t raw = readWackButtonMaskRaw();

  if (raw != wackLastRawMask) {
    wackLastRawMask = raw;
    wackLastChange  = millis();
  }

  if (millis() - wackLastChange >= WACK_DEBOUNCE_MS) {
    if (raw != wackDebouncedMask) {
      wackDebouncedMask = raw;

      // Only trigger on press combinations (ignore all-up)
      if (wackDebouncedMask != 0) {
        handleWackSpread(wackDebouncedMask);
      }
    }
  }
}

//////////////////// DIGIT BITMAP (4x7) FOR TIMER ////////////////////////////

const uint8_t digitBits[10][7][4] = {
  // 0
  {{0,1,1,0},{1,0,0,1},{1,0,0,1},{1,0,0,1},{1,0,0,1},{1,0,0,1},{0,1,1,0}},
  // 1
  {{0,0,1,0},{0,1,1,0},{1,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0},{1,1,1,1}},
  // 2
  {{0,1,1,0},{0,0,0,1},{0,0,0,1},{0,1,1,0},{1,0,0,0},{1,0,0,0},{1,1,1,1}},
  // 3
  {{0,1,1,0},{0,0,0,1},{0,0,0,1},{0,1,1,0},{0,0,0,1},{0,0,0,1},{0,1,1,0}},
  // 4
  {{1,0,0,1},{1,0,0,1},{1,0,0,1},{0,1,1,0},{0,0,0,1},{0,0,0,1},{0,0,0,1}},
  // 5
  {{1,1,1,1},{1,0,0,0},{1,0,0,0},{0,1,1,0},{0,0,0,1},{0,0,0,1},{1,1,1,0}},
  // 6
  {{0,1,1,0},{1,0,0,0},{1,0,0,0},{1,1,1,0},{1,0,0,1},{1,0,0,1},{0,1,1,0}},
  // 7
  {{1,1,1,1},{0,0,0,1},{0,0,1,0},{0,0,1,0},{0,1,0,0},{0,1,0,0},{1,0,0,0}},
  // 8
  {{0,1,1,0},{1,0,0,1},{1,0,0,1},{0,1,1,0},{1,0,0,1},{1,0,0,1},{0,1,1,0}},
  // 9
  {{0,1,1,0},{1,0,0,1},{1,0,0,1},{0,1,1,0},{0,0,0,1},{1,0,0,1},{0,1,1,0}}
};

// Draw a single digit at xOffset (0–3 for left, 4–7 for right, or 2–5 for centered)
void drawDigit(int digit, int xOffset, uint32_t color) {
  if (digit < 0 || digit > 9) return;

  for (int y = 0; y < 7; y++) {
    for (int x = 0; x < 4; x++) {
      if (digitBits[digit][y][x]) {
        setMatrixXY(x + xOffset, y + 1, color); // shift down one row
      }
    }
  }
}

// Draw the entire 2-digit countdown value
void drawCountdownDigits(int value) {
  if (value < 0) value = 0;
  if (value > 99) value = 99;

  // Clear matrix first
  fillMatrixColor(OFF());

  if (value >= 10) {
    int tens = value / 10;
    int ones = value % 10;
    drawDigit(tens, 0, BLUE());  // left digit (blue)
    drawDigit(ones, 4, RED());   // right digit (red)
  } else {
    drawDigit(value, 2, RED());  // centered single digit
  }

  matrix.show();
}

//////////////////// 60s TIMER LOGIC ////////////////////////////

bool timerActive      = false;
unsigned long nextTickMS = 0;
int timerRemaining    = 60;

void enterTimerMode() {
  screen = MODE_TIMER;
  timerActive     = true;
  timerRemaining  = 60;
  nextTickMS      = millis() + 1000;

  drawCountdownDigits(timerRemaining);
  needRedraw = true;
}

void updateTimerMode() {
  if (!timerActive) return;

  unsigned long now = millis();
  if (now >= nextTickMS) {
    nextTickMS += 1000;
    timerRemaining--;

    drawCountdownDigits(timerRemaining);
    needRedraw = true;

    if (timerRemaining <= 0) {
      timerActive = false;
      timerFinishEffect();
      fillMatrixColor(OFF());
      screen = MENU;
      needRedraw = true;
    }
  }
}

//////////////////// OLED SCREENS ////////////////////////////

void drawMenu() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  u8g2.drawStr(0,10,"HRONK Robotics");
  u8g2.drawStr(0,30, menuIndex==0 ? "> 60s LED Timer" : "  60s LED Timer");
  u8g2.drawStr(0,45, menuIndex==1 ? "> Wack-a-Mole"    : "  Wack-a-Mole");

  u8g2.sendBuffer();
}

void drawTimerScreen() {
  char buf[20];
  int remaining = timerRemaining;
  if (remaining < 0) remaining = 0;
  sprintf(buf,"Remaining: %2d", remaining);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0,10,"HRONK Robotics");
  u8g2.drawStr(0,25,"Digit LED Timer");
  u8g2.drawStr(0,40,buf);
  u8g2.drawStr(0,55,"Double-tap CTRL to exit");
  u8g2.sendBuffer();
}

void drawWackScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  u8g2.drawStr(0,10,"HRONK Robotics");
  u8g2.drawStr(0,25,"Blue vs Red");
  u8g2.drawStr(0,40,"Hit button to start!");
  u8g2.drawStr(0,55,"Double-tap CTRL exit");

  u8g2.sendBuffer();
}

//////////////////// CONTROL BUTTON (MENU / MODES) ////////////////////////////

void handleControlButton() {
  bool raw = digitalRead(CTRL_BTN);

  if (millis() - lastDebounce < DEBOUNCE_MS) return;
  lastDebounce = millis();

  bool pressed = (raw == LOW);

  if (pressed && !ctrlWasDown) {
    ctrlDownTime = millis();
    ctrlHoldTriggered = false;
    ctrlWasDown = true;
  }

  // HOLD → select current menu mode
  if (pressed && ctrlWasDown && !ctrlHoldTriggered &&
      millis() - ctrlDownTime >= HOLD_MS) {

    ctrlHoldTriggered = true;

    if (screen == MENU) {
      if (menuIndex == 0) {
        enterTimerMode();
      } else {
        screen = MODE_WACK;
        wackColorState = 0;
        fillMatrixColor(OFF());
        needRedraw = true;
      }
    }
  }

  // RELEASE
  if (!pressed && ctrlWasDown) {
    unsigned long dur = millis() - ctrlDownTime;

    if (dur < HOLD_MS && !ctrlHoldTriggered) {
      // TAP
      if (screen == MENU) {
        menuIndex = (menuIndex + 1) % 2;
        needRedraw = true;
        lastCtrlTapTime = millis();
      } else {
        // check for double-tap exit
        if (millis() - lastCtrlTapTime < DOUBLE_MS) {
          screen = MENU;
          fillMatrixColor(OFF());
          timerActive    = false;
          wackColorState = 0;
          needRedraw     = true;
        }
        lastCtrlTapTime = millis();
      }
    }

    ctrlWasDown = false;
  }
}

//////////////////// SETUP ////////////////////////////

void setup() {
  Serial.begin(115200);

  pinMode(CTRL_BTN, INPUT_PULLUP);

  pinMode(PIN_TL, INPUT_PULLUP);
  pinMode(PIN_TR, INPUT_PULLUP);
  pinMode(PIN_BL, INPUT_PULLUP);
  pinMode(PIN_BR, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);

  matrix.begin();
  matrix.setBrightness(64);
  fillMatrixColor(OFF());

  u8g2.begin();
  drawMenu();
  needRedraw = false;

  randomSeed(esp_random());
}

//////////////////// MAIN LOOP ////////////////////////////

void loop() {
  handleControlButton();

  if (screen == MODE_TIMER)
    updateTimerMode();
  else if (screen == MODE_WACK)
    updateWackMode();

  if (needRedraw) {
    needRedraw = false;

    if      (screen == MENU)       drawMenu();
    else if (screen == MODE_TIMER) drawTimerScreen();
    else if (screen == MODE_WACK)  drawWackScreen();
  }
}
