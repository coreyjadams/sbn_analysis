#ifndef EVENTSAMPLE_CXX
#define EVENTSAMPLE_CXX

#include "EventSample.h"

namespace sbn{
  
  EventSample::EventSample(readConfig r, int signal, int basel, int energy, int mode){
    
    // initialize the default bins, 0 to 3 GeV in steps of 25 MeV:
    double emin = 0.0;
    double emax = 3.0;
    double nbins = 120.0;
    double binSize = (emax - emin)/nbins;
    defaultBins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      defaultBins.at(i) = emin + i*binSize;
    }
    
    // set the data members:
    this->signal = (SIGNAL) signal;
    this->basel  = (BASEL)  basel;
    this->energy = (ENERGY) energy;
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

}

#endif
