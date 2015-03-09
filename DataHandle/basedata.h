#ifndef BASEDATA
#define BASEDATA 

#include <vector>
#include <iostream>
#include <string> 

namespace sbn{
  enum SIGNAL   {kNue, kNumu, kPhot, kNSignal};
  enum BASEL    {kNearDet, kUboone, kT600,kNBaseline};
  enum ENERGY   {kTrue, kCaloWithNeutrals, kCaloNoNeutrals, kCCQE, kElep, kNEnergy};
  enum MODE     {kNu, kNubar,kNMode};
  enum NUEBKG   {kNueFromNueCC_muon, kNueFromNueCC_chargeKaon,
                 kNueFromNueCC_neutKaon, kNueFromEScatter, kNueFromNC_pi0,
                 kNueFromNC_delta0, kNueFromNumuCC, kDirt, kOther,kNnuebkg};
  enum NUMUBKG  {kNumuIntrinsic, kNumuFromPi, kNnumubkg};
  enum OTHBKG   {kNothbkg};

  const std::vector<std::string> signalMap {"nue", "numu", "phot", "unknown"};
  const std::vector<std::string> baselMap  {"100m", "470m", "600m"};
  const std::vector<std::string> energyMap {"etrue", "ecalo1", "ecalo2", "eccqe", "elep"};
  const std::vector<std::string> modeMap   {"nu", "nubar"};

  const std::vector<std::string> nueMap    {"NueFromNueCC_muon", "NueFromNueCC_chargeKaon",
                                            "NueFromNueCC_neutKaon", "NueFromEScatter", 
                                            "NueFromNC_pi0", "NueFromNC_delta0", 
                                            "NueFromNumuCC", "Dirt", "Other"};
  const std::vector<std::string> numuMap   {"NumuIntrinsic", "NumuFromPi"};

  const std::vector<float> nueBins {0.200,0.350,0.500,0.650,0.800,0.950,
                                    1.100,1.300,1.500,1.750,2.000,3.000};

  // const std::vector<std::string> multwMap  {"flux", "xsec"};

  // This config is needed for the ntuple readers
  struct readConfig
  {
    // Standard config for most files:
    std::string path;
    SIGNAL      signal;
    BASEL       basel;
    ENERGY      energy;
    MODE        mode;
    bool        forceRemake;
    // Multiweight configuration:
    bool        usingMultiWeights;
    int         multiWeightSource;
    bool        absolute_MWSource;
    int         nWeights;
    // Nue specific stuff:
    bool        includeCosmics;
    bool        includeDirt;
    bool        includeOsc;
    double      showerContainmentDist;
    double      minDistanceToStart;
    double      minVertexEnergySignal;
    double      minVertexEnergyPhoton;
    double      minShowerGap;
    // Oscillation specific stuff:
    double      dm2min;
    double      dm2max;
    double      sin22thmin;
    double      sin22thmax;
    int         npoints;
  };


}
#endif
