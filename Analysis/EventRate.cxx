#ifndef EVENTRATE_CXX
#define EVENTRATE_CXX

#include "EventRate.h"

namespace sbn {

  void EventRate::mainLoop(){
    // Configure the data handler:
    dh.setEnergy(kCaloNoNeutrals);
    dh.setMode(kNu);


    dh.config().path                  = "/media/cadams/data_linux/testFramework/";
    // config.npoints               = 500;
    // config.forceRemake           = false;
    // config.includeOsc            = true;
    // config.multiWeightSource     = 0;
    // config.usingMultiWeights     = false;
    // config.absolute_MWSource     = false;
    // config.nWeights              = 250;
    // // config.includeCosmics        = r.includeCosmics;
    // // config.includeDirt           = r.includeDirt;
    // config.showerContainmentDist = -999;
    // config.minDistanceToStart    = -999;
    // config.minVertexEnergySignal = 0;
    // config.minVertexEnergyPhoton = 0.05;
    // config.minShowerGap          = 3.0;
    // this -> setBins(nueBins);
    // return;

    // Standard config for most files:
    dh.config().forceRemake = false;
    // Multiweight configuration:
    dh.config().usingMultiWeights=false;
    dh.config().multiWeightSource=0;
    dh.config().absolute_MWSource=false;
    dh.config().nWeights=250;
    // Nue specific stuff:
    dh.config().includeCosmics = false;
    dh.config().includeDirt = false;
    dh.config().includeOsc = false;
    dh.config().showerContainmentDist = -999;
    dh.config().minDistanceToStart = -999;
    dh.config().minVertexEnergySignal = 0.0;
    dh.config().minVertexEnergyPhoton = 0.05;
    dh.config().minShowerGap = 3.0;
    // // Oscillation specific stuff:
    dh.config().dm2min = 0.01;
    dh.config().dm2max = 100;
    dh.config().sin22thmin = 0.0001;
    dh.config().sin22thmax = 1.0;
    dh.config().npoints = 500;

    dh.addDetector(kUboone);
    // dh.addDetector(kNearDet);
    dh.addSignal(kNue);

    dh.setBins(kNue, nueBins);

    if (! dh.init() ){
      std::cerr << "Init failed to complete.  Exiting.\n\n";
      exit(-1);
    }
    dh.read();


    // Try to get something out of the DataHandle
    std::vector<float> nominalData = dh.getData();
    float totalEvents = 0;
    for (unsigned int i = 0; i < nominalData.size(); i ++){
      std::cout << "nominalData[" << i << "]:\t" << nominalData[i] << std::endl;
      totalEvents += nominalData[i];
    }
    std::cout << "total number of events is: " << totalEvents << std::endl;


  }
} // sbn

#endif

/*
Total CC Event Rate in LAr1-ND : 1428860
Total NC Event Rate in LAr1-ND : 23622.3
20364 Events in Bin 1
81874 Events in Bin 2
59326.3 Events in Bin 3
67403.8 Events in Bin 4
75618.6 Events in Bin 5
80370.6 Events in Bin 6
84337.5 Events in Bin 7
82147.1 Events in Bin 8
81001.8 Events in Bin 9
76792.4 Events in Bin 10
72175.6 Events in Bin 11
66893.1 Events in Bin 12
61440.1 Events in Bin 13
55264.6 Events in Bin 14
203553 Events in Bin 15
110012 Events in Bin 16
93517.9 Events in Bin 17
35935.4 Events in Bin 18

For an exposure of 6.6e20
Total CC Event Rate in T600 : 240888
Total NC Event Rate in T600 : 4604.44
2500.21 Events in Bin 1
10004.2 Events in Bin 2
7302.05 Events in Bin 3
8287.44 Events in Bin 4
9450.72 Events in Bin 5
10295.7 Events in Bin 6
10718.7 Events in Bin 7
11499.1 Events in Bin 8
11602.3 Events in Bin 9
11701.9 Events in Bin 10
11752.1 Events in Bin 11
11241.5 Events in Bin 12
10886.7 Events in Bin 13
10628.2 Events in Bin 14
43613.4 Events in Bin 15
27804.5 Events in Bin 16
22209.8 Events in Bin 17
6163.54 Events in Bin 18


For an exposure of 1.32e21
Total CC Event Rate in uBooNE : 61744
Total NC Event Rate in uBooNE : 1185.93
668.956 Events in Bin 1
2572.15 Events in Bin 2
1864.93 Events in Bin 3
2130.84 Events in Bin 4
2361.49 Events in Bin 5
2619.67 Events in Bin 6
2769.66 Events in Bin 7
2904.46 Events in Bin 8
2972.99 Events in Bin 9
2963 Events in Bin 10
2924.7 Events in Bin 11
2847.8 Events in Bin 12
2773.54 Events in Bin 13
2629.63 Events in Bin 14
11171.7 Events in Bin 15
7197.74 Events in Bin 16
5854.2 Events in Bin 17
1652.11 Events in Bin 18
*/