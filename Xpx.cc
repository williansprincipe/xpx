// Xpx.cc
// Xpx class implementation
// wpfernandes 2015-11-02 15h00min as RegionTest.cc
//             2015-11-23 11h00min

#include "Xpx.h"
#include "LNLSFile.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/DirectoryIterator.h"
#include <string>

rvEnum Xpx::extract_theta(const std::string& base_name,
                          sEnum& signal, int& theta_ref)
{ std::string theta_str;
  std::size_t pos = (base_name.find("_"));
  if (pos == std::string::npos)
  { return ERR_THETA_EXTRACTING_FAILED;
  }
  if (base_name.substr(pos + 1, 1) == "-")
  { signal = sEnum::NEGATIVE;
    theta_str = base_name.substr(pos + 2, 2);
  }
  else
  { signal = sEnum::POSITIVE;
    theta_str = base_name.substr(pos + 1, 2);
  }
  if (theta_str.empty())
  { return ERR_THETA_EXTRACTING_FAILED;
  }
  if (theta_str.size() == 1)
  { if (theta_str < "0" || theta_str > "9")
    { return ERR_THETA_EXTRACTING_FAILED;
    }
    theta_str = "0" + theta_str;
  }
  else
  { if (theta_str.substr(0, 1) < "0" || theta_str.substr(0, 1) > "9" ||
        theta_str.substr(1, 1) < "0" || theta_str.substr(1, 1) > "9")
    { return ERR_THETA_EXTRACTING_FAILED;
    }
    theta_ref = std::stoi(theta_str);
  }
  return SUCCESS;
}

rvEnum Xpx::extract_phi(const std::string& extension, int& phi_ref)
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
  phi_ref = std::stoi(phi_str);
  return SUCCESS;
}

rvEnum Xpx::lnls_files_meta_infos_load(const std::string& path_str)
// signal = -1 => MgKa or AlKa
// signal = +1 => synchrotron beam
{ Poco::DirectoryIterator it(path_str);
  Poco::DirectoryIterator end;
  Poco::Path path;
  while (it != end)
  { path = it.path();
    if (it->isFile())
    { LNLSFileMetaInfo f_i; // f_i does mean lnls_file_meta_info
      f_i.name(path.getFileName());
      post.dbg << "path.getFileName() = \"" << path.getFileName() << "\"\n";

      post.dbg << "Extracting theta gives theta = ";
      { rvEnum rv = extract_theta(path.getBaseName(), f_i.signal_ref(),
                                  f_i.theta_ref());
        if (rv != SUCCESS)
        { post.dbg << "error (" << static_cast<int>(rv) << ") extracting theta.\n";
          return rv;
        }
      }
      post.dbg << f_i.theta_ref() << ".";

      post.dbg << " Extracting phi gives phi = ";
      { rvEnum rv = extract_phi(path.getExtension(), f_i.phi_ref());
        if (rv != SUCCESS)
        { post.dbg << "error (" << static_cast<int>(rv) << ") extracting phi.\n";
          return rv;
        }
      }
      post.dbg << f_i.phi_ref() << ".\n";
//      post.dbg << "lnls_files_meta_infos[] = {name =\"" << f_i.name()\
               << "\", signal = " << static_cast<int>(f_i.signal())\
               << ", theta = " << f_i.theta() << ", phi = " << f_i.phi()\
               << "}\n";
      lnls_files_meta_infos_add(f_i);
    } // if (path.isFile())
    ++it;
  }
  return SUCCESS;
}

rvEnum Xpx::yesno_is_XPD_directory(std::string path_str)
{ post.dbg << "Inside yesno_is_XPD_directory(\"" << path_str << "\")\n";
  { rvEnum rv = lnls_files_meta_infos_load(path_str);
    if (!rv == SUCCESS) 
    { return rv;
    }
  }
  return CTRL_YES;
}

rvEnum Xpx::ArgumentsNameExistingFilesOrDirectories()
{ rvEnum rv = SUCCESS;
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

rvEnum Xpx::treatDirectory(Poco::Path xpx_path, Poco::File xpx_file)
{ post.dbg << "inside Xpx::treatDirectory(xpx_path.toString() = \""
           << xpx_path.toString() << "\").\n";
  rvEnum rv = yesno_is_XPD_directory(xpx_path.toString());
  if (rv != CTRL_YES && rv != CTRL_NO)
  { return rv;
  }
  return SUCCESS;
}
 
rvEnum Xpx::treatXPSFile(Poco::Path xpx_path, Poco::File xps_file)
{ post.dbg << "inside Xpx::treatXPSFile(xpx_path.getFileName() = \""
           << xpx_path.getFileName() << "\").\n";
  return SUCCESS;
}

rvEnum Xpx::doRun()
{ { rvEnum args_existance = ArgumentsNameExistingFilesOrDirectories();
    if (args_existance != SUCCESS)
    { return args_existance;
    }
  }

  rvEnum rv = SUCCESS;
  std::string arg;
  Poco::Path xpx_path;
  auto it_args = args.cbegin();
  ++it_args;

  while (it_args != args.end() && rv == SUCCESS)
  { arg = *it_args;
    if (!xpx_path.tryParse(arg))
    { post.err << "Error. Poco::Path::tryParse unable to parse \""
               << arg << "\" at Xpx::doRun.\n";
      post.log << "Error. Poco::Path::tryParse unable to parse \""
               << arg << "\" at Xpx::doRun.\n";
      return ERR_POCO_PARSE_STRING_FAILED;
    }
    xpx_path.parse(arg);
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
