// Parser.h
// Options and Arguments from command line Parser 
// wpfernandes 2015-07-28 10h45min

#ifndef PARSER_H
#define PARSER_H

#include "Settings.h"
#include "Post.h"
#include "Help.h"
#include "globalVals.h"
#include <string>
#include <list>
#include <vector>
#include <algorithm>

enum valtypeEnum {vtNONE,vtSTRING,vtINT,vtDOUBLE,vtFILE_NAME}; 
// vv = value validation; fv = file validation; dv = directory validation
enum validationEnum {
  vvNONE,
  vvMUSTEXIST,
  vvMUSTNOTEXIST,
  fvMUSTEXIST,
  fvMUSTNOTEXIST,
  fdMUSTEXIST,
  fdMUSTNOTEXIST,
  dvMUSTEXIST}; 

const std::string omaskNONE="";
const std::string omsgNONE ="";
const std::string parser_wildcard="ARGUMENT"; // this wildcard inside error
                                              // msgs is replaced by argv_i
typedef std::vector<std::string> parser_args_type;

struct ControlStruct //-----------------------------------------------
{ std::string option_id;
  valtypeEnum value_type;
  std::string value_mask;
  validationEnum validation;
  std::string error_msg;
};

struct ArgsCtrlStruct //----------------------------------------------
{ int index;
  valtypeEnum value_type;
  validationEnum validation;
  std::string error_msg;
};

struct OptionStruct  //------------------------------------------------
{ OptionStruct() = default;
  OptionStruct(std::string id, std::string value) : id(id), value(value) {}
  ~OptionStruct() = default;
  std::string id;
  std::string value;
};

class Parser //--------------------------------------------------
{public:
  Parser()=delete;
  Parser(Settings& settings, Post& post, Help& help) : settings(settings),
  post(post), help(help) { }
  ~Parser()=default;
  void addControl(std::string option_id,
                  valtypeEnum value_type,
                  std::string value_mask,
                  validationEnum validation,
                  std::string error_msg);
  void addArgControl(int index,
                     valtypeEnum value_type,
                     validationEnum validation,
                     std::string error_msg);
  int gatherOptions(std::vector<std::string>& cmd_line_args_buffer);
  void options_add(std::string id, std::string value)
  { options_.emplace_back(id, value);
  };
  int checkArguments();
  ctrlEnum parse(std::vector<std::string> cmd_line_args_buffer);
  void eraseFirstArgument() { if (!args_.empty()) args_.erase(args_.begin()); }
  bool has_option(std::string option);
  ctrlEnum preproccessOptions();
  std::string replaceArgName(std::string error_msg, const std::string& this_arg);
  std::vector<OptionStruct>::const_iterator optionsBegin()
  { return options_.begin(); }
  std::vector<OptionStruct>::const_iterator optionsEnd() { return options_.end(); }
  std::vector<std::string>::const_iterator argsBegin() { return args_.begin(); }
  std::vector<std::string>::const_iterator argsEnd() { return args_.end(); }
  std::vector<OptionStruct> options() { return options_; }
  std::vector<std::string>& args() { return args_; }
  std::string arg(int index) { return args_[index]; }
  bool options_empty() { return options_.empty(); }
  int options_size() { return options_.size(); }
  bool args_empty() { return args_.empty(); }
  int args_size() { return args_.size(); }
 private:
  Settings& settings;
  Post& post;
  Help& help;
  std::vector<ControlStruct>controls_;
  std::vector<OptionStruct>options_;
  std::list<ArgsCtrlStruct>args_controls_;
  std::vector<std::string>args_;
};
#endif
// end of file Parser.h
