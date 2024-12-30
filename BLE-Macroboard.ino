// Shit code writted by yours truly

// Include Packages and Define
// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);
// BLE Keyboard
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

// Pin adresses (Left to right on board)
int a = 15;
int b = 18;
int c = 4;
int d = 19;

// Variables
int states[4] = {HIGH, HIGH, HIGH, HIGH};

// Run once on awake
void setup() {
  // Start Serial Monitor
  Serial.begin(115200);
  Serial.println("Starting BLE!");

  // Start Keyboard
  bleKeyboard.begin();

  // Initialize Display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); //Not even gonna pretend like I know what this does.
  display.clearDisplay;
  display.setTextColor(PINK);
  display.setTextSize(2);

  display.setCursor(20, 10);
  display.println("NERV");
  display.setTextSize(1);
  display.println("God is in his heaven, all is right with the world");
  
  // Set Pin modes
  pinMode(a, INPUT_PULLUP);
  pinMode(b, INPUT_PULLUP);
  pinMode(c, INPUT_PULLUP);
  pinMode(d, INPUT_PULLUP);
}

void loop() {
  // Read button states
  states[0] = digitalRead(a);
  states[1] = digitalRead(b);
  states[2] = digitalRead(c);
  states[3] = digitalRead(d);

  // If a button state is LOW, it's pressed
  if(states[0] == LOW) {
    Serial.println(0);
  }
  if(states[1] == LOW) {
    Serial.println(1);
  }
  if(states[2] == LOW) {
    Serial.println(2);
  }
  if(states[3] == LOW) {
    Serial.println(3);
  }
}