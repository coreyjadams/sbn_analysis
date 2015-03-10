#ifndef BASEUTILS_CXX
#define BASEUTILS_CXX

#include "BaseUtils.h"

namespace sbn {


  TH1F* BaseUtils::makeHistogram(const std::vector<float> & input,
                                 double lowbin, double highbin)
  {
    // This function determines the bin and calls the other function,
    // attempting to not repeat code.
    std::vector<float> bins;
    bins.reserve(input.size() +1);
    float spacing = (highbin - lowbin)/ (input.size());
    for (unsigned int i = 0; i < bins.size(); i++ )
      bins.push_back(spacing*i + lowbin);
    return makeHistogram(input, bins);
  }

  TH1F* BaseUtils::makeHistogram(const std::vector<float> & input,
                                 const std::vector<float> & bins)
  {
    if (bins.size() != input.size()+1){
      std::cerr << "Error in make histogram, number of bins doesn't match input size.\n";
      std::cerr << "  bins: " << bins.size() << "\n";
      std::cerr << "  input: " << input.size() << "\n";
      exit(-1);
    }
    static int i = 0;
    char name[200];
    sprintf(name, "result_%i", i);
    TH1F * result = new TH1F(name,name,input.size(),&(bins[0]));
    for (unsigned int j = 0; j < input.size(); j++){
        result ->SetBinContent(j+1, input[j]);
    }
    i++;
    return result;
  }

} // sbn

#endif
