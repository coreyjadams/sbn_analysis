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

    TH1F* AppendHistograms(TH1F * hist1, TH1F* hist2, TH1F* hist3);
    TH1F* AppendHistograms(TH1F * hist1, TH1F* hist2);

    std::vector<std::vector<float> > CollapseMatrix(
                              std::vector<std::vector<float> > entries,
                              int nbins_nue, int nbins_numu, int nL);
    TMatrix CollapseMatrix(TMatrix entries,
                              int nbins_nue, int nbins_numu, int nL);

    bool fileExists(const char *filename);

    std::vector<float> rebinVector(std::vector<float> inputVector,
                                   std::vector<float> bins);
    // std::vector<float> rebinVector(std::vector<float> & inputVector,
                                   // std::string binSchemeName);

    float dm2IndexToValue(int dm2Index, const readConfig & r);  
    float sin22thIndexToValue(int sin22thIndex, const readConfig & r);  
    unsigned int dm2ValueToIndex(int dm2, const readConfig & r);  
    unsigned int sin22thValueToIndex(int sin22th, const readConfig & r);  

  private:
    std::vector<float> default_bins;


  };
}

#endif
/** @} */ // end of doxygen group 

