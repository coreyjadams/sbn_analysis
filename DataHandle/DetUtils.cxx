#ifndef DETUTILS_CXX
#define DETUTILS_CXX

#include "DetUtils.h"

namespace sbn{

  void DetUtils::appendVector(std::vector<float> & inputVec,
                              const std::vector<float> & vecToAppend)
  {        
    inputVec.reserve(inputVec.size() + vecToAppend.size());
    for (auto & el : vecToAppend) inputVec.push_back(el);
    return;
  }
  
} //sbn

#endif
