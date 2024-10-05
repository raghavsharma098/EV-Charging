#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// IR Sensor Pin
const int irSensorPin = 2;

// Relay Pin
const int relayPin = 3;

// Billing Variables
unsigned long startTime;
unsigned long chargingDuration;
float chargingRate = 0.10; // Charge per minute
bool isCharging = false;

void setup() {
    // Initialize Serial for debugging
    Serial.begin(9600);
    
    // Initialize OLED display
    display.begin(SSD1306_I2C_ADDRESS, OLED_RESET);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Initialize IR sensor and relay
    pinMode(irSensorPin, INPUT);
    pinMode(relayPin, OUTPUT);
}

void loop() {
    if (digitalRead(irSensorPin) == HIGH) {
        if (!isCharging) {
            startCharging();
        }
    } else {
        if (isCharging) {
            stopCharging();
        }
    }
    delay(100);
}

void startCharging() {
    isCharging = true;
    startTime = millis();
    digitalWrite(relayPin, HIGH); // Activate charger
    displayChargingStatus();
}

void stopCharging() {
    isCharging = false;
    chargingDuration = (millis() - startTime) / 60000; // Duration in minutes
    digitalWrite(relayPin, LOW); // Deactivate charger
    displayBill();
}

void displayChargingStatus() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Charging...");
    display.display();
}

void displayBill() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Charging complete.");
    display.setCursor(0, 10);
    display.print("Duration: ");
    display.print(chargingDuration);
    display.print(" min");
    display.setCursor(0, 20);
    display.print("Total Bill: $");
    display.print(chargingDuration * chargingRate);
    display.display();
    delay(5000); // Display bill for 5 seconds
}
