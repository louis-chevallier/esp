

#include <string>
#include <stdio.h>


using namespace std;

namespace utillc {
  string S;
  
  int millis() {
    return 0;
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

  template <> string convert(const double &a) {  return to_string(a); }
  template <> string convert(const float &a) {  return to_string(a); }


  template <> string convert(const long int &a) {
    return to_string(a);
  }

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
  
  string ss = utillc::S +  utillc::convert(__LINE__) + convert(__FILE__) + ";;";

  string sss = convert("abc");
  

#define P(x,y) utillc::fufu(x,y)
  
#if NOEKO == 1
#define EKOT(x) 
#define EKOX(x) 
#define EKO() 
#else
#define EKOT(x) printf((utillc::S + utillc::convert(__FILE__) + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms] " + utillc::convert(x) + ".\n").c_str()); utillc::seko=utillc::millis()
#define EKOX(x) printf((utillc::S + __FILE__ + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms] " + #x + "=" + utillc::convert(x) + ".\n").c_str()); utillc::seko=utillc::millis()
#define EKO()   printf((utillc::S + __FILE__ + ":" + utillc::convert(__LINE__) + ": [" + utillc::convert(utillc::millis()-utillc::seko) + "ms]\n").c_str()); utillc::seko=utillc::millis()
#endif
}


int a = 36;
float b = 3.14;

int main() {
  EKOX(P(a, P(b, a)));
  EKOX(a);
}
