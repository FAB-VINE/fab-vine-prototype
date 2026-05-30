#include <SoftwareSerial.h>

/// Demo cretaed to test serial comunications uses a NodeMCU v1 board (ESP8266) 
/// as placeholder dev borad. Full duplex comunication is achived with each
/// neighbor with serial (RX, TX). Currently only left and right are considered


//////////////// Serial port definitions ////////////////////
#define BAUD_RATE 9600

#define PIN_LEFT_RX D1
#define PIN_LEFT_TX D2

#define PIN_RIGHT_RX D5
#define PIN_RIGHT_TX D6

SoftwareSerial SerialLeft(PIN_LEFT_RX, PIN_LEFT_TX);
SoftwareSerial SerialRight(PIN_RIGHT_RX, PIN_RIGHT_TX);

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

void setup() {   
  // initialize inbuilt LED pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // set a random value
  selfState = random(5,254);
  processState();

  Serial.begin(BAUD_RATE);
  SerialLeft.begin(BAUD_RATE);
  SerialRight.begin(BAUD_RATE);

}

void loop() {
  unsigned long currentMillis = millis();
 
  // BLINK
  if(currentMillis - lastBlinkTime > blinkInterval){
    lastBlinkTime = currentMillis;
    isLedOn = !isLedOn;
    digitalWrite(LED_BUILTIN, isLedOn);
    Serial.print("blinkDelay: ");
    Serial.println(blinkInterval);
  }

  // SEND
  if(currentMillis - lastSendTime > sendInterval){
    lastSendTime = currentMillis;

    Serial.println(" Send state");
    SerialLeft.write(selfState);
    SerialRight.write(selfState);
  }

  // RECIEVE 
  if(SerialLeft.available()){
    leftNeighborState =  SerialLeft.read();
    Serial.print("Left: ");
    Serial.println(leftNeighborState);
  }
  if(SerialRight.available()){
    rightNeighborState = SerialRight.read();
    Serial.print("Right: ");
    Serial.println(rightNeighborState);
  }

  // PROCESS
  if(currentMillis - lastComputeTime > computeInterval){
    lastComputeTime = currentMillis;
    Serial.println("COMPUTE :)");
    processState();
  } 
}

void processState(){
  float t = 0.187f;
  if(leftNeighborState != 255 && leftNeighborState > selfState){
    selfState = lerp(selfState, leftNeighborState, t);
  }
  if(rightNeighborState != 255 && rightNeighborState > selfState){
    selfState = lerp(selfState, rightNeighborState, t);
  }
  blinkInterval = selfState * 3;
}

float lerp(float a, float b, float x) {
  return a + x * (b - a);
}
