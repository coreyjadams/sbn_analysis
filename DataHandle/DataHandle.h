/**
 * \file DataHandle.h
 *
 * \ingroup DataHandle
 * 
 * \brief Class def header for a class DataHandle
 *
 * @author cadams
 */

/** \addtogroup DataHandle

    @{*/
#ifndef DATAHANDLE_H
#define DATAHANDLE_H

#include <iostream>
#include "basedata.h"
#include <vector>
#include <map>
#include "Detector.h"
/**
   \class DataHandle
   This class is the main interface class for accessing the data.
   It can manage the detector objects, which in turn manage EventSample
   objects.  Use this class to get your data, and to set up configurations
   for the analysis.

   In general, use the readConfig struct to manage parameters.  The struct 
   is modified slightly for each detector or sample, but it controls the
   behavior of the event rates.  It's also saved to the file with histograms
   so that you can, at a later time, review the parameters that were used.

   It's assumed that you will *not* interface directly with subclasses like
   Detector.h and EventSample.h, so input parameter checking is largely handled
   here in this class.  To use this object, a script might go like:
   .
   . -> Set all config variables
   . -> Initialize detectors
   . -> Select Samples/Signals
   . -> Call DataHandle.init()
   . -> Call DataHandle.read()
   . if DataHandle.read() returns true, everything is read in and ready to use
 */
namespace sbn {

class DataHandle{

public:

  /// Default constructor
  DataHandle(){}

  /// Default destructor
  ~DataHandle(){}

  void addSignal(int signal);
  void addDetector(int baseline);
  void setEnergy(int energy);
  void setMode(int mode);
  
  bool init(); //uses private readConfig member as default
  bool read();

  // This function returns a reference to one of the Detector.h objects.
  // Useful if you just want that detector's rates
  const Detector & getDetector(int baseline);
  //

  // These functions allow you to get the nominal or oscillated data
  // from all detectors and all signals at once.
  // For just one detector (combined or signal sample) go through
  // the getDetector function and the subsequent member functions
  // from Detector.h
  const std::vector<float> getData();

private:

  // This map manages the detector objects.
  std::map<int, Detector> detMap;

  // this variable caches the combined data:
  std::vector<float> combinedData;

  readConfig config;

};
} // sbn

#endif
/** @} */ // end of doxygen group 

