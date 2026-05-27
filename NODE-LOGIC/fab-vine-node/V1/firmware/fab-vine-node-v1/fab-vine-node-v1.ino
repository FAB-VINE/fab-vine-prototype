// Fab Vine Node V1
// OLED expression test firmware for the unified Fab Vine node.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_ADDRESS 0x3C
#define SDA_PIN D4
#define SCL_PIN D5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// =====================================
// Expressions
// =====================================

enum Face {
  NORMAL,
  BLINK,
  WINK,
  SURPRISED,
  LOOK_LEFT,
  LOOK_RIGHT,
  YES_FACE,
  NO_FACE
};

Face currentFace = NORMAL;

unsigned long lastChange = 0;
unsigned long faceStart = 0;
unsigned long duration = 1600;

bool yesState = false;
bool noState = false;

// =====================================
// Setup
// =====================================

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  currentFace = NORMAL;
  faceStart = millis();
  lastChange = millis();
  drawFace();
}

// =====================================
// Loop
// =====================================

void loop() {
  if (currentFace == YES_FACE) {
    updateYesFace();
    return;
  }

  if (currentFace == NO_FACE) {
    updateNoFace();
    return;
  }

  if (millis() - lastChange > duration) {
    nextFace();
  }
}

// =====================================
// Face state control
// =====================================

void nextFace() {
  currentFace = (Face)((currentFace + 1) % 8);
  faceStart = millis();

  if (currentFace == YES_FACE) {
    yesState = false;
  }

  if (currentFace == NO_FACE) {
    noState = false;
  }

  if (currentFace == BLINK) {
    doubleBlink();
  } else {
    drawFace();
  }

  switch (currentFace) {
    case BLINK:
      duration = 1800;
      break;

    case WINK:
      duration = 700;
      break;

    case SURPRISED:
      duration = 1200;
      break;

    case YES_FACE:
    case NO_FACE:
      duration = 2500;
      break;

    default:
      duration = 1600;
      break;
  }

  lastChange = millis();
}

void triggerYes() {
  currentFace = YES_FACE;
  yesState = false;
  faceStart = millis();
  lastChange = millis();
  drawYesFace();
}

void triggerNo() {
  currentFace = NO_FACE;
  noState = false;
  faceStart = millis();
  lastChange = millis();
  drawNoFace();
}

void returnToNormal() {
  currentFace = NORMAL;
  faceStart = millis();
  lastChange = millis();
  duration = 1600;
  drawFace();
}

void updateYesFace() {
  if (millis() - lastChange > 220) {
    yesState = !yesState;
    drawYesFace();
    lastChange = millis();
  }

  if (millis() - faceStart > 2500) {
    returnToNormal();
  }
}

void updateNoFace() {
  if (millis() - lastChange > 220) {
    noState = !noState;
    drawNoFace();
    lastChange = millis();
  }

  if (millis() - faceStart > 2500) {
    returnToNormal();
  }
}

// =====================================
// Draw face
// =====================================

void drawFace() {
  display.clearDisplay();

  switch (currentFace) {
    case NORMAL:
      eyesNormal();
      break;

    case BLINK:
      eyesBlink();
      break;

    case WINK:
      eyesWink();
      break;

    case SURPRISED:
      eyesSurprised();
      break;

    case LOOK_LEFT:
      eyesLookLeft();
      break;

    case LOOK_RIGHT:
      eyesLookRight();
      break;

    case YES_FACE:
      drawYesFace();
      return;

    case NO_FACE:
      drawNoFace();
      return;
  }

  display.display();
}

// =====================================
// Double blink
// =====================================

void doubleBlink() {
  for (int i = 0; i < 2; i++) {
    display.clearDisplay();
    eyesBlink();
    display.display();
    delay(120);

    display.clearDisplay();
    eyesNormal();
    display.display();
    delay(120);
  }
}

// =====================================
// Eye base
// =====================================

void eye(int x, int y, int w, int h) {
  display.fillRoundRect(x, y, w, h, 8, SSD1306_WHITE);
}

// =====================================
// Pupils
// =====================================

void pupils(int x1, int y1, int x2, int y2) {
  display.fillCircle(x1, y1, 4, SSD1306_BLACK);
  display.fillCircle(x2, y2, 4, SSD1306_BLACK);
}

// =====================================
// Normal
// =====================================

void eyesNormal() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(34, 30, 94, 30);
}

// =====================================
// Blink
// =====================================

void eyesBlink() {
  eye(18, 28, 32, 5);
  eye(78, 28, 32, 5);
}

// =====================================
// Wink
// =====================================

void eyesWink() {
  eye(18, 28, 32, 5);
  eye(78, 18, 32, 24);
  display.fillCircle(94, 30, 4, SSD1306_BLACK);
}

// =====================================
// Surprised
// =====================================

void eyesSurprised() {
  eye(20, 12, 28, 38);
  eye(80, 12, 28, 38);
  pupils(34, 30, 94, 30);
}

// =====================================
// Look left
// =====================================

void eyesLookLeft() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(26, 30, 86, 30);
}

// =====================================
// Look right
// =====================================

void eyesLookRight() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(42, 30, 102, 30);
}

// =====================================
// Yes
// =====================================

void drawYesFace() {
  display.clearDisplay();
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);

  if (yesState) {
    pupils(34, 24, 94, 24);
  } else {
    pupils(34, 36, 94, 36);
  }

  display.display();
}

// =====================================
// No
// =====================================

void drawNoFace() {
  display.clearDisplay();
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);

  if (noState) {
    pupils(24, 30, 84, 30);
  } else {
    pupils(44, 30, 104, 30);
  }

  display.display();
}
