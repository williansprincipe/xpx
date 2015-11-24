// Parser.cc
// Options and Arguments from command line Parser
// wpfernandes 2015-07-28 10h45min

#include "Parser.h"
#include "globalVals.h"
#include "Help.h"

#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include "Poco/File.h"

void Parser::addControl(std::string option_id, valtypeEnum value_type,
                        std::string value_mask, validationEnum validation,
                        std::string error_msg)
{ ControlStruct control;
  control.option_id = option_id;
  control.value_type = value_type;
  control.value_mask = value_mask;
  control.validation = validation;
  control.error_msg = error_msg;
  controls_.push_back(control);
}

void Parser::addArgControl(int index, valtypeEnum value_type,
                           validationEnum validation, std::string error_msg)
{ ArgsCtrlStruct arg_control;
  arg_control.index = index;
  arg_control.value_type = value_type;
  arg_control.validation = validation;
  arg_control.error_msg = error_msg;
  args_controls_.push_back(arg_control);
}

int Parser::gatherOptions(std::vector<std::string>& cmd_line_args_buffer)
{ rvEnum return_value = UNKNOWN;
  if (!controls_.empty())
  { auto it1 = cmd_line_args_buffer.begin();
    while (it1 != cmd_line_args_buffer.end())
    { if (*it1 == "=")
      { it1 = cmd_line_args_buffer.erase(it1);
      }
      else
      { bool breaked = false;  // Please, nothing is broken here!
        auto it2 = controls_.begin();
        while (it2 != controls_.end())
        { std::string potential_option_id =
              it1->substr(0, it2->option_id.size());
          if (potential_option_id == it2->option_id)
          { OptionStruct option;
            option.id = it2->option_id;
            std::string contiguous_options_or_value =
                (*it1).substr(it2->option_id.size());
            if (contiguous_options_or_value.empty())
            { if (it2->validation == vvMUSTEXIST)
              { do
                { it1 = cmd_line_args_buffer.erase(it1);
                } while ((it1 != cmd_line_args_buffer.end()) && (*it1 == "="));
                if (it1 == cmd_line_args_buffer.end())
                { post.dbg << __FILE__ << ":" << __LINE__ << " "
                           << "Error. Option \"" << option.id
                           << "\" is a key that needs a value "
                           << "which was not found.\n";
                  post.err << it2->error_msg;
                  return_value = ERR_OPTION_VALUE_NOT_FOUND;
                  breaked = true;
                  break;
                }
                else
                { option.value = *it1;
                  option.value.erase(std::remove(option.value.begin(),
                                                 option.value.end(), '='),
                                     option.value.end());
                  options_.push_back(option);
                }
              }
              else
              { options_.push_back(option);
              }
              it1 = cmd_line_args_buffer.erase(it1);
              breaked = true;
              break;
            }
            else
            { if (it2->validation == vvMUSTNOTEXIST)
              { option.value = "";
                if (potential_option_id.substr(0, 1) == "-")
                { if (potential_option_id.substr(0, 2) == "--")

                  { post.dbg << __FILE__ << ":" << __LINE__ << " "
                             << "Error. Option \"" << option.id
                             << "\" does not admit any value but the value \""
                             << contiguous_options_or_value
                             << "\" was given.\n";
                    post.err << it2->error_msg;
                    return_value = ERR_UNEXPECTED_OPTION_VALUE;
                    breaked = true;
                    break;
                  }
                  else
                  { *it1 = "-" + contiguous_options_or_value;
                    options_.push_back(option);
                    post.dbg << __FILE__ << ":" << __LINE__ << " "
                             << "Through here. *it1 = '" + *it1 + "'\n";
                    breaked = true;
                    break;
                  }
                }
                else
                { post.dbg << __FILE__ << ":" << __LINE__ << " "
                           << "Error. Option \"" << option.id << "\""
                           << " is does not admit any value but the value \""
                           << contiguous_options_or_value + "\" was given.\n";
                  post.err << it2->error_msg;
                  return_value = ERR_UNEXPECTED_OPTION_VALUE;
                  breaked = true;
                  break;
                }
              }
              else
              { if (it2->validation == vvMUSTEXIST)
                { option.value = contiguous_options_or_value;
                  post.dbg << __FILE__ << ":" << __LINE__ << " "
                           << "option \"" + option.id + "\" received value \""
                           << option.value + "\".\n";
                  it1 = cmd_line_args_buffer.erase(it1);
                  option.value.erase(std::remove(option.value.begin(),
                                                 option.value.end(), '='),
                                     option.value.end());
                  if ((it2->validation == vvMUSTEXIST) &&
                      (option.value.empty()))
                  { if (it1 != cmd_line_args_buffer.end())
                    { while ((it1 != cmd_line_args_buffer.end()) &&
                             (*it1 == "="))
                      { it1 = cmd_line_args_buffer.erase(it1);
                      }
                      if (it1 != cmd_line_args_buffer.end())
                      { option.value = *it1;
                        it1 = cmd_line_args_buffer.erase(it1);
                      }
                    }
                  }
                  option.value.erase(std::remove(option.value.begin(),
                                                 option.value.end(), '='),
                                     option.value.end());
                  if ((it2->validation == vvMUSTEXIST) &&
                      (option.value.empty()))
                  { post.dbg << __FILE__ << ":" << __LINE__ << " "
                             << "Error. Option \"" << option.id
                             << "\" is a key that needs a value "
                             << "which was not found.\n";
                    post.err << it2->error_msg;
                    return_value = ERR_OPTION_VALUE_NOT_FOUND;
                  }

                  options_.push_back(option);
                  breaked = true;
                  break;
                }
              }
            }
          }
          else
          { ++it2;
          }
        }
        if (!breaked)
        { if (it1->substr(0, 1) == "-")
          { post.err << "Error! Unknown option '" + *it1 + "'.\n";
            it1 = cmd_line_args_buffer.erase(it1);
            return_value = ERR_UNKNOWN_OPTION;
          }
          else
          { ++it1;
          }
        }
      }
    }
    args_ = cmd_line_args_buffer;  // what rested
    // return_value = NORMAL;
  }
  return return_value;
}

std::string Parser::replaceArgName(std::string error_msg,
                                   const std::string& this_arg)
{ std::string::size_type pos = error_msg.find(parser_wildcard);
  if (pos != std::string::npos)
  { error_msg.replace(pos, parser_wildcard.size(), this_arg);
  }
  return error_msg;
}

int Parser::checkArguments()
{ rvEnum return_value;
  for (auto& arg_control : args_controls_)
  { if (args_size() > arg_control.index)
    { std::string this_arg = arg(arg_control.index);
      if (arg_control.validation == fvMUSTEXIST)
      { std::ifstream ifs(this_arg);
        if (!ifs.is_open())
        { post.err << replaceArgName(arg_control.error_msg, this_arg);
          post.dbg << "Error. The argument \"" << this_arg
                   << "\" should be the name of a file, but such "
                   << "file does not exist.\n";
          return_value = ERR_FILE_NOT_FOUND;
        }
        else
        { ifs.close();
        }
      }
      else
      { if (arg_control.validation == fvMUSTNOTEXIST)
        { std::ifstream ifs(this_arg);
          if (ifs.is_open())
          { post.err << replaceArgName(arg_control.error_msg, this_arg);
            post.dbg << "Error. The argument \"" << this_arg
                     << "\" should not be a name of an existing file, but"
                     << " there is already a file with this name.\n";
            return_value = ERR_FILE_ALREADY_EXISTS;
            // todo change for Poco Path and test for existing directory too.
          }
          else
          { ifs.close();
          }
        }
        else
        { if (arg_control.validation == dvMUSTEXIST)
          { Poco::File input_path(this_arg);
            if (input_path.exists())
            { post.log << "Check 1 of 2 ok: argument \"" << this_arg
                       << "\" exists.\n\n";
              if (input_path.isDirectory())
              { post.log << "Check 2 of 2 ok: argument \"" << this_arg
                         << "\" is a directory.\n";
              }
              else
              { post.log << "Check 2 of 2 failed: argument \"" << this_arg
                         << "\" exidts but it is not a directory.\n";
                post.err << replaceArgName(arg_control.error_msg, this_arg);
                post.dbg << "Error. The argument \"" << this_arg
                         << "\" should not be a name of an existing "
                         << "directory. In fact the name is being used, "
                         << "but is not a a directory.\n";
                return_value = ERR_NOT_A_DIRECTORY;
              }
            }
            else
            { post.log << "Check 1 of 2 failed: argument \"" << this_arg
                       << "\" dosn 't exist.\n";
              post.err << replaceArgName(arg_control.error_msg, this_arg);
              post.dbg << __FILE__ << ":" << __LINE__ << " "
                       << "Error. The argument \"" << this_arg
                       << "\" should be a name of an existing directory, but "
                       << " such directory could not be found.\n";
              return_value = ERR_DIRECTORY_NOT_FOUND;
            }
          }
        }
      }
    }
    else
    { if (!has_option("-h"))
      { if (arg_control.validation == vvMUSTEXIST)
        { post.dbg << __FILE__ << ":" << __LINE__ << " "
                   << "Error. It was expected an argument at position "
                   << arg_control.index
                   << " but such argument was not found.\n";
          post.err << arg_control.error_msg;
          return_value = ERR_ARGUMENT_NOT_FOUND;
        }
        //        else
        //        { if (arg_control.validation == vvDEFAULT)
        //          { arg(arg_control.index) =
        //          }
        //        }
      }
    }
  }
  return return_value;
}

rvEnum Parser::parse(std::vector<std::string> cmd_line_args_buffer)
{ gatherOptions(cmd_line_args_buffer);
  if (args_size() == 1)
  { help.brief();
    return CTRL_STOP;
  }
  else
  { checkArguments();
  }

  return preproccessOptions();
}

bool Parser::has_option(std::string option_id)
{ bool found = false;
  for (auto& it : options_)
  { if (it.id == option_id)
    { found = true;
      break;
    }
  }
  return found;
}

rvEnum Parser::preproccessOptions()  // some options set or unset flags
{ 
  if (has_option("-h")||has_option("--help"))
  { return CTRL_STOP;
  }
  
  if (has_option("--createlogfile"))
  { // post.log.openFile();
  }

  if (has_option("--debug"))
  { // post.dbg.set_allow_urge(true);
  }

  if (has_option("-v") || has_option("--verbose"))
  { // post.msg.set_copy_buff_to_urge(true);
  }
  /*
    if (has_option("--totalquiet"))
    { post.msg.set_allow_urge(false);
      post.wrn.set_allow_urge(false);
      post.err.set_allow_urge(false);
      post.dbg.set_allow_urge(false);
    }

    if (has_option("--showonlyerrors"))
    { post.msg.set_allow_urge(false);
      post.wrn.set_allow_urge(false);
      post.err.set_allow_urge(true);
      post.dbg.set_allow_urge(false);
    }
    if (has_option("--bufferederror"))
    { post.err.set_allow_urge(false);
      post.err.set_copy_buff_to_urge(true);
    }

  */
  return SUCCESS;
}
// end of file Parser.cc

