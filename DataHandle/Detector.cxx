#ifndef DETECTOR_CXX
#define DETECTOR_CXX

#include "Detector.h"

namespace sbn {

  Detector::Detector(readConfig r, int basel){
    // set the data members:
    this->config = r;
    //override the baseline in config
    this->config.basel = (BASEL) basel; 

    return;
  }

  Detector::~Detector(){
    for (auto it = samples.begin(); it != samples.end(); it++)
      delete it -> second;
  }

  void Detector::addSample(int sample){
    samples[sample] = new EventSample(config, sample);
  }

  bool Detector::hasSample(int sample){
    if (samples.find(sample) == samples.end()) {
      return false;
    }
    std::cout << "Determined that " << sample << " is present.\n\n\n\n\n";
    return true;
  }

  bool Detector::init(){
    bool result = true;
    for (auto event : samples){
      result = (result && event.second->init());
    }
    return result;
  }


  bool Detector::read(){
    bool result = true;
    for (auto event : samples){
      result = (result && event.second->read());
    }
    return result;

    // At the end of reading, make sure the combined bins are filled:
    if (binsCombined.size() != 0) binsCombined.clear();
    for (auto & es : samples)
      utils.appendVector(binsCombined, es.second->getBins()); 
  }


  // // Return the combined vector of data
  std::vector<float> const & Detector::getCombinedData(){
    if (dataCombined.size() != 0) return dataCombined;
    else{
      for (auto & es : samples)
        utils.appendVector(dataCombined,es.second->getData());
    }
    return dataCombined;
  }
  // //The osc data, returns the closest match in sin22th, dm2 space
  void  Detector::getCombinedDataOsc(std::vector<float> & dataOsc,
                                     std::vector<float> sin22th,
                                     float dm2){
    // No caching, just build the vector and return it
    
    // Do some checking on the input:
    if (sin22th.size() != samples.size() ){
      std::cerr << "Error, asking for combined data Osc with sinth22 points "
                << "not equal to the number of samples!" << std::endl;
      exit(-1);
    }
    if (dataOsc.size() != 0)
      dataOsc.clear();

    // Now fill in the data:
    unsigned int i = 0;
    for(auto & es : samples){
      utils.appendVector(dataOsc, es.second->getDataOsc(sin22th[i], dm2));
      i++;
    }
    return;
  }
  // //The osc data, returns the point of each as requested
  void  Detector::getCombinedDataOsc(std::vector<float> & dataOsc,
                                     std::vector<int> sin22th_points,
                                     int dm2_point){
    // No caching, just build the vector and return it
    
    // Do some checking on the input:
    if (sin22th_points.size() != samples.size() ){
      std::cerr << "Error, asking for combined data Osc with sinth22 points "
                << "not equal to the number of samples!" << std::endl;
      exit(-1);
    }
    if (dataOsc.size() != 0)
      dataOsc.clear();

    // Now fill in the data:
    unsigned int i = 0;
    for(auto & es : samples){
      utils.appendVector(dataOsc, es.second->getDataOsc(sin22th_points[i], dm2_point));
      i++;
    }
    return;
  }

  std::vector<float> const &  Detector::getCombinedBins(){
    if (binsCombined.size() != 0) return binsCombined;
    else{
      for (auto & es : samples){
        utils.appendVector(binsCombined, es.second->getBins());
      }
    }
    return binsCombined;
  }

  // Return the uncombined samples of data:
  // These just go through the appropriate EventSample object
  std::vector<float> const &  Detector::getSampleData(int sample){
    return samples[sample]->getData();
  }

  //The osc data, returns the closest match in sin22th, dm2 space
  std::vector<float> Detector::getSampleDataOsc(int sample, 
                                                float sin22th, 
                                                float dm2){
    return samples[sample]->getDataOsc(sin22th, dm2);
  }

  //The osc data, returns the point of each as requested
  std::vector<float> Detector::getSampleDataOsc(int sample,
                                                int sin22th_point, 
                                                int dm2_point){
    return samples[sample]->getDataOsc(sin22th_point, dm2_point);
  }

  std::vector<float> const &  Detector::getSampleBins(int sample){
      return samples[sample]->getBins();
  }

  void Detector::setBins(int sample, const std::vector<float> & bins){
    samples[sample] -> setBins(bins);
  }


  std::vector<std::vector<float> > Detector::getSampleDataByType(int signal){
    if (signal < 0 || signal >= kNSignal ){
      std::cerr << "Error, the signal you requested (" << signal
                << " == " << signalMap[signal] << ") is not an option.\n"
                << "  See basedata.h for a list of available signals."
                << std::endl;
      exit(-1);
    }
    if (samples.find(signal) == samples.end()){
      std::cerr << "Error, the sample you requested hasn't been configured\n";
      exit(-1);
    }
    return samples[signal] -> getDataByType();

  }



} // sbn


#endif
