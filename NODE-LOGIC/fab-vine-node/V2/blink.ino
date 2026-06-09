#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "src/PacketCommunication.h"

#define FACE_COUNT 2
#define LED_PIN 15
//////////////// Serial port definitions ////////////////////
#define BAUD_RATE 9600

#define NO_TX_PIN -1

#define PIN_LEFT_RX D9
#define PIN_RIGHT_RX D1


// oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_ADDRESS 0x3C
#define OLED_SDA_PIN D4
#define OLED_SCL_PIN D5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


SoftwareSerial SerialLeft(PIN_LEFT_RX, NO_TX_PIN);
SoftwareSerial SerialRight(PIN_RIGHT_RX, NO_TX_PIN);

//////////////// end Serial port definitions ////////////////

bool isLedOn = false;
unsigned long blinkInterval = 500;
unsigned long lastBlinkTime = 0;

unsigned long lastSendTime = 0;
unsigned long sendInterval = 1000;

unsigned long lastComputeTime = 0;
unsigned long computeInterval = 2000;

uint8_t selfState = 0;
uint8_t rightNeighborState = 255;  
uint8_t leftNeighborState = 255;

///////////// Serial packets /////////

struct PacketData{
  uint8_t state;
  uint8_t neighborCount[2];
};

PacketData broadcastData;
PacketData rightNeighborData;
PacketData leftNeighborData;

PacketReciever<PacketData> recieverLeft;
PacketReciever<PacketData> recieverRight;
///////////// End serial packets /////


///////////// Negibors Detection /////

enum Face : uint8_t {FaceLeft, FaceRight};

unsigned long neighborsTimeOut = 3000;
unsigned long lastSeen[FACE_COUNT] = {0};
bool hasNeighbor[FACE_COUNT] = {false, false};

///////////// end Negibors Detection /////
void setup() {   
  // initialize inbuilt LED pin as an output.
  pinMode(LED_PIN, OUTPUT);

  // set a random value
  selfState = random(5,254);
  processState();

  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE, SERIAL_8N1, D7, D6);

  SerialLeft.begin(BAUD_RATE);
  SerialRight.begin(BAUD_RATE);

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  while (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    delay(100);
  }
  display.clearDisplay();

}

void loop() {
  unsigned long currentMillis = millis();
 
  // BLINK
  if(currentMillis - lastBlinkTime > blinkInterval){
    lastBlinkTime = currentMillis;
    isLedOn = !isLedOn;
    digitalWrite(LED_PIN, isLedOn);
    Serial.println(blinkInterval);
  }

  // SEND
  if(currentMillis - lastSendTime > sendInterval){
    lastSendTime = currentMillis;

    processFaceDetection(currentMillis);

    Serial.print(" Send state: ");
    Serial.println(selfState);
    broadcastData.state = selfState;
    sendPackage(Serial1, broadcastData);
  }

  // RECIEVE 
  if(SerialLeft.available() && recieverLeft.processIncomingData(SerialLeft, leftNeighborData)){
    
    lastSeen[FaceLeft] = currentMillis;
    hasNeighbor[FaceLeft] = true;

    leftNeighborState =  leftNeighborData.state;
    Serial.print("Left: ");
    Serial.println(leftNeighborState);
  }
  if(SerialRight.available() && recieverRight.processIncomingData(SerialRight, rightNeighborData)){
    
    lastSeen[FaceRight] = currentMillis;
    hasNeighbor[FaceRight] = true;
    
    rightNeighborState = rightNeighborData.state;
    Serial.print("Right: ");
    Serial.println(rightNeighborState);
  }

  // PROCESS
  if(currentMillis - lastComputeTime > computeInterval){

    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(selfState);
    display.setCursor(0, 30);
    display.print(hasNeighbor[FaceLeft]? "L" : ".");
    display.print(hasNeighbor[FaceRight]? "R" : ".");
    display.display();

    // Show neighbor status
    Serial.print(" NEIGHBORS: ");
    Serial.print(hasNeighbor[FaceLeft]? "L" : ".");
    Serial.println(hasNeighbor[FaceRight]? "R" : ".");

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

void processFaceDetection(const unsigned long& currentTime){
  for (int i = 0; i< FACE_COUNT; ++i) {
    Face face = (Face) i;
    if(!hasNeighbor[face]) continue;

    if(currentTime - lastSeen[face] > neighborsTimeOut){
      Serial.print("NEIGHBOR: Lost on ");
      Serial.println(face);
      hasNeighbor[face] = false;
      lastSeen[face] = 0;
    }
  }

  // calculate grid pos
  if(hasNeighbor[FaceLeft]){
    uint8_t leftNeighbors = leftNeighborData.neighborCount[FaceLeft];

    broadcastData.neighborCount[FaceLeft] = leftNeighbors > 0? leftNeighbors + 1 : 1;
  }
  else{
    broadcastData.neighborCount[FaceLeft] = 0;
  }

}

void processState(){
  float t = 0.187f;
  if(hasNeighbor[FaceLeft] && leftNeighborState > selfState){
    selfState = myLerp(selfState, leftNeighborState, t);
  }
  if(hasNeighbor[FaceRight] && rightNeighborState > selfState){
    selfState = myLerp(selfState, rightNeighborState, t);
  }
  blinkInterval = selfState * 3;
}

float myLerp(float a, float b, float x) {
  float delta = x * (b - a);
  if(delta <= 1.0f){
    return b;
  }
  return a + delta;
}