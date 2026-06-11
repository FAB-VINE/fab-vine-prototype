#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#include "src/PacketCommunication.h"

#define FACE_COUNT 4
#define LED_PIN 15
//////////////// Serial port definitions ////////////////////
#define BAUD_RATE 9600

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



SoftwareSerial SerialLeft(PIN_LEFT_RX, NO_TX_PIN);
SoftwareSerial SerialRight(PIN_RIGHT_RX, NO_TX_PIN);
SoftwareSerial SerialTop(PIN_TOP_RX, NO_TX_PIN);
SoftwareSerial SerialBottom(PIN_BOTTOM_RX, NO_TX_PIN);

SoftwareSerial* rxPorts[FACE_COUNT] = {&SerialLeft, &SerialRight, &SerialTop, &SerialBottom};


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
uint8_t topNeighborState = 255;  
uint8_t bottomNeighborState = 255;

///////////// Serial packets /////////

struct PacketData{
  uint8_t state;
  uint8_t neighborCount[2];
};

PacketData broadcastData;

PacketData rightNeighborData;
PacketData leftNeighborData;
PacketData topNeighborData;
PacketData bottomNeighborData;

// PacketReciever<PacketData> recieverLeft;
// PacketReciever<PacketData> recieverRight;
// PacketReciever<PacketData> recieverTop;
// PacketReciever<PacketData> recieverBottom;

PacketReciever<PacketData> faceRecivers[FACE_COUNT];

///////////// End serial packets /////


///////////// Negibors Detection /////

enum Face : uint8_t {FaceLeft, FaceRight, FaceTop, FaceBottom};

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
  Serial1.begin(BAUD_RATE, SERIAL_8N1, -1, D6);

  for(int i=0; i<FACE_COUNT; ++i){
    rxPorts[i]->begin(BAUD_RATE);
  }

  // SerialLeft.begin(BAUD_RATE);
  // SerialRight.begin(BAUD_RATE);
  // SerialTop.begin(BAUD_RATE);
  // SerialBottom.begin(BAUD_RATE);

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
  if(currentMillis - lastBlinkTime > blinkInterval){
    lastBlinkTime = currentMillis;
    isLedOn = !isLedOn;
    digitalWrite(LED_PIN, isLedOn);
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
  while (SerialLeft.available()>0) {
    if(faceRecivers[FaceLeft].processIncomingData(SerialLeft, leftNeighborData)){
      
      lastSeen[FaceLeft] = currentMillis;
      hasNeighbor[FaceLeft] = true;

      leftNeighborState =  leftNeighborData.state;
      Serial.print("Left: ");
      Serial.println(leftNeighborState);
    }
  }
  while (SerialRight.available()>0) {

    if(faceRecivers[FaceRight].processIncomingData(SerialRight, rightNeighborData)){
      
      lastSeen[FaceRight] = currentMillis;
      hasNeighbor[FaceRight] = true;
      
      rightNeighborState = rightNeighborData.state;
      Serial.print("Right: ");
      Serial.println(rightNeighborState);
    }
  }
  while (SerialTop.available()>0) {
    if(SerialTop.available() && faceRecivers[FaceTop].processIncomingData(SerialTop, topNeighborData)){
      
      lastSeen[FaceTop] = currentMillis;
      hasNeighbor[FaceTop] = true;
      
      topNeighborState = topNeighborData.state;
      Serial.print("Top: ");
      Serial.println(topNeighborState);
    }
  }
  while (SerialBottom.available()>0) {
    if(SerialBottom.available() && faceRecivers[FaceBottom].processIncomingData(SerialBottom, bottomNeighborData)){
      
      lastSeen[FaceBottom] = currentMillis;
      hasNeighbor[FaceBottom] = true;
      
      bottomNeighborState = bottomNeighborData.state;
      Serial.print("Bottom: ");
      Serial.println(bottomNeighborState);
    }
  }

  updateDisplay();
  
  // PROCESS
  if(currentMillis - lastComputeTime > computeInterval){


    // Show neighbor status
    Serial.print(" NEIGHBORS: ");
    Serial.print(hasNeighbor[FaceLeft]? "L" : ".");
    Serial.print(hasNeighbor[FaceRight]? "R" : ".");
    Serial.print(hasNeighbor[FaceTop]? "T" : ".");
    Serial.println(hasNeighbor[FaceBottom]? "B" : ".");

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

void updateDisplay(){
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(selfState);
  display.setCursor(0, 30);
  display.print(hasNeighbor[FaceLeft]? "L" : ".");
  display.print(hasNeighbor[FaceRight]? "R" : ".");
  display.print(hasNeighbor[FaceTop]? "T" : ".");
  display.println(hasNeighbor[FaceBottom]? "B" : ".");

  display.display();

  uint32_t color = pixels.Color(255 - selfState, 0, selfState);
  for (int i = LED_COUNT - 1; i > 0; i--) {
    pixels.setPixelColor(i, pixels.getPixelColor(i-1));
  }
  pixels.setPixelColor(0, color);
  pixels.show();
}

void processState(){
  float t = 0.187f;
  if(hasNeighbor[FaceLeft] && leftNeighborState > selfState){
    selfState = myLerp(selfState, leftNeighborState, t);
  }
  if(hasNeighbor[FaceRight] && rightNeighborState > selfState){
    selfState = myLerp(selfState, rightNeighborState, t);
  }
  if(hasNeighbor[FaceTop] && topNeighborState > selfState){
    selfState = myLerp(selfState, topNeighborState, t);
  }
  if(hasNeighbor[FaceBottom] && bottomNeighborState > selfState){
    selfState = myLerp(selfState, bottomNeighborState, t);
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