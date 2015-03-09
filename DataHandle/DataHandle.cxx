#ifndef DATAHANDLE_CXX
#define DATAHANDLE_CXX

#include "DataHandle.h"

namespace sbn {

  void DataHandle::addSignal(int signal){

    // verify that the signal exists.  Check against basedata.h:
    if (signal < 0 || signal >= kNSignal ){
      std::cerr << "Error, the signal you requested (" << signal
                << " == " << signalMap[signal] << ") is not an option.\n"
                << "  See basedata.h for a list of available signals."
                << std::endl;
      return;
    }

    // Detectors own signals, so just call the function to add it 
    // for each detector
    for (auto & det : detMap){
      det.second.addSample(signal);
    }
  }
  void DataHandle::addDetector(int baseline){

    if (baseline < 0 || baseline >= kNBaseline ){
      std::cerr << "Error, the baseline you requested (" << baseline
                << " == " << baselMap[baseline] << ") is not an option.\n"
                << "  See basedata.h for a list of available signals."
                << std::endl;
      return;
    }

    // Is this detector already on the list?  no duplicates allowed:
    if (detMap.find(baseline) != detMap.end() ){
      detMap[baseline] = Detector(config, (BASEL) baseline);
    }
    else{
      std::cerr << "Error creating Detector at (" << baseline
                << " == " << baselMap[baseline] << ").\n"
                << "  The requested detector already exists."
                << std::endl;
      return;
    }

    return;
  }
//   void setEnergy(int energy);
//   void setMode(int mode);
  
  //uses private readConfig member as default
  bool DataHandle::init(){
    bool result = true;
    for (auto & det : detMap )
      result = result && det.second.init();
    return result;  
  }


  bool DataHandle::read(){
    bool result = true;
    for (auto & det : detMap )
      result = result && det.second.read();
    return result;
  }

// const std::vector<float> getData();
// const Detector & getDetector(int baseline);

} // sbn

#endif
