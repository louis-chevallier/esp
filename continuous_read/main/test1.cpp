#include <string>
#include <stdio.h>
#include "utillc.h"
#include "esp_heap_caps.h"
#include "rtc_wdt.h"
#include "esp_wifi.h"
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
  esp_err_t results = esp_wifi_stop();
  rtc_wdt_protect_off();    // Turns off the automatic wdt service
  rtc_wdt_enable();         // Turn it on manually
  rtc_wdt_set_time(RTC_WDT_STAGE0, 2000000);  // Define how long you desire to let dog wait.
  
  std::string tt = "";
  K;
  heap_caps_check_integrity_all(true);
  K;
  {
    auto vv = (utillc::AA + __FILE__ + ":" + __LINE__ + ": [" + (utillc::millis()-utillc::seko) + "ms] " + utillc::build("a", (utillc::AA + a)) + ".\n").s.c_str();
  }
  K;
  int ss = 0;
  heap_caps_check_integrity_all(true);
  K;
  EKO();
  for (int ii = 0; ii < 2000; ii++) {
    heap_caps_check_integrity_all(true);
    //EKOX(ii);
    //EKOX(a - b - a - "abc");
    heap_caps_check_integrity_all(true);
    //K;
    //EKOX(a - b);// - c - a - a - tt - b - c - a);
    auto xx = utillc::AA + c - a - a - tt - b - c - a;
    //K;
    heap_caps_check_integrity_all(true);
    //EKO();
    //tt += to_string(ii);
    ss += xx.s.size();
    //EKOX(a - b - a - "abc");
  }
  EKOX(ss);
  K;
  heap_caps_check_integrity_all(true);  
}
