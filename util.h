// util.h
// convenience
// wpfernandes 2015-10-25 03h00min

#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace utl
{ // begin of global stuff inside namespace utl
  extern const char* default_time_format; // defined in util.cc
  // the overload bellow aim to get better performance for uses
  // of time_string with c_char argument, which I, unfoundedly, believe will be
  // a very common usage inside xpd all files time_stamp reading.
  // Ok. I just realize time_string returns now_time, so it will never be used
  // like that, but I will keep this to reminds me of how to correctly set
  // extern const char* like this.
  std::string time_string(const char* format = default_time_format);
  std::string time_string(std::string format);
  std::istream& safeGetline(std::istream& is, std::string& t);
} // end of namespace utl
#endif


