#include "ESP32-PSRAM.h"

void setup() {
  Serial.begin(115200);
  
  // Initialize HIMEM file system
  if (!HIMEM.begin()) {
    Serial.println("HIMEM initialization failed!");
    return;
  }
  
  // Create and write to a file
  auto file = HIMEM.open("test.txt", FILE_WRITE);
  if (file) {
    file.println("Hello HIMEM!");
    file.println("This is a test file.");
    file.close();
    Serial.println("File written successfully");
  } else {
    Serial.println("Failed to open file for writing");
  }
  
  // Check if file exists
  if (HIMEM.exists("test.txt")) {
    Serial.println("test.txt exists");
  }
  
  // Read from the file
  file = HIMEM.open("test.txt", FILE_READ);
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
  if (HIMEM.remove("test.txt")) {
    Serial.println("File removed");
  } else {
    Serial.println("Failed to remove file");
  }
}

void loop() {
  // Nothing here
}