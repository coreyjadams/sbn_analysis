import ROOT
from ROOT import TFile, TLatex, TString, TCanvas, TH1F, TLegend, lar1
import math

from matplotlib import pyplot as plt

def add_plot_label(label, x, y, size=0.05, color=1, font=62, align=22):
    latex = TLatex(x, y, label)
    latex.SetNDC()
    latex.SetTextSize(size)
    latex.SetTextColor(color)
    latex.SetTextFont(font)
    latex.SetTextAlign(align)
    latex.Draw()
    return


def plotFracUncerts(matrix,matrixFileName,nueRates):

    title = ""
    # Determine what uncert this is:
    if "xsec" in matrixFileName:
        title = r'$\nu_{e}$ Cross Section Fractional Uncertaintes'
        # background.back() .SetTitle("#nu_{#mu} Cross Section Fractional
        # Uncertaintes");

    else:
        if "flux" in matrixFileName:
            title = r'$\nu_{e}$ Flux Fractional Uncertaintes'
            # background.back() .SetTitle("#nu_{#mu} Flux Fractional
            # Uncertaintes");

        else:
            title = r'$\nu_{e}$ Fractional Uncertaintes'
            # background.back() .SetTitle("#nu_{#mu} Fractional Uncertaintes");


    # Get the x values:
    xVals = []
    xLow  = []
    xHigh = []
    for i in range(nueRates.front().GetNbinsX()):
        xLow.append(nueRates.front().GetXaxis().GetBinLowEdge(i+1))
        xVals.append(nueRates.front().GetXaxis().GetBinCenter(i+1))
        xHigh.append(nueRates.front().GetXaxis().GetBinUpEdge(i+1))
    

    yErr = []

    for l,m,h in zip(xLow, xVals,xHigh):
        print l,m,h
        yErr.append(h-l)
    dataSets = []


    curr_bin = 1;

    legends = []
    for iDet in range(nueRates.size()):

        dataSets.append([])

        # Set up the plot to look nice...
        for iBin in range(nueRates.at(iDet).GetNbinsX()):
            val = 100 * math.sqrt(matrix.GetBinContent(curr_bin, curr_bin))
            dataSets[-1].append(val)
            # print "Adding error ", val, " at bin ", iBin
            curr_bin += 1

        thisName = nueRates.at(iDet).GetName()
        if "100m" in thisName:
            legends.append("SBND")
        if "470m" in thisName:
            legends.append("MicroBooNE")
        if "600m" in thisName:
            legends.append("T600")

        #     print "Adding plot for 100m"
        #     # temphist.SetLineWidth("#nu_{e} Rates at SBND");
        #     tempHist.SetLineWidth(1)
        #     tempHist.SetLineStyle(2)
        #     # tempHist.SetMarkerStyle(8);
        #     # tempHist.SetMarkerColor(1);
        #     tempHist.Draw("same")
        #     leg.AddEntry(tempHist, "SBND")

        # if "470m" in thisName:
        #     tempHist.SetLineWidth(0)
        #     tempHist.SetMarkerStyle(3)
        #     tempHist.SetMarkerColor(2)
        #     # rates.at(iDet).SetTitle("#nu_{e} Rates at MicroBooNE");
        #     tempHist.Draw("P same")
        #     leg.AddEntry(tempHist, "uBooNE")

        # if "600m" in thisName:
        #     tempHist.SetLineWidth(0)
        #     tempHist.SetMarkerStyle(5)
        #     tempHist.SetMarkerColor(4)
        #     # rates.at(iDet).SetTitle("#nu_{e} Rates at T600");
        #     tempHist.Draw("P same")
        #     leg.AddEntry(tempHist, "T600")

    print xVals

    styles = ['bs','rv','go']

    fig,ax = plt.subplots()

    for dset,style,lbl in zip(dataSets,styles,legends):
        ax.plot(xVals,dset,style,label=lbl,markersize=10)
        ax.grid(True,which='major')
        ax.legend(numpoints=1)


    ax.axis([0.2,3,0,20])
    plt.xlabel("Reconstructed Neutrino Energy [GeV]",fontsize=20)
    plt.ylabel("Uncert. [%]",fontsize=20)
    plt.title(title,fontsize=20,y=1.02)

    plt.tick_params(axis='both', which='major', labelsize=14)
    plt.tick_params(axis='both', which='minor', labelsize=14)

    for item in [fig, ax]:
        item.patch.set_visible(False)


    plt.show()

    name = matrixFileName.rstrip('.root') + '_fracUncert.png'

    with open(name, 'w') as outfile:
        fig.canvas.print_png(outfile)



def plot_matrix(matrixName="corrMatHist", matrixFileName=None, useNumu=False):

        # file = TFile("/data_linux/nominal_ntuples/matrices/matrixFile_nue_ND_100m_uB_T600_onaxis_xsec_0_ecalo2_nu_vePhot0.05_gap3.root")

    plotUtils = lar1.PlotUtils()

    if matrixFileName is None:
        matrixFileName = "/data_linux/nominal_ntuples/matrices/matrixFile_nue_ND_100m_uB_T600_onaxis_xsec_0_ecalo2_nu_vePhot0.05_gap3.root"

    f = TFile(matrixFileName)

    use100m = "ND" in matrixFileName
    use470m = "uB" in matrixFileName
    use600m = "T600" in matrixFileName

    nueEventRates = ROOT.vector(TH1F)()

    if use100m:
        nueEventRates.push_back(f.Get("nueEventRates_100m"))
    if use470m:
        nueEventRates.push_back(f.Get("nueEventRates_470m"))
    if use600m:
        nueEventRates.push_back(f.Get("nueEventRates_600m_onaxis"))

    nBins_nue = nueEventRates.front().GetNbinsX()

    histNumu = ROOT.vector(TH1F)()

    if useNumu:
        if use100m:
            histNumu.append(f.Get("numuEventRates_100m"))
        if use470m:
            histNumu.append(f.Get("numuEventRates_470m"))
        if use600m:
            histNumu.append(f.Get("numuEventRates_600m_onaxis"))

        nBins_numu = histNumu[0].GetNbinsX()

    else:
        nBins_numu = 0

    # Get the matrix:
    matrix = f.Get(matrixName)

    print matrix

    canv = plotUtils.plot_Matrix(f.Get(matrixName), matrixName,
                                 use100m, use470m, use600m,
                                 nBins_nue, nBins_numu)

    # nd = input("Waiting for enter ...")

    # # for the name of the pdf file to print:
    matrixFileName.rstrip('.root')  # chop off ".root"
    outName = matrixFileName
    outName += "_" + matrixName + ".pdf"
    canv.Print(outName, "pdf")

    if matrixName is "fracMatHist":
        plotFracUncerts(matrix, matrixFileName, nueEventRates)

    #     Uncerts.front().Print(matrixFileName+"_nue_uncerts.pdf","pdf");
    #   # if (numuEventRates.size() != 0)
    #       # Uncerts.back().Print(matrixFileName+"_numu_uncerts.pdf","pdf");

    # # TCanvas * canv = plot_Matrix(matrix, matrixName,
    # #                              use100m, use470m, use600m,
    # #                              nBins_nue, nBins_numu)


if __name__ == '__main__':

    path = "/data_linux/nominal_ntuples/matrices/"

    name = "matrixFile_nue_ND_100m_uB_T600_onaxis_xsec_0_ecalo2_nu_vePhot0.05_gap3.root"
    plot_matrix("corrMatHist", path+name)
    plot_matrix("fracMatHist", path+name)
    name = "matrixFile_nue_ND_100m_uB_T600_onaxis_flux_6_ecalo2_nu_vePhot0.05_gap3.root"
    plot_matrix("corrMatHist", path+name)
    plot_matrix("fracMatHist", path+name)
