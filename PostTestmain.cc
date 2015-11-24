// PostTestmain.cc
// test suit for Post object
// wpf 2015-07-31 16h10min

//#include "MyStream.h"
//#include <iostream>
#include "PostTest.h"
// #include "Parser.h"
//#include "Post.h"

void addOptionsAndArgumentsControls(Parser& parser)
{ parser.addControl("-h",vtNONE,omaskNONE,vvMUSTNOTEXIST,
    "Error! Option '-h' doesn't need and doesn' admit value.\n");
  parser.addControl("--help",vtNONE,omaskNONE,vvMUSTNOTEXIST,
    "Error! Option '--help' doesn't need and doesn' admit value.\n");
  parser.addControl("-v",vtNONE,omaskNONE,vvMUSTNOTEXIST,
    "Error! Option '-v' doesn't need and doesn' admit value.\n");
  parser.addControl("--verbose",vtNONE,omaskNONE,vvMUSTNOTEXIST,
    "Error! Option '--verbose' doesn't need and doesn' admit value.\n");
  parser.addControl("-f",vtNONE,omaskNONE,vvMUSTEXIST,
    "Error! Option '-f' needs a value, which could not be found.\n");
  parser.addControl("--createlogfile",vtNONE,omaskNONE,vvMUSTNOTEXIST,
    "Error!  Option \"--createlogfile\" doesn't need and doesn' admit value.\n");
//  parser.addArgControl(1,vtNONE,fvMUSTEXIST,
//    "Error! First argument must be a file name and this file must exist.\n");
}

//=============================================================================

//  std::ostream cout2(std::cout.rdbuf());
/*
    my::MyStream my::log("log.log");
    my::MyStream my::dbg(std::cout);
    my::MyStream my::msg(std::cout);
    my::MyStream my::wrn(std::cerr);
    my::MyStream my::err(std::cerr);
*/
int main(int argc, char* argv[])
{ 
  Post post;
  post.set_dbg_buf(false);
  Parser parser(post);
  addOptionsAndArgumentsControls(parser);
  std::vector<std::string> cmd_line_args(argv,argv+argc);
  parser.parse(cmd_line_args);
  parser.preproccessOptions();

  PostTest post_test(post, parser.options(), parser.args());
//  PostTest post_test(post);
  post_test.doTest();

  return 0;
}
// eof PostTestmain.cc
