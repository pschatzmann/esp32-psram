#include "ESP32-PSRAM.h"


void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  // Select an implementation
  //RingBufferStreamRAM dataLogger(2048);
  //RingBufferStreamPSRAM dataLogger(2048);
  RingBufferStreamHIMEM dataLogger(2048);
  
  // Simulate data logging
  for (int i = 0; i < 10; i++) {
    dataLogger.printf("Sensor reading #%d: Temperature=%.1f°C, Humidity=%.1f%%\n", 
                     i, 20.0 + random(100) / 10.0, 40.0 + random(200) / 10.0);
  }
  
  Serial.printf("Logged %d bytes of data\n", dataLogger.available());
  
  // Process the logged data
  Serial.println("Reading logged data:");
  String line;
  while (dataLogger.available()) {
    char c = dataLogger.read();
    if (c == '\n') {
      Serial.printf("Processing: %s\n", line.c_str());
      line = "";
    } else {
      line += c;
    }
  }
  
}

void loop() {
  // Nothing here
}