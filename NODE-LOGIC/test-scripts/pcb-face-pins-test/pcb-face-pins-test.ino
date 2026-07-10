
#define PIN_BOTTOM_RX D9
#define PIN_FRONT_RX D1
#define PIN_LEFT_RX D2
#define PIN_RIGHT_RX D7
#define PIN_POSTERIOR_RX D8
#define PIN_TOP_RX D3



void setup() {
  pinMode(PIN_FRONT_RX, INPUT_PULLUP);
  pinMode(PIN_LEFT_RX, INPUT_PULLUP);
  pinMode(PIN_RIGHT_RX, INPUT_PULLUP);
  pinMode(PIN_TOP_RX, INPUT_PULLUP);
  pinMode(PIN_POSTERIOR_RX, INPUT_PULLUP);
  pinMode(PIN_BOTTOM_RX, INPUT_PULLUP);  
}

void loop() {}
