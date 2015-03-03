/**
 * \file Detector.h
 *
 * \ingroup DataHandle
 * 
 * \brief Class def header for a class Detector
 *
 * @author cadams
 */

/** \addtogroup DataHandle

    @{*/
#ifndef DETECTOR_H
#define DETECTOR_H

#include <iostream>

#include "EventSample.h"
#include "basedata.h"
#include <map>
/**
   \class Detector
   User defined class Detector ... these comments are used to generate
   doxygen documentation!
 */
namespace sbn {

  class Detector{

  public:

    /// Default constructor
    Detector(){}
    Detector(readConfig r, int basel, int mode);

    /// Default destructor
    ~Detector(){}

    // Work flow functions:
    bool init(); //Calls init on all owned EventSample classes
    bool read(); //Calls read on all owned EventSample classes

    // Return the combined vector of data
    std::vector<float> const &  getCombinedData();
    //The osc data, returns the closest match in sin22th, dm2 space
    std::vector<float> const &  getCombinedDataOsc(float sin22th, float dm2) const;
    //The osc data, returns the point of each as requested
    std::vector<float> const &  getCombinedDataOsc(int sin22th_point, int dm2_point) const;

    std::vector<float> const &  getCombinedBins() const;

  private:

    BASEL  basel;
    MODE   mode;

    // each Detector gets a container of signals
    std::map<int, EventSample> samples;

    // These data are made when needed but then cached in case needed again:
    std::vector<float> bins;  //not default bins, but the specific, combined bins of all signals
    std::vector<float> data;  //again, combined data
    //combined oscillated data ... the dm2 points ONLY are cached, scale before each return
    std::vector<std::vector<float>> dataOsc; 

    readConfig config;

  };

} // sbn

#endif
/** @} */ // end of doxygen group 

