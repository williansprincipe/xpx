// Xpx.cc
// Xpx class implementation
// wpfernandes 2015-11-02 15h00min as RegionTest.cc
//             2015-11-23 11h00min

#include "Xpx.h"
#include "LNLSFile.h"
#include "Settings.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include <string>
#include <algorithm>

ctrlEnum Xpx::ArgumentsNameExistingFilesOrDirectories()
{ ctrlEnum rv = SUCCESS;
  int i = 0;
  auto it_args = args.cbegin();
  ++it_args;
  while (it_args != args.end())
  { std::string xpd_directory_name_or_xps_file_name = args[1];
    Poco::Path xpx_path(xpd_directory_name_or_xps_file_name);
    Poco::File xpx_file(xpx_path);
    if (!xpx_file.exists())
    { post.err << "Argument \"" << xpd_directory_name_or_xps_file_name
               << "\" is not an existing file or directory.\n";
      rv = ERR_ARGUMENT_ISNT_DIRECTORY_OR_FILE;
      if (++i > 10)
      { post.err << "Supressing further error messages.\n";
        break;
      }
    }
    ++it_args;
  }
  return rv;
}

bool Xpx::is_two_chars_a_month(char c1, char c2)
{ std::string s("JjFfMmAaSsOoNnDd");
  std::string::size_type n = s.find(c1);
  if (n == std::string::npos)
  { //post.dbg << "First letter is not a month first letter.\n";
    return false;
  }

  s = "AaEePpUuCcOoBbGg";
  n = s.find(c2);
  if (n == std::string::npos)
  { //post.dbg << "Second letter is not a month second letter.\n";
    return false;
  }
  //post.dbg << "is_two_char_a_month returned true\n";
  return true;
}

bool Xpx::is_lnls_file_name(const std::string& file_name)
{
  //post.dbg << "Inside is_lnls_file_name(file_name = \"" << file_name << "\")\n";
  std::string::size_type l = file_name.size();

  //post.dbg << "file_name.size() = " << file_name.size() << ".\n";

  if (l < 8 || l > 12)
  { return false;
  }

  if (!is_two_chars_a_month(file_name[0], file_name[1]))
  { return false;
  }

  //post.dbg << "Reached here: first two char is month!\n";
  char c = file_name[2];
  if (c < '0' || c > '3')
  { return false;
  }

  c = file_name[3];
  if (c < '0' || c > '9')
  { return false;
  }

  c = file_name[4];
  if (c != '_')
  { return false;
  }

  //post.dbg << "Reached here: the '_' character is correctly placed!\n";
  std::string::size_type p = 5;
  c = file_name[p];
  std::string::size_type d = 7; // expected max dot position if signal == +1
  if (c == '-')
  { d = 8; // new expected max dot position once signal == -1
  }
  else
  { --p;
  }

  //post.dbg << "Reached here: p = " << p << " and d = " << d << ".\n";
  while (++p < d && (c = file_name[p]) != '.')
  { if (c < '0' || c > '9')
    { return false;
    }
  }
  
  //post.dbg << "Reached here: theta (" << file_name.substr(p-2,2)<< ") is number.\n";
  if (p == l)
  { return false;
  }

  c = file_name[p];
  if (c != '.')
  { return false;
  }

  //post.dbg << "Reached here: dot '.' is correctly placed!\n";
  std::string::size_type r = (l - 1) - p;
  //post.dbg << "r = " << r << ", l = " << l << ", p = " << p << ".\n";
  if ( r > 3 || r < 1)
  { return false;
  }

  c = file_name[++p];
  if (c < '0')
  { return false;
  }

  if (c > '9' || (r == 3 && c > '4'))
  { return false;
  }
  
  while (++p < l)
  { c = file_name[p];
    if (c < '0' || c > '9')
    { return false;
    }
  }
  return true;
}

int Xpx::lnls_files_count(std::string path_str)
{ int n = 0;
  //post.dbg << "Inside lnls_files_count(path_str = \"" << path_str << "\").\n";
  Poco::DirectoryIterator it(path_str);
  Poco::DirectoryIterator end;
  while (it != end)
  { if (it->isFile())
    { if (is_lnls_file_name(it.path().getFileName()))
      { ++n;
      }
    }
    ++it;
  }
  //post.dbg << "Number of lnls_files: n = " << n << ".\n";
  return n;
}

ctrlEnum Xpx::get_theta_from_base_name(const std::string& base_name,
                          sEnum& signal, int& theta_by_ref)
{ std::string theta_str;
  std::string::size_type pos = base_name.find("_");
  if (pos == std::string::npos)
  { return ERR_THETA_EXTRACTING_FAILED;
  }
  if (base_name[pos + 1] == '-')
  { signal = sEnum::NEGATIVE;
    theta_str = base_name.substr(pos + 2, 2);
  }
  else
  { signal = sEnum::POSITIVE;
    theta_str = base_name.substr(pos + 1, 2);
  }
//  post.dbg << "base_name = \"" << base_name << "\", theta_str = \"";
//  post.dbg << theta_str << "\".\n";
  if (theta_str.empty())
  { return ERR_THETA_EXTRACTING_FAILED;
  }
  if (theta_str[0] < '0' || theta_str[0] > '9')
  { return ERR_THETA_EXTRACTING_FAILED;
  }
  if (theta_str.size() > 1)
  { if (theta_str[1] < '0' || theta_str[1] > '9')
    { return ERR_THETA_EXTRACTING_FAILED;
    }
  }
  theta_by_ref = std::stoi(theta_str);
  return SUCCESS;
}

ctrlEnum Xpx::get_phi_from_extension(const std::string& extension, int& phi_by_ref)
{ std::string phi_str = extension;
  if (phi_str.empty())
  { return ERR_PHI_EXTRACTING_FAILED;
  }

  if (phi_str.size() == 1)
  { if (phi_str < "0" || phi_str > "9")
    { return ERR_PHI_EXTRACTING_FAILED;
    }
    phi_str = "00" + phi_str;
  }
  else
  { if (phi_str.size() == 2)
    { if (phi_str.substr(0,1) < "0" || phi_str.substr(0,1) > "9"
       || phi_str.substr(1,1) < "0" || phi_str.substr(1,1) > "9")
      { return ERR_PHI_EXTRACTING_FAILED;
      }
      phi_str = "0" + phi_str;
    }
    else
    { if (phi_str.substr(0,1) < "0" || phi_str.substr(0,1) > "3"
       || phi_str.substr(1,1) < "0" || phi_str.substr(1,1) > "9"
       || phi_str.substr(2,1) < "0" || phi_str.substr(2,1) > "9")
      { return ERR_PHI_EXTRACTING_FAILED;
      }
    }
  }
  phi_by_ref = std::stoi(phi_str);
  return SUCCESS;
}

ctrlEnum Xpx::lnls_files_meta_infos_load(const std::string& path_str,
                                         ctrlEnum yesno_error_tolerant,
                                         ctrlEnum yesno_log_error)
// signal = -1 => MgKa or AlKa
// signal = +1 => synchrotron beam
{ Poco::DirectoryIterator it(path_str);
  Poco::DirectoryIterator end;
  n_processed_files(0);
  theta_min(90);
  theta_max(0);
  phi_min(90);
  phi_max(0);
  while (it != end)
  { if (it->isFile())
    { plusplus_n_processed_files();
      LNLSFileMetaInfo f_i; // f_i does mean lnls_file_meta_info
      f_i.name(it.path().getFileName());
      //post.dbg << "it.path().getFileName() = \"" << it.path().getFileName() << "\"\n";

      //post.dbg << "get_theta_from_base_name gives theta = ";
      { ctrlEnum rv = get_theta_from_base_name(
            it.path().getBaseName(), f_i.signal_ref(), f_i.theta_by_ref());
        if (rv != SUCCESS)
        { post.dbg << "error (" << static_cast<int>(rv) << ") extracting theta";
          post.dbg << " from " << it.path().getBaseName() << ".\n";

          if (yesno_log_error == CTRL_YES)
          { post.log << "Error while extracting theta from file_name \""
                     << it.path().getBaseName() << "\".\n";
          }
          if (yesno_error_tolerant == CTRL_YES)
          { ++it;
            continue;
          }
          else
          { return rv;
          }
        }
      }
      if (f_i.theta_by_ref() < theta_min())
      { theta_min(f_i.theta_by_ref());
      }
      if (f_i.theta_by_ref() > theta_max())
      { theta_max(f_i.theta_by_ref());
      }
      //post.dbg << f_i.theta_by_ref() << ".";

      //post.dbg << " get_phi_from_extension gives phi = ";
      { ctrlEnum rv = get_phi_from_extension(it.path().getExtension(), f_i.phi_by_ref());
        if (rv != SUCCESS)
        { post.dbg << "error (" << static_cast<int>(rv) << ") extracting phi.\n";
          return rv;
        }
      }
      if (f_i.phi_by_ref() < phi_min())
      { phi_min(f_i.phi_by_ref());
      }
      if (f_i.phi_by_ref() > phi_max())
      { phi_max(f_i.phi_by_ref());
      }
      //post.dbg << f_i.phi_by_ref() << ".\n";
      lnls_files_meta_infos_add(f_i);
    }
    ++it;
  }
  return SUCCESS;
}

ctrlEnum Xpx::yesno_is_XPD_directory(std::string path_str)
{ //post.dbg << "Inside yesno_is_XPD_directory(\"" << path_str << "\")\n";
  if (lnls_files_count(path_str) == 0)
  { return CTRL_NO;
  }

  { ctrlEnum yes_load_error_tolerant = CTRL_YES;
    ctrlEnum yes_log_error =
        (settings(kv::LOG_ERROR_EXTRACTING_ANGLES_FROM_FILE_NAME) == "yes"
             ? CTRL_YES
             : CTRL_NO);
    ctrlEnum rv = lnls_files_meta_infos_load(path_str, yes_load_error_tolerant,
                                         yes_log_error);
    if (!rv == SUCCESS) 
    { return rv;
    }
    post.dbg << "n_processed_files() = " << n_processed_files() << ".\n";
    post.dbg << "lnls_files_meta_infos_size() = ";
    post.dbg << lnls_files_meta_infos_size() << ".\n";
  }

  if (lnls_files_meta_infos_size() < 250)
  { return CTRL_NO;
  }
  return CTRL_YES;
}

void Xpx::lnls_files_meta_infos_sort()
{ std::sort(lnls_files_meta_infos_.begin(), lnls_files_meta_infos_.end(),
            [](const LNLSFileMetaInfo& lhs, const LNLSFileMetaInfo& rhs)
            { return (1000 * lhs.theta() + lhs.phi()) <
                     (1000 * rhs.theta() + rhs.phi());
            });
}

ctrlEnum Xpx::do_pizza_with_lnls_files_meta_infos()
{ post.dbg << "Inside do_pizza_with_lnls_files_meta_infos()\n";
  lnls_files_meta_infos_sort();
  post.dbg << "Sort done.\n";
  for (auto& f_i : lnls_files_meta_infos())
  { post.dbg << f_i.name() << ", " << static_cast<int>(f_i.signal()) << ", ";
    post.dbg << f_i.theta() << ", " << f_i.phi() << ".\n";
  }
  return SUCCESS;
}

ctrlEnum Xpx::do_all_xps_graphs_from_this_xpx_path()
{ post.dbg << "Inside do_all_xps_graphs_from_this_xpx_path().";
  post.dbg << " All to be done.\n";
  return SUCCESS;
}

ctrlEnum Xpx::treatDirectory(Poco::Path xpx_path, Poco::File xpx_file)
{ //post.dbg << "inside Xpx::treatDirectory(xpx_path.toString() = \""         ;
  //post.dbg << xpx_path.toString() << "\").\n";
  ctrlEnum rv = yesno_is_XPD_directory(xpx_path.toString());
  if (rv != CTRL_YES && rv != CTRL_NO)
  { return rv;
  }
  if (rv == CTRL_YES)
  { if (settings(kv::LOG_ERROR_EXTRACTING_ANGLES_FROM_FILE_NAME) == "yes")
    {
      post.log << "The directory \"" << xpx_path.toString() << "\" was "
               << "identified as a XPD directory.\n"
               << "  It has " << n_processed_files() << ", from which "
               << lnls_files_meta_infos_size() << " have typical LNLS file "
               << "names.\n"
               << "  Intervals (degrees): " << theta_min() << " < theta < "
               << theta_max() << " and " << phi_min()
               << " < phi < " << phi_max() << ".\n";
    }

    rv = do_pizza_with_lnls_files_meta_infos();
    if (rv != SUCCESS)
    { return rv;
    }
  }
  else
  { rv = do_all_xps_graphs_from_this_xpx_path();
    if (rv != SUCCESS)
    { return rv;
    }
  }

  return SUCCESS;
}
 
ctrlEnum Xpx::treatXPSFile(Poco::Path xpx_path, Poco::File xps_file)
{ //post.dbg << "inside Xpx::treatXPSFile(xpx_path.getFileName() = \"";
  //post.dbg << xpx_path.getFileName() << "\").\n";
  return SUCCESS;
}

ctrlEnum Xpx::doRun()
{ { ctrlEnum args_existance = ArgumentsNameExistingFilesOrDirectories();
    if (args_existance != SUCCESS)
    { return args_existance;
    }
  }

  ctrlEnum rv = SUCCESS;
  Poco::Path xpx_path;
  auto it_args = args.cbegin();
  ++it_args;

  while (it_args != args.end() && rv == SUCCESS)
  { if (!xpx_path.tryParse(*it_args))
    { post.err << "Error. Poco::Path::tryParse unable to parse \""
               << *it_args << "\" at Xpx::doRun.\n";
      post.log << "Error. Poco::Path::tryParse unable to parse \""
               << *it_args << "\" at Xpx::doRun.\n";
      return ERR_POCO_PARSE_STRING_FAILED;
    }
    xpx_path.parse(*it_args);
    Poco::File xpx_file(xpx_path);
    if (xpx_file.isDirectory())
    { rv = treatDirectory(xpx_path, xpx_file);
    }
    else
    { rv = treatXPSFile(xpx_path, xpx_file);
    }
    ++it_args;
  }
  return rv;
}

// end of Xpx.cc
