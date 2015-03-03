#ifndef DETECTOR_CXX
#define DETECTOR_CXX

#include "Detector.h"

namespace sbn {

  Detector::Detector(readConfig r, int basel, int mode){
    // set the data members:
    this->basel  = (BASEL)  basel;
    this->mode   = (MODE)   mode;

    // copy the necessary information from the mother's readConfig:
    // Only copying information that is generic to this detector
    // config.signal                = r.signal;
    config.path                  = r.path;
    config.baseline              = r.baseline;
    config.mode                  = r.mode;
    // config.energy                = r.energy;
    config.npoints               = r.npoints;
    config.forceRemake           = r.forceRemake;
    config.usingMultiWeights     = r.usingMultiWeights;
    config.absolute_MWSource     = r.absolute_MWSource;
    config.nWeights              = r.nWeights;
    // config.includeCosmics        = r.includeCosmics;
    // config.includeDirt           = r.includeDirt;
    // config.showerContainmentDist = r.showerContainmentDist;
    // config.minDistanceToStart    = r.minDistanceToStart;
    // config.minVertexEnergySignal = r.minVertexEnergySignal;
    // config.minVertexEnergyPhoton = r.minVertexEnergyPhoton;
    // config.minShowerGap          = r.minShowerGap;
    return;
  }

  bool Detector::init(){
    bool result = true;
    for (auto event : samples){
      result = (result && event.second.init());
    }
    return result;
  }


  bool Detector::read(){
    bool result = true;
    for (auto event : samples){
      result = (result && event.second.read());
    }
    return result;
  }

} // sbn


#endif
