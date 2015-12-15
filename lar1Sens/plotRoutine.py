from ROOT import lar1
import ROOT

n = lar1.NueAppearanceFitter()
# n.setFileSource("/media/cadams/data_linux/two_horns/");
# n.setFileSource("/media/cadams/data_linux/two_horns_lowe/");
n.setFileSource("/data_linux/horn_upgrade/");
# n.setFileSource("/media/cadams/data_linux/testFramework/");
# n.setFileSource("/media/cadams/data_linux/100MeV/");

n.setVerbose(True);
n.setDebug(False);
n.setSpecialNameText("");
n.setSpecialNameTextOsc("");
n.setSpecialNameText_far("");
n.setSpecialNameTextOsc_far("");
n.setMode("nu");


n.setUse100m(       True);
n.setUse470m(       True);
n.setUseT600_onaxis(True);

yearsRunning = 3.0;
n.setLAr1NDPOT(yearsRunning*2.2e20);
# n.setUboonePOT(yearsRunning*2.2e20);
n.setUboonePOT(6.6e20+yearsRunning*2.2e20);
n.setIcarusPOT(yearsRunning*2.2e20);

# n.extendFileName("fullCosmics");

n.setLAr1NDScale(1.0*(1.0-0.015));
n.setUbooneScale(1.0*(1.0-0.02 ));
n.setIcarusScale(1.0*(1.0-0.03 ));
n.setLAr1NDCosmicScale(1-0.95);
n.setUbooneCosmicScale(1-0.95);
n.setIcarusCosmicScale((1-0.95));
n.extendFileName("lessCosmics");

# n.setEnergyType("elep");
# n.setEnergyType("eccqe");
n.setEnergyType("ecalo2");
n.setShapeOnlyFit(False);
n.setForceRemake(False);
n.setUseInfiniteStatistics(False);
# n.setElectContainedDist(100);

n.setTopologyCut(0.05,3.0);
# n.setMinVertexEnergySignal(0.05); 

# n.setCosmicsFile("output/no_cut/histos_for_corey.root");
# n.setMinDistanceToStart(0);

nue_bins = ROOT.vector(float)()
# nue_bins.push_back(0.100);
nue_bins.push_back(0.200);
nue_bins.push_back(0.350);
nue_bins.push_back(0.500);
nue_bins.push_back(0.650);
nue_bins.push_back(0.800);
nue_bins.push_back(0.950);
nue_bins.push_back(1.100);
nue_bins.push_back(1.300);
nue_bins.push_back(1.500);
nue_bins.push_back(1.750);
nue_bins.push_back(2.000);
nue_bins.push_back(3.000);
# double emin = 0.200;
# double emax = 3.0;
# double nbins = 120.0 - 8.0;
# double binSize = (emax - emin)/nbins;
# nue_bins.resize(nbins+1);
# for (double i = 0; i <= nbins; i++) {
#   nue_bins.at(i) = emin + i*binSize;
# }

# 
numu_bins = ROOT.vector(float)();
numu_bins.push_back(.200);
numu_bins.push_back(.300);
numu_bins.push_back(.400);
numu_bins.push_back(.450);
numu_bins.push_back(.500);
numu_bins.push_back(.550);
numu_bins.push_back(.600);
numu_bins.push_back(.650);
numu_bins.push_back(.700);
numu_bins.push_back(.750);
numu_bins.push_back(.800);
numu_bins.push_back(.850);
numu_bins.push_back(.900);
numu_bins.push_back(.950);
numu_bins.push_back(1.000);
numu_bins.push_back(1.250);
numu_bins.push_back(1.500);
numu_bins.push_back(2.000);
numu_bins.push_back(2.500);
numu_bins.push_back(3.000);
# std::cout << "Nue bins:\n";
# for (int i = 0; i < nue_bins.size(); i++) std::cout << nue_bins[i] << "\n";
# std::cout << "Numu bins:\n";
# for (int i = 0; i < numu_bins.size(); i++) std::cout << numu_bins[i] << "\n";
# std::cout << std::endl;

n.setNueBins(nue_bins);
n.setNumuBins(numu_bins);

n.setIncludeFosc(True);
n.setIncludeNumus(False);
n.setIncludeCosmics(False);
n.setIncludeDirt(False);


n.setInflateSystematics(False);
n.setSystematicInflationAmount(0.00);


# Gotta define the backgrounds:
kUnisim=0
kPiMinus = 1
kPiPlus = 2
kKaon0 = 3
kKMinus = 4
kKPlus = 5
kTotal_flux = 6
kTotal_xsec = 0
# # n.setMultiWeightSource(kUnisim);
# # n.setMultiWeightSource(kPiMinus);
# # n.setMultiWeightSource(kPiPlus);
# # n.setMultiWeightSource(kKaon0);
# # n.setMultiWeightSource(kKMinus);
# # n.setMultiWeightSource(kKPlus);  
n.setMultiWeightSource(kTotal_xsec);
n.setAbsolute_MWSource(False);

n.setUseMultiWeights(False);
n.setUseXSecWeights(False);
n.setUseFluxWeights(False);

n.setNpoints(500);
n.setNWeights(250);

n.setSavePlots(True);

covMatList = ROOT.vector(ROOT.string)();
covMatListSource = ROOT.vector(int)();
# covMatList.push_back("xsec");
# covMatListSource.push_back(kTotal_xsec);
# covMatList.push_back("flux");
# covMatListSource.push_back(kTotal_flux);

n.setIncludeDetSys(False);

n.setCovMatList(covMatList);
n.setCovMatListSource(covMatListSource);


n.setBuildCovarianceMatrix(False);
n.setMakeRatioPlots(False);
n.setLoop(True);
n.setMakeSimplePlot(True);
n.setMakeEventRatePlots(True);
n.setMakeAltSensPlot(False);

n.Run();