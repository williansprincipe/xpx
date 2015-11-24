// Plot2DTest.cc
// Plot2DTest object implementation
// wpf 2015-10-20 18h00min

#include "Plot2DTest.h"
#include "Plot2D.h"
#include "util.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

bool Plot2DTest::cin()  // ----------------------------------------
{ std::string script_file_path_string;
  std::string data_source_file_name;
  int region_number;
  std::string region_number_string_buffer;
  std::string region_label;
  std::string title;
  std::string xlabel;
  std::string ylabel;
  std::string reversed_yes_or_no;
  bool reversed;
  std::string image_extension;
  bool input_ended = false;
  std::string next_step = "input";
  while (!input_ended)
  { std::cout
        << "You will need to enter, without spaces:\n"
        << " (1) plot script file name (it can be path-and-file-name),\n"
        << " (2) data source file name,\n"
        << " (3) region number as an integer,\n"
        << " (4) region label,\n"
        << " (5) plot title,\n (6) xlabel,\n (7) ylabel,\n"
        << " (8) reverse: \"Y\"es or \"N\"o, and\n"
        << " (9) default image file name extension (e.g. png, bmp, etc.).\n"
        << "We will use the vectors: x = {0,1,2,3,4} and"
        << " y = {0,2,4,6,8}. You will NOT need to enter them.\n";
    std::cout
        << "\nEnter (1) plot script file name (it can be path-and-file-name): ";
    std::cin >> script_file_path_string;
    std::cout << "Enter (2) data source file name: ";
    std::cin >> data_source_file_name;
    bool input_region_number_finished = false;
    while (!input_region_number_finished)
    { std::cout << "Enter (3) region number (an integer): ";
      std::cin >> region_number_string_buffer;
      try
      { region_number = std::stoi(region_number_string_buffer);
        input_region_number_finished = true;
      }
      catch (std::invalid_argument argument)
      { if (argument.what() == std::string("stoi"))
        { std::cout << "Your input could not be processed.\n"
                    << "Enter an integer (only digits)!\n";
        }
        else
        { std::cout << "Unexpected error \"" << argument.what() << "\".\n";
          input_region_number_finished = true;
          input_ended = true;
          next_step = "abort";
        }
      }
    }

    if (next_step != "abort")
    { std::cout << "enter (4) region label: ";
      std::cin >> region_label;
      std::cout << "enter (5) plot title: ";
      std::cin >> title;
      std::cout << "enter (6) plot xlabel: ";
      std::cin >> xlabel;
      std::cout << "enter (7) plot ylabel: ";
      std::cin >> ylabel;
      std::cout << "(8) is x axis reversed? \"y\"es or \"n\"o: ";
      std::cin >> reversed_yes_or_no;
      reversed = (reversed_yes_or_no == "y" || reversed_yes_or_no == "y" ||
                  reversed_yes_or_no == "yes");
      image_extension = "none";
      while (image_extension != "pdf" && image_extension != "png")
      { std::cout << "(9) choose default image file name extension"
                  << " among pdf and png: ";
        std::cin >> image_extension;

        if (image_extension == "pdf" || image_extension == "pdf")
        { image_extension = "pdf";
        }
        else
        { if (image_extension == "png" || image_extension == "png")
          { image_extension = "png";
          }
        }
      }

      std::cout << "we will use the vectors: x = {0,1,2,3,4} and"
                << " y = {0,2,4,6,8}.\n";
      std::cout << "\n\nInput finished.\n";

      next_step = "none";
      while ((next_step != "stop") && (next_step != "continue") &&
             (next_step != "reenter"))
      { std::cout << "you should choose between:\n"
                  << "\"stop\", \"continue\" or \"reenter\".\n"
                  << "enter your selection and press <enter> or <return>: ";
        std::cin >> next_step;
        if ((next_step == "stop") || (next_step == "stop"))
        { next_step = "stop";
        }
        else
        { if ((next_step == "continue") || (next_step == "cont") ||
              (next_step == "continue") || (next_step == "cont"))
          { next_step = "continue";
          }
          else
          { if ((next_step == "reenter") || (next_step == "reenter"))
            { next_step = "reenter";
            }
          }
        }
      }
      if (next_step != "reenter")
      { input_ended = true;
      }
    }

    if (next_step == "continue")
    { std::cout << "\nJust for confirmation, you entered:\n"
                << "plot script file name: \"" << script_file_path_string
                << "\",\n"
                << "data source file name: \"" << data_source_file_name
                << "\",\n"
                << "region number: " << region_number << ",\n"
                << "region label: \"" << region_label << "\",\n"
                << "title: \"" << title << "\",\n"
                << "xlabel: \"" << xlabel << "\",\n"
                << "ylabel: \"" << ylabel << "\",\n"
                << "reversed: \"" << reversed_yes_or_no << "\","
                << "that I understood as reversed "
                << (reversed ? "\"Y\"es" : "\"N\"o") << "\n"
                << "and, finally, image_extension: \"" << image_extension
                << "\".\n"
                << "As it was said before, the vectors will be x = {0,1,2,3,4}"
                << " and y = {0,2,4,6,8}.\n";
      // loading plot2d with information
      plot2d.script_file_path_parse(script_file_path_string);
      plot2d.set_data_source_file_name(data_source_file_name);
      plot2d.set_region_number(region_number);
      plot2d.set_region_label(region_label);
      plot2d.set_title(title);
      plot2d.set_xlabel(xlabel);
      plot2d.set_ylabel(ylabel);
      plot2d.set_reversed(reversed);
      plot2d.set_image_extension(image_extension);
    }
    else
    { if (next_step == "stop")
      { std::cout << "Stoped by user.\n";
      }
      else
      { if (next_step == "abort")
        { std::cout << "Execution aborted.\n";
        }
      }
    }
  }

  return (next_step == "continue");
}

void Plot2DTest::cout()  // ---------------------------------------------------
{ std::cout << "\nThe object contains:\n"
            << "plot2d.script_file_path = \""
            << plot2d.script_file_path_to_string() << "\",\n"
            << "data source file name = \"" << plot2d.data_source_file_name()
            << "\",\n"
            << "region number = " << plot2d.region_number() << ",\n"
            << "region label = \"" << plot2d.region_label() << "\",\n"
            << "plot2d.title = \"" << plot2d.title() << "\",\n"
            << "plot2d.xlabel = \"" << plot2d.xlabel() << "\",\n"
            << "plot2d.ylabel = \"" << plot2d.ylabel() << "\",\n"
            << "plot2d.reversed = "
            << (plot2d.reversed() ? "\"true\"" : "\"false\"") << "\n"
            << "image_extension = \"" << (plot2d.image_extension()) << "\".\n";
  std::cout << "\n";

  std::cout << "----------------\nThe pairs in the object are:\n";
  if (plot2d.xs_size() != plot2d.ys_size())
  {  // arv = rvNONSTOPINGERROR;
    // todo: status
  }
  auto x_it = plot2d.xs_begin();
  auto y_it = plot2d.ys_begin();
  while (x_it != plot2d.xs_end() and y_it != plot2d.ys_end())
  { std::cout << "(" << *x_it << ", " << *y_it << ")\n";
    ++x_it;
    ++y_it;
  }
  std::cout << "\n";
}

void Plot2DTest::
    doTest()  // ---------------------------------------------------
// rvEnum arv = rv;
// todo: status
{ post.msg << "Plot2DTest\n";
  post.msg << utl::time_string("%Y-%m-%d %Hh %Mmin") << "\n";

  std::cout << "\n";
  if (cin())
  { cout();
    plot2d.genPlotScript();
  }
}
// end of Plot2DTest.cc
