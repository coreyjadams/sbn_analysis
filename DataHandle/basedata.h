#ifndef BASEDATA
#define BASEDATA 

namespace sbn{
  enum SIGNAL {kNue, kNumu, kPhot, kNSignal};
  enum BASEL  {kNearDet, kUboone, kT600};
  enum ENERGY {kTrue, kCaloWithNeutrals, kCaloNoNeutrals, kCCQE, kElep};
  enum MODE   {kNu, kNubar};

  const std::vector<std::string> signalMap {"nue", "numu", "phot", "unknown"};
  const std::vector<std::string> baselMap  {"100", "470", "600"};
  const std::vector<std::string> energyMap {"etrue", "ecalo1", "ecalo2", "eccqe", "elep"};
  const std::vector<std::string> modeMap   {"nu", "nubar"};

  // This config is needed for the ntuple readers
  struct readConfig
  {
  	std::string signal;
    std::string path;
    std::string baseline;
    std::string mode;
    std::string energy;
    int	        npoints;
    bool        forceRemake;
    bool        usingMultiWeights;
    bool        absolute_MWSource;
    int         nWeights;
    bool        includeCosmics;
    bool        includeDirt;
    double      showerContainmentDist;
    double      minDistanceToStart;
    double      minVertexEnergySignal;
    double      minVertexEnergyPhoton;
    double      minShowerGap;
  };

  
}
#endif
