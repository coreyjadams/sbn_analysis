import ROOT
from ROOT import sbn
import math

from matplotlib import pyplot as plt



dh = sbn.DataHandle()

utils = sbn.PlotUtils()

# bool includeSignal;

dh.setEnergy(sbn.kCaloNoNeutrals)
dh.setMode(sbn.kNu)
dh.config().path = "/data_linux/nominal_ntuples/"
# dh.config().path = "/data_linux/horn_upgrade_repeat/"
# Standard config for most files:
dh.config().forceRemake = False

# Multiweight configuration:
dh.config().usingMultiWeights = False
dh.config().multiWeightSource = 0
dh.config().absolute_MWSource = False
dh.config().nWeights = 250

# Nue specific stuff:
dh.config().includeCosmics = True
dh.config().includeDirt = True
dh.config().includeOsc = True
dh.config().showerContainmentDist = -999
dh.config().minDistanceToStart = -999
dh.config().minVertexEnergySignal = 0.0
dh.config().minVertexEnergyPhoton = 0.05
dh.config().minShowerGap = 3.0

# Oscillation specific stuff:
dh.config().dm2min = 0.01
dh.config().dm2max = 100
dh.config().sin22thmin = 0.0001
dh.config().sin22thmax = 1.0
dh.config().npoints = 500

# Giunti Best Fit Points:    
dm2_giunti = 1.8
sin22th_giunti = 0.002 

# Kopp Best Fit Points:
dm2_kopp = 0.43
sin22th_kopp = 0.013

# LSND Best Fit Points:
# dm2_lsnd = 1.2
# sin22th_lsnd = 0.003
dm2_lsnd = 2.0
sin22th_lsnd = 0.001

sin22th = sin22th_giunti
dm2 = dm2_giunti

# sin22th = sin22th_kopp
# dm2 = dm2_kopp

# scale some of the backgrounds:
dh.config().backgroundScales[sbn.kCosmics] = 1-0.95
# dh.config().backgroundScales[sbn.kNueFromNC_pi0] = 10.

detectors = [sbn.kNearDet, sbn.kUboone, sbn.kT600]
scales = [1.0-0.015, 1.0 - 0.02, 1.0-0.03]

for det, scale in zip(detectors, scales):
    dh.addDetector(det)


dh.addSignal(sbn.kNue)

dh.setBins(sbn.kNue, sbn.nueBins)

_init_val = dh.init()

if not _init_val:
    print "Error, init failed."
    exit(-1)


for det, scale in zip(detectors, scales):
    dh.getDetector(det).setScale(sbn.kNue, scale)


dh.read()

colorMap = dict()

colorMap.update({sbn.kNueFromNueCC_muon: ROOT.kGreen + 3})
colorMap.update({sbn.kNueFromNueCC_chargeKaon: ROOT.kGreen + 2})
colorMap.update({sbn.kNueFromNueCC_neutKaon: ROOT.kGreen - 6})
colorMap.update({sbn.kNueFromEScatter: ROOT.kOrange - 8})
colorMap.update({sbn.kNueFromNC_pi0: ROOT.kOrange - 8})
colorMap.update({sbn.kNueFromNC_delta0: ROOT.kBlack})
colorMap.update({sbn.kNueFromNumuCC: ROOT.kBlue - 9})
colorMap.update({sbn.kDirt: 15})
colorMap.update({sbn.kCosmics:  ROOT.kPink})
colorMap.update({sbn.kOther: 8})


energyMapFancy = ["True Energy", "Reco. Energy", "Reco. Energy",
                  "CCQE Energy", "Lepton Candidate Energy"]

namesFancy = ["#mu #rightarrow #nu_{e}",
              "K^{+} #rightarrow #nu_{e}",
              "K^{0} #rightarrow #nu_{e}",
              "#nu - e^{-}",
              "NC Single #gamma",
              "#Delta #rightarrow N#gamma",
              "#nu_{#mu} CC",
              "Dirt",
              "Cosmics",
              "Other",
              "Signal"]

stackList = []
canvasList = []
legList = []

detectorsFancy = dict()
detectorsFancy.update({sbn.kNearDet: "SBND"})
detectorsFancy.update({sbn.kUboone: "MicroBooNE"})
detectorsFancy.update({sbn.kT600: "ICARUS-T600"})

baselinesFancy = dict()
baselinesFancy.update({sbn.kNearDet: "110m"})
baselinesFancy.update({sbn.kUboone: "470m"})
baselinesFancy.update({sbn.kT600: "600m"})

forceScales = dict()
forceScales.update({sbn.kNearDet: 43500 })
forceScales.update({sbn.kUboone: 1500 })
forceScales.update({sbn.kT600: 5800 })


for det in detectors:

    # Try to get something out of the DataHandle
    nominalData = dh.getDetector(det).getSampleData(sbn.kNue)
    oscData = dh.getDetector(det).getSampleDataOsc(sbn.kNue, sin22th, dm2)

    print oscData.size()
    print nominalData.size()

    x = []
    y_bkg = []
    y_sig = []
    y_sigToNoise = []


    i = 1
    for bin_sig, bin_bkg in zip(oscData,nominalData):
        if (bin_bkg != 0):
          stb = bin_sig/bin_bkg
        else:
          stb = 0.0
        print "Signal is ", bin_sig
        print "Background is ", bin_bkg
        print "Signal to background is ", stb
        y_bkg.append(bin_bkg)
        y_sig.append(bin_sig)
        y_sigToNoise.append(stb)
        x.append(i)
        i += 1

    # Two subplots, the axes array is 1-d
    f, axarr = plt.subplots()
    axarr.plot(x, y_bkg)
    # axarr.set_title('Sharing X axis')
    # axarr.scatter(x, y)
    plt.show()