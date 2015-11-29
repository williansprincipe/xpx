// LNLSFile.h
// LNLSFile class declaration
// wpfernandes 2015-11-23 10h50min

#ifndef LNLSFILE_H
#define LNLSFILE_H

#include "Settings.h"
#include "Post.h"
#include "Parser.h"
#include "Region.h"

#include <string>
#include <vector>

class LNLSFile
{
 public:
  LNLSFile()=delete;
  LNLSFile( Settings& settings, Post& post, std::vector<std::string> args) :
           settings(settings), post(post), args(args) { }
 ~LNLSFile()=default;

  ctrlEnum read_header();
  ctrlEnum check_for_another_region();
  ctrlEnum read_regions();
  ctrlEnum load_file();
  ctrlEnum setNRegionAndRegionNameAtEachRegion();
  void genAll();
  void writeAll();

  void displaySequencesInfo(Sequences& sequences);
  void displayPlot2DInfo(Plot2D& plot2d);
  void displayPlot2DsInfo(std::vector<Plot2D>& plot2ds);
  void displayRegionInfo(Region& region);
  void displayRegionsInfo();
  ctrlEnum doTest();

  void source_file_name(const std::string& source_file_name)
  { source_file_name_ = source_file_name; }
  const std::string& source_file_name() const { return source_file_name_; }

 private:
  Settings& settings;
  Post& post;
  std::vector<OptionStruct> options;
  std::vector<std::string> args;
  std::string source_file_name_;
  std::string experiment_;
  std::ifstream ifs_;
  std::vector<Region> regions_;
};

#endif
// end of LNLSFile.h
