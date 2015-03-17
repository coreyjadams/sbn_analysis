/**
 * \file EventRate.h
 *
 * \ingroup Analysis
 * 
 * \brief Class def header for a class EventRate
 *
 * @author cadams
 */

/** \addtogroup Analysis

    @{*/
#ifndef EVENTRATE_H
#define EVENTRATE_H

#include <iostream>
#include "DataHandle.h"
#include "EventRateUtils.h"

#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"


/**
   \class EventRate
   This class is the base class for making an event rate plot
   You can use it to pull in your data, build a legend, create a stacked histogram, etc.
 */
namespace sbn{
  class EventRate{

  public:

    /// Default constructor
    EventRate(){}

    /// Default destructor
    ~EventRate(){}

    void mainLoop();

    TH1F * getStackedHistogram();

    void setSignalValues(float sin22th, float dm2);

  private:

    DataHandle dh;

    EventRateUtils utils;

    bool includeSignal;

  // const std::vector<TString>  nueMapFancy { "#mu #rightarrow #nu_{e}", "K^{+} #rightarrow #nu_{e}",
  //                 "K^{0} #rightarrow #nu_{e}", "#nu - e^{-}", "NC Single #gamma",
  //                 "#Delta #rightarrow N#gamma", "#nu_{#mu} CC", "Dirt",
  //                 "Cosmics", "Other"};

  // const std::vector<TString> 
  //   numuMapFancy {"#nu_{#mu} CC", "NC #pi^{+/-}"};

  };

}

#endif
/** @} */ // end of doxygen group 

