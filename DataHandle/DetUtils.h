/**
 * \file DetUtils.h
 *
 * \ingroup DataHandle
 * 
 * \brief Class def header for a class DetUtils
 *
 * @author cadams
 */

/** \addtogroup DataHandle

    @{*/
#ifndef DETUTILS_H
#define DETUTILS_H

#include <iostream>
#include <vector>
#include "SampleUtils.h"

namespace sbn {

/**
   \class DetUtils
   User defined class DetUtils ... these comments are used to generate
   doxygen documentation!
 */
class DetUtils : SampleUtils {

public:

  /// Default constructor
  DetUtils(){}

  /// Default destructor
  ~DetUtils(){}

  void appendVector(std::vector<float> & inputVector, 
                    const std::vector<float> & vecToAppend);

};
} // sbn

#endif
/** @} */ // end of doxygen group 

