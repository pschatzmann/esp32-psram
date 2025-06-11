#include "esp32-psram.h"

void setup() {
  Serial.begin(115200);
  
  // Create a vector in high memory
  VectorHIMEM<int> himem_vector;
  
  // Add elements
  for(int i = 0; i < 1000; i++) {
    himem_vector.push_back(i);
  }
  
  Serial.printf("Vector size: %d\n", himem_vector.size());
  Serial.printf("First element: %d\n", himem_vector.front());
  Serial.printf("Last element: %d\n", himem_vector.back());
}

void loop() {
  // Do nothing
}