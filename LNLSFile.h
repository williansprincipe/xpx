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
  LNLSFile(Settings& settings, Post& post, const Poco::Path& directory, const std::string& source_file_name)
      : settings(settings),
        post(post),
        path_(Poco::Path(directory,source_file_name)),
        source_file_name_(source_file_name),
        loaded_(false) { }
 ~LNLSFile()=default;

  ctrlEnum open_file_and_read_header();
  ctrlEnum check_for_another_region();
  ctrlEnum load_regions();
  ctrlEnum load();
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

  std::string path_to_string() { return path_.toString(); }

  const bool loaded() const { return loaded_; }

 private:
  Settings& settings;
  Post& post;
  Poco::Path path_;
  std::string source_file_name_;
  bool loaded_;
  std::string experiment_;
  std::ifstream ifs_;
  std::vector<Region> regions_;
};

#endif
// end of LNLSFile.h
