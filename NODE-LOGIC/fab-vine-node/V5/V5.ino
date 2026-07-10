#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include <WiFi.h>

#define IS_MIRROR true

//////// uncomment to enable USB Serial (it may halt the XIAO in not connected to a computer)
// #define SERIAL_DEBUG 1

#ifdef SERIAL_DEBUG
  #define DBG_PRINT(x) Serial.print(x)
  #define DBG_PRINTLN(x) Serial.println(x)
#else
  #define DBG_PRINT(x)
  #define DBG_PRINTLN(x)
#endif
////////// To enable face letters on display uncomment the next line
#define DEBUG_MODE 1

#include "src/Faces.h"
#include "src/PacketCommunication.h"
#include "src/NeighborDetection.h"
#include "src/NodeLights.h"
#include "src/NodeEyes.h"


#define LED_PIN 15
//////////////// Serial port definitions ////////////////////
#define BAUD_RATE 19200

#if IS_MIRROR
  #define PIN_BOTTOM_RX D9
  #define PIN_FRONT_RX D2
  #define PIN_LEFT_RX D1
  #define PIN_RIGHT_RX D8
  #define PIN_POSTERIOR_RX D7
  #define PIN_TOP_RX D3
#else
  // normal one
  #define PIN_BOTTOM_RX D9
  #define PIN_FRONT_RX D1
  #define PIN_LEFT_RX D2
  #define PIN_RIGHT_RX D7
  #define PIN_POSTERIOR_RX D8
  #define PIN_TOP_RX D3
#endif


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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
NodeEyes nodeEyes{display};

Adafruit_NeoPixel pixels(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
NodeLights nodeLights{pixels};

const Color palette[3] = {{18, 92, 235}, {0, 205, 105}, {145, 38, 225}};


///////////// Neighbors Detection /////
NeighborDetection neighborDetection{
  PIN_LEFT_RX,
  PIN_RIGHT_RX,
  PIN_TOP_RX,
  PIN_BOTTOM_RX,
  PIN_FRONT_RX,
  PIN_POSTERIOR_RX
};

///////////// end Neighbors Detection /////

//////////////// end Serial port definitions ////////////////

bool isLedOn = false;
unsigned long blinkInterval = 500;
unsigned long lastBlinkTime = 0;

unsigned long lastSendTime = 0;
unsigned long sendInterval = 1000;

unsigned long lastComputeTime = 0;
unsigned long computeInterval = 2000;

unsigned long lastOledFrame = 0;
unsigned long lastEyeChange = 0;
unsigned long eyeInterval = 5200;
unsigned long expressionStartedAt = 0;
unsigned long expressionDuration = 0;

uint8_t selfState = 0;

///////////// Serial packets /////////



///////////// End serial packets /////


void setup() {

  //test wifi for power consumption calculations
  // WiFi.softAP("FabVine", "12345");

 // initialize inbuilt LED pin as an output.
  pinMode(LED_PIN, OUTPUT);

  // set a random value
  selfState = random(5, 254);
  processState();

  // pinMode(BUTTON_PIN, INPUT_PULLUP);

  #ifdef SERIAL_DEBUG
   Serial.begin(BAUD_RATE);
  #endif
  neighborDetection.begin(BAUD_RATE);

  pixels.begin();
  pixels.clear();
  pixels.show();

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  while (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    delay(100);
  }
  display.clearDisplay();

  nodeLights.breath();

  neighborDetection.setOnFaceDisconnectedCallback([](Face face){
    DBG_PRINT("FaceDisconnected");
    DBG_PRINTLN(face);
    Color faceDisconnected{255, 0, 0};
    nodeLights.pointToFace(face, faceDisconnected, 2000);
  });
  neighborDetection.setOnFaceConnectedCallback([](Face face){
    DBG_PRINT("FaceConnected");
    DBG_PRINTLN(face);
    Color connectedColor(0,150,0);
    nodeLights.pointToFace(face, connectedColor, 2000);
  });
}

void loop() {
  unsigned long currentMillis = millis();

  // BLINK
  if (currentMillis - lastBlinkTime > blinkInterval) {
    lastBlinkTime = currentMillis;
    isLedOn = !isLedOn;
    digitalWrite(LED_PIN, isLedOn);
  }

  // SEND
  if (currentMillis - lastSendTime > sendInterval) {
    lastSendTime = currentMillis;
    DBG_PRINT(" Send state: ");
    DBG_PRINTLN(selfState);
    neighborDetection.sendBroadcastData(selfState);
  }

  
  neighborDetection.update(currentMillis);

  // PROCESS
  if (currentMillis - lastComputeTime > computeInterval) {

    const bool* hasNeighbor = neighborDetection.hasNeighborConnected;
    // Show neighbor status
     DBG_PRINT(" NEIGHBORS: ");
     DBG_PRINT(hasNeighbor[FaceLeft] ? "L" : ".");
     DBG_PRINT(hasNeighbor[FaceRight] ? "R" : ".");
     DBG_PRINT(hasNeighbor[FaceTop] ? "T" : ".");
     DBG_PRINT(hasNeighbor[FaceBottom] ? "B" : ".");
     DBG_PRINT(hasNeighbor[FaceFront] ? "F" : ".");
     DBG_PRINTLN(hasNeighbor[FacePosterior] ? "P" : ".");

    // // show grid pos
    //  DBG_PRINT(" GridPos: ");
    //  DBG_PRINT(broadcastData.neighborCount[0]);
    //  DBG_PRINT(",");
    //  DBG_PRINTLN(broadcastData.neighborCount[1]);

    lastComputeTime = currentMillis;
    DBG_PRINTLN("COMPUTE :)");
    processState();
    if(!nodeLights.IsPointing()){
      nodeLights.setColor(0,225 - selfState, selfState);
    }
  }

  updateVisuals(currentMillis);
}

void updateVisuals(const unsigned long& currentMillis) {
  if (currentMillis - lastOledFrame >= OLED_FRAME_MS) {
    lastOledFrame = currentMillis;
    updateExpression(currentMillis);
    
    const bool* connectedFaces = neighborDetection.hasNeighborConnected;
    bool hasNeighbor[6];
    for (int i = 0; i < FACE_COUNT; i++)
    {
      hasNeighbor[i] = connectedFaces[i];
    }
    
    nodeEyes.update(hasNeighbor);
  }
  nodeLights.update(currentMillis);
}

/// TODO: refactor to pass most of this logic inside the NodeEyes class
void updateExpression(const unsigned long& currentMillis) {
  EyeExpression currentExpression = nodeEyes.getCurrentExpression();
  if (currentExpression != EyesNormal && currentMillis - expressionStartedAt >= expressionDuration) {
    nodeEyes.setEyesExpression(EyesNormal);
    expressionDuration = 0;
  }

  if (currentExpression == EyesNormal && currentMillis - lastEyeChange >= eyeInterval) {
    lastEyeChange = currentMillis;
    expressionStartedAt = currentMillis;
    expressionDuration = 180;

    uint8_t connectedFaces = neighborDetection.countConnectedFaces();
    
    if (connectedFaces == 0) {
      nodeEyes.setEyesExpression(EyesBlink);
      eyeInterval = 4300;
    } else if (connectedFaces >= 3) {
      nodeEyes.setEyesExpression((random(0, 2) == 0) ? EyesLookLeft : EyesLookRight);
      expressionDuration = 420;
      eyeInterval = 3600;
    } else {
      nodeEyes.setEyesExpression(EyesWink);
      expressionDuration = 260;
      eyeInterval = 5200;
    }
  }
}

void processState() {
  float t = 0.187f;
  unsigned int targetValue = 0;
  int neighborCount = 0;
    const bool* hasNeighbor = neighborDetection.hasNeighborConnected;

  for(int i=0; i<FACE_COUNT; i++){
    if (hasNeighbor[i]) {
      targetValue += neighborDetection.neighborsData[i].state;
      neighborCount++;
    }
  }
  if(neighborCount > 0){
    targetValue = targetValue / neighborCount;
    selfState = lerp(selfState, targetValue, t);
  }

  blinkInterval = selfState * 3;
}
