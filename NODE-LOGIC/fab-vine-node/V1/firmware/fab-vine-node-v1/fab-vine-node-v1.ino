// Fab Vine Node V1
// Offline MVP firmware for the unified Fab Vine node.
//
// MVP behavior:
// - Detect physical neighbor connections through six face DETECT lines.
// - Count how many faces currently have neighbors.
// - React locally with OLED expression and WS2812B / NeoPixel light.
// - Show slight social anxiety in purple when alone; remove red when connected.
// - Keep WiFi, MQTT, and microphone logic out of this first physical test.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <esp_system.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_ADDRESS 0x3C
#define OLED_SDA_PIN D4
#define OLED_SCL_PIN D5

#define LED_PIN D10
#define LED_COUNT 40
#define FACE_COUNT 6
#define LED_ROW_LENGTH 10
#define DATA_SIGNAL_PIN D8

const uint8_t facePins[FACE_COUNT] = { D0, D1, D2, D3, D6, D7 };

// Physical 1-based zig-zag positions. Position 1 maps to LEDs 1, 11, 21, 31.
// Edit this map when the final physical face-to-strip geometry changes.
const uint8_t faceLedPositions[FACE_COUNT] = { 1, 3, 5, 6, 8, 10 };

#define DETECT_DEBOUNCE_MS 40
#define LED_FRAME_MS 30
#define IDLE_BLINK_INTERVAL_MS 5200
#define CONNECTED_WINK_INTERVAL_MS 6500
#define GREETING_DURATION_MS 2500
#define REMOVED_DURATION_MS 1200
#define IDLE_ANXIETY_PERIOD_MS 1800
#define ELECTION_MIN_DELAY_MS 700
#define ELECTION_RANDOM_WINDOW_MS 3000
#define FOLLOWER_RECHECK_MS 200

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// =====================================
// State
// =====================================

enum Face {
  NORMAL,
  BLINK,
  WINK,
  SURPRISED,
  LOOK_LEFT,
  LOOK_RIGHT,
  YES_FACE,
  NO_FACE,
  LEADER_FACE
};

enum NodeState {
  NODE_IDLE,
  NEIGHBOR_CONNECTED,
  CONNECTED_IDLE,
  NEIGHBOR_REMOVED
};

enum ElectionState {
  NO_ELECTION,
  ELECTION_WAIT,
  LEADER,
  FOLLOWER
};

NodeState nodeState = NODE_IDLE;
ElectionState electionState = NO_ELECTION;
Face currentFace = NORMAL;

bool faceConnected[FACE_COUNT] = { false, false, false, false, false, false };
bool lastRawFaceConnected[FACE_COUNT] = { false, false, false, false, false, false };
bool lastStableFaceConnected[FACE_COUNT] = { false, false, false, false, false, false };
unsigned long lastFaceDebounce[FACE_COUNT] = { 0, 0, 0, 0, 0, 0 };

uint8_t neighborCount = 0;
uint8_t previousNeighborCount = 0;
bool anyNeighborConnected = false;

unsigned long lastFaceFrame = 0;
unsigned long lastIdleBlink = 0;
unsigned long lastConnectedWink = 0;
unsigned long lastLedFrame = 0;
unsigned long faceStart = 0;

bool yesState = false;
int noDirection = -1;

uint16_t localTicket = 0;
uint16_t remoteTicket = 0;
unsigned long electionStartAt = 0;
unsigned long lastFollowerRecheck = 0;

// =====================================
// Setup
// =====================================

void setup() {
  for (uint8_t faceIndex = 0; faceIndex < FACE_COUNT; faceIndex++) {
    pinMode(facePins[faceIndex], INPUT_PULLUP);
  }

  releaseDataSignal();

  pixels.begin();
  pixels.clear();
  pixels.show();

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  currentFace = NORMAL;
  faceStart = millis();
  lastFaceFrame = millis();
  lastIdleBlink = millis();
  lastConnectedWink = millis();
  drawFace();
}

// =====================================
// Loop
// =====================================

void loop() {
  updateFaceDetection();
  updateNodeState();
  updateLeaderElection();
  updateFacePattern();
  updateLedPattern();
}

// =====================================
// Face detection
// =====================================

void updateFaceDetection() {
  bool changedStableFace = false;

  for (uint8_t faceIndex = 0; faceIndex < FACE_COUNT; faceIndex++) {
    bool rawConnected = digitalRead(facePins[faceIndex]) == LOW;

    if (rawConnected != lastRawFaceConnected[faceIndex]) {
      lastRawFaceConnected[faceIndex] = rawConnected;
      lastFaceDebounce[faceIndex] = millis();
    }

    if (millis() - lastFaceDebounce[faceIndex] > DETECT_DEBOUNCE_MS && rawConnected != lastStableFaceConnected[faceIndex]) {
      lastStableFaceConnected[faceIndex] = rawConnected;
      faceConnected[faceIndex] = rawConnected;
      changedStableFace = true;
    }
  }

  if (changedStableFace) {
    updateNeighborCount();
  }
}

void updateNeighborCount() {
  previousNeighborCount = neighborCount;
  neighborCount = 0;

  for (uint8_t faceIndex = 0; faceIndex < FACE_COUNT; faceIndex++) {
    if (faceConnected[faceIndex]) {
      neighborCount++;
    }
  }

  anyNeighborConnected = neighborCount > 0;
}

// =====================================
// Node state control
// =====================================

void updateNodeState() {
  if (anyNeighborConnected && previousNeighborCount == 0 && neighborCount > 0) {
    enterNeighborConnected();
    previousNeighborCount = neighborCount;
  }

  if (!anyNeighborConnected && previousNeighborCount > 0 && neighborCount == 0) {
    enterNeighborRemoved();
    previousNeighborCount = neighborCount;
  }

  if (nodeState == NEIGHBOR_CONNECTED && millis() - faceStart > GREETING_DURATION_MS) {
    enterConnectedIdle();
  }

  if (nodeState == NEIGHBOR_REMOVED && millis() - faceStart > REMOVED_DURATION_MS) {
    enterIdle();
  }
}

void enterIdle() {
  nodeState = NODE_IDLE;
  resetLeaderElection();
  currentFace = NORMAL;
  faceStart = millis();
  lastFaceFrame = millis();
  lastIdleBlink = millis();
  drawFace();
}

void enterNeighborConnected() {
  nodeState = NEIGHBOR_CONNECTED;
  beginLeaderElection();
  triggerYes();
}

void enterConnectedIdle() {
  nodeState = CONNECTED_IDLE;
  currentFace = electionState == LEADER ? LEADER_FACE : NORMAL;
  faceStart = millis();
  lastFaceFrame = millis();
  lastConnectedWink = millis();
  drawFace();
}

void enterNeighborRemoved() {
  nodeState = NEIGHBOR_REMOVED;
  resetLeaderElection();
  triggerNo();
}

// =====================================
// Leader election
// =====================================

void beginLeaderElection() {
  localTicket = (uint16_t)(esp_random() & 0xFFFF);
  remoteTicket = 0;
  electionStartAt = millis() + ELECTION_MIN_DELAY_MS + (localTicket % ELECTION_RANDOM_WINDOW_MS);
  lastFollowerRecheck = millis();
  electionState = ELECTION_WAIT;
  releaseDataSignal();
}

void resetLeaderElection() {
  electionState = NO_ELECTION;
  localTicket = 0;
  remoteTicket = 0;
  electionStartAt = 0;
  lastFollowerRecheck = 0;
  releaseDataSignal();
}

void updateLeaderElection() {
  if (!anyNeighborConnected) {
    return;
  }

  if (electionState == ELECTION_WAIT) {
    if (digitalRead(DATA_SIGNAL_PIN) == LOW) {
      electionState = FOLLOWER;
      lastFollowerRecheck = millis();
      if (nodeState == CONNECTED_IDLE) {
        currentFace = NORMAL;
        drawFace();
      }
      return;
    }

    if (millis() >= electionStartAt) {
      electionState = LEADER;
      pullDataSignalLow();
      if (nodeState == CONNECTED_IDLE) {
        currentFace = LEADER_FACE;
        drawFace();
      }
      return;
    }
  }

  if (electionState == LEADER) {
    pullDataSignalLow();
    return;
  }

  if (electionState == FOLLOWER) {
    releaseDataSignal();
    if (millis() - lastFollowerRecheck < FOLLOWER_RECHECK_MS) {
      return;
    }

    lastFollowerRecheck = millis();
    if (digitalRead(DATA_SIGNAL_PIN) == HIGH) {
      beginLeaderElection();
    }
  }
}

void pullDataSignalLow() {
  pinMode(DATA_SIGNAL_PIN, OUTPUT);
  digitalWrite(DATA_SIGNAL_PIN, LOW);
}

void releaseDataSignal() {
  pinMode(DATA_SIGNAL_PIN, INPUT_PULLUP);
}

void triggerYes() {
  currentFace = YES_FACE;
  yesState = false;
  faceStart = millis();
  lastFaceFrame = 0;
  drawYesFace();
}

void triggerNo() {
  currentFace = NO_FACE;
  noDirection = -1;
  faceStart = millis();
  lastFaceFrame = 0;
  drawNoFace();
}

void updateFacePattern() {
  if (nodeState == NEIGHBOR_CONNECTED) {
    updateYesFace();
    return;
  }

  if (nodeState == NEIGHBOR_REMOVED) {
    updateNoFace();
    return;
  }

  if (nodeState == NODE_IDLE && millis() - lastIdleBlink > IDLE_BLINK_INTERVAL_MS) {
    doubleBlink();
    currentFace = NORMAL;
    lastIdleBlink = millis();
    drawFace();
    return;
  }

  if (nodeState == CONNECTED_IDLE && millis() - lastConnectedWink > CONNECTED_WINK_INTERVAL_MS) {
    if (electionState == LEADER) {
      currentFace = LEADER_FACE;
      drawFace();
      lastConnectedWink = millis();
      return;
    }

    currentFace = WINK;
    drawFace();
    delay(180);
    currentFace = NORMAL;
    drawFace();
    lastConnectedWink = millis();
  }
}

void updateYesFace() {
  if (millis() - lastFaceFrame > 220) {
    yesState = !yesState;
    drawYesFace();
    lastFaceFrame = millis();
  }
}

void updateNoFace() {
  if (millis() - lastFaceFrame > 220) {
    noDirection *= -1;
    drawNoFace();
    lastFaceFrame = millis();
  }
}

// =====================================
// LED patterns
// =====================================

void updateLedPattern() {
  if (millis() - lastLedFrame < LED_FRAME_MS) {
    return;
  }

  lastLedFrame = millis();

  switch (nodeState) {
    case NODE_IDLE:
      drawAnxiousIdleLeds();
      break;

    case NEIGHBOR_CONNECTED:
      drawBreathingLeds(0, 125, 105, 34, 155, 1000);
      break;

    case CONNECTED_IDLE:
      if (electionState == LEADER) {
        drawBreathingLeds(0, 80, 180, 32, 185, 1800);
      } else {
        drawBreathingLeds(0, 92, 88, 14, 86, 2600);
      }
      break;

    case NEIGHBOR_REMOVED:
      drawBreathingLeds(70, 0, 100, 8, 45, 1000);
      break;
  }
}

void drawConnectedFaceLeds(uint8_t minLevel, uint8_t maxLevel, uint8_t blueBase, unsigned long periodMs) {
  unsigned long phase = millis() % periodMs;
  unsigned long halfPeriod = periodMs / 2;
  uint8_t level;

  if (phase < halfPeriod) {
    level = minLevel + ((maxLevel - minLevel) * phase) / halfPeriod;
  } else {
    level = maxLevel - ((maxLevel - minLevel) * (phase - halfPeriod)) / halfPeriod;
  }

  pixels.clear();

  for (uint8_t faceIndex = 0; faceIndex < FACE_COUNT; faceIndex++) {
    if (faceConnected[faceIndex]) {
      uint8_t green = level;
      uint8_t blue = (blueBase * level) / 255;
      drawZigZagPosition(faceLedPositions[faceIndex], pixels.Color(0, green, blue));
    }
  }

  pixels.show();
}

void drawZigZagPosition(uint8_t physicalPosition, uint32_t color) {
  if (physicalPosition < 1 || physicalPosition > LED_ROW_LENGTH) {
    return;
  }

  uint8_t columnIndex = physicalPosition - 1;

  for (uint8_t rowStart = 0; rowStart < LED_COUNT; rowStart += LED_ROW_LENGTH) {
    pixels.setPixelColor(rowStart + columnIndex, color);
  }
}

void drawAnxiousIdleLeds() {
  unsigned long phase = millis() % IDLE_ANXIETY_PERIOD_MS;
  unsigned long halfPeriod = IDLE_ANXIETY_PERIOD_MS / 2;
  uint8_t baseLevel;

  if (phase < halfPeriod) {
    baseLevel = 14 + (58 * phase) / halfPeriod;
  } else {
    baseLevel = 72 - (58 * (phase - halfPeriod)) / halfPeriod;
  }

  uint8_t jitter = ((millis() / 137) % 2 == 0) ? 0 : 10;
  uint8_t level = constrain(baseLevel + jitter, 14, 82);
  uint8_t red = (level * 70) / 255;
  uint8_t blue = (level * 150) / 255;
  uint32_t color = pixels.Color(red, 0, blue);

  for (int i = 0; i < LED_COUNT; i++) {
    pixels.setPixelColor(i, color);
  }

  pixels.show();
}

void drawBreathingLeds(uint8_t red, uint8_t green, uint8_t blue, uint8_t minLevel, uint8_t maxLevel, unsigned long periodMs) {
  unsigned long phase = millis() % periodMs;
  unsigned long halfPeriod = periodMs / 2;
  uint8_t level;

  if (phase < halfPeriod) {
    level = minLevel + ((maxLevel - minLevel) * phase) / halfPeriod;
  } else {
    level = maxLevel - ((maxLevel - minLevel) * (phase - halfPeriod)) / halfPeriod;
  }

  uint8_t scaledRed = (red * level) / 255;
  uint8_t scaledGreen = (green * level) / 255;
  uint8_t scaledBlue = (blue * level) / 255;
  uint32_t color = pixels.Color(scaledRed, scaledGreen, scaledBlue);

  for (int i = 0; i < LED_COUNT; i++) {
    pixels.setPixelColor(i, color);
  }

  pixels.show();
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

    case LEADER_FACE:
      drawLeaderFace();
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

void thickLine(int x1, int y1, int x2, int y2) {
  for (int offset = -2; offset <= 2; offset++) {
    display.drawLine(x1 + offset, y1, x2 + offset, y2, SSD1306_WHITE);
    display.drawLine(x1, y1 + offset, x2, y2 + offset, SSD1306_WHITE);
  }
}

void xEye(int centerX, int centerY) {
  thickLine(centerX - 15, centerY - 15, centerX + 15, centerY + 15);
  thickLine(centerX + 15, centerY - 15, centerX - 15, centerY + 15);
}

void checkEye(int centerX, int centerY) {
  thickLine(centerX - 15, centerY, centerX - 5, centerY + 11);
  thickLine(centerX - 5, centerY + 11, centerX + 17, centerY - 13);
}

// =====================================
// Normal
// =====================================

void eyesNormal() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(34, 30, 94, 30);
}

void leaderBrows() {
  thickLine(18, 10, 50, 10);
  thickLine(78, 10, 110, 10);
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

  int eyeY = yesState ? 13 : 23;
  int pupilY = yesState ? 25 : 35;

  eye(18, eyeY, 32, 24);
  eye(78, eyeY, 32, 24);
  pupils(34, pupilY, 94, pupilY);

  display.display();
}

// =====================================
// No
// =====================================

void drawNoFace() {
  display.clearDisplay();

  int eyeShift = noDirection * 5;

  eye(18 + eyeShift, 18, 32, 24);
  eye(78 + eyeShift, 18, 32, 24);

  display.fillCircle(34 + eyeShift, 30, 5, SSD1306_BLACK);
  display.fillCircle(94 + eyeShift, 30, 5, SSD1306_BLACK);

  display.display();
}

// =====================================
// Leader
// =====================================

void drawLeaderFace() {
  display.clearDisplay();
  leaderBrows();
  eyesNormal();
  display.display();
}
