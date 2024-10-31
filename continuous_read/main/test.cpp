

#include <string>
#include <stdio.h>
#include "utillc.h"

auto tt = "abc";
int a = 36;
float b = 3.14;

int main() {
  EKOX(a - b - a - "abc");
  EKOX(a - b - a - "abc");  
  EKOX(b);
  
  for (int ii = 0; ii < 12; ii++) {
    EKOX(a - b);
    EKOX(a);
    EKOX(tt);
    EKO();
    EKOT("coucou");
    EKO();
  }
}
