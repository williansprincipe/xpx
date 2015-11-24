// Plot2D.h
// Plot2D object declaration
// wpfernandes 2015-10-20 17h00min

#ifndef PLOT2D_H
#define PLOT2D_H

#include "Post.h"
#include "globalVals.h"
#include "Settings.h"
#include "Poco/Path.h"
#include <string>
#include <vector>

class Sequences;

class Plot2D
{public:
  //  Plot2D()=delete;
  Plot2D(Settings& settings, Post& post,
         Sequences& sequences)
      : settings(settings),
        post(post), sequences_(sequences)
  { }
  ~Plot2D() = default;

  // service member functions =================================================
  rvEnum writePlot();
  rvEnum writePlotAs(const std::string& script_file_name);

  // exposed member functions =================================================
  void script_file_path(Poco::Path script_file_path)
  { script_file_path_ = script_file_path;
  }
  rvEnum script_file_path_parse(std::string script_file_path_string);
  const std::string& script_file_name()
  { return script_file_path_.getFileName();
  }
  void script_file_name(const std::string& script_file_name)
  { script_file_path_.setFileName(script_file_name);
  }
  const std::string script_base_name()
  { return script_file_path_.getBaseName();
  }
  void script_base_name(const std::string& script_base_name)
  { script_file_path_.setBaseName(script_base_name);
  }
  const std::string script_extension()
  { return script_file_path_.getExtension();
  }
  void script_extension(const std::string& script_extension)
  { script_file_path_.setExtension(script_extension);
  }
  std::string script_file_path_toString()
  { return script_file_path_.toString();
  }

  void data_file_path(Poco::Path data_file_path)
  { data_file_path_ = data_file_path;
  }
  rvEnum data_file_path_parse(std::string script_file_path_string);
  const std::string& data_file_name()
  { return data_file_path_.getFileName();
  }
  void data_file_name(const std::string& data_file_name)
  { data_file_path_.setFileName(data_file_name);
  }
  const std::string data_base_name()
  { return data_file_path_.getBaseName();
  }
  void data_base_name(const std::string& data_base_name)
  { data_file_path_.setBaseName(data_base_name);
  }
  const std::string data_extension()
  { return data_file_path_.getExtension();
  }
  void data_extension(const std::string& data_extension)
  { data_file_path_.setExtension(data_extension);
  }
  std::string data_file_path_toString()
  { return data_file_path_.toString();
  }

  const std::string& source_file_name() const { return source_file_name_; }
  void source_file_name(std::string source_file_name)
  { source_file_name_ = source_file_name;
  }

  int region_number() const { return region_number_; }
  void region_number(int region_number) { region_number_ = region_number; }

  const std::string& label() const { return label_; }
  void label(std::string label) { label_ = label; }

  const std::string& image_file_name() const { return image_file_name_; }
  void image_file_name(const std::string& image_file_name)
  { image_file_name_ = image_file_name;
  }

  Sequences& sequences() { return sequences_; }

  void title(std::string title) { title_ = title; }
  void xlabel(std::string xlabel) { xlabel_ = xlabel; }
  void ylabel(std::string ylabel) { ylabel_ = ylabel; }
  void reversed(bool reversed) { reversed_ = reversed; }

  std::string title() { return title_; }
  std::string xlabel() { return xlabel_; }
  std::string ylabel() { return ylabel_; }
  bool reversed() { return reversed_; }

  vsz_t custom_sequences_names_size() { return custom_sequences_names_.size(); }
  bool custom_sequences_names_empty()
  { return custom_sequences_names_.empty();
  }
  std::string custom_sequences_names(vsz_t index)
  { return custom_sequences_names_[index];
  }
  void custom_sequences_names_add(std::string custom_sequence_name)
  { custom_sequences_names_.push_back(custom_sequence_name);
  }

  vsz_t sequences_to_plot_size() { return sequences_to_plot_.size(); }
  bool sequences_to_plot_empty() { return sequences_to_plot_.empty(); }
  vsz_t sequences_to_plot(vsz_t index) { return sequences_to_plot_[index]; }
  void sequences_to_plot_add(vsz_t index)
  { sequences_to_plot_.push_back(index);
  }

 private:
  Settings& settings;
  Post& post;

  // internal member functions used by writePlot()
  rvEnum writeScriptHeader(std::ofstream& ofs);
  rvEnum writeScript(std::ofstream& ofs);
  rvEnum writeData(std::ofstream& ofs);
  rvEnum writeEmbeddedData(std::ofstream& ofs);

  Sequences& sequences_;
  std::vector<std::string>  custom_sequences_names_;

  Poco::Path script_file_path_;
  Poco::Path data_file_path_;
  std::string source_file_name_;
  int region_number_;
  std::string label_;
  std::string image_file_name_;

  std::string title_;
  std::string xlabel_;
  std::string ylabel_;
  bool reversed_;
  std::vector<vsz_t> sequences_to_plot_;



};
#endif
