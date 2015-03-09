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
#include "DetUtils.h"
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
    Detector(readConfig r, int basel);

    /// Default destructor
    ~Detector(){}

    void addSample(int sample);

    // Work flow functions:
    bool init(); // uses private config as default
    bool read(); //Calls read on all owned EventSample classes

    // Return the combined vector of data
    std::vector<float> const & getCombinedData();
    //The osc data, returns the closest match in sin22th, dm2 space
    // Each signal type gets its own sin22th (can extend to dm2)
    void getCombinedDataOsc(std::vector<float> & dataOsc,
                            std::vector<float> sin22th,
                            float dm2);
    //The osc data, returns the point of each as requested
    // Each signal type gets its own sin22th (can extend to dm2)
    void getCombinedDataOsc(std::vector<float> & dataOsc,
                            std::vector<int> sin22th_point,
                            int dm2_point);

    std::vector<float> const &  getCombinedBins();

    // Return the uncombined samples of data:
    // These just go through the appropriate EventSample object
    std::vector<float> const &  getSampleData(int sample);
    //The osc data, returns the closest match in sin22th, dm2 space
    std::vector<float> getSampleDataOsc(int sample, 
                                        float sin22th, 
                                        float dm2);
    //The osc data, returns the point of each as requested
    std::vector<float> getSampleDataOsc(int sample,
                                        int sin22th_point, 
                                        int dm2_point);

    std::vector<float> const &  getSampleBins(int sample);


  private:

    DetUtils utils;

    // each Detector gets a container of signals
    std::map<int, EventSample> samples;
    //not default bins, but the specific, combined bins of all signals
    std::map<int, std::vector<float> > bins;

    // These data are made when needed but then cached in case needed again:
    std::vector<float> binsCombined;
    std::vector<float> dataCombined;  //again, combined data

    readConfig config;

  };

} // sbn

#endif
/** @} */ // end of doxygen group 

