// PostTest.cc
// test suit for Post object
// wpf 2015-07-28 10h31min

#include "PostTest.h"
//#include "MyStream.h"
#include "Parser.h"
#include "util.h"

#include <iostream>
#include <string>
#include <vector>

#include <chrono>
#include <ctime>
#include <string>

// -----------------------------------------------------------------
void PostTest::doTest()
{ 
//  extern std::ostream cout2;
  post.msg << "PostTest\n";
  post.msg << utl::time_string("%Y-%m-%d %Hh %Mmin");

  post.msg << "\nthings...\n";
  post.msg << "Flags\n";
  post.msg << " Initial values\n";
  post.msg << "  post.msg.msg.copy_buff_to_urge = .\n";
  post.msg << "  post.msg.msg.copy_urge_to_buff = .\n";
  post.msg << "  post.msg.msg.allow_urge = .\n";
  post.msg << "  post.msg.msg.allow_buff = .\n";
}
// eof PostTest.cc
