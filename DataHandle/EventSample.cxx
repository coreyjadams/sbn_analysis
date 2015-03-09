#ifndef EVENTSAMPLE_CXX
#define EVENTSAMPLE_CXX

#include "EventSample.h"

namespace sbn{
  
  EventSample::EventSample(readConfig r, int input_signal){
    
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
    this -> config = r;
    this -> config.signal = (SIGNAL) input_signal;

  }


  bool EventSample::init(){
    // Pass this EventSample's config to the reader.
    return reader.init(config);
  }

  bool EventSample::read(){
    reader.getData(nominalData_unbinned, nominalDataByType_unbinned);
    reader.getDataOsc(oscData_unbinned);
    // Now bin the data 


    nominalData = utils.rebinVector(nominalData_unbinned, activeBins);
   
    nominalDataByType.resize(nominalDataByType_unbinned.size());
    for (unsigned int i = 0; i < nominalDataByType_unbinned.size(); i++)
      nominalDataByType[i] = utils.rebinVector(nominalDataByType_unbinned[i], activeBins);
   
    oscData.resize(oscData_unbinned.size());
    for (unsigned int i = 0; i < oscData_unbinned.size(); i++)
      oscData[i] = utils.rebinVector(oscData_unbinned[i], activeBins);

    return true;
  }

    /// Get the data:
    std::vector<float> const &  EventSample::getData() const{
      return nominalData;
    }
    //The osc data, returns the closest match in sin22th, dm2 space
    std::vector<float>  EventSample::getDataOsc(float sin22th, float dm2) {

      // Already have sin22th, just get the closest dm2point:
      int dm2_point = utils.dm2ValueToIndex(dm2,config);
      std::vector<float> returnVec;
      returnVec.reserve(oscData[dm2_point].size());
      for(unsigned int i =0; i < returnVec.size();i++)
        returnVec.push_back(sin22th*oscData[dm2_point][i]);
      return returnVec;

    }
    //The osc data, returns the point of each as requested
    std::vector<float>  EventSample::getDataOsc(int sin22th_point, int dm2_point) {
      // dm2_point is already the index needed to get the right osc data;
      // need the correct scale factor, so set it up:
      float sin22th = utils.sin22thIndexToValue(sin22th_point,config);
      std::vector<float> returnVec;
      returnVec.reserve(oscData[dm2_point].size());
      for(unsigned int i =0; i < returnVec.size();i++)
        returnVec.push_back(sin22th*oscData[dm2_point][i]);
      return returnVec;
    }

    std::vector<std::vector<float>> const &  EventSample::getDataByType(){
      return nominalDataByType;
    }

    // Useful to be able to get the bins ...
    std::vector<float> const &  EventSample::getBins() const{
      return defaultBins;
    }

}

#endif
