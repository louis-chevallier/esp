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
  typedef  void *pvc;
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
  template <> string convert(const double &a) {  return to_string(a); }
  template <> string convert(const float &a) {  return to_string(a); }

  template <> string convert(const unsigned int &a) {
    return to_string(a);
  }

  template <> string convert(const long unsigned int &a) {
    return to_string(a);
  }
  template <> string convert(const uint8_t &a) {
    return to_string(a);
  }
  template <typename T> string convert( T * const &a) {
    return to_string((unsigned int &)a);
  }

  template <typename T1, typename T2> struct Su  {   const T1 &t1; const T2 &t2; };
  template <typename T1, typename T2> Su<T1, T2> fufu(const T1 &t1, const T2 &t2) {
    return Su<T1, T2>({t1, t2});
  }
  template <typename T1, typename T2> string convert(const Su<T1, T2> &su) {
    return S + "{ " + convert(su.t1) + ", " + convert(su.t2) + " }";
  }
  
  
#if NOEKO == 1
#define EKOT(x) printf("%s:%d: [%ld ms] %s\n", __FILE__, __LINE__, (utillc::millis()-utillc::seko), x); utillc::seko=utillc::millis()
#define EKOX(x) 
#define EKO() 
#else

#define P(x,y) utillc::fufu(x,y)
#define EKOT(x) printf((utillc::S + utillc::convert(__FILE__) + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms] " + utillc::convert(x) + ".\n").c_str()); utillc::seko=utillc::millis()
#define EKOX(x) printf((utillc::S + __FILE__ + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms] " + #x + "=" + utillc::convert(x) + ".\n").c_str()); utillc::seko=utillc::millis()
#define EKO()   printf((utillc::S + __FILE__ + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms]\n").c_str()); utillc::seko=utillc::millis()

#endif
}
