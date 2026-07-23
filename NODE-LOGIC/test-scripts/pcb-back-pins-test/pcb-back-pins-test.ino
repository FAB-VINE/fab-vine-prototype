
#define PIN_BOTTOM_RX D9
#define PIN_FRONT_RX D1
#define PIN_LEFT_RX D2
#define PIN_RIGHT_RX D7
#define PIN_POSTERIOR_RX D8
#define PIN_TOP_RX D3



void setup() {

  Serial.begin(19200);

  pinMode(PIN_FRONT_RX, INPUT_PULLUP);
  pinMode(PIN_LEFT_RX, INPUT_PULLUP);
  pinMode(PIN_RIGHT_RX, INPUT_PULLUP);
  pinMode(PIN_TOP_RX, INPUT_PULLUP);
  pinMode(PIN_POSTERIOR_RX, INPUT_PULLUP);
  pinMode(PIN_BOTTOM_RX, INPUT_PULLUP);  
  
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
}

void loop() {
  Serial.println("Hello from FabVine node!");
  delay(100);
}
