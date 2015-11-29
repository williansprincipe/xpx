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

  const std::string& name() const { return name_; }
  std::string& name_ref() { return name_; }
  void name(const std::string& name) { name_ = name; }

  sEnum signal() const { return signal_; }
  sEnum& signal_ref() { return signal_; }
  void signal(const sEnum signal) { signal_ = signal; }

  int theta() const { return theta_; }
  int& theta_by_ref() { return theta_; }
  void theta(const int theta) { theta_ = theta; }
  
  int phi() const { return phi_; }
  int& phi_by_ref() { return phi_; }
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

  ctrlEnum ArgumentsNameExistingFilesOrDirectories();
  bool is_two_chars_a_month(char c1, char c2);
  bool is_lnls_file_name(const std::string& file_name);
  int lnls_files_count(std::string path_str);
  ctrlEnum get_theta_from_base_name(const std::string& base_name,
                       sEnum& signal_ref, int& theta_by_ref);
  ctrlEnum get_phi_from_extension(const std::string& extension, int& phi_by_ref);
  void lnls_files_meta_infos_add(LNLSFileMetaInfo f_i)
  { lnls_files_meta_infos_.push_back(f_i);
  }
  ctrlEnum lnls_files_meta_infos_load(const std::string& path_str,
                                      ctrlEnum yesno_error_tolerant,
                                      ctrlEnum yesno_log_error);
  ctrlEnum yesno_is_XPD_directory(std::string path_str);
  ctrlEnum do_pizza_with_lnls_files_meta_infos();
  ctrlEnum do_all_xps_graphs_from_this_xpx_path();
  ctrlEnum treatDirectory(Poco::Path xpx_path, Poco::File xpx_file);
  ctrlEnum treatXPSFile(Poco::Path xpx_path, Poco::File xps_file);
  ctrlEnum doRun();

  void n_processed_files(const int n_processed_files)
  { n_processed_files_ = n_processed_files;
  }
  const int n_processed_files() { return n_processed_files_; }
  int plusplus_n_processed_files() { return ++n_processed_files_; }

  int theta_min() const { return theta_min_; }
  void theta_min(const int a_theta) { theta_min_ = a_theta; }

  int theta_max() const { return theta_max_; }
  void theta_max(const int a_theta) { theta_max_ = a_theta; }

  int phi_min() const { return phi_min_; }
  void phi_min(const int a_phi) { phi_min_ = a_phi; }

  int phi_max() const { return theta_max_; }
  void phi_max(const int a_phi) { phi_max_ = a_phi; }

  vsz_t lnls_files_meta_infos_size() { return lnls_files_meta_infos_.size(); }
  void lnls_files_meta_infos_sort();
  const std::vector<LNLSFileMetaInfo>& lnls_files_meta_infos()
  { return lnls_files_meta_infos_;
  }

  std::vector<LNLSFile>& lnls_files() { return lnls_files_; }

 private:
  Settings& settings;
  Post& post;
  std::vector<std::string>& args;
  int n_processed_files_;
  int theta_min_;
  int theta_max_;
  int phi_min_;
  int phi_max_;
  std::vector<LNLSFileMetaInfo> lnls_files_meta_infos_;
  std::vector<LNLSFile> lnls_files_;
};

#endif
// end of Xpx.h
