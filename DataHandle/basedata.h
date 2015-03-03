#ifndef BASEDATA
#define BASEDATA 

namespace sbn{
  enum SIGNAL   {kNue, kNumu, kPhot, kNSignal};
  enum BASEL    {kNearDet, kUboone, kT600};
  enum ENERGY   {kTrue, kCaloWithNeutrals, kCaloNoNeutrals, kCCQE, kElep};
  enum MODE     {kNu, kNubar};
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

  // const std::vector<std::string> multwMap  {"flux", "xsec"};

  // This config is needed for the ntuple readers
  struct readConfig
  {
    std::string path;
    SIGNAL      signal;
    BASEL       basel;
    ENERGY      energy;
    MODE        mode;
    int	        npoints;
    bool        forceRemake;
    bool        usingMultiWeights;
    int         multiWeightSource;
    bool        absolute_MWSource;
    int         nWeights;
    bool        includeCosmics;
    bool        includeDirt;
    bool        includeOsc;
    double      showerContainmentDist;
    double      minDistanceToStart;
    double      minVertexEnergySignal;
    double      minVertexEnergyPhoton;
    double      minShowerGap;
  };


}
#endif
