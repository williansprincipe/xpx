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

enum class signalEnum {NEGATIVE = -1, POSITIVE = 1};

class LNLSFileMetaInfo
{public:
  LNLSFileMetaInfo(): signal_(signalEnum::POSITIVE), theta_(0), phi_(0) { }
 ~LNLSFileMetaInfo()=default;

  const std::string& name() const { return name_; }
  std::string& name_ref() { return name_; }
  void name(const std::string& name) { name_ = name; }

  signalEnum signal() const { return signal_; }
  signalEnum& signal_by_ref() { return signal_; }
  void signal(const signalEnum signal) { signal_ = signal; }

  int theta() const { return theta_; }
  int& theta_by_ref() { return theta_; }
  void theta(const int theta) { theta_ = theta; }
  
  int phi() const { return phi_; }
  int& phi_by_ref() { return phi_; }
  void phi(const int phi) { phi_ = phi; }

 private:
  std::string name_;
  signalEnum signal_;
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
  int lnls_files_count();
  int lnls_files_size() { return lnls_files_.size(); }
  ctrlEnum get_theta_from_base_name(const std::string& base_name,
                                    ctrlEnum expected_signal,
                                    ctrlEnum yesno_log_error,
                                    signalEnum& signal_by_ref,
                                    int& theta_by_ref);
  ctrlEnum get_phi_from_extension(const std::string& extension, int& phi_by_ref);
  void lnls_files_meta_infos_add(LNLSFileMetaInfo f_i)
  { lnls_files_meta_infos_.push_back(f_i);
  }
  ctrlEnum lnls_files_meta_infos_load(ctrlEnum yesno_error_tolerant,
                                      ctrlEnum yesno_log_error);
  ctrlEnum yesno_is_xpd_directory();
  ctrlEnum lnls_files_add(LNLSFileMetaInfo f_i);
  void lnls_files_add(LNLSFile lnls_file)
  { lnls_files_.push_back(std::move(lnls_file));
  }
  ctrlEnum lnls_files_write_all();
  ctrlEnum write_pizza();
  ctrlEnum do_pizza_with_lnls_files_meta_infos();
  ctrlEnum do_all_xps_graphs_from_this_xpx_path();
  ctrlEnum treat_instant_input_directory();
  ctrlEnum treatXPSFile(Poco::Path xpx_path, Poco::File xps_file);
  ctrlEnum doRun();

  void n_processed_files(const int n_processed_files)
  { n_processed_files_ = n_processed_files;
  }
  const int n_processed_files() const { return n_processed_files_; }
  int plusplus_n_processed_files() { return ++n_processed_files_; }

  const int n_positive() const { return n_positive_; }
  int plusplus_n_positive() { return ++n_positive_; }

  const int n_negative() { return lnls_files_meta_infos_size() - n_positive(); }

  const int theta_min() const { return theta_min_; }
  void theta_min(const int theta_min) { theta_min_ = theta_min; }

  const int theta_max() const { return theta_max_; }
  void theta_max(const int theta_max) { theta_max_ = theta_max; }

  const int phi_min() const { return phi_min_; }
  void phi_min(const int phi_min) { phi_min_ = phi_min; }

  const int phi_max() const { return phi_max_; }
  void phi_max(const int phi_max) { phi_max_ = phi_max; }

  const Poco::Path& current_directory() const { return current_directory_; }
  void current_directory(const int current_directory)
  { current_directory_ = current_directory;
  }

  std::string instant_input_directory_to_string() const
  { return instant_input_directory_.toString();
  }
  const Poco::Path& instant_input_directory() const
  { return instant_input_directory_;
  }
  void instant_input_directory(const Poco::Path& instant_input_directory)
  { instant_input_directory_ = instant_input_directory;
  }

  const Poco::Path& instant_output_directory() const
  { return instant_output_directory_;
  }
  void instant_output_directory(const int instant_output_directory)
  { instant_output_directory_ = instant_output_directory;
  }

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
  Poco::Path current_directory_;
  Poco::Path instant_input_directory_;
  Poco::Path instant_output_directory_;
  // ensemble of lnls files meta information
  int n_positive_; // fotons from lnls beam line
  // int n_negative_; // fotons from convencional x-ray gun
  // n_negative_ = lnls_files_meta_infos_.size() - n_positive
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
