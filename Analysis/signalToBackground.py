import ROOT
from ROOT import sbn
import math

from matplotlib import pyplot as plt

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

# sin22th = sin22th_giunti
# dm2 = dm2_giunti

sin22th = sin22th_kopp
dm2 = dm2_kopp


dhlist = [sbn.DataHandle(), sbn.DataHandle()]
pathlist = ["/data_linux/nominal_ntuples/", "/data_linux/horn_upgrade_repeat/"]


utils = sbn.PlotUtils()


for dh, path in zip(dhlist, pathlist):

    dh.setEnergy(sbn.kCaloNoNeutrals)
    dh.setMode(sbn.kNu)
    dh.config().path = path
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

    # scale some of the backgrounds:
    dh.config().backgroundScales[sbn.kCosmics] = 1-0.95

# Scale the dirt background in the upgrade by 2:
dhlist[-1].config().backgroundScales[sbn.kDirt] = 2.0

detectors = [sbn.kNearDet, sbn.kUboone, sbn.kT600]
scales = [1.0-0.015, 1.0 - 0.02, 1.0-0.03]

for det in detectors:
    for dh in dhlist:
        dh.addDetector(det)

for dh in dhlist:
    dh.addSignal(sbn.kNue)

    dh.setBins(sbn.kNue, sbn.nueBins)

    _init_val = dh.init()

    if not _init_val:
        print "Error, init failed."
        exit(-1)


for det, scale in zip(detectors, scales):
    for dh in dhlist:
        dh.getDetector(det).setScale(sbn.kNue, scale)

for dh in dhlist:
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
forceScales.update({sbn.kNearDet: 43500})
forceScales.update({sbn.kUboone: 1500})
forceScales.update({sbn.kT600: 5800})

for dh, path in zip(dhlist, pathlist):
    for det in detectors:

        # Try to get something out of the DataHandle
        nominalData = dh.getDetector(det).getSampleData(sbn.kNue)
        oscData = dh.getDetector(det).getSampleDataOsc(sbn.kNue, sin22th, dm2)

        x = []
        x_errs = []
        y_bkg = []
        y_sig = []
        y_sigToNoise = []
        y_err = []
        y_stat_sigma = []

        i = 1
        for bin_sig, bin_bkg in zip(oscData, nominalData):
            if (bin_bkg != 0):
                stb = bin_sig/bin_bkg
            else:
                stb = 0.0
            y_bkg.append(bin_bkg)
            y_sig.append(bin_sig)
            y_sigToNoise.append(stb)
            y_err.append(stb*math.sqrt(1.0/bin_bkg + 1.0/bin_sig))
            y_stat_sigma.append(bin_sig / math.sqrt(bin_bkg))
            # if det is sbn.kT600:
            # print str(stb) + " +/- " + str(stb*math.sqrt(1.0/bin_bkg +
            # 1.0/bin_sig))
            i += 1

        for i_bin in range(sbn.nueBins.size()-1):
            x.append(0.5 * (sbn.nueBins.at(i_bin + 1) + sbn.nueBins.at(i_bin)))
            x_errs.append(
                0.5 * (sbn.nueBins.at(i_bin + 1) - sbn.nueBins.at(i_bin)))

        # Two subplots, the axes array is 1-d
        f, axarr = plt.subplots()
        axarr.errorbar(x, y_sigToNoise, yerr=y_err, xerr=x_errs)
        axarr.set_title(
            'Signal to Background Ratio, {d}'.format(d=detectorsFancy[det]))
        axarr.set_xlim([0.2, 3.0])
        axarr.set_ylim([0.0, 0.75])
        plt.grid(True)
        plt.xlabel(r"$E^{\nu}_{calo}$")
        plt.ylabel("Signal / Background",color='b')

        for tl in axarr.get_yticklabels():
            tl.set_color('b')

        ax2 = axarr.twinx()
        ax2.plot(x, y_stat_sigma,'g.',markersize=20)
        ax2.set_ylim([0.0, 10.0])
        plt.ylabel("$\sigma_{Stat.}$",color='g',size='xx-large')

        for tl in ax2.get_yticklabels():
            tl.set_color('g')


        f.text(0.44, 0.825-0.035, detectorsFancy[det], size='x-large', weight='bold')
        f.text(0.44, 0.775-0.035, r"($\Delta m^2$,sin${}^2 2\theta$)" +
               " =({d}, {s})".format(d=dm2, s=sin22th), size='x-large', style='italic')
        f.text(0.44, 0.725-0.035, "Statistical Uncert. Only",
               size='x-large', style='italic')

        if "upgrade" in path:
            f.text(
                0.44, 0.675-0.035, "Upgrade, {p} POT".format(p=6.6e20), size='x-large', style='italic')
        else:
            f.text(
                0.44, 0.675-0.035, "Nominal, {p} POT".format(p=6.6e20), size='x-large', style='italic')

        # plt.show()
        dirPath = "/home/cadams/Dropbox/Talks/BeamUpgradeMeeting/HornUpgradePlots3/SignalToBackground/"
        if 'upgrade' in path:
            name = "stb_upgrade_"
        else:
            name = "stb_nominal_"

        name += "nue_appearance_"

        plotName = name + "{d}_{l}_{d2}_{s2}".format(d=detectorsFancy[det],
                                                     l=baselinesFancy[det],
                                                     d2=dm2, s2=sin22th)

        # plt.show()

        plt.savefig(dirPath + plotName + ".pdf")
        plt.savefig(dirPath + plotName + ".png")
