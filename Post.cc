// Post.cc
// messenger from child proccess to father proccess and log
// wpfernandes 2015-08-07 11h03min

#include "Post.h"
#include <streambuf>

// Logger ---------------------------------------------------------------------
std::streambuf::int_type Logger::overflow(int_type c)
{ if (traits_type::eq_int_type(traits_type::eof(), c))
    return traits_type::not_eof(c);
  switch (c)
  {case '\n':
   case '\r': 
   {// prefix = "[FIX]";
    buffer_ += c;
//    if (buffer_.size() > 1)
//    sb_p_->sputn(prefix.c_str(), prefix.size());
    int_type rc = sb_p_->sputn(buffer_.c_str(), buffer_.size());
    buffer_.clear();
    return rc;
   }
   default:
    buffer_ += c;
    return c;
  }
}

// Buf ---------------------------------------------------------------------
std::streambuf::int_type my::Buf::overflow(int_type c)
{ bool eof = false;
  for (std::streambuf *Buf : buffers_)
      eof |= (Buf->sputc(c) == traits_type::eof());
  return eof ? traits_type::eof() : c;
}

// Buf ---------------------------------------------------------------------
my::MyStream::~MyStream()
{ for (auto it_streams : streams_)
  { it_streams->close();
    // Bug: crashes with g++ if delete is allowed to execute.
    //delete it_streams;
  }
}

void my::MyStream::attach(char const *name)
{ std::ofstream *sb_p = new std::ofstream(name);
  streams_.push_back(sb_p);
  outputs_.attach(sb_p->rdbuf());
}

template <typename T, typename...pack>
void my::MyStream::attach(T &t, pack&...p)
{ attach(t);
  attach(p...);
}

// eof Post.cc
