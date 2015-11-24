// PostTest.h
// tester for Post object
// wpf 2015-07-28 10h31min

#ifndef POSTTEST_H
#define POSTTEST_H

#include "Post.h"
#include "Parser.h"
#include <string>
#include <vector>

class PostTest
{public:
  PostTest()=delete;
  PostTest(Post& post) : post(post) { }
 ~PostTest()=default;
  PostTest(Post& post, std::vector<OptionStruct> options,
           std::vector<std::string> args) :
           post(post), options(options), args(args) { }
  void doTest();
 private:
  Post& post;
  std::vector<OptionStruct> options;
  std::vector<std::string> args;
};

#endif
// eof PostTest.h

