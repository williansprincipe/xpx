// LNLSFile.cc
// LNLSFile class implementation
// wpfernandes 2015-11-23 10h50min

#include "LNLSFile.h"
#include "Region.h"
#include "util.h"
#include "globalVals.h"

#include <string>
#include <vector>
#include <iomanip>

ctrlEnum LNLSFile::open_file_and_read_header() // -------------------------------------------
{ ctrlEnum r_v = SUCCESS;
  
  ifs_.open(path_to_string());
  if (!ifs_)
  { r_v = ERR_UNABLE_TO_OPEN_FILE;
    post.err << "Unable to open file \"" << source_file_name()
             << "\".\n";
    return r_v;
  }

  if (!utl::safeGetline(ifs_,experiment_))
  { r_v = ERR_UNABLE_TO_READ_EVEN_THE_FIRST_LINE;
    post.err << "Unable to read even the first line of file \""
             << source_file_name() << "\".\n";
    return r_v;
  }

  if (!ifs_)
  { if (ifs_.eof())
    { r_v = ERR_UNEXPECTED_EOF;
      post.err << "Unexpected end of file \""
               << source_file_name() << "\" before first region header.\n"
               << "Only the first line could be read and contains\n"
               << "\"" << experiment_ << "\".\n";
    }
    else
    { r_v = ERR_UNABLE_TO_READ_FILE;
      post.err << "The file \"" << source_file_name()
               << " is unavailable to be read before first region header\".\n"
               << "Only the first line could be read and contains\n"
               << "\"" << experiment_ << "\".\n";
    }
    return r_v;
  }
  return r_v;
}

ctrlEnum LNLSFile::check_for_another_region() // ----------------------------------
{ if (!ifs_)
  { return WRN_IFSTREAM_FAILED;
  }

  double test_buffer;
  std::ifstream::pos_type initial_position = ifs_.tellg();
  if (!(ifs_ >> test_buffer)) // first field of region
  { if (ifs_.eof())
    { return CTRL_NO;
    }
    return WRN_READ_FAILED;
  }

  ifs_.seekg(initial_position);
  return CTRL_YES; // assumed 
}

ctrlEnum LNLSFile::load_regions() // ------------------------------------------
{ if (!ifs_) 
  { post.dbg << "load_regions() function called with istream.fail()==true.\n";
    return ERR_IFSTREAM_FAILED;
  }
  while (ifs_)
  { post.dbg << "=======> Before reading a region.\n";
    ctrlEnum there_is_a_region_ahead = check_for_another_region();
    switch (there_is_a_region_ahead)
    { case WRN_IFSTREAM_FAILED:
      { continue;
      }
      case WRN_READ_FAILED:
      { continue;
      }
      case CTRL_NO:
      { if (regions_.size() == 0)
        { post.err << "There are no regions in the file \""
                   << source_file_name() << "\".\n";
          return ERR_FILE_WITH_NO_REGIONS;
        }
        return SUCCESS;
        break; // for clarity
      }
      case CTRL_YES:
      { post.dbg << "Creating region_buf(post, source_file_name = \""
                 << source_file_name() << "\", number = " << regions_.size() + 1
                 << ", ifs = \"" << ifs_ << "\").\n";
        std::string file_name_prefix(source_file_name());
        std::replace(file_name_prefix.begin(), file_name_prefix.end(), '.',
                     '_');
        Region region_buf(settings, post, source_file_name(),
                          regions_.size() + 1, ifs_, file_name_prefix);

        { ctrlEnum r_v;
          if ((r_v = region_buf.load()) != SUCCESS)
          { post.err << "Fail to read region number "
                     << region_buf.number() << ".\n";
            return r_v;
          }
        }
        post.dbg << "Region " << region_buf.number() << " successfully read.\n";
        regions_.push_back(std::move(region_buf));
        post.dbg << "Regions_.push_back worked.\n";
        
        break; // for clarity
      }
      default:
      { post.err << "Unexpected result (" << there_is_a_region_ahead
                 << ") of function \"check_for_another_region()\".\n";
        return ERR_UNEXPECTED_RETURN_OF_FUNCTION;
      }
    }
  }
  return SUCCESS;
}

ctrlEnum LNLSFile::setNRegionAndRegionNameAtEachRegion()
{ 
  std::string aux(source_file_name());
  std::replace(aux.begin(), aux.end(), '.', '_');
  int i = 0;
  for (auto& region : regions_)
  { region.n_regions(static_cast<int>(regions_.size())); // works without cast
    if (regions_.size() == 1)
    { region.region_name(aux);
    }
    else
    { region.region_name(aux + settings("region_infix") + std::to_string(++i));
    }
    post.dbg << "region.region_name() = \"" << region.region_name() << "\"\n";
  }
  return SUCCESS;
}

ctrlEnum LNLSFile::load() // ---------------------------------------------
{ { ctrlEnum r_v;
    if ((r_v = open_file_and_read_header()) != SUCCESS)
    { //r_v = ERR_UNABLE_TO_READ_FILE_HEADER;
      post.err << "Fail to read file header.\n";
      return r_v;
    }
  }

  { ctrlEnum r_v;
    if ((r_v = load_regions()) != SUCCESS)
    { //r_v = ERR_UNABLE_TO_READ_FILE_REGIONS;
      post.err << "Fail to read one of the regions.\n";
      return r_v;
    }
  }

  setNRegionAndRegionNameAtEachRegion(); // each region know how many regions are there.

  return SUCCESS;
}

// ============================================================================
void LNLSFile::genAll()
{ int i = 0; // i_regions
  for (auto& region:regions_)
  { post.dbg << "Generating default plot2d in region number " << ++i << ".\n";
    region.genDefaultPlot2D();
  }
}

void LNLSFile::writeAll()
{ int i = 0; // i_regions
  for (auto& region : regions_)
  { post.dbg << "Writing plot scripts for region number " << ++i << ".\n";
    region.writeAll();
  }
}

// ============================================================================
void LNLSFile::displaySequencesInfo(Sequences& sequences)
{
  post.msg << "  Sequences of this region (there is (are) "
           << sequences.number_of() << " and one of them has size "
           << sequences.size_of_one() << ".)\n";

  post.msg << "        ";
  for (vsz_t j = 0; j < sequences.number_of(); ++j)
  { post.msg << std::setw(10) << sequences.name(j).substr(0,10) << "  ";
  }
  post.msg << "\n";
  post.msg << std::fixed;
  for (vsz_t i = 0; i < sequences.size_of_one(); ++i)
  { post.msg << " " << std::setw(3) << i << ": ";
    for (vsz_t j = 0; j < sequences.values_matrix()[i].size(); ++j)
    { post.msg << "  " << std::setw(10) << std::setprecision(2) <<
      sequences.values_matrix()[i][j];
    }
    post.msg << "\n";
  }
}

void LNLSFile::displayPlot2DInfo(Plot2D& plot2d)
{ post.msg << "    plot2d.script_file_path_toString() = \""\
           << plot2d.script_file_path_toString() << "\".\n";                
  post.msg << "    plot2d.image_file_name() = \"" << plot2d.image_file_name()\
           << "\".\n";
  post.msg << "    plot2d.title() = \"" << plot2d.title() << "\".\n";
  post.msg << "    plot2d.xlabel() = \"" << plot2d.xlabel() << "\".\n";
  post.msg << "    plot2d.ylabel() = \"" << plot2d.ylabel() << "\".\n";
  post.msg << "    Indices of the sequences to plot: ";
  if (plot2d.sequences_to_plot_empty())
  { post.msg << "none.\n";
  }
  else
  { for (vsz_t i = 0; i < plot2d.sequences_to_plot_size(); ++i)
    { post.msg << plot2d.sequences_to_plot(i);
      if (i + 1 < plot2d.sequences_to_plot_size())
      { post.msg << ", ";
      }
    }
    post.msg << ".\n";
  }
}

void LNLSFile::displayPlot2DsInfo(std::vector<Plot2D>& plot2ds)
{ post.msg << "    There is(are) " << plot2ds.size()
           << " plot(s) added to this region.\n";
  for (auto& plot2d : plot2ds)
  { displayPlot2DInfo(plot2d);
  }
}

void LNLSFile::displayRegionInfo(Region& region)
{ post.msg << "region.source_file_name_ = \""
           << region.source_file_name() << "\".\n";
  post.msg << "region.number_ = " << region.number() << ".\n";
  post.msg << "region.region_name_ = \"" << region.region_name() << "\".\n";
  post.msg << "region.label_ = \"" << region.label() << "\".\n";
  post.msg << "region.n_regions_ = " << region.n_regions() << ".\n";

  post.msg << "region.energy_high_ = "   << region.energy_high() << ",  "
           << "region.energy_low_ = "    << region.energy_low() << ",\n"
           << "region.energy_step_ = "   << region.energy_step() << ",  "
           << "region.scans_ = "         << region.scans() << ",  "
           << "region.dwell_ = "         << region.dwell() << ",\n"
           << "region.n_counts_ = "      << region.n_counts() << ",  "
           << "region.energy_pass_ = "   << region.energy_pass() << ".\n";

  post.msg << "Region header's second line:\n"
           << region.energy_high() << ",  "

           << "region.lo_hi_ = \""         << region.lo_hi() << "\",  "         
           << "region.magpe_ = \""         << region.magpe() << "\",\n"
           << "region.ke_factor_ = \""     << region.ke_factor() << "\",  "
           << "region.excitation_ = \""    << region.excitation() << "\",\n"
           << "region.work_function_ = \"" << region.work_function() << "\",  "
           << "region.ck_ = \""            << region.ck() << "\",\n"
           << "region.det_ = \""           << region.det() << "\",  "
           << "region.ent_ = \""           << region.ent() << "\",  "
           << "region.exit_ = \""          << region.exit() << "\".\n";
  displaySequencesInfo(region.sequences());
  displayPlot2DsInfo(region.plot2ds());
}

void LNLSFile::displayRegionsInfo()
{ int i = 0; // i_regions
  post.msg << "\nThere is(are) " << regions_.size() << " regions loaded. "\
           << "==========================================\n";
  for (auto& region : regions_)
  { post.msg << "i_regions = " << ++i << " of " << regions_.size() << ". "\
           << "---------------------------------------------------\n";
    displayRegionInfo(region);
  }
}


// ============================================================================
// ============================================================================
ctrlEnum LNLSFile::doTest() // ------------------------------------------------
{ post.msg << "LNLSFile\n";
  post.msg << utl::time_string("%Y-%m-%d %Hh %Mmin") << "\n";

  source_file_name("MA26_-18_OK.51");

//  source_file_name( args.size() < 2? "MA26_-18_OK.51" : args[1]);
//  post.msg << "Source file: \"" << source_file_name() << "\".\n";

  post.msg << "\nReading... ===============================================\n";

  { ctrlEnum r_v;
    if ((r_v = load()) != SUCCESS)
    { post.err << "Error. Fail to read file \"" << source_file_name() << "\".\n";
      return r_v;
    }
  }

  post.msg << "Number of regions read = " << regions_.size() << "\n";

  post.msg << "\nGenerating one plot2d per region... ======================\n";

  genAll();

  post.msg << "\nDisplaying info of all regions ======================\n";


  displayRegionsInfo();

  post.msg << "\nGenerating one pyxplot script file for each plot2d... ====\n";

  writeAll();

  post.msg << "\nEnd of regiontest.\n";
   
  return SUCCESS;
}

// end of LNLSFile.cc
