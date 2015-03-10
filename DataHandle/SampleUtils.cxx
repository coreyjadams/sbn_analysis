#ifndef SAMPLEUTILS_CXX
#define SAMPLEUTILS_CXX

#include "SampleUtils.h"

namespace sbn{
SampleUtils::SampleUtils(){
    double emin = 0.0;
    double emax = 3.0;
    double nbins = 120.0;
    double binSize = (emax - emin)/nbins;
    default_bins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      default_bins.at(i) = emin + i*binSize;
    }
    
}

bool SampleUtils::fileExists(const char *filename) const{
  std::ifstream ifile(filename);
  return ifile.is_open();
}

std::vector<float> SampleUtils::rebinVector(std::vector<float> inputVector,
                                            std::vector<float> bins)
  {

    if (bins.size() < 2 || inputVector.size()+1 != default_bins.size()){
      std::cerr << "Error in the input bins for rebinVector!" << std::endl;
      exit(-9);
    }

    // the input vector is meant to have the default bins which are defined above
    // So we loop over the input vector and pack the data into the appropriate
    // spots in the output vector

    std::vector<float> result;
    result.resize(bins.size()-1);

    // loop over the input vector of data (inputVector) and know that
    // the bin boundaries for the data at inputVector[i] are 
    // default_bins[i] and default_bins[i+1].
    // For each bin, we have to find out what spot in the result vector
    // the data gets added to.  Again, result[i] has bin boundaries given
    // by bins[i] to bins[i+1]
    // 
    // I'm not going to check for pathological cases where bins is more fine than
    // the default bins, sorry.  Just don't do that.
    for(unsigned int inputBin = 0; inputBin < inputVector.size(); inputBin++){
      // So, we want to find out where to put this input data
      // into the output data.  That means seeing where this bin's boundaries
      // lie wrt the input bins.
      // std::cout << "This is bin " << inputBin << "(E = " << default_bins[inputBin] << ")"
                // << " with content " << inputVector[inputBin] << std::endl;
      // 
      // first catch the case that the first bin is not at zero:
      if (default_bins[inputBin] < bins.front()) continue;
      // and catch the case that we're being more restrictive than the 
      // default bins at the high end:
      if (default_bins[inputBin] > bins.back()) continue;
      // Next, look at each bin and find where this input bin [lower] 
      // is greater than or equal to a final bin:
      unsigned int tempLowerBin = 0;
      for (tempLowerBin = 0; tempLowerBin < bins.size(); ++tempLowerBin)
      {
        if (default_bins[inputBin] == bins[tempLowerBin]){
          // std::cout << "  Found a match at " << tempLowerBin <<": "
                    // << default_bins[inputBin] << "==" << bins[tempLowerBin]
                    // << std::endl;
          break;
        }
        if (default_bins[inputBin] < bins[tempLowerBin]){
          // std::cout << "  bin is greater at " << tempLowerBin <<": "
                    // << default_bins[inputBin] << "< " << bins[tempLowerBin]
                    // << std::endl;
          tempLowerBin --;
          break;
        }
      }
      // Now do the rebinning:
      // std::cout << "Adding content " << result[tempLowerBin] << " to the final bin spot: " << tempLowerBin<<"\n";
      result[tempLowerBin] += inputVector[inputBin];
    }

    return result;

  }
  
  float SampleUtils::dm2IndexToValue(int dm2Index, const readConfig & r){
    // method: find the range of dm2 values from readConfig
    // use the index to calculate the value
    return r.dm2min*pow(r.dm2max/r.dm2min,dm2Index/r.npoints);
  }
  float SampleUtils::sin22thIndexToValue(int sin22thIndex, const readConfig & r){
    return r.sin22thmin*pow(r.sin22thmax/r.sin22thmin,sin22thIndex/r.npoints);

  }  
  unsigned int SampleUtils::dm2ValueToIndex(int dm2, const readConfig & r){
    float val= r.npoints*(log(dm2/r.dm2min) / log(r.dm2max/r.dm2min));
    return (int) nearbyint(val);
  }
  unsigned int SampleUtils::sin22thValueToIndex(int sin22th, const readConfig & r){
    float val = r.npoints*(log(sin22th/r.sin22thmin) / log(r.sin22thmax/r.sin22thmin));
    return (int) nearbyint(val);
  } 



}
#endif
