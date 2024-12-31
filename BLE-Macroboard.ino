// Shit code written by yours truly
// ephemera/vanillacrepes.

// Include Packages and Define

#include <Wire.h>

// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// BLE Keyboard
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Eph - Lfrith", "vanillacrepes", 100);

// Define threshold to go from short to long press
#define PRESS_THRESHOLD 500

// Pin adresses (Left to right on board)
int addr[4] = {15, 18, 4, 19};

// Variables
bool lStates[4] = {HIGH, HIGH, HIGH, HIGH};
bool states[4] = {HIGH, HIGH, HIGH, HIGH};
char values[4] = {'a', 'b', 'c', 'd'};
bool canFire[4] = {1, 1, 1, 1};

unsigned long pressedTime[4] = {};
unsigned long elapsedTime[4] = {};

// Run once on awake
void setup() {
  // Start Serial Monitor
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Starting BLE!");

  // Start Keyboard
  bleKeyboard.begin();

  // Initialize Display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR); //Not even gonna pretend like I know what this does.
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);

  // Oooooo evangelion reference oooooo

  display.setCursor(0, 0);
  display.println("NERV");
  display.setTextSize(1);
  display.println("God is in his heaven, all is right with the world");

  display.display();
  
  // Set Pin modes (PULLUP since they are connected to ground)
  pinMode(addr[0], INPUT_PULLUP);
  pinMode(addr[1], INPUT_PULLUP);
  pinMode(addr[2], INPUT_PULLUP);
  pinMode(addr[3], INPUT_PULLUP);
}

void loop() {
  for(int i = 0; i < 4; i++) {
    // Read button states
    states[i] = digitalRead(addr[i]);

    // Button code
    detectPress(i);

    // Set last states
    lStates[i] = states[i];
  }
}

void write() {
  // Fancy blablabla shiz noone gaf lil bro
  display.clearDisplay();
  display.setTextSize(2);

  display.setCursor(0, 0);
  display.println("NERV");
  display.setTextSize(1);
  display.println("God is in his heaven, all is right with the world");
}

void detectPress(int b) {
  // Do nothing if key isn't pressed
  if(states[b] == HIGH) return;

  // Check for connection
  if(bleKeyboard.isConnected()) {
    if(states[b] == LOW && lStates[b] == HIGH) { // Key has just been pressed
      // Send assigned keystroke
      bleKeyboard.write(values[b]);
      // Record time at press
      pressedTime[b] = millis();

      // Update OLED
      write();
      display.println(String("\nAericht Says: \n0") + String(b) + " pressed.");
      display.display();
      
      // Debug
      Serial.println("Pressed");
    }

    if(states[b] == LOW && lStates[b] == LOW) { // Key held
      // Get current time
      elapsedTime[b] = millis();

      // Get total time the key has been pressed
      long pressDuration = elapsedTime[b] - pressedTime[b];

      // If it passes the threshold, spam the shi like in a keyb
      if(pressDuration > PRESS_THRESHOLD) {
        bleKeyboard.write(values[b]);

        // Update OLED
        write();
        display.println(String("\nAericht Says: \n0") + String(b) + " held down.");
        display.display();

        // Debug
        Serial.println("Held");
      }
    }
  }
}