// Generate an LED gamma8-correction table for Arduino sketches.
// Written in Processing (www.processing.org), NOT for Arduino!
// Copy-and-paste the program's output into an Arduino sketch.

#include <iostream>
#include <cmath>
using namespace std;
 
float gamma8   = 2.8; // Correction factor
int   max_in  = 100, // Top end of INPUT range
      max_out = 1024; // Top end of OUTPUT range
 
int main (int argc, char** argv) {
  cout << "const uint8_t PROGMEM gamma8[] = {";
  for(int i=0; i<=max_in; i++) {
    if(i > 0) cout << ',';
    if((i & 15) == 0) cout << "\n  ";
    cout << (int)(pow((float)i / (float)max_in, gamma8) * max_out + 0.5);
  }
  cout << (" };\n");
}
