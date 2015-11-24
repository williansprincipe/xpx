// Help.h
// Help class declaration
// wpfernandes 2015-11-23 22h14min

#ifndef HELP_H
#define HELP_H

#include "Post.h"

class Help
{public:
  Help()=delete;
  Help(Post& post): post(post) { }
 ~Help()=default;
  void header();
  void usage();
  void brief();
  void detailed();

 private:
  Post& post;
};

#endif
