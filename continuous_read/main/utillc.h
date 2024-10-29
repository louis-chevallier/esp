#pragma once
#include <string>
#include <stdio.h>
#include "esp_timer.h"

using namespace std;

namespace utillc {
  string S;
  
  int millis() {
    return esp_timer_get_time()/1000;
  }
  
  long seko = millis();
  
  template <class A> string convert(const A &a) {
    return string(a);
  }

  typedef  char *pc;
  typedef  char * const pcc;
  
  template <> string convert(const pc &a) {
    return string(a);
  }

  template <> string convert(const int &a) {
    return to_string(a);
  }

  template <> string convert(const long int &a) {
    return to_string(a);
  }

  template <> string convert(const unsigned int &a) {
    return to_string(a);
  }
  
  
  string ss = utillc::S +  utillc::convert(__LINE__) + convert(__FILE__) + ";;";

  string sss = convert("abc");
  


  
#if NOEKO == 1
#define EKOT(x) 
#define EKOX(x) 
#define EKO() 
#else
#define EKOT(x) printf((utillc::S + utillc::convert(__FILE__) + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms] " + utillc::convert(x) + ".\n").c_str()); utillc::seko=utillc::millis()
#define EKOX(x) printf((utillc::S + __FILE__ + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms] " + #x + "=" + utillc::convertconvert(x) + ".\n").c_str()); utillc::seko=utillc::millis()
#define EKO()   printf((utillc::S + __FILE__ + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms]\n").c_str()); utillc::seko=utillc::millis()
#endif
}
