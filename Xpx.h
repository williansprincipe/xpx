// Xpx.h
// Xpx class declaration
// wpfernandes 2015-11-02 15h00min as RegionTest.h
//             2015-11-23 11h00min

#ifndef XPX_H
#define XPX_H

#include "Settings.h"
#include "Post.h"
#include "LNLSFile.h"

#include <string>
#include <vector>

#include "Poco/Path.h"
#include "Poco/File.h"

class Xpx
{
 public:
  Xpx()=delete;
  Xpx( Settings& settings, Post& post, std::vector<std::string>& args) :
           settings(settings), post(post), args(args) { }
 ~Xpx()=default;

  rvEnum ArgumentsNameExistingFilesOrDirectories();
  rvEnum treatDirectory(Poco::Path xpx_path, Poco::File xpx_file);
  rvEnum treatXPSFile(Poco::Path xpx_path, Poco::File xps_file);
  rvEnum doRun();

  std::vector<LNLSFile>& lnls_files() { return lnls_files_; }

 private:
  Settings& settings;
  Post& post;
  std::vector<std::string>& args;
  std::vector<LNLSFile> lnls_files_;
};

#endif
// end of Xpx.h
