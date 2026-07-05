#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#include "src/PacketCommunication.h"

#define FACE_COUNT 6
#define LED_PIN 15
#define BUTTON_PIN A0
//////////////// Serial port definitions ////////////////////
#define BAUD_RATE 19200

#define NO_TX_PIN -1

////////// To enable face letters on display uncomment the next line
#define DEBUG_MODE

/////
/*
Faces 1 -> Bottom
Faces 2 -> Front
Faces 3 -> Left
Faces 4 -> Right
Faces 5 -> Posterior
Faces 6 -> Top
*/

#define PIN_BOTTOM_RX D9
#define PIN_FRONT_RX D1
#define PIN_LEFT_RX D2
#define PIN_RIGHT_RX D7
#define PIN_POSTERIOR_RX D3
#define PIN_TOP_RX D8


// oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_ADDRESS 0x3C
#define OLED_SDA_PIN D4
#define OLED_SCL_PIN D5

#define LED_STRIP_PIN D10
#define LED_COUNT 40
#define LED_FRAME_MS 30
#define OLED_FRAME_MS 80
#define MATRIX_BRIGHTNESS 40

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_NeoPixel pixels(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix screen(8,8,A0);

enum Face : uint8_t { FaceLeft,
                      FaceRight,
                      FaceTop,
                      FaceBottom,
                      FaceFront,
                      FacePosterior };

enum EyeExpression : uint8_t { EyesNormal,
                               EyesBlink,
                               EyesWink,
                               EyesLookLeft,
                               EyesLookRight };

SoftwareSerial SerialLeft(PIN_LEFT_RX, NO_TX_PIN);
SoftwareSerial SerialRight(PIN_RIGHT_RX, NO_TX_PIN);
SoftwareSerial SerialTop(PIN_TOP_RX, NO_TX_PIN);
SoftwareSerial SerialBottom(PIN_BOTTOM_RX, NO_TX_PIN);
SoftwareSerial SerialFront(PIN_FRONT_RX, NO_TX_PIN);
SoftwareSerial SerialPosterior(PIN_POSTERIOR_RX, NO_TX_PIN);


// keep the same order as Face enum to correctly address each face across the code
SoftwareSerial* rxPorts[FACE_COUNT] = { &SerialLeft, &SerialRight, &SerialTop, &SerialBottom, &SerialFront, &SerialPosterior };


//////////////// end Serial port definitions ////////////////

bool isLedOn = false;
unsigned long blinkInterval = 500;
unsigned long lastBlinkTime = 0;

unsigned long lastSendTime = 0;
unsigned long sendInterval = 1000;

unsigned long lastComputeTime = 0;
unsigned long computeInterval = 2000;

unsigned long lastLedFrame = 0;
unsigned long lastOledFrame = 0;
unsigned long lastEyeChange = 0;
unsigned long eyeInterval = 5200;
unsigned long expressionStartedAt = 0;
unsigned long expressionDuration = 0;

uint8_t selfState = 0;
EyeExpression currentExpression = EyesNormal;

///////////// Serial packets /////////

struct PacketData {
  uint8_t state;
  uint8_t neighborCount[2];
};

PacketData broadcastData;
PacketData neighborData[FACE_COUNT];

PacketReciever<PacketData> faceRecivers[FACE_COUNT];

///////////// End serial packets /////


///////////// Negibors Detection /////


unsigned long neighborsTimeOut = 3000;
unsigned long lastSeen[FACE_COUNT];
bool hasNeighbor[FACE_COUNT];

///////////// end Negibors Detection /////

void setup() {
  // reset state arrays
  for (int face = 0; face < FACE_COUNT; face++) {
    lastSeen[face] = 0;
    hasNeighbor[face] = false;
  }

  // initialize inbuilt LED pin as an output.
  pinMode(LED_PIN, OUTPUT);

  // set a random value
  selfState = random(5, 254);
  processState();

  // pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE, SERIAL_8N1, -1, D6);

  for (int i = 0; i < FACE_COUNT; ++i) {
    rxPorts[i]->begin(BAUD_RATE);
  }

  pixels.begin();
  pixels.clear();
  pixels.show();

  screen.begin();
  screen.clear();
  screen.setBrightness(MATRIX_BRIGHTNESS);
  drawZoiLogoOnMatrix();
  screen.show();

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  while (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    delay(100);
  }
  display.clearDisplay();
}

void loop() {
  unsigned long currentMillis = millis();

  // BLINK
  if (currentMillis - lastBlinkTime > blinkInterval) {
    lastBlinkTime = currentMillis;
    isLedOn = !isLedOn;
    digitalWrite(LED_PIN, isLedOn);
  }

  // detect press
  // if (digitalRead(BUTTON_PIN) == LOW) {
  //   Serial.print("Button pressed");
  //   selfState = 0;
  // }

  // SEND
  if (currentMillis - lastSendTime > sendInterval) {
    lastSendTime = currentMillis;

    processFaceDetection(currentMillis);

    Serial.print(" Send state: ");
    Serial.println(selfState);
    broadcastData.state = selfState;
    sendPackage(Serial1, broadcastData);
  }

  // RECIEVE
  for (uint8_t i = 0; i < FACE_COUNT; ++i) {
    // loop all faces
    Face face = (Face)i;
    // proces serial buffer
    while (rxPorts[face]->available() > 0) {
      if (faceRecivers[face].processIncomingData(rxPorts[face]->read(), neighborData[face])) {

        lastSeen[face] = currentMillis;
        hasNeighbor[face] = true;

        Serial.print(face);
        Serial.print(": ");
        Serial.println(neighborData[face].state);
      }
    }
  }

  updateVisuals(currentMillis);

  // PROCESS
  if (currentMillis - lastComputeTime > computeInterval) {


    // Show neighbor status
    Serial.print(" NEIGHBORS: ");
    Serial.print(hasNeighbor[FaceLeft] ? "L" : ".");
    Serial.print(hasNeighbor[FaceRight] ? "R" : ".");
    Serial.print(hasNeighbor[FaceTop] ? "T" : ".");
    Serial.print(hasNeighbor[FaceBottom] ? "B" : ".");
    Serial.print(hasNeighbor[FaceFront] ? "F" : ".");
    Serial.println(hasNeighbor[FacePosterior] ? "P" : ".");

    // show grid pos
    Serial.print(" GridPos: ");
    Serial.print(broadcastData.neighborCount[0]);
    Serial.print(",");
    Serial.println(broadcastData.neighborCount[1]);

    lastComputeTime = currentMillis;
    Serial.println("COMPUTE :)");
    processState();
  }
}

void processFaceDetection(const unsigned long& currentTime) {
  for (int i = 0; i < FACE_COUNT; ++i) {
    Face face = (Face)i;
    if (!hasNeighbor[face]) continue;

    if (currentTime - lastSeen[face] > neighborsTimeOut) {
      Serial.print("NEIGHBOR: Lost on ");
      Serial.println(face);
      hasNeighbor[face] = false;
      lastSeen[face] = 0;
    }
  }

  // calculate grid pos
  if (hasNeighbor[FaceLeft]) {
    uint8_t leftNeighbors = neighborData[FaceLeft].neighborCount[FaceLeft];

    broadcastData.neighborCount[FaceLeft] = leftNeighbors > 0 ? leftNeighbors + 1 : 1;
  } else {
    broadcastData.neighborCount[FaceLeft] = 0;
  }
}

void updateVisuals(const unsigned long& currentMillis) {
  if (currentMillis - lastOledFrame >= OLED_FRAME_MS) {
    lastOledFrame = currentMillis;
    updateExpression(currentMillis);
    updateDisplay();
  }

  if (currentMillis - lastLedFrame >= LED_FRAME_MS) {
    lastLedFrame = currentMillis;
    updateLeds(currentMillis);
  }
}

void updateExpression(const unsigned long& currentMillis) {
  if (currentExpression != EyesNormal && currentMillis - expressionStartedAt >= expressionDuration) {
    currentExpression = EyesNormal;
    expressionDuration = 0;
  }

  if (currentExpression == EyesNormal && currentMillis - lastEyeChange >= eyeInterval) {
    lastEyeChange = currentMillis;
    expressionStartedAt = currentMillis;
    expressionDuration = 180;

    uint8_t connectedFaces = countConnectedFaces();
    if (connectedFaces == 0) {
      currentExpression = EyesBlink;
      eyeInterval = 4300;
    } else if (connectedFaces >= 3) {
      currentExpression = (random(0, 2) == 0) ? EyesLookLeft : EyesLookRight;
      expressionDuration = 420;
      eyeInterval = 3600;
    } else {
      currentExpression = EyesWink;
      expressionDuration = 260;
      eyeInterval = 5200;
    }
  }
}

void updateDisplay() {
  display.clearDisplay();

  switch (currentExpression) {
    case EyesBlink:
      eyesBlink();
      break;
    case EyesWink:
      eyesWink();
      break;
    case EyesLookLeft:
      eyesLookLeft();
      break;
    case EyesLookRight:
      eyesLookRight();
      break;
    case EyesNormal:
    default:
      eyesNormal();
      break;
  }

  drawFaceConnectionMarkers();

#ifdef DEBUG_MODE
  drawConnectedFaceLetters();
#endif

  display.display();
}

void updateLeds(const unsigned long& currentMillis) {
  uint8_t connectedFaces = countConnectedFaces();
  bool connected = connectedFaces > 0;
  uint8_t red = connected ? 0 : 58;
  uint8_t green = connected ? 92 : 0;
  uint8_t blue = connected ? 130 : 178;
  uint8_t minLevel = connected ? 18 : 10;
  uint8_t maxLevel = connected ? 145 : 62;
  unsigned long periodMs = blinkInterval * 2;

  if (connectedFaces >= 3) {
    green = 80;
    blue = 190;
    maxLevel = 180;
    periodMs = 2500;
  }

  drawSmoothBreathingLeds(red, green, blue, minLevel, maxLevel, periodMs, currentMillis);
}

void drawSmoothBreathingLeds(uint8_t red, uint8_t green, uint8_t blue, uint8_t minLevel, uint8_t maxLevel, unsigned long periodMs, const unsigned long& currentMillis) {
  float phase = (float)(currentMillis % periodMs) / (float)periodMs;
  float wave = (sin((phase * TWO_PI) - HALF_PI) + 1.0f) * 0.5f;
  float eased = wave * wave * (3.0f - (2.0f * wave));
  uint8_t level = minLevel + (uint8_t)((maxLevel - minLevel) * eased);

  uint8_t scaledRed = (red * level) / 255;
  uint8_t scaledGreen = (green * level) / 255;
  uint8_t scaledBlue = (blue * level) / 255;
  uint32_t color = pixels.Color(scaledRed, scaledGreen, scaledBlue);

  for (int i = 0; i < LED_COUNT; i++) {
    pixels.setPixelColor(i, color);
  }

  pixels.show();
}

uint8_t countConnectedFaces() {
  uint8_t connectedFaces = 0;
  for (uint8_t i = 0; i < FACE_COUNT; i++) {
    if (hasNeighbor[i]) {
      connectedFaces++;
    }
  }

  return connectedFaces;
}

void drawZoiLogoOnMatrix() {
  const uint8_t off = 0;
  const uint8_t red = 1;
  const uint8_t darkRed = 2;
  const uint8_t logo[8][8] = {
    { off, off, off, off, off, off, off, off },
    { off, red, red, red, red, red, red, off },
    { off, off, off, red, darkRed, darkRed, red, off },
    { off, off, off, off, off, off, off, off },
    { off, off, off, off, off, off, off, off },
    { off, red, darkRed, darkRed, red, off, off, off },
    { off, red, red, red, red, red, red, off },
    { off, off, off, off, off, off, off, off }
  };

  uint16_t colors[] = {
    screen.Color(0, 0, 0),
    screen.Color(210, 44, 38),
    screen.Color(165, 36, 32)
  };

  screen.clear();
  for (uint8_t y = 0; y < 8; y++) {
    for (uint8_t x = 0; x < 8; x++) {
      screen.drawPixel(x, y, colors[logo[y][x]]);
    }
  }
}


void drawFaceConnectionMarkers() {
  // Physical face numbers:
  // 1 Bottom, 2 Front, 3 Left, 4 Right, 5 Back/Posterior, 6 Top.
  // Markers are visible while a face is waiting for a neighbor, and disappear once connected.
  if (!hasNeighbor[FaceBottom]) {
    display.drawFastHLine(60, 60, 8, SSD1306_WHITE);
  }
  if (!hasNeighbor[FaceFront]) {
    display.drawFastHLine(2, 61, 8, SSD1306_WHITE);
    display.drawFastVLine(2, 53, 8, SSD1306_WHITE);
  }
  if (!hasNeighbor[FaceLeft]) {
    display.drawFastHLine(2, 2, 8, SSD1306_WHITE);
    display.drawFastVLine(2, 2, 8, SSD1306_WHITE);
  }
  if (!hasNeighbor[FacePosterior]) {
    display.drawFastHLine(118, 2, 8, SSD1306_WHITE);
    display.drawFastVLine(125, 2, 8, SSD1306_WHITE);
  }
  if (!hasNeighbor[FaceRight]) {
    display.drawFastHLine(118, 61, 8, SSD1306_WHITE);
    display.drawFastVLine(125, 53, 8, SSD1306_WHITE);
  }
  if (!hasNeighbor[FaceTop]) {
    display.drawFastHLine(60, 3, 8, SSD1306_WHITE);
  }
}

void eye(int x, int y, int w, int h) {
  display.fillRoundRect(x, y, w, h, 8, SSD1306_WHITE);
}

void pupils(int x1, int y1, int x2, int y2) {
  display.fillCircle(x1, y1, 4, SSD1306_BLACK);
  display.fillCircle(x2, y2, 4, SSD1306_BLACK);
}

void eyesNormal() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(34, 30, 94, 30);
}

void eyesBlink() {
  eye(18, 28, 32, 5);
  eye(78, 28, 32, 5);
}

void eyesWink() {
  eye(18, 28, 32, 5);
  eye(78, 18, 32, 24);
  display.fillCircle(94, 30, 4, SSD1306_BLACK);
}

void eyesLookLeft() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(26, 30, 86, 30);
}

void eyesLookRight() {
  eye(18, 18, 32, 24);
  eye(78, 18, 32, 24);
  pupils(42, 30, 102, 30);
}

void processState() {
  float t = 0.187f;
  unsigned int targetValue = 0;
  int neighborCount = 0;
  for(int i=0; i<FACE_COUNT; i++){
    if (hasNeighbor[i]) {
      targetValue +=  neighborData[i].state;
      neighborCount++;
    }
  }
  if(neighborCount > 0){
    targetValue = targetValue / neighborCount;
    selfState = myLerp(selfState, targetValue, t);
  }

  blinkInterval = selfState * 3;
}

float myLerp(float a, float b, float x) {
  float delta = x * (b - a);
  return a + delta;
}


#ifdef DEBUG_MODE
//// DEBUG HELPER ////

void drawConnectedFaceLetters(){
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 2);
    display.print(selfState);
    display.setCursor(20, 50);
    display.print(hasNeighbor[FaceLeft] ? "L" : ".");
    display.print(hasNeighbor[FaceRight] ? "R" : ".");
    display.print(hasNeighbor[FaceTop] ? "T" : ".");
    display.print(hasNeighbor[FaceBottom] ? "B" : ".");
    display.print(hasNeighbor[FaceFront] ? "F" : ".");
    display.print(hasNeighbor[FacePosterior] ? "P" : ".");
}
#endif
