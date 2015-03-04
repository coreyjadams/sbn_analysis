/**
 * \file NtupleReader.h
 *
 * \ingroup DataHandle
 * 
 * \brief Class def header for a class NtupleReader
 *
 * @author cadams
 */

/** \addtogroup DataHandle

    @{*/
#ifndef NTUPLEREADER_H
#define NTUPLEREADER_H

// std includes
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>

// sbs_analysis includes
#include "basedata.h"

// root includes
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"


/**
   \class NtupleReader
   This class interfaces with files to read in vectors and set them 
   up for the EventSample class.  It only stores information needed to find
   the files and read them in.  All other information, like the vectors
   themselves, is stored in EventSample.
   */
namespace sbn{
  class NtupleReader{

  public:

    /// Default constructor
    NtupleReader(){}
    NtupleReader(readConfig r);

    /// Default destructor
    ~NtupleReader(){}

    bool init(readConfig r);


    // this function gets the nominal data
    // it also passes the breakdown of the data,
    // and the legend that accompanies it.
    void readData(std::vector<float> &               output,
                  std::vector<std::vector<float> > & nominalDataByType);
    void readDataOsc(std::vector<std::vector<float> > & outputOsc);

    // std::vector<float> GetCosmicsFromFile();
    // std::vector<float> GetDirtFromFile();

  private:

    // Worker Function, verifies that a file exists 
    bool fileExists(const  char *filename) const;
    // Worker Function returns true if arg1 (a file) is newer than arg2 (also a file)
    bool UpToDate(const char *, const char *) const;
    // Worker function populates the fileName* variables, checks they exist
    bool buildFileNames();
    
    // The following 4 worker functions are called by readData 
    // (or readDataOsc) and handle reading in from file 
    // or generating the new file.
    void Loop(std::vector<float> &               output,
              std::vector<std::vector<float> > & nominalDataByType);
    void LoopOsc(std::vector<std::vector<float> > & outputOsc);

    void Read(std::vector<float> &               output,
              std::vector<std::vector<float> > & nominalDataByType);
    void ReadOsc(std::vector<std::vector<float> > & outputOsc);    

    // This struct holds all the file specifiy information 
    readConfig config;

    // the bins things are read in as - rebinning happens upstreams
    std::vector<float> defaultBins;  

    // Root strings for the names of the files:
    TString fileNameHists;
    TString fileNameHistsOsc;
    TString fileNameSource;
    TString fileNameSourceOsc;

    // Control whether to remake the histogram files
    bool remakeHist;
    bool remakeHistOsc;



  };
}
#endif
/** @} */ // end of doxygen group 

