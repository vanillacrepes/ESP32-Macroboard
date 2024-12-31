// Shit code written by yours truly
// ephemera/vanillacrepes.

// Include Packages, Define, Initialize

#include <Wire.h>

// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// BLE Keyboard
#include <BleKeyboard.h>
#include <cstdint>

// Fancy shit yap yap yap I'm bored
String deviceName = "Lfrith";
String manufacturerName = "vanillacrepes";
BleKeyboard bleKeyboard(deviceName.c_str(), manufacturerName.c_str(), 100);

// Define threshold to go from short to long press
#define PRESS_THRESHOLD 500
#define DEBOUNCE_TIME 50

// Pin adresses (Left to right on board)
int addr[4] = {15, 18, 4, 19};

// Variables
bool lStates[4] = {HIGH, HIGH, HIGH, HIGH};
bool states[4] = {HIGH, HIGH, HIGH, HIGH};
bool canFire[4] = {1, 1, 1, 1};

unsigned long pressedTime[4] = {};
unsigned long elapsedTime[4] = {};

unsigned long lastDebounceTimes[4] = {};

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

  write();

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
  display.println(deviceName);
  display.setTextSize(1);
  display.println("Manufactured by: \n" + manufacturerName + ".");
}

void detectPress(int b) {
  // Do nothing if key isn't pressed
  if(states[b] == HIGH) return;

  // Do nothing if debounce time hasn't elapsed
  if(millis() - lastDebounceTimes[b] < DEBOUNCE_TIME) return;

  // Check for connection
  if(bleKeyboard.isConnected()) {
    if(states[b] == LOW && lStates[b] == HIGH) { // Key has just been pressed
      // Send assigned keystroke
      keyFn(b);
      // Record time at press
      pressedTime[b] = millis();

      // Update OLED
      write();
      display.println(String("\n//Log: \n0") + String(b) + " pressed.");
      display.display();

      // Reset debounce time
      lastDebounceTimes[b] = millis();
    }

    if(states[b] == LOW && lStates[b] == LOW) { // Key held
      // Get current time
      elapsedTime[b] = millis();

      // Get total time the key has been pressed
      long pressDuration = elapsedTime[b] - pressedTime[b];

      // If it passes the threshold, spam that shi like keybor
      if(pressDuration > PRESS_THRESHOLD) {
        keyFn(b);

        // Update OLED
        write();
        display.println(String("\n//Log: \n0") + String(b) + " held down.");
        display.display();
      }

      // Reset debounce time
      lastDebounceTimes[b] = millis();
    }
  }
}

// Not the best way to go about this.
void keyFn(int t) {
  if(t == 0) bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
  if(t == 1) bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  if(t == 2) bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
  if(t == 3) bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
}