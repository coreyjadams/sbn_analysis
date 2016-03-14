#ifndef EVENTRATE_CXX
#define EVENTRATE_CXX

#include "EventRate.h"


namespace sbn {

void EventRate::mainLoop() {
    // Configure the data handler:
    dh.setEnergy(kCaloNoNeutrals);
    dh.setMode(kNu);


    dh.config().path                  = "/data_linux/horn_upgrade/";
    // Standard config for most files:
    dh.config().forceRemake = false;
    // Multiweight configuration:
    dh.config().usingMultiWeights = false;
    dh.config().multiWeightSource = 0;
    dh.config().absolute_MWSource = false;
    dh.config().nWeights = 250;
    // Nue specific stuff:
    dh.config().includeCosmics = false;
    dh.config().includeDirt = false;
    dh.config().includeOsc = true;
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

    // dh.addDetector(kUboone);
    dh.addDetector(kNearDet);
    dh.addSignal(kNue);

    dh.setBins(kNue, nueBins);

    if (! dh.init() ) {
        std::cerr << "Init failed to complete.  Exiting.\n\n";
        exit(-1);
    }
    dh.read();


    // Try to get something out of the DataHandle
    std::vector<float> nominalData = dh.getData();
    std::vector<float> oscData;
    dh.getOscDataByValue(oscData, 0.013, 0.43, (int)kNue);

    // get the data by type?
    std::vector<std::vector<float> > dataByType = dh.getDetector(kNearDet).getSampleDataByType(kNue);



    std::cout << "OscData size is " << oscData.size() << std::endl;
    float totalEvents = 0;
    for (unsigned int i = 0; i < nominalData.size(); i ++) {
        std::cout << "nominalData[" << i << "]:\t" << nominalData[i]
                  << "\toscData[" << i << "]:\t" << oscData[i]
                  << std::endl;
        totalEvents += nominalData[i];
    }
    std::cout << "total number of events is: " << totalEvents << std::endl;


    // Make the histograms from the data-by-type
    std::vector<int > nueColorMap   {kGreen + 3, kGreen + 2, kGreen - 6, kOrange - 8,
                                     kOrange - 8, kBlack, kBlue - 9, 15, kPink, 8};

    std::vector<std::string> energyMapFancy
    {   "True Energy", "Reco. Energy", "Reco. Energy",
        "CCQE Energy", "Lepton Candidate Energy"};



    THStack * stack = new THStack("eventsByType",
                                  Form("Event Rates at L = %s",
                                       baselMap[kNearDet].c_str()) );
    std::vector<TH1F*> histogramsByType;
    histogramsByType.resize(dataByType.size());
    for (unsigned int i = 0; i < dataByType.size(); i ++ ) {
        histogramsByType[i] = utils.makeHistogram(dataByType[i], nueBins);
        histogramsByType[i] -> SetFillColor(nueColorMap[i]);
        stack -> Add(histogramsByType[i]);
    }


    TH1F * signalHist = utils.makeHistogram(oscData, nueBins);
    stack -> Add(signalHist);

    TCanvas * c = new TCanvas("sc0", "Stacked event rates", 1.5 * 700, 1.5 * 500);

    double max = 1.75 * (stack -> GetMaximum());
    TH1F *chr = c->DrawFrame(nueBins.front() - 0.01, 0.0, nueBins.back(), max);

    chr->GetYaxis()->SetTitle("Events / GeV");
    chr->GetYaxis()->SetTitleSize(0.06);
    chr->GetYaxis()->SetTitleOffset(0.9);
    chr->GetYaxis()->CenterTitle();
    chr->GetXaxis()->CenterTitle();
    chr->GetXaxis()->SetLabelSize(0.05);

    TString EnergyLabel = energyMapFancy[dh.config().energy];
    EnergyLabel += " (GeV)";
    chr->GetXaxis()->SetTitle(EnergyLabel);
    chr->GetXaxis()->SetTitleOffset(0.95);
    chr->GetXaxis()->SetTitleSize(0.07);
    // chr->GetXaxis()->SetLimits(emin-0.01,emax);
    chr->Draw();

    stack -> Draw("hist same ");

    // leg->Draw();



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