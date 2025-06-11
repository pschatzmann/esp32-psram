#include "ESP32-PSRAM.h"

void setup() {
  Serial.begin(115200);
  
  // Initialize PSRAM file system
  if (!PSRAM.begin()) {
    Serial.println("PSRAM initialization failed!");
    return;
  }
  
  // Create and write to a file
  auto file = PSRAM.open("test.txt", FILE_WRITE);
  if (file) {
    file.println("Hello PSRAM!");
    file.println("This is a test file.");
    file.close();
    Serial.println("File written successfully");
  } else {
    Serial.println("Failed to open file for writing");
  }
  
  // Check if file exists
  if (PSRAM.exists("test.txt")) {
    Serial.println("test.txt exists");
  }
  
  // Read from the file
  file = PSRAM.open("test.txt", FILE_READ);
  if (file) {
    Serial.println("File contents:");
    while (file.available()) {
      Serial.write(file.read());
    }
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }
  
  // Remove the file
  if (PSRAM.remove("test.txt")) {
    Serial.println("File removed");
  } else {
    Serial.println("Failed to remove file");
  }
}

void loop() {
  // Nothing here
}