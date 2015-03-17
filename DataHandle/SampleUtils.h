/**
 * \file SampleUtils.h
 *
 * \ingroup DataHandle
 * 
 * \brief Class def header for a class SampleUtils
 *
 * @author cadams
 */

/** \addtogroup DataHandle

    @{*/
#ifndef SAMPLEUTILS_H
#define SAMPLEUTILS_H

#include <iostream>
#include "TH1F.h"
#include <fstream>
#include <sstream>
#include "TLatex.h"
#include <algorithm>
#include "TMatrix.h"
#include "TFile.h"
#include "basedata.h"
/**
   \class SampleUtils
   User defined class SampleUtils ... these comments are used to generate
   doxygen documentation!
 */
namespace sbn{
class SampleUtils{
  public:

    /// Default constructor
    SampleUtils();

    /// Default destructor
    ~SampleUtils(){}

    // Worker Function, verifies that a file exists 
    bool fileExists(const char *filename) const;

    std::vector<float> rebinVector(std::vector<float> inputVector,
                                   std::vector<float> bins);

    float dm2IndexToValue(int dm2Index, const readConfig & r);  
    float sin22thIndexToValue(int sin22thIndex, const readConfig & r);  
    unsigned int dm2ValueToIndex(float dm2, const readConfig & r);  
    unsigned int sin22thValueToIndex(float sin22th, const readConfig & r);  

  private:
    std::vector<float> default_bins;


  };
}

#endif
/** @} */ // end of doxygen group 

