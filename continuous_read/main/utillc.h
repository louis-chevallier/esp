#pragma once
#include <string>
#include <stdio.h>
#include <sstream>
#include "esp_timer.h"
#include <vector>
#include <ranges>

using namespace std;

namespace utillc {
  string S;
  
  int millis() {
    return esp_timer_get_time()/1000;
  }
  
  static long seko = millis();

  static vector<string> split(const string &s, const char &sep =',') {
    string r;
    vector<string> strings;
    istringstream f(s);
    while (getline(f, r, sep)) {
      strings.push_back(r);
    }
    return strings;
  }

  
  typedef  char *pc;
  typedef  void *pvc;
  typedef const char * const pcc;

  struct A {
    string s;
    A(const string &t) : s(t){}
    //A(const pc &t) : s(t) {}
    A(const pcc &t) : s(t) {}
    //A(const pvc &t) : s(t) {}
    A(const int &t) : s(to_string(t)) {}
    A(const float &t) : s(to_string(t)) {}
    A(const double &t) : s(to_string(t)) {}
    A(const long int &t) : s(to_string(t)) {}
    A(const long unsigned int &t) : s(to_string(t)) {}
    A(const uint8_t &t) : s(to_string(t)) {}

    template <typename T, int N> A(const T (&v)[N]) {
      s = "[";
      for(int i = 0; i < N; i++) {
        const T& vv = v[i];
        auto d = i>0 ? ", " :  "";
        s = s + d + A(vv).s;
      }
      s = s + "]";
      
    }
    template <typename T> A(const vector<T> &v) {
      s = "[";
      for(int i = 0; i < v.size(); i++) {
        const T& vv = v[i];
        auto d = i>0 ? ", " :  "";
        s = s + d + A(vv).s;
      }
      s = s + "]";      
    }

    A operator,(const A &aa) {
      return A(s + ", " + A(aa).s);
    }

    operator string() const {
      return s;
    }

    A operator-(const A &aa) {
      return A(s + ", " + A(aa).s);
    }

    A operator+(const A &aa) {
      return A(s + A(aa).s);
    }
  };

  static utillc::A AA("");

  static string build(const string &s1, const string &s2) {
    auto l1 = split(s1, '-');
    auto l2 = split(s2, ',');
    string r;
    for (int i = 0; i < l1.size(); i++) {
      const auto &a = l1[i];
      const auto &b = l2[i];
      auto d = i>0 ? " @ " :  "";
      r = r + d + a + "=" + b;
    }
    return r;
  }

#if NOEKO == 1
#define EKOT(x) printf("%s:%d: [%ld ms] %s\n", __FILE__, __LINE__, (utillc::millis()-utillc::seko), x); utillc::seko=utillc::millis()
#define EKOX(x) 
#define EKO() 
#else

  
  
#define E utillc::AA
#define EKOT(x) printf((utillc::AA + __FILE__ + ":" + __LINE__ + ": [" + (utillc::millis()-utillc::seko) + "ms] " + x + ".\n").s.c_str()); utillc::seko=utillc::millis(); fflush(stdout)
#define EKOX(x) printf((utillc::AA + __FILE__ + ":" + __LINE__ + ": [" + (utillc::millis()-utillc::seko) + "ms] " + utillc::build(#x, (utillc::AA + x)) + ".\n").s.c_str()); utillc::seko=utillc::millis() ;fflush(stdout)
#define EKO() printf((utillc::AA + __FILE__ + ":" + __LINE__ + ": [" + (utillc::millis()-utillc::seko) + "ms].\n").s.c_str()); utillc::seko=utillc::millis()  ;fflush(stdout)
#define EK(x) (utillc::AA + " " + utillc::build(#x, (utillc::AA + x))).s
#endif
}
