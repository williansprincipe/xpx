// util.cc
// convenience
// wpfernandes 2015-10-25 03h00min

#include "util.h"
#include "globalVals.h"
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>

namespace utl
{

 /* extern */ const char* default_time_format = "%Y%m%d%H%M%S";

//begin of global stuff inside namespace util ---------------------------------
//  std::string time_string(std::string format) // format is optional
  std::string time_string(const char* format) // format is optional
  { std::chrono::time_point<std::chrono::system_clock> time;
    time = std::chrono::system_clock::now();
    std::time_t time_t_time = std::chrono::system_clock::to_time_t(time);
    char buffer_c_str[100];
    std::string time_string;
    if (strftime(buffer_c_str,100,format,std::localtime(&time_t_time)))
    { time_string = buffer_c_str;
    }
    else
    { time_string = "DateAndTimeUnavailable";
    }
  return time_string;
  }

  std::string time_string(std::string format)
 { return utl::time_string(format.c_str());
 }

// the function safeGetline bellow was authored by user763305.
// from: http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
// Post date: 20130121. Retrieve date: 20151107
std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

} // end of namespace utl
