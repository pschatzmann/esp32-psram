#include "esp32-psram.h"

void setup() {
  Serial.begin(115200);
  
  // Create a vector in PSRAM
  VectorPSRAM<int> psram_vector;
  
  // Add elements
  for(int i = 0; i < 1000; i++) {
    psram_vector.push_back(i);
  }
  
  Serial.printf("Vector size: %d\n", psram_vector.size());
  Serial.printf("First element: %d\n", psram_vector.front()); // or psram_vector[0]
  Serial.printf("Last element: %d\n", psram_vector.back());
}

void loop() {
  // Do nothing
}