// Xpxmain.cc
// xpx xpxotoelectron main
// wpfernandes 2015-11-23 14h15min

#include "Xpx.h"
#include "Parser.h"
#include "Post.h"
#include "Help.h"
#include "Settings.h"

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
{ Settings settings;
  Post post;
  Help help(post);
  Parser parser(settings, post, help);
  addOptionsAndArgumentsControls(parser);
  std::vector<std::string> cmd_line_args(argv,argv+argc);
  { rvEnum run_ctrl = parser.parse(cmd_line_args);
    if (run_ctrl == CTRL_STOP)
    { return SUCCESS;
    }
    if (run_ctrl != SUCCESS)
    { return run_ctrl;
    }
  }
  Xpx xpx(settings, post, parser.args());
  return (xpx.doRun());
}
// eof Xpxmain.cc
