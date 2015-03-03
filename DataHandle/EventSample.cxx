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

    this -> config = r;
  }

  bool EventSample::init(){
    return reader.init(config);
  }

  bool EventSample::read(){
    reader.readData(nominalData, nominalDataByType);
    /* 
      perform
      checks
      on
      data
    */
    return true;
  }

}

#endif
