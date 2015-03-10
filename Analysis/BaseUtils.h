/**
 * \file BaseUtils.h
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class BaseUtils
 *
 * @author cadams
 */

/** \addtogroup Analysis

    @{*/
#ifndef BASEUTILS_H
#define BASEUTILS_H

#include <iostream>
#include "SampleUtils.h"

/**
   \class BaseUtils
   User defined class BaseUtils ... these comments are used to generate
   doxygen documentation!
 */
namespace sbn{

class BaseUtils : public SampleUtils{

public:

  /// Default constructor
  BaseUtils(){}

  /// Default destructor
  ~BaseUtils(){}

  TH1F* makeHistogram(const std::vector<float> & input, 
                      double lowbin, double highbin);
  TH1F* makeHistogram(const std::vector<float> & input, 
                      const std::vector<float> & bins);

  std::vector<float> collapseVector(std::vector<float> input,
                                    int nbins_nue, int nbins_numu, int nL);

  std::vector<float> smoothVector(const std::vector<float> & data, unsigned int range = 3);

  float getMaximum (const std::vector<float> &);

};
} //sbn

#endif
/** @} */ // end of doxygen group 

