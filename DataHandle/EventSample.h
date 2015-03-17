/**
 * \file EventSample.h
 *
 * \ingroup DataHandle
 * 
 * \brief Class def header for a class EventSample
 *
 * @author cadams
 */

/** \addtogroup DataHandle

    @{*/
#ifndef EVENTSAMPLE_H
#define EVENTSAMPLE_H

#include <iostream>
#include <vector> 

#include "basedata.h"
#include "NtupleReader.h"
#include "SampleUtils.h"
/**
   \class EventSample
   User defined class EventSample ... these comments are used to generate
   doxygen documentation!
 */

namespace sbn{


  class EventSample{


  public:

    /// Default constructor
    // EventSample(){}
    EventSample(readConfig r, int signal);

    /// Default destructor
    ~EventSample(){}

    // Work flow functions:
    bool init(); //initializes the reader object, verifies that the files are there
    bool read(); //reads in (or generates) the data

    /// Get the data:
    std::vector<float> const &  getData() const;
    //The osc data, returns the closest match in sin22th, dm2 space
    std::vector<float> getDataOsc(float sin22th, float dm2);
    //The osc data, returns the point of each as requested
    std::vector<float> getDataOsc(int sin22th_point, int dm2_point);

    std::vector<std::vector<float>> const & getDataByType() const;

    // Useful to be able to get the bins ...
    std::vector<float> const &  getBins() const;

    // Not dealing with multiweights yet ...

    void setBins  (std::vector<float> bins) {activeBins = bins;}

  private:

    /* This section is the configuration data */
    readConfig config;

    SampleUtils utils;


    /* This section is useful data like bins */
    std::vector<float> defaultBins;  // the bins things are read in as 
    std::vector<float> activeBins;   // the bins things are returned as

    /*This section is the event data*/
    std::vector<float> nominalData;
    std::vector<std::vector<float> > oscData;
    std::vector<float> nominalData_unbinned;
    std::vector<std::vector<float> > oscData_unbinned;

    /*This section is a breakdown of the nominal data by type. */
    std::vector<std::vector<float> >   nominalDataByType;
    std::vector<std::vector<float> >   nominalDataByType_unbinned;

    /*This section is multiweight data, if needed */
    std::vector<std::vector<float> > nominalDataMultiWeight;
    std::vector<std::vector<float> > nominalDataMultiWeight_unbinned;
 
    NtupleReader reader;

  };

} //namespace sbn

#endif
/** @} */ // end of doxygen group 

