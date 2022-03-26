#include <avr/pgmspace.h>
#include "src/libraries/FastLED/FastLED.h"
#include "src/bitmaps/digdug.h"
#include "src/bitmaps/frogger.h"
#include "src/bitmaps/galaga.h"
#include "src/bitmaps/pacman.h"
#include "src/bitmaps/smb.h"
#include "src/bitmaps/qbert.h"

#define DATA_PIN        4
#define BUTTON_PIN      3
#define BUTTONTWO_PIN   2
#define NUM_LEDS        256
#define BRIGHTNESS      16
#define LED_TYPE        WS2812B
#define COLOR_ORDER     GRB
#define COLS            16

const int MAX_SELECTION = 4;
const int MAX_SELECTION_TWO[] = { 0, 3, 3, 4 };
 
CRGB leds[NUM_LEDS];

volatile int selection = 0;
volatile int selectionTwo = 0;
long lastDebounceTime = 0; 
long lastDebounceTimeTwo = 0;  
long debounce = 100;         

void setup() {
  delay(3000); // power-up safety delay
  Serial.begin(9600);
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonToggled, RISING);
  pinMode(BUTTONTWO_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTONTWO_PIN), buttonTwoToggled, RISING);

  selection = 1;
  selectionTwo = 0;
  
  Serial.print("Setup Complete");
}

void drawBitmap(const long* bitmap) {
  FastLED.clear();
  bool LTR = true;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (LTR) {
      for (int j = 0; j < COLS; j++) {
        leds[i + j] = pgm_read_dword(&(bitmap[i + j]));
      }
    } else {
      for (int j = COLS; j > 0; j--) {
        leds[i + COLS - j] = pgm_read_dword(&(bitmap[i + j - 1]));
      }
    }

    i = i + COLS - 1;
    LTR = !LTR;
  }

  FastLED.show();
}

void drawAnimation(const long* bitmaps, int s, int delayTime) {
  for (int i = 0; i < s; i++) {
    drawBitmap(bitmaps[i]);
    delay(delayTime);
  }
}

void buttonToggled() {
  if (millis() - lastDebounceTime > debounce) {
    selectionTwo = 0;
    selection++;
    if (selection > MAX_SELECTION)  selection = 1;
    lastDebounceTime = millis();
  }
}

void buttonTwoToggled() {
  if (millis() - lastDebounceTimeTwo > debounce) {
    selectionTwo++;
    if (selectionTwo > MAX_SELECTION_TWO[selection])  selectionTwo = 0;
    lastDebounceTimeTwo = millis();
  }
}

void galagaLoop() {
  switch (selectionTwo) {
    case 1:
      drawAnimation(GalagaBoss, GalagaBossSize, 500);
      break;
    case 2:
      drawAnimation(GalagaGoei, GalagaGoeiSize, 500);
      break;
    case 3:
      drawAnimation(GalagaZako, GalagaZakoSize, 500);
      break;
    default:
      drawBitmap(GalagaShip);
      break;
  }
}

void digDugLoop() {
  switch (selectionTwo) {
    case 1:
      drawAnimation(DigDugTaizoShovel, DigDugTaizoShovelSize, 500);
      break;
    case 2:
      drawAnimation(DigDugPooka, DigDugPookaSize, 500);
      break;
    case 3:
      drawAnimation(DigDugFygar, DigDugFygarSize, 500);
      break;
    default:
      drawBitmap(DigDugTaizoShovel01);
      break;
  }
}

void miscLoop() {
  switch (selectionTwo) {
    case 1:
      drawAnimation(Frogger, FroggerSize, 500);
      break;
    case 2:
      drawAnimation(Qbert, QbertSize, 500);
      break;
    case 3:
      drawAnimation(PacManMsPac, PacManMsPacSize, 100);
      break;
    case 4:
      drawAnimation(PacManBlinky, PacManBlinkySize, 500);
      break;
    default:
      drawBitmap(SMBMario01);
      break;
  }
}

void loop() {
  switch (selection) {
    case 1:
      galagaLoop();
      break;
    case 2:
      digDugLoop();
      break;
    case 3:
      miscLoop();
      break;
  }
}
