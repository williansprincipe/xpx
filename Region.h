// Region.h
// Region declarations
// wpfernandes 2015-11-02 14h30min

#ifndef REGION_H
#define REGION_H

#include "Plot2D.h"
#include "Post.h"
#include "globalVals.h"
#include "Settings.h"

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

class Sequences
{public:
  Sequences()=default;
 ~Sequences()=default;

  bool empty() { return values_matrix_.empty(); }
  vsz_t number_of() { return names_.size(); }
  vsz_t size_of_one() { return values_matrix_.size(); }

  std::vector<std::string>& names() { return names_; }
  std::string& name(vsz_t index) { return names_[index]; }
  void names_add(std::string name) { names_.push_back(name); }
  std::vector<std::vector<double>>& values_matrix() { return values_matrix_; }

  double A() { return A_; }
  void A(double A) { A_ = A; }
  double mu() { return mu_; }
  void mu(double mu) { mu_ = mu; }
  double sigma() { return sigma_; }
  void sigma(double sigma) { sigma_ = sigma; }
  double area() { return area_; }
  void area(double area) { area_ = area; }
     
 private:
  std::vector<std::string> names_;
  std::vector<std::vector<double>> values_matrix_;
  double A_;
  double mu_;
  double sigma_;
  double area_;
};

class Region
{public:
  Region() = delete;
  Region(Settings& settings, Post& post, const std::string& source_file_name,
         int theta, int phi, 
         int number, std::ifstream& ifs, const std::string& region_name)
      : settings(settings),
        post(post),
        source_file_name_(source_file_name),
        theta_(theta),
        phi_(phi),
        number_(number),
        ifs_(ifs)
  { }
  ~Region() = default;

  // exposed member functions =================================================
  ctrlEnum load(); // load = read_header + set label + load_counts
//  ctrlEnum Shirley();
  void plot2ds_add_move(Plot2D& plot2d) // also used by add_default_plot2d
  { plot2ds_.push_back(std::move(plot2d));
  }
  ctrlEnum genDefaultPlot2D();
  void genOnePlotPerRegion();
  void genOnePlotScriptPerPlot();
  void write_all();

  // accessors and mutators ===================================================

  const std::string& region_name() { return region_name_; }
  void region_name(const std::string& region_name) { region_name_
    = region_name;
  }

  const int theta() const { return theta_; }
  void theta(const int theta)
  { theta_ = theta;
  }

  const int phi() const { return phi_; }
  void phi(const int phi)
  { phi_ = phi;
  }

  const std::string& label() const { return label_; }
  void label(const std::string& label) { label_ = label; }

  const int n_regions() const { return n_regions_; }
  void n_regions(int n_regions)
  { n_regions_ = n_regions;
  }

  // accessors with no mutators ===============================================
  const std::string& source_file_name() const { return source_file_name_; }
  int number() { return number_; }
  double energy_high()  { return energy_high_; }
  double energy_low()   { return energy_low_; }
  double energy_step()  { return energy_step_; } 
  double scans() { return scans_; }
  double dwell()        { return dwell_; }
  int    n_counts()     { return n_counts_; }
  double energy_pass()  { return energy_pass_; }

  std::string lo_hi()         { return lo_hi_; }
  std::string magpe()         { return magpe_; }
  std::string ke_factor()     { return ke_factor_; }
  std::string excitation()    { return excitation_; }
  std::string work_function() { return work_function_; }
  std::string ck()            { return ck_; }
  std::string det()           { return det_; }
  std::string ent()           { return ent_; }
  std::string exit()          { return exit_; }

  Sequences& sequences() { return sequences_; }

  std::vector<Plot2D>& plot2ds() {return plot2ds_; }
  const std::vector<double>::size_type plot2ds_size()  {return plot2ds_.size(); }
  std::vector<Plot2D>::iterator plot2ds_begin() {return plot2ds_.begin();}
  std::vector<Plot2D>::iterator plot2ds_end()   {return plot2ds_.end();  }

 private:
  Settings& settings;
  Post& post;
  const double area6sigma = 2.49986088948310; // sqrt(2*pi)*erf(3/2*sqrt(2))
  const std::string& source_file_name_;
  int theta_;
  int phi_;
  const int number_;
  std::ifstream& ifs_;

  std::string region_name_; // used for script file and image file
  std::string label_;
  int n_regions_; // I chose to keep one copy per region

  // members functions called internally by load.
  ctrlEnum read_header(); 
  void set_default_plot2d_sequences_names();
  void set_after_read_header();
  // load_mode value makes load_counts create different number of sequences
  // lmXPS: 3 (energy, count, count/s);
  // lmXPD: 6 (energy, count, count/s, smooth, shirley, smooth_minus_shirley).
  ctrlEnum load_counts(); 
  void set_after_load_counts();

  // members functions called internally by load_counts.
  ctrlEnum Smooth(unsigned int n_iteractions); // iif load_mode == lmXPD
  ctrlEnum Shirley(unsigned int n_iteractions);
  ctrlEnum Fit(unsigned int max_n_iters);

  // members of xps data of one region row 1
  double energy_high_;
  double energy_low_;
  double energy_step_;
  double scans_;
  double dwell_;
  int n_counts_;
  double energy_pass_;
  // row 2
  std::string lo_hi_;        
  std::string magpe_;
  std::string ke_factor_;
  std::string excitation_;
  std::string work_function_;
  std::string ck_;
  std::string det_;
  std::string ent_;
  std::string exit_;

  struct data
  { size_t n;
    double* y;
    double* r_s; // residual_sigma
  };
  static int gaussian_f(const gsl_vector* x, void* data, gsl_vector* f);
  static int gaussian_df(const gsl_vector* x, void* data, gsl_matrix* J);
  static int gaussian_fdf(const gsl_vector* x, void* data, gsl_vector* f,
                          gsl_matrix* J);
  
  Sequences sequences_;

  std::vector<Plot2D> plot2ds_;
};

#endif

// Important: XPSData don't stores a vector of Sequences. Is stores the names in
// one vector and the values in a matrix. Though XPSData has a function member
// named sequence(index) that returns the indexth sequence.
//
// There is an example bellow of sequences_names_ vector and
// sequences_values_matrix_ structure
//
// sequence_names: row vector with index 0 < n < n_sequences -1
//     Example: {"energies", "shirley", "count_minus_shirley", "smoothed"}
//
// sequences_values_matrix_{m x n}
// Index of lines: 0 < m < n_counts - 1
// Index of columns: 0 < n < n_sequences - 1
// Column (m = all, n = 0) = sequence x = energies
// Column (m = all, n = 1) = sequence y1 = count
// Column (m = all, n = 2) = sequence y2 = shirley_background
// Column (m = all, n = 3) = sequence y3 = count - shirley_background
// Column (m = all, n = 4) = sequence y4 = smooth
//
//     Example of sequences_values_matrix 
//                the title row is the row vector sequence_names
//=============================================================================
// \__   column   0          1          2          3           4          5
//    \__     energies    count      1_per_s    shirley     1_-_3     smoothed  
// row   \---------------------------------------------------------------------
//   0          858.0      1832       1832       1832        1832       1832
//   1          857.5      1830       1830       1830        1830       1830
//   2          857.0      1847       1847       1847        1847       1847
//   3          856.5      1772       1772       1772        1772       1772
//   4          856.0      1770       1770       1770        1770       1770
//   5          855.5      1855       1855       1855        1855       1855
//   6          855.0      1867       1867       1867        1867       1867
//   7          854.5      1959       1959       1959        1959       1959
//   8          854.0      2200       2200       2200        2200       2200
//   9          853.5      2552       2552       2552        2552       2552
//  10          853.0      3182       3182       3182        3182       3182
//  11          852.5      3472       3472       3472        3472       3472
//  12          852.0      2813       2813       2813        2813       2813
//  13          851.5      1839       1839       1839        1839       1839
//  14          851.0      1425       1425       1425        1425       1425
//  15          850.5      1240       1240       1240        1240       1240
//  16          850.0      1257       1257       1257        1257       1257
//  17          849.5      1221       1221       1221        1221       1221
//  18          849.0      1179       1179       1179        1179       1179
//  19          848.5      1170       1170       1170        1170       1170
//  20          848.0      1161       1161       1161        1161       1161
//=============================================================================

// end of Region.h
