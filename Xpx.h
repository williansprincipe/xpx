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

enum class sEnum {NEGATIVE = -1, POSITIVE = 1};

class LNLSFileMetaInfo
{public:
  LNLSFileMetaInfo(): signal_(sEnum::POSITIVE), theta_(0), phi_(0) { }
 ~LNLSFileMetaInfo()=default;

  const std::string& name() { return name_; }
  std::string& name_ref() { return name_; }
  void name(const std::string& name) { name_ = name; }

  sEnum signal() { return signal_; }
  sEnum& signal_ref() { return signal_; }
  void signal(const sEnum signal) { signal_ = signal; }

  int theta() { return theta_; }
  int& theta_ref() { return theta_; }
  void theta(const int theta) { theta_ = theta; }

  int phi() { return phi_; }
  int& phi_ref() { return phi_; }
  void phi(const int phi) { phi_ = phi; }

 private:
  std::string name_;
  sEnum signal_;
  int theta_;
  int phi_;
};

class Xpx
{
 public:
  Xpx()=delete;
  Xpx( Settings& settings, Post& post, std::vector<std::string>& args) :
           settings(settings), post(post), args(args) { }
 ~Xpx()=default;

  rvEnum ArgumentsNameExistingFilesOrDirectories();
  rvEnum extract_theta(const std::string& base_name,
                       sEnum& signal_ref, int& theta_ref);
  rvEnum extract_phi(const std::string& extension, int& phi_ref);
  void lnls_files_meta_infos_add(LNLSFileMetaInfo f_i)
  { lnls_files_meta_infos_.push_back(f_i);
  }
  rvEnum lnls_files_meta_infos_load(const std::string& path_str);
  rvEnum yesno_is_XPD_directory(std::string path_str);
  rvEnum treatDirectory(Poco::Path xpx_path, Poco::File xpx_file);
  rvEnum treatXPSFile(Poco::Path xpx_path, Poco::File xps_file);
  rvEnum doRun();

  std::vector<LNLSFile>& lnls_files() { return lnls_files_; }

 private:
  Settings& settings;
  Post& post;
  std::vector<std::string>& args;
  std::vector<LNLSFileMetaInfo> lnls_files_meta_infos_;
  std::vector<LNLSFile> lnls_files_;
};

#endif
// end of Xpx.h
