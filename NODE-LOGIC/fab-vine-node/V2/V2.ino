#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#include "src/PacketCommunication.h"

#define FACE_COUNT 6
#define LED_PIN 15
#define BUTTON_PIN A0
//////////////// Serial port definitions ////////////////////
#define BAUD_RATE 19200

#define NO_TX_PIN -1

/////
/*
Faces 1 -> Bottom
Faces 2 -> Front
Faces 3 -> Left
Faces 4 -> Right
Faces 5 -> Back
Faces 6 -> Top
*/

#define PIN_LEFT_RX D2
#define PIN_RIGHT_RX D3
#define PIN_TOP_RX D7
#define PIN_BOTTOM_RX D9
#define PIN_FRONT_RX D1
#define PIN_POSTERIOR_RX D8


// oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_ADDRESS 0x3C
#define OLED_SDA_PIN D4
#define OLED_SCL_PIN D5

#define LED_STRIP_PIN D10
#define LED_COUNT 40

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_NeoPixel pixels(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);


enum Face : uint8_t { FaceLeft,
                      FaceRight,
                      FaceTop,
                      FaceBottom,
                      FaceFront,
                      FacePosterior };

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

uint8_t selfState = 0;

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

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE, SERIAL_8N1, -1, D6);

  for (int i = 0; i < FACE_COUNT; ++i) {
    rxPorts[i]->begin(BAUD_RATE);
  }

  pixels.begin();
  pixels.clear();
  pixels.show();

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
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.print("Button pressed");
    selfState = 0;
  }

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

  updateDisplay();

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

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(selfState);
  display.setCursor(0, 30);
  display.print(hasNeighbor[FaceLeft] ? "L" : ".");
  display.print(hasNeighbor[FaceRight] ? "R" : ".");
  display.print(hasNeighbor[FaceTop] ? "T" : ".");
  display.print(hasNeighbor[FaceBottom] ? "B" : ".");
  display.print(hasNeighbor[FaceFront] ? "F" : ".");
  display.println(hasNeighbor[FacePosterior] ? "P" : ".");

  display.display();

  uint32_t color = pixels.Color(255 - selfState, 0, selfState);
  for (int i = LED_COUNT - 1; i > 0; i--) {
    pixels.setPixelColor(i, pixels.getPixelColor(i - 1));
  }
  pixels.setPixelColor(0, color);
  pixels.show();
}

void processState() {
  float t = 0.187f;

  // if (hasNeighbor[FaceLeft] && neighborData[FaceLeft].state > selfState) {
  //   selfState = myLerp(selfState, neighborData[FaceLeft].state, t);
  // }
  // if (hasNeighbor[FaceRight] && neighborData[FaceRight].state > selfState) {
  //   selfState = myLerp(selfState, neighborData[FaceRight].state, t);
  // }
  // if (hasNeighbor[FaceTop] && neighborData[FaceTop].state > selfState) {
  //   selfState = myLerp(selfState, neighborData[FaceTop].state, t);
  // }
  // if (hasNeighbor[FaceBottom] && neighborData[FaceBottom].state > selfState) {
  //   selfState = myLerp(selfState, neighborData[FaceBottom].state, t);
  // }
  // if (hasNeighbor[FaceFront] && neighborData[FaceFront].state > selfState) {
  //   selfState = myLerp(selfState, neighborData[FaceFront].state, t);
  // }
  // if (hasNeighbor[FacePosterior] && neighborData[FacePosterior].state > selfState) {
  //   selfState = myLerp(selfState, neighborData[FacePosterior].state, t);
  // }
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
  // if (delta <= 1.0f) {
  //   return b;
  // }
  return a + delta;
}