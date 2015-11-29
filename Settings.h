// Settings.h
// settings declarations
// wpfernandes 2015-11-15 10h00min

#ifndef SETTINGS_H
#define SETTINGS_H
#include <string>
#include <vector>
#include <map>

enum class kv
{ 
  KEY                                                      =  0,
  VALUE                                                    =  1,
  IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_PDF                  =  2,
  IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_PNG                  =  3,
  IMAGE_FILE_EXTENSION_FOR_IMAGE_TYPE_UNKNOWN              =  4,
  SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_PYXPLOT           =  5,
  SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_GNUPLOT           =  6,
  SCRIPT_FILE_EXTENSION_FOR_PLOT_PROGRAM_UNKNOWN           =  7,
  DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_PYXPLOT             =  8,
  DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_GNUPLOT             =  9,
  DATA_FILE_EXTENSION_FOR_PLOT_PROGRAM_UNKNOWN             = 10,
  TERMINAL_FOR_IMAGE_TYPE_PDF_FOR_PLOT_PROGRAM_PYXPLOT     = 11,
  TERMINAL_FOR_IMAGE_TYPE_PDF_FOR_PLOT_PROGRAM_GNUPLOT     = 12,
  TERMINAL_FOR_IMAGE_TYPE_PNG_FOR_PLOT_PROGRAM_PYXPLOT     = 13,
  TERMINAL_FOR_IMAGE_TYPE_PNG_FOR_PLOT_PROGRAM_GNUPLOT     = 14,
  TERMINAL_FOR_IMAGE_TYPE_UNKNOWN_FOR_PLOT_PROGRAM_PYXPLOT = 15,
  TERMINAL_FOR_IMAGE_TYPE_UNKNOWN_FOR_PLOT_PROGRAM_GNUPLOT = 16,
  PLOT_PROGRAM                                             = 17,
  WRITE_MODE                                               = 18,
  IMAGE_TYPE                                               = 19,
  LOAD_MODE                                                = 20,
  PDF                                                      = 21,
  PNG                                                      = 22,
  PPL                                                      = 23,
  GPI                                                      = 24,
  DAT                                                      = 25,
  UNKNOWN                                                  = 26,
  PYXPLOT                                                  = 27,
  GNUPLOT                                                  = 28,
  XPS                                                      = 29,
  XPD                                                      = 30,
  SCRIPT_WITH_EMBEDDED_DATA                                = 31,
  ONLY_SCRIPT                                              = 32,
  ONLY_DATA                                                = 33,
  SCRIPT_AND_DATA                                          = 34,
  REGION_INFIX                                             = 35,
  UNSIGNED_INT_MAXIMUM_NUMBER_OF_ITERACTIONS               = 36,
  DEFAULT_XPD_DIRECTORY_NAME                               = 37,
  CONFIG_FILE_PATH_AND_NAME                                = 38,
  LOG_ERROR_EXTRACTING_ANGLES_FROM_FILE_NAME               = 39,
  LOG_ERROR_PROCESSING_REGION                              = 40,
  LOG_ERROR_PROCESSING_FILE                                = 41,
  LOG_ABOUT_DIRECTORY                                      = 42,
  AUTO                                                     = 43,
  TO_AUTOMATIZE_LAST_KNOWN                                 = 44,
  TO_RESERVE_ROOM                                          = 1023
};

class Settings
{public:
  Settings();
 ~Settings() = default;
 std::string operator() (kv key);
 std::string operator() (const std::string& key);
 std::string& operator[] (const kv& key);
 private:
  void debug(std::string file_name, int line);
  void predefine_settings(); void load_settings();
  int number_of_settings_ = static_cast<int>(kv::TO_AUTOMATIZE_LAST_KNOWN);
  std::map<std::string, kv> keys_;
  std::map<kv, std::string> settings_;
};

#endif
