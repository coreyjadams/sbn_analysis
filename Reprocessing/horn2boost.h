/**
 * \file horn2boost.h
 *
 * \ingroup Reprocessing
 * 
 * \brief Class def header for a class horn2boost
 *
 * @author cadams
 */

/** \addtogroup Reprocessing

    @{*/
#ifndef HORN2BOOST_H
#define HORN2BOOST_H

#include <iostream>
#include "Rtypes.h"
#include <vector>
 
/**
   \class horn2boost
   User defined class horn2boost ... these comments are used to generate
   doxygen documentation!
 */
class TH1D;
class horn2boost { 
  
 public:
  horn2boost(std::string hor2conf="");
  ~horn2boost();

  Double_t GetWeight(Int_t idet, Int_t ntype, Int_t ptype, Double_t energy);
  TH1D*    GetWeightHist(Int_t idet, Int_t ntype, Int_t ptype);

  std::string GetEnv( const std::string & var );

 private:

  Double_t fBinSize;
  Double_t kMaxWeight;
  std::vector<Double_t> fGS[2][3][4][4]; //fGS[conf][det][ntype][ptype]
  
  std::vector<std::string> kDet;
  std::vector<std::string> kNu;
  std::vector<std::string> kConf;
};

#endif
/** @} */ // end of doxygen group 

