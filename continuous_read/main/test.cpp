#include <string>
#include <stdio.h>
#include "utillc.h"
#include "esp_heap_caps.h"

auto tt = "abc";
int a = 36;
float b = 3.14;
std::string c = "xyz";

#define K   printf("%s:%d:\n", __FILE__, __LINE__); fflush(stdout)

#if ESP==1
extern "C" { void app_main(); }
void app_main()
#else
int main()  
#endif
{
  std::string tt = "";
  K;
  heap_caps_check_integrity_all(true);
  K;
  {
    auto vv = (utillc::AA + __FILE__ + ":" + __LINE__ + ": [" + (utillc::millis()-utillc::seko) + "ms] " + utillc::build("a", (utillc::AA + a)) + ".\n").s.c_str();
  }
  K;
  
  heap_caps_check_integrity_all(true);
  K;
  for (int ii = 0; ii < 122; ii++) {
    heap_caps_check_integrity_all(true);
    EKOX(ii);
    //EKOX(a - b - a - "abc");
    heap_caps_check_integrity_all(true);
    K;
    EKOX(a - b - c - a - a - tt - b - c - a);
    K;
    heap_caps_check_integrity_all(true);
    EKO();
    tt += to_string(ii);
    
    heap_caps_check_integrity_all(true);    
    EKOT("coucou");
    K;
    heap_caps_check_integrity_all(true);    
    return;
  }
  EKO();
  /*
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
  */
}
