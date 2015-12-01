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
#include <memory>

class RsrcIfs
{public:
  // Default constructor
  RsrcIfs()
  : pifs_(new std::ifstream) { }

  // Copy constructor
  RsrcIfs(const RsrcIfs& other) = delete;

  // Move constructor noexcept needed to enable optimizations in containers
  RsrcIfs(RsrcIfs&& other) noexcept : pifs_(std::move(other.pifs_)) { }

  // Destructor. best-practice: explicitly annotated with noexcept
  ~RsrcIfs() noexcept {}

  // Copy assignment operator
  RsrcIfs& operator=(const RsrcIfs& other) = delete;

  // Move assignment operator
  RsrcIfs& operator=(RsrcIfs&& other) = delete;

  std::ifstream& ifs() { return *pifs_; }

 private:
  std::unique_ptr<std::ifstream> pifs_;
};

class LNLSFile
{
 public:
  LNLSFile()=delete;

  LNLSFile(const LNLSFile&) = delete;
  LNLSFile(LNLSFile&&) = default;
  LNLSFile& operator=(const LNLSFile&) = delete;
  LNLSFile& operator=(LNLSFile&&) = default;

  LNLSFile(Settings& settings, Post& post, const std::string& directory,
           const std::string& source_file_name, int theta, int phi)
      : settings(settings),
        post(post),
        directory_(directory),
        source_file_name_(source_file_name),
        path_(Poco::Path(directory, source_file_name)),
        theta_(theta),
        phi_(phi),
        loaded_(false) { }

 ~LNLSFile()=default;

  ctrlEnum open_file_and_read_header();
  ctrlEnum check_for_another_region();
  ctrlEnum load_regions();
  std::vector<Region>& regions() { return regions_; }
  ctrlEnum load();
  ctrlEnum setNRegionAndRegionNameAtEachRegion();
  void genAll();
  void write_all();

  void displaySequencesInfo(Sequences& sequences);
  void displayPlot2DInfo(Plot2D& plot2d);
  void displayPlot2DsInfo(std::vector<Plot2D>& plot2ds);
  void displayRegionInfo(Region& region);
  void displayRegionsInfo();
  ctrlEnum doTest();

  void directory(const std::string& directory)
  { directory_ = directory; }
  const std::string& directory() const { return directory_; }

  void source_file_name(const std::string& source_file_name)
  { source_file_name_ = source_file_name; }
  const std::string& source_file_name() const { return source_file_name_; }

  void theta(const int theta)
  { theta_ = theta; }
  int theta() const { return theta_; }

  void phi(const int phi)
  { phi_ = phi; }
  int phi() const { return phi_; }

  std::string path_to_string() { return path_.toString(); }

  const bool loaded() const { return loaded_; }

  std::ifstream& ifs() { return ifs_.ifs(); }

 private:
  Settings& settings;
  Post& post;
  std::string directory_;
  std::string source_file_name_;
  Poco::Path path_;
  int theta_;
  int phi_;
  bool loaded_;
  std::string experiment_;
  RsrcIfs ifs_;
  std::vector<Region> regions_;
};

#endif
// end of LNLSFile.h
