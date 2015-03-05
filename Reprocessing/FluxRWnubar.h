/**
 * \file FluxRWnubar.h
 *
 * \ingroup Reprocessing
 * 
 * \brief Class def header for a class FluxRWnubar
 *
 * @author cadams
 */

/** \addtogroup Reprocessing

    @{*/
#ifndef FLUXRWNUBAR_H
#define FLUXRWNUBAR_H

#include <iostream>
#include "Rtypes.h"
#include <vector>

/**
   \class FluxRWnubar
   User defined class FluxRWnubar ... these comments are used to generate
   doxygen documentation!
 */

class TH1D;
class FluxRWnubar {

   public:
    FluxRWnubar();
    ~FluxRWnubar();

    Double_t GetWeight(Double_t energy, Int_t ntype, Int_t ptype);
    TH1D*    GetWeightHist(Int_t ntype, Int_t ptype);
    std::string GetEnv( const std::string & var );
    
   private:

    Double_t fBinSize;
    std::vector<Double_t> fGS[4][4]; //fGS[ntype][ptype]
    std::vector<Double_t> fMB[4][4]; //fMB[ntype][ptype]

  };

#endif
/** @} */ // end of doxygen group 

