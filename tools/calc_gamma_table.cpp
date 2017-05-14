//
// Compile with
//   g++ calc_gamma_table.cpp 
//

#include <iostream>
#include <cmath>
using namespace std;
 
float gamma8   = 2.8; // Gamma correction factor
int   max_in  = 100, // Maximum input value, e.g 100 for 100%
      max_out = 1024; // Maximum output value. Set this to PWMRANGE of your arduino-compatible device, e.g 1024 for ESP8266
 
int main (int argc, char** argv) {
  cout << "const uint8_t PROGMEM gamma8[] = {";
  for(int i=0; i<=max_in; i++) {
    if(i > 0) cout << ',';
    if((i & 15) == 0) cout << "\n  ";
    cout << (int)(pow((float)i / (float)max_in, gamma8) * max_out + 0.5);
  }
  cout << (" };\n");
}
