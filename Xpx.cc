// Xpx.cc
// Xpx class implementation
// wpfernandes 2015-11-02 15h00min as RegionTest.cc
//             2015-11-23 11h00min

#include "Xpx.h"
#include "LNLSFile.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include <string>

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
