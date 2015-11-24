// Post.h
// messenger from child proccess to father proccess
// wpfernandes 2015-07-28 17h10min

#ifndef POST_H
#define POST_H

#include <iostream>
#include <streambuf>
#include <vector>
#include <fstream>

class Logger: public std::streambuf // ----------------------------------------
{public:
  Logger(std::streambuf* sb_p): sb_p_(sb_p) {}
 ~Logger() { overflow('\n'); }
 private:
  typedef std::basic_string<char_type> string;
  int_type        overflow(int_type c);
  std::string     prefix;
  std::streambuf* sb_p_;
  string          buffer_;
};

namespace my
{ class Buf : public std::streambuf //----------------------------------------
  {public:
    typedef std::char_traits<char> traits_type;
    typedef traits_type::int_type  int_type;
    Buf() {}
    void attach(std::streambuf *sb_p) { buffers_.push_back(sb_p); }
    void attach(std::ofstream &sb_p) { buffers_.push_back(sb_p.rdbuf()); }
    int_type overflow(int_type c);
   private:
    std::vector<std::streambuf *> buffers_;
  };

  class MyStream : public std::ostream // -------------------------------------
  {public: 
    template <typename...pack>
    MyStream(pack&...p) : std::ostream(&Log_), Log_(&outputs_)
    { attach(p...); }
   ~MyStream();
   private:
    std::vector<std::ofstream *> streams_;
    Buf outputs_;
    Logger Log_; 
    void attach(std::ostream &sb_p) { outputs_.attach(sb_p.rdbuf()); }
    void attach(char const *name);
    template <typename T, typename...pack>
    void attach(T &t, pack&...p);
  };
}

class Post
{public:
  Post() : log("log.log"),
           dbg(std::cout),
           msg(std::cout),
           wrn(std::cerr),
           err(std::cerr)
  { }
 // Post(std::string log_file_name);
 ~Post()=default;
  my::MyStream log;
  my::MyStream dbg;
  my::MyStream msg;
  my::MyStream wrn;
  my::MyStream err;

  void set_log_buf(bool log_buf) { log_buf_ = log_buf; }
  bool log_buf() { return log_buf_; }
  
  void set_dbg_buf(bool dbg_buf) { dbg_buf_ = dbg_buf; }
  bool dbg_buf() { return dbg_buf_; }
  
  void set_msg_buf(bool msg_buf) { msg_buf_ = msg_buf; }
  bool msg_buf() { return msg_buf_; }
  
  void set_wrn_buf(bool wrn_buf) { wrn_buf_ = wrn_buf; }
  bool wrn_buf() { return wrn_buf_; }
  
  void set_err_buf(bool err_buf) { err_buf_ = err_buf; }
  bool err_buf() { return err_buf_; }
  
 private:
  bool log_buf_;
  bool dbg_buf_;
  bool msg_buf_;
  bool wrn_buf_;
  bool err_buf_;
  std::vector<std::string> buffered_;
  std::string last_;
};


#endif
// end of Post.h
