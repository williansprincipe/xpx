// Plot2DTest.h
// Plot2DTest object implementation
// wpf 2015-10-20 18h00min

#ifndef PLOT2DTEST_H
#define PLOT2DTEST_H

#include "Post.h"
#include "Parser.h"
#include <string>
#include <vector>

#include "Plot2D.h"

class Plot2DTest
{
 public:
  Plot2DTest()=delete;
  Plot2DTest(Post& post,
           std::vector<OptionStruct> options,
           std::vector<std::string> args) :
           post(post), options(options), args(args) { }
 ~Plot2DTest()=default;
  bool cin();
  void cout();
  void doTest();
 private:
    Post& post; // always without underline
  std::vector<OptionStruct> options; // always without underline
  std::vector<std::string> args; // always without underline
  std::vector<double> xs_{0,1,2,3,4};
  std::vector<double> ys_{0,8,4,6,2};
  Plot2D plot2d{post,xs_,ys_}; // tested class is never underlined
};

#endif
