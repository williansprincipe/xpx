// Plot2DTestmain.cc
// Plot2DTestmain
// wpf 2015-10-20 18h00min

#include "Plot2DTest.h"
#include "Parser.h"
#include "Post.h"

void addOptionsAndArgumentsControls(Parser& parser)
{ parser.addControl("-h",vtNONE,omaskNONE,vvMUSTNOTEXIST,"Error! Option \
\"-h\" doesn't need and doesn' admit value.\n");
  parser.addControl("--help",vtNONE,omaskNONE,vvMUSTNOTEXIST,"Error! Option \
\"--help\" doesn't need and doesn' admit value.\n");
  parser.addControl("-v",vtNONE,omaskNONE,vvMUSTNOTEXIST,"Error! Option \
\"-v\" doesn't need and doesn' admit value.\n");
  parser.addControl("--verbose",vtNONE,omaskNONE,vvMUSTNOTEXIST,"Error! \
Option \"--verbose\" doesn't need and doesn' admit value.\n");
  parser.addControl("-f",vtNONE,omaskNONE,vvMUSTEXIST,"Error! Option \"-f\" \
needs a value, which could not be found.\n");
  parser.addControl("--createlogfile",vtNONE,omaskNONE,vvMUSTNOTEXIST,"Error! \
Option \"--createlogfile\" doesn't need and doesn' admit value.\n");
// parser.addArgControl(1,vtNONE,fvMUSTEXIST,"Error! First argument must be 
// a file name and this file must exist.\n");
}

//=============================================================================
int main(int argc, char* argv[])
{ 
  Post post;
  Parser parser(post);
  addOptionsAndArgumentsControls(parser);
  std::vector<std::string> cmd_line_args(argv,argv+argc);
  parser.parse(cmd_line_args);
  parser.preproccessOptions();

  Plot2DTest plot2d_test(post, parser.options(), parser.args());
  plot2d_test.doTest();

  return 0;
}
// eof Plot2DTestmain.cc
