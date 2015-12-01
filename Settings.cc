// Settings.cc
// settings definitions
// wpfernandes 2015-11-15 10h00min

#include "Settings.h"
#include <iostream> // todo: remove this
#include <string>
#include <fstream>
#include <limits>
#include <map>

void Settings::debug(std::string file_name, int line)
{ std::cout << "Debug: " << file_name << ":" << line << '\n';
  for (auto& pair:keys_)
  { std::cout << "       " << pair.first << ":" << static_cast<int>(pair.second)
              << ": " << settings_[pair.second] << '\n';
  }
  std::cout << '\n';
}

void Settings::predefine_settings()
{ keys_["image_file_extension_for_image_type_pdf"] =
      kv::IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_PDF;
  settings_[kv::IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_PDF] = "pdf";

  keys_["image_file_extension_for_image_type_png"] =
      kv::IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_PNG;
  settings_[kv::IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_PNG] = "png";

  keys_["image_file_extension_for_image_type_unknown"] =
      kv::IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_UNKNOWN;
  settings_[kv::IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_UNKNOWN] = "unknown";

  keys_["script_file_extension_for_plot_program_pyxplot"] =
      kv::SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_PYXPLOT;
  settings_[kv::SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_PYXPLOT] = "ppl";

  keys_["script_file_extension_for_plot_program_gnuplot"] =
      kv::SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_GNUPLOT;
  settings_[kv::SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_GNUPLOT] = "gpi";

  keys_["script_file_extension_for_plot_program_unknown"] =
      kv::SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_UNKNOWN;
  settings_[kv::SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_UNKNOWN] = "unknown";

  keys_["data_file_extension_for_plot_program_pyxplot"] =
      kv::DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_PYXPLOT;
  settings_[kv::DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_PYXPLOT] = "dat";

  keys_["data_file_extension_for_plot_program_gnuplot"] =
      kv::DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_GNUPLOT;
  settings_[kv::DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_GNUPLOT] = "dat";

  keys_["data_file_extension_for_plot_program_unknown"] =
      kv::DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_UNKNOWN;
  settings_[kv::DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_UNKNOWN] = "unknown";

  keys_["terminal_for_image_type_pdf_for_plot_program_pyxplot"] =
      kv::TERMINAL_FOR_IMAGE_TYPE_PDF_FOR_PLOT_PROGRAM_PYXPLOT;
  settings_[kv::TERMINAL_FOR_IMAGE_TYPE_PDF_FOR_PLOT_PROGRAM_PYXPLOT] = "pdf";

  keys_["terminal_for_image_type_pdf_for_plot_program_gnuplot"] =
      kv::TERMINAL_FOR_IMAGE_TYPE_PDF_FOR_PLOT_PROGRAM_GNUPLOT;
  settings_[kv::TERMINAL_FOR_IMAGE_TYPE_PDF_FOR_PLOT_PROGRAM_GNUPLOT] = "pdf";

  keys_["terminal_for_image_type_png_for_plot_program_pyxplot"] =
      kv::TERMINAL_FOR_IMAGE_TYPE_PNG_FOR_PLOT_PROGRAM_PYXPLOT;
  settings_[kv::TERMINAL_FOR_IMAGE_TYPE_PNG_FOR_PLOT_PROGRAM_PYXPLOT] = "png";

  keys_["terminal_for_image_type_png_for_plot_program_gnuplot"] =
      kv::TERMINAL_FOR_IMAGE_TYPE_PNG_FOR_PLOT_PROGRAM_GNUPLOT;
  settings_[kv::TERMINAL_FOR_IMAGE_TYPE_PNG_FOR_PLOT_PROGRAM_GNUPLOT] = "png";

  keys_["terminal_for_image_type_unknown_for_plot_program_pyxplot"] =
      kv::TERMINAL_FOR_IMAGE_TYPE_UNKNOWN_FOR_PLOT_PROGRAM_PYXPLOT;
  settings_[kv::TERMINAL_FOR_IMAGE_TYPE_UNKNOWN_FOR_PLOT_PROGRAM_PYXPLOT] =
      "unknown";

  keys_["terminal_for_image_type_unknown_for_plot_program_gnuplot"] =
      kv::TERMINAL_FOR_IMAGE_TYPE_UNKNOWN_FOR_PLOT_PROGRAM_GNUPLOT;
  settings_[kv::TERMINAL_FOR_IMAGE_TYPE_UNKNOWN_FOR_PLOT_PROGRAM_GNUPLOT] =
      "unknown";

  keys_["plot_program"] = kv::PLOT_PROGRAM;
  settings_[kv::PLOT_PROGRAM] = "pyxplot";

  keys_["image_type"] = kv::IMAGE_TYPE;
  settings_[kv::IMAGE_TYPE] = "pdf";

  keys_["load_mode"] = kv::LOAD_MODE;
  settings_[kv::LOAD_MODE] = "auto";

  keys_["write_mode"] = kv::WRITE_MODE;
  settings_[kv::WRITE_MODE] = "script_with_embedded_data";

  keys_["region_infix"] = kv::REGION_INFIX;
  settings_[kv::REGION_INFIX] = "_region_";

  keys_["unsigned_int_maximum_number_of_iteractions"] =
      kv::UNSIGNED_INT_MAXIMUM_NUMBER_OF_ITERACTIONS;
  settings_[kv::UNSIGNED_INT_MAXIMUM_NUMBER_OF_ITERACTIONS] = "500";

  keys_["default_xpd_directory_name"] =
      kv::DEFAULT_XPD_DIRECTORY_NAME;
  settings_[kv::DEFAULT_XPD_DIRECTORY_NAME] = "raw";

  keys_["config_file_path_and_name"] = kv::CONFIG_FILE_PATH_AND_NAME;
  settings_[kv::CONFIG_FILE_PATH_AND_NAME] = "/home/willians/.xpx.conf";

  keys_["log_error_extracting_angles_from_file_name"] =
      kv::LOG_ERROR_EXTRACTING_ANGLES_FROM_FILE_NAME;
  settings_[kv::LOG_ERROR_EXTRACTING_ANGLES_FROM_FILE_NAME] = "yes";

  keys_["log_error_processing_region"] =
      kv::LOG_ERROR_PROCESSING_REGION;
  settings_[kv::LOG_ERROR_PROCESSING_REGION] = "yes";

  keys_["log_error_processing_file"] =
      kv::LOG_ERROR_PROCESSING_FILE;
  settings_[kv::LOG_ERROR_PROCESSING_FILE] = "yes";

  keys_["log_about_directory"] =
      kv::LOG_ABOUT_DIRECTORY;
  settings_[kv::LOG_ABOUT_DIRECTORY] = "yes";

  keys_["output_directory_name"] =
      kv::OUTPUT_DIRECTORY_NAME;
  settings_[kv::OUTPUT_DIRECTORY_NAME] = "xpx_output";
}

void Settings::load_settings()
{ std::ifstream ifs(settings_[kv::CONFIG_FILE_PATH_AND_NAME]);
  if (ifs)
  { std::string key_str;
    kv key;
    std::string value;
    while (ifs >> key_str >> value)
    { if (key_str.substr(0,1) != "#")
      { std::map<std::string, kv>::const_iterator pos =
            keys_.find(key_str); 
        if (pos == keys_.end())
        { key = static_cast<kv>(number_of_settings_++); 
        }
        else
        { key = pos->second;
        }
        settings_[key] = value;
      }
      ifs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
  }
}

Settings::Settings()
{ predefine_settings();
  load_settings();
  //debug(__FILE__, __LINE__);
}

std::string Settings::operator() (kv key)
{ std::map<kv, std::string>::const_iterator pos =
      settings_.find(key); 
  if (pos == settings_.end())
  { return std::string("key_" + std::to_string(static_cast<int>(key)) +
                       "_not_found");
  }
  return pos->second;
}

std::string Settings::operator() (const std::string& key_str)
{ kv key;
  { std::map<std::string, kv>::const_iterator pos =
        keys_.find(key_str); 
    if (pos == keys_.end())
    { return std::string("key_" + key_str + "_not_found"); 
    }
    key = pos->second;
  }

  return (operator()(key));
}

std::string& Settings::operator[] (const kv& key)
{ std::map<kv, std::string>::iterator pos =
      settings_.find(key); 
  if (pos == settings_.end())
  { return settings_.insert(std::make_pair(key, std::string())).first->second;
  }
  return pos->second;
}


// end of file Settings.cc
