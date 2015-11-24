// Region.cc
// Region definitions
// wpfernandes 2015-11-02 14h30min

#include "Region.h"
#include "Post.h"
#include "globalVals.h"

#include <string>
#include <fstream>
#include <limits>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_sf_pow_int.h> // gaussian_f

rvEnum Region::genDefaultPlot2D() // ----------------------------------------
{ post.dbg << __FILE__ << ":" << __LINE__ << ": " << "Begin of add_plot2.\n";
  if (sequences().empty())
  { post.err << "Unabel to create a plot to region number " << number() \
             << ". This region is empty.\n";
    return CTRL_REGION_IS_EMPTY;
  }
  // regionobj.source_file_name_ will be plot2dobj.region_name_
  Plot2D plot2d(settings, post, sequences());
  plot2d.source_file_name(source_file_name());
  plot2d.region_number(number());
  plot2d.label(label());
  plot2d.script_file_path_parse(
      region_name() + "." + settings("script_file_extension_for_plot_program_" +
                                     settings("plot_program")));
  plot2d.data_file_path_parse(
      region_name() + "." + settings("data_file_extension_for_plot_program_" +
                                     settings("plot_program")));
  plot2d.image_file_name(region_name() + "." +
                         settings("image_file_extension_for_image_type_" +
                                  settings("image_type")));
  plot2d.title("NONE");
  plot2d.xlabel("Energia de liga\\\\c{c}\\~{a}o por eléctron-volt");
  plot2d.ylabel("Contagens por segundo");
  plot2d.reversed(true);

  // values_matrix[0] stores energy, that is used as x sequence by other seqs
  // sequences_to_plot[i] = 0 is to plot model
  if (settings("load_mode") == "xps")
  { plot2d.sequences_to_plot_add(2); // count per second HERE SEQUENCE exp ...
  }
  else
  { plot2d.sequences_to_plot_add(0); // model (gaussian)
    plot2d.sequences_to_plot_add(3); // after shirley removal
    plot2d.sequences_to_plot_add(4); // shirley
    plot2d.sequences_to_plot_add(5); // after shirley removal
  }
  post.dbg << __FILE__ << ":" << __LINE__ << ": " << "Before push_back.\n";
  plot2ds_add_move(plot2d);
  post.dbg << __FILE__ << ":" << __LINE__ << ": " << "End of add_plot2.\n";

  return SUCCESS;
}

// ============================================================================
rvEnum Region::read_header() // -----------------------------------------------
{ if (!ifs_)
  { post.err << "File fail just before reading the header of region number "
             << number() << ".\n";
    return ERR_IFSTREAM_FAILED;
  }

  ifs_ >> energy_high_
       >> energy_low_
       >> energy_step_
       >> scans_
       >> dwell_
       >> n_counts_ 
       >> energy_pass_;
  ifs_.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

  if (!ifs_)
  { post.err << "File fail while trying to read the line number 1 of "
             << "the header of the region number " << number() << ".\n";
    return ERR_IFSTREAM_FAILED;
  }

  ifs_ >>  lo_hi_ // region header line 2.
       >>  magpe_
       >>  ke_factor_
       >>  excitation_
       >>  work_function_
       >>  ck_
       >>  det_
       >>  ent_
       >>  exit_;
  ifs_.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
  if (!ifs_)
  { post.err << "File fail while trying to read the line number 2 of "
             << "the header of the region number " << number() << ".\n";
    return ERR_IFSTREAM_FAILED;
  }
  
  return SUCCESS;
}

void Region::set_default_plot2d_sequences_names() // --------------------------
{ sequences().names_add(settings("sequence_name_for_energy"));
  sequences().names_add(settings("sequence_name_for_count"));
  sequences().names_add(settings("sequence_name_for_count_per_second"));
  if (settings("load_mode") == "xpd")
  { sequences().names_add(
        settings("sequence_name_for_count_per_second_smoothed"));
    sequences().names_add(
        settings("sequence_name_for_shirley_background"));
    sequences().names_add(settings(
"sequence_name_for_count_per_second_smoothed_minus_shirley_background"));
  }
}

void Region::set_after_read_header() // ---------------------------------------
{ label(std::string("Energies from ") +
        std::to_string(energy_high()) + " to " +
        std::to_string(energy_high()));
  set_default_plot2d_sequences_names();
}

rvEnum Region::load_counts() // -----------------------------------------------
{ post.dbg << "Just before read counts. n_counts_ = " << n_counts_\
           << ", ifs_ = " << (ifs_? "true" : "false") << ".\n";
  if (settings("load_mode") != "xps" && settings("load_mode") != "xpd")
  { post.err << "Unexpected value of load_mode (\"" << settings("load_mode")
             << "\").\n";
    return ERR_UNEXPECTED_VALUE_OF_CONTROL_VARIABLE;
  }
  bool is_xps {(settings("load_mode") == "xps")};
  double count;
  int i = 0; // i_counts_
  double energy = energy_high();
  while (i < n_counts_)
  { if (!(ifs_ >> count)) // obs: if (!ifs_ >> count) don't do it!!!
    { post.err << "Unable to read the count number " << i + 1 << " of "\
               << n_counts_ << " expected counts of region number "\
               << number() << ".\n";
      return ERR_UNABLE_TO_READ_FILE_REGION_COUNT;
    }
    ifs_.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    ++i; 
    if ((energy+0.00005) < (energy_low_+0.00005))
    { post.wrn << "Energy bellow limmit while reading.";
      return WRN_ENERGY_BELLOW_LIMIT;
    }
    if (is_xps)
    { sequences().values_matrix().emplace_back(
        std::vector<double>({energy, count, count / scans() / dwell()}));
    }
    else
    { sequences().values_matrix().emplace_back(std::vector<double>(
          {energy, count, count / scans() / dwell(), 0.0, 0.0, 0.0}));
    }
    post.dbg << " i = " << i << ", energy = " << energy << ", count = " \
             << count << ".\n";
    energy += energy_step();
  }
  post.dbg << "sequences_values_matrix_.size() == " \
           << sequences().values_matrix().size() << "\n";
  return SUCCESS;
}

rvEnum Region::Smooth(unsigned int n_iteractions) // --------------------------
{ //need check n_iteractions < 0
  std::vector<std::vector<double>>& m = sequences().values_matrix(); // simplify
  if (n_iteractions == 0 || m.size() < 3) // just copy
  { for (vsz_t i=0; i < m.size(); ++i)
    { m[i][3] = m[i][2];
    }
    return SUCCESS;
  }
  if (n_iteractions == 1) // output directly on smoothed_sequence
  { vsz_t i = 0;
    m[i][3] = (2 * m[i][2] +
        m[i + 1][2]) / 3;
    while (++i < m.size() - 1)
    { m[i][3] = (m[i - 1][2] + 2 * m[i][2] + m[i + 1][2]) / 4;
    }
    m[i][3] = (m[i - 1][2] + 2 * m[i][2]) / 3;
  }
  std::vector<double> aux1(m.size()), aux2(m.size());
  for (vsz_t i = 0; i < m.size(); ++i)
  { aux1[i] = m[i][2];
  }
  for (unsigned int i_iteractions = 0; i_iteractions < n_iteractions;
       ++i_iteractions)
  { vsz_t i = 0;
    aux2[i] = (2 * aux1[i] + aux1[i + 1]) / 3;
    while (++i < m.size() - 1)
    { aux2[i] = (aux1[i - 1] + 2 * aux1[i] + aux1[i + 1]) / 4;
    }
    aux2[i] = (aux1[i - 1] + 2 * aux1[i]) / 3;
    if (i_iteractions < (n_iteractions - 1))
    { aux2.swap(aux1);
    }
  }
  for (vsz_t i = 0; i < m.size(); ++i)
  { m[i][3] = aux2[i];
  }
  return SUCCESS;
}

rvEnum Region::Shirley(unsigned int n_iteractions) // ----------------------------------
{ //need check n_iteractions < 0
  std::vector<std::vector<double>>& m = sequences().values_matrix(); // simplify

  std::vector<double> aux(m.size()), aux2(m.size());
  for (vsz_t i = 0; i < m.size(); ++i)
  { aux[i] = m[i][3];
  }
  post.dbg << "Inside shirley.\naux.size() = " << aux.size() << "\n";
  double higher_base = *(aux.begin());
  double lower_base = *(std::prev(aux.end()));
  post.dbg << "higher_base = " << higher_base << "\nlower_base = ";
  post.dbg << lower_base << "\n";
  std::vector<double> shirley_background(aux.size(), lower_base);
  double total_util_area;
  double partial_util_area;

  for (unsigned int i = 0; i < n_iteractions; ++i)
  { total_util_area = 0;
    std::vector<double>::const_iterator osbkgit = shirley_background.begin();
    for (auto& c : aux)
    { total_util_area += c - *osbkgit++;
    }
    post.dbg << "total_util_area = " << total_util_area << "\n";

    partial_util_area = 0;
    auto rit_cs = aux.rbegin();
    auto osbkgrit = shirley_background.rbegin();
    while (rit_cs != aux.rend())
    { partial_util_area += *rit_cs - *osbkgrit;
      post.dbg << *rit_cs << "(" << partial_util_area << "); ";
      *osbkgrit =
          lower_base +
          (higher_base - lower_base) * (partial_util_area / total_util_area);
      ++rit_cs;
      ++osbkgrit;
    }
    post.dbg << "\n";
  }

  double A_max = 0;
  vsz_t i_of_max;
  for (vsz_t i = 0; i < m.size(); ++i)
  { m[i][4] = shirley_background[i];
    m[i][5] = m[i][3] - m[i][4];
    if (m[i][5] > A_max)
    { A_max = m[i][5];
      i_of_max = i;
    }
  }
  sequences().A(A_max);
  sequences().mu(static_cast<double>(i_of_max)); // 0 < x < m.size() reversed
  sequences().sigma(static_cast<double>(m.size()) / 6.0); // good if centered
  return SUCCESS;
}

/*
  2200 +----------+-----------+----------+----------+-----------+----------+
       |                                                      count ****** |
  2000 +                                 ******             shirley ssssss-+
       |                                *      *                           |
  1800 +                               *       |*                          +
       |                              *        |*                          |
  1600 +                              *        |.*                         +
       |                             *         |.*                         |
  1400 +                            *          |..*                        +
       +                           *           |..*                        |
  1200 +                           *           |..*                        +
       |                          *            |...*                       |
  1000 +                        **             |...*                       +
       |                       *               |....**                     |
   800 +                     **                |......*                    +
       |     |            ***                  |.......**                  |
   600 +    \|/      *****           |         |.........*                 +
       **************ssssssssssssss \|/        |.........                  |
   400 +                           ssssssssssss|..........*****      |     +
       |                                       sssss...........*    \|/    |
   200 +- - - lower_base_line - - - - - - - - -.....ssssssssssss******- - -+
       |                                                                   |
     0 +----------+-----------+----------+-----^----+-----------+----------+
      310   /|\  309         308    /|\ 307        306         305  /|\   304
             |                       |                               |
        higher_base          shirley_background                  lower_base
         
- The total_util_area is that under the count curve (under de '*' curve) and
      above the lower_base_liner. It includes dashed and non dashed areas.
- The partial_util_area (at energy 306.5 eV) is that shadded with dots '.',
      calculated before the existence of the shirley curve ('s').
- The shirley_background grows up from the lower_base to the
      higher_base at the rate partial_util_area / total_util_area
- The same calculation is done several (5) times.
*/

/*static*/ int Region::gaussian_f(const gsl_vector* x, void* data,
                                  gsl_vector* f)
{ size_t n = (static_cast<struct data*>(data))->n;
  double* y = (static_cast<struct data*>(data))->y;
  double* r_s = (static_cast<struct data*>(data))->r_s;
  double A = gsl_vector_get(x, 0);
  double mu = gsl_vector_get(x, 1);
  double sigma = gsl_vector_get(x, 2);
  for (size_t i = 0; i < n; ++i)
  { double t = i;
    double Yi = A * exp(-1.0 / 2.0 * gsl_sf_pow_int((t - mu), 2) /
                        gsl_sf_pow_int(sigma, 2));
    gsl_vector_set(f, i, (Yi - y[i]) / r_s[i]);
  }
  return GSL_SUCCESS;
}

/*static*/ int Region::gaussian_df(const gsl_vector* x, void* data,
                                   gsl_matrix* J)
{ size_t n = (static_cast<struct data*>(data))->n;
  double* r_s = (static_cast<struct data*>(data))->r_s;
  double A = gsl_vector_get(x, 0);
  double mu = gsl_vector_get(x, 1);
  double sigma = gsl_vector_get(x, 2);
  for (size_t i = 0; i < n; ++i)
  { double t = i;
    double s = r_s[i];
    double e = exp(-1.0 / 2.0 * gsl_sf_pow_int((t - mu), 2) /
                   gsl_sf_pow_int(sigma, 2));
    gsl_matrix_set(J, i, 0, e / s);
    gsl_matrix_set(J, i, 1, -(mu - t) * A * e / s);
    gsl_matrix_set(J, i, 2, gsl_sf_pow_int(mu - t, 2) * A * e /
                                gsl_sf_pow_int(sigma, 3) / s);
  }
  return GSL_SUCCESS;
}

/*static*/ int Region::gaussian_fdf(const gsl_vector* x, void* data,
                                    gsl_vector* f, gsl_matrix* J)
{ gaussian_f(x, data, f);
  gaussian_df(x, data, J);
  return GSL_SUCCESS;
}

rvEnum Region::Fit(unsigned int max_n_iters)  // ----------------------------------
{ //need check n_iteractions < 0
  std::vector<std::vector<double>>& m = sequences().values_matrix(); // simplify

  post.dbg << "Inside Fit.\naux.size() = " << m.size() << "\n";
  const gsl_multifit_fdfsolver_type* T;
  gsl_multifit_fdfsolver *s;
  int status;
  unsigned int i, iter = 0;
  const size_t n = n_counts(); // todo: peak as part of sequence use begin end
  const size_t p = 3; // todo: change to 2
  // gsl_matrix* covar = gsl_matrix_alloc(p,p)
  double y[n];
  double r_s[n];
  struct data d = {n, y, r_s};
  gsl_multifit_function_fdf f;
  post.dbg << "Before iteractions x_init = {sequences().A(), sequences().mu(), " \
           << "sequences().sigma()} = {" << sequences().A() << ","\
           << sequences().mu() << "," << sequences().sigma() << "}\n";
  double x_init[3] = {sequences().A(), sequences().mu(), sequences().sigma()};
  gsl_vector_view x = gsl_vector_view_array(x_init, p);
  f.f = &Region::gaussian_f;
  f.df = &Region::gaussian_df;
  f.fdf = &Region::gaussian_fdf;
  f.n = n;
  f.params = &d;
  for (i = 0; i < n; ++i)
  { y[i] = m[i][5];
    r_s[i] = 1.0;
    post.dbg << "y[i = " << i << "] = " << y[i] << "\n";
  }
  T = gsl_multifit_fdfsolver_lmsder;
  s = gsl_multifit_fdfsolver_alloc(T, n, p);
  gsl_multifit_fdfsolver_set(s, &f, &x.vector);
  do
  { ++iter;
    status = gsl_multifit_fdfsolver_iterate(s);
    if (status)
    { post.dbg << "status = " << status << " = " << gsl_strerror(status)
               << " from within if status\n ";
          break;
    }
    status = gsl_multifit_test_delta(s->dx, s->x, 1e-4, 1e-4);
  } while (status == GSL_CONTINUE && iter < max_n_iters);
  post.dbg << "iter = " << iter << ".\n";

  sequences().A(gsl_vector_get(s->x, 0));
  sequences().mu(energy_high()-(gsl_vector_get(s->x,
        1))/(n)*(energy_high()-energy_low()+1));
  sequences().sigma((gsl_vector_get(s->x, 2))/(n)*(energy_high()-energy_low()+1));
  post.dbg << "After iteractions x_init = {sequences().A(), sequences().mu(), " \
           << "sequences().sigma()} = {" << sequences().A() << "," <<
           sequences().mu()\
           << "," << sequences().sigma() << "}\n";
  sequences().area(area6sigma*sequences().A()*sequences().sigma());
  return SUCCESS;
}

void Region::set_after_load_counts() // ---------------------------------------
{ //calculateCountPerSecond();
  if (settings("load_mode") == "xpd")
  { { unsigned int n_iteractions = 5;
      Smooth(n_iteractions);
    }

    { unsigned int n_iteractions = 4;
      Shirley(n_iteractions);
    }

    { unsigned int max_n_iters =
          std::stoul(settings("unsigned_int_maximum_number_of_iteractions"));
      post.dbg << "{sequences().A(), sequences().mu(), sequences().sigma()} = {"\
               << sequences().A() << ", " << sequences().mu() << ", "\
               << sequences().sigma() << "}\n";
      Fit(max_n_iters);
      post.dbg << "{sequences().A(), sequences().mu(), sequences().sigma()} = {"\
               << sequences().A() << ", " << sequences().mu() << ", "\
               << sequences().sigma() << "}\n";
    }
  }
}

rvEnum Region::load() // ------------------------------------------------------
{ { rvEnum r_v;
    if ( (r_v = read_header()) != SUCCESS )
    { post.err << "Fail to read the region header.\n";
      return r_v;
    }
  }

  set_after_read_header();

  { rvEnum r_v;
    if ( (r_v = load_counts()) != SUCCESS )
     { post.err << "Fail to read counts.\n";
       return r_v;
     }
  }

  set_after_load_counts();

  return SUCCESS;
}

void Region::writeAll() // -----------------------
{ int j = 0;  // i_plot_scripts_of_a_region
  for (auto& plot2d : plot2ds())
  {
    post.dbg << "Writing the plot scripts for plot number " << ++j << ".\n";
    plot2d.writePlot();
  }
}

/*
   // old shirley with vector
   std::vector<double> Region::shirleyMatrixColumn(const std::vector<double> &v)
{ int n = v.size();
  int begin = 0;
  int end = n-1;
  double a = v[begin];
  double b = v[end];
  double sum1; double sum2;
  std::vector<double> background(n);
  std::vector<double> background0(n,b);
  for (int i0 = 1; i0 <= 6; ++i0) // i0 = i_iteraction
  { for (int i1 = end; i1 >= begin; --i1) // i1 = i_count
    { a = v[begin]; b = v[end];
      sum1 = 0; sum2 = 0;
      for (int i2 = end; i2 >= i1; --i2) // i2 = i_count_low_energy_partial_area
        sum1 += v[i2] - background0[i2];
      for (int i3 = end; i3 >= begin; --i3) // i3 = i_count_total_area
        sum2 += v[i3] - background0[i3];
      if (sum2 == 0) // no peak
      { for (int i = i1; i >=begin; --i)
        { background[i] = 10000000; // todo better way to flag this data error
          i1 = begin - 1; // quits the for loop
        }
      }
      else
        background[i1] = b + (a-b) * sum1/sum2;
    }
    background0 = background;
  }
  return background;
}
*/

// end of Region.cc
