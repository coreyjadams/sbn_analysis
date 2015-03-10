/**
 * \file PlotUtils.h
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class PlotUtils
 *
 * @author cadams
 */

/** \addtogroup Analysis

    @{*/
#ifndef PLOTUTILS_H
#define PLOTUTILS_H

#include <iostream>
#include "BaseUtils.h"

// Root includes
#include "TLatex.h"
#include "TColor.h"
#include "TStyle.h"

/**
   \class PlotUtils
   User defined class PlotUtils ... these comments are used to generate
   doxygen documentation!
 */
namespace sbn {
   
  class PlotUtils : public  BaseUtils {

  public:

    /// Default constructor
    PlotUtils(){}

    /// Default destructor
    ~PlotUtils(){}

    void add_plot_label(char* label, double x, double y,
                        double size = 0.05, int color = 1,
                        int font = 72, int align = 22 );

    void set_plot_style();
    

  };

} // sbn   

#endif
/** @} */ // end of doxygen group 

