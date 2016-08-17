import ROOT
from ROOT import sbn
import math


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

ROOT.gStyle.SetOptStat(0000)
ROOT.gStyle.SetOptFit(0000)
ROOT.gStyle.SetPadBorderMode(0)
ROOT.gStyle.SetPadBottomMargin(0.15)
ROOT.gStyle.SetPadLeftMargin(0.15)
ROOT.gStyle.SetPadRightMargin(0.05)
ROOT.gStyle.SetFrameBorderMode(0)
ROOT.gStyle.SetCanvasBorderMode(0)
ROOT.gStyle.SetPalette(0)
ROOT.gStyle.SetCanvasColor(0)
ROOT.gStyle.SetPadColor(0)


# dhlist = [sbn.DataHandle(), sbn.DataHandle()]
dhlist = [sbn.DataHandle()]
# pathlist = ["/data_linux/nominal_ntuples/", "/data_linux/horn_upgrade_repeat/"]
pathlist = ["/data_linux/nominal_ntuples/"]


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
    # dh.config().backgroundScales[sbn.kNueFromNC_pi0] = 10.0

# Scale the dirt background in the upgrade by 2:
# dhlist[-1].config().backgroundScales[sbn.kDirt] = 2.0

detectors = [sbn.kNearDet, sbn.kUboone, sbn.kT600]
scales = [1.0-0.015, 2*(1.0 - 0.02), 1.0-0.03]

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

namesBasic = ["  nue_mu   ",
              "  nue_k+   ",
              "  nue_k0   ",
              "  nu-e     ",
              "  nc-gam   ",
              "  del-gm   ",
              "  numucc   ",
              "  Dirt     ",
              "  Cosmic   ",
              "  Other    ",
              "  Signal   "]

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
        # get the data by type?
        dataByType = dh.getDetector(det).getSampleDataByType(sbn.kNue)

        stackList.append(ROOT.THStack(
            "eventsByType",
            "Event Rates at L = {s}".format(s=sbn.baselMap[sbn.kNearDet])))

        # set up the legend
        legList.append(ROOT.TLegend(0.75, 0.5, .95, 0.9))
        legList[-1].SetTextFont(72)
        legList[-1].SetTextSize(0.04)
        legList[-1].SetFillStyle(0)
        legList[-1].SetFillColor(0)
        legList[-1].SetBorderSize(0)

        histogramsByType = []

        null = ROOT.vector(float)(sbn.nueBins.size()-1, 0)
        totalHist = utils.makeHistogram(null, sbn.nueBins)

        for i in xrange(len(dataByType)):
            if i == sbn.kNueFromEScatter:
                continue

            if i == sbn.kOther:
                continue

            if i == sbn.kNueFromNC_delta0:
                continue

            histogramsByType.append(
                utils.makeHistogram(dataByType[i], sbn.nueBins))
            totalHist.Add(histogramsByType[-1])
            histogramsByType[-1].SetFillColor(colorMap[i])
            stackList[-1].Add(histogramsByType[-1])
            legList[-1].AddEntry(histogramsByType[-1], namesFancy[i])

        signalHist = utils.makeHistogram(oscData, sbn.nueBins)
        # stackList[-1].Add(signalHist)

        # totalHist.Add(signalHist)

        for b in xrange(signalHist.GetNbinsX()):
            totalHist.SetBinError(b+1, math.sqrt(totalHist.GetBinContent(b+1)))

        canvasList.append(
            ROOT.TCanvas("sc{i}".format(i=det), "Stacked event rates",
                         int(1.5 * 700),
                         int(1.5 * 500)))

        canvasList[-1].cd()

        maxVal = 1.75 * (stackList[-1].GetMaximum())
        frame = canvasList[-1].DrawFrame(
            sbn.nueBins.front() - 0.01, 0.0, sbn.nueBins.back(), maxVal)

        frame.GetYaxis().SetTitle("Events / GeV")
        frame.GetYaxis().SetTitleSize(0.06)
        frame.GetYaxis().SetTitleOffset(0.9)
        frame.GetYaxis().CenterTitle()
        frame.GetXaxis().CenterTitle()
        frame.GetXaxis().SetLabelSize(0.05)

        EnergyLabel = energyMapFancy[dh.config().energy]
        EnergyLabel += " (GeV)"
        frame.GetXaxis().SetTitle(EnergyLabel)
        frame.GetXaxis().SetTitleOffset(0.95)
        frame.GetXaxis().SetTitleSize(0.07)
        # frame.SetMaximum(forceScales[det])
        frame.Draw()
        print maxVal

        # sig = "Signal: ( #Deltam^{{2}} = {dm} eV^{{  2}}, sin^{{2}} 2#theta_{{#mue}} = {s2})".format(
        # dm=dm2, s2=sin22th)

        name = "{d}, {l}".format(d=detectorsFancy[det], l=baselinesFancy[det])
        utils.add_plot_label(name, 0.2, 0.87, 0.05, 1, 62, 12)
        # utils.add_plot_label(sig, 0.2, 0.81, 0.04, 1, 62, 12)
        utils.add_plot_label(
            "Statistical Uncertainty Only", 0.2, 0.76, 0.04, 1, 62, 12)

        if 'upgrade' in path:
            modeLabel = "Upgrade, 6.6e20 POT"
        else:
            if det is not sbn.kUboone:
                modeLabel = "Nominal, 6.6e20 POT"
            else:
                modeLabel = "Nominal, 1.32e21 POT"
        utils.add_plot_label(modeLabel, 0.2, 0.71, 0.04, 1, 62, 12)

        stackList[-1].Draw("hist same ")

        totalHist.Draw("E0 hist same")

        legList[-1].Draw()

        dirPath = "/home/cadams/Dropbox/Thesis/sbn_figures/"

        if 'upgrade' in path:
            name = "upgrade_"
            txtname = "upgrade_"
        else:
            name = "nominal_"
            txtname = "nominal_"

        name += "nue_appearance_nosig_"

        # plotName = name + "{d}_{l}_{d2}_{s2}".format(d=detectorsFancy[det],
        plotName = name + "{d}_{l}".format(d=detectorsFancy[det],
                                           l=baselinesFancy[det],
                                           d2=dm2, s2=sin22th)
        canvasList[-1].Print(dirPath + plotName + ".pdf", "pdf")
        canvasList[-1].Print(dirPath + plotName + ".png", "png")

        # Write the data out into a text file, too:
        # txtname +=
        # "eventRates_{d}_{l}_{d2}_{s2}.txt".format(d=detectorsFancy[det],
        txtname += "eventRates_nosig_{d}_{l}.txt".format(d=detectorsFancy[det],
                                                         l=baselinesFancy[det],
                                                         d2=dm2, s2=sin22th)

        f = open(dirPath + txtname, 'w')
        # First line is the categories:
        f.write('   bins   ')
        for name in namesBasic:
            f.write(name)

        f.write('  Total   ')
        f.write('\n')

        # Now go through the event list:
        for i_bin in range(sbn.nueBins.size() - 1):

            # First line is the bin range:
            f.write('{0:0.2f}'.format(sbn.nueBins.at(i_bin)))
            f.write('-')
            f.write('{0:0.2f} '.format(sbn.nueBins.at(i_bin+1)))

            total = 0.0
            for vec in dataByType:
                f.write('{:^11}'.format(int(round(vec[i_bin])), align="^"))
                total += vec[i_bin]

            # Write the signal too:
            f.write('{:^11}'.format(int(round(oscData[i_bin]))))
            f.write('{:^11}'.format(int(round(total)), align="^"))
            f.write('\n')

        # Now do the total events:
        f.write('  Total   ')
        for vec in dataByType:
            total = 0.0
            for point in vec:
                total += point
            f.write('{:^11}'.format(int(round(total)), align="^"))
