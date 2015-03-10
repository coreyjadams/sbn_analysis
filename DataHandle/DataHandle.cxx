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
    if (detMap.find(baseline) == detMap.end() ){
      detMap[baseline] = Detector(_config, (BASEL) baseline);
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
  void DataHandle::setEnergy(int energy){
    this->_config.energy = (ENERGY) energy;
  }

  void DataHandle::setMode(int mode){
    this->_config.mode = (MODE) mode;
  }  

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

  const std::vector<float> & DataHandle::getData(){
    // Put together the nominal data.
    // It gets cached and stored for later.
    if (combinedData.size() != 0) return combinedData;
    else{
      // Stitch together the data from the detectors:
      for (auto & det : detMap)
      utils.appendVector(combinedData, det.second.getCombinedData());
    }
    return combinedData;
  }
  const Detector & DataHandle::getDetector(int baseline){
    // Be sure that the requested detector exists before trying to return it
    // Otherwise, map will initialize it which will cause errors
    if (detMap.find(baseline) != detMap.end()){
      return detMap[baseline];
    }
    else{
      std::cerr << "Error: the baseline you requested from getDector is not configured\n";
      exit(-1);
    }
  }


  // These functions allow you to get the combined data of specified signals
  // They're overloaded to accept a vector or just a single int to specify the signal
  std::vector<float> DataHandle::getData(const std::vector<int> & signals){
    // Append the vectors and return just those:
    std::vector<float> returnData;
    for (auto & det : detMap){
      for (auto i : signals){
        if (i < 0 || i > kNSignal){
          std::cerr << "Error: the signal you requested does not exist.\n";
          exit(-1);
        }
        utils.appendVector(returnData, det.second.getSampleData(i));
      }
    }
    return returnData;
  }
  void DataHandle::getOscData(std::vector<float> & oscData, const std::vector<float> & sin22th, 
                  float dm2, const std::vector<int> & signals){
    if (oscData.size() != 0) oscData.clear();
    // signals and sin22th had better be the same size!
    if (sin22th.size() != signals.size()){
      std::cerr << "Error: asking for signals and sin22ths of different ranges.\n";
      exit(-1);
    }
    for (auto & det : detMap){
      for (unsigned int i =0; i < signals.size(); i++){
        if (signals[i] < 0 || signals[i] > kNSignal){
          std::cerr << "Error: the signal you requested does not exist.\n";
          exit(-1);
        }
        utils.appendVector(oscData, det.second.getSampleDataOsc(signals[i], sin22th[i], dm2) );
      }
    }
  }

  void DataHandle::getOscData(std::vector<float> & oscData, std::vector<int> sin22th_point, 
                  int dm2_point, const std::vector<int> & signals){
    if (oscData.size() != 0) oscData.clear();
    // signals and sin22th had better be the same size!
    if (sin22th_point.size() != signals.size()){
      std::cerr << "Error: asking for signals and sin22ths of different ranges.\n";
      exit(-1);
    }
    for (auto & det : detMap){
      for (unsigned int i =0; i < signals.size(); i++){
        if (signals[i] < 0 || signals[i] > kNSignal){
          std::cerr << "Error: the signal you requested does not exist.\n";
          exit(-1);
        }
        utils.appendVector(oscData, det.second.getSampleDataOsc(signals[i], 
                           sin22th_point[i], dm2_point) );
      }
    }
  }
  
  // The overloaded versions:
  std::vector<float> DataHandle::getData(int signal){
    std::vector<int> sig = {signal};
    return getData(sig);
  }
  void DataHandle::getOscData(std::vector<float> & oscData, 
                              float sin22th, 
                              float dm2, int signal){
    std::vector<int> sig = {signal};
    std::vector<float> sin22th_vec = {sin22th};
    getOscData(oscData, sin22th_vec, dm2, sig);
    return;
  }
  void DataHandle::getOscData(std::vector<float> & oscData, 
                              int sin22th_point, 
                              int dm2_point, int signal){
    std::vector<int> sig = {signal};
    std::vector<int> sin22th_vec = {sin22th_point};
    getOscData(oscData, sin22th_vec, dm2_point, sig);
    return;
  }

  void DataHandle::setBins(int signal, const std::vector<float> & bins){
    if (signal >= 0 || signal < kNSignal){
      for (auto & det : detMap){
        if ( ! det.second.hasSample(signal) ){
          std::cerr << "Error: the signal you requested to add bins for does not exist.\n";
          exit(-1);
        }
        det.second.setBins(signal, bins);
      }
    }    
  }


} // sbn

#endif
