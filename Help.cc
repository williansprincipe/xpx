// Help.cc
// Help class implementation
// wpfernandes 2015-11-23 22h14min

#include "Help.h"
#include "Post.h"

void Help::header()
{ post.msg << "xpx -- XPS and XPD Graphics from LNLS data.\n"
           << "Version 0.07 -- 2015-11-14 -- W. P. Fernandes.\n";
}

void Help::usage()
{
  post.msg << "Usage: xpx [options] file_or_directory\n"
           << "Run xpx -h for more information on options.\n";
  post.msg
      << "The output is(are) scrypt file(s) for pyxplot or gnuplot.\n"
      << "xpx accepts multiple arguments in one run.\n"
      << "IMPORTANT: xpx OVERWRITES old scrypts without warnings.\n";
}

void Help::brief()
{ header();
  usage();
}

void Help::detailed()
{ header();
  usage();
  post.msg << "\nxpx help\n    Options\n"
           << "       -h or --help displays this help.\n";
}
// end of Help.cc

