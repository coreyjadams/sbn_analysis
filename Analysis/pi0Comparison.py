import ROOT
from ROOT import TFile, TCanvas, TLegend


def plotUpgradeRatiosCalo(fnominal, fupgrade):

    c = TCanvas("calo","calo",800,800)
    pi0_calo_nominal = fnominal.Get("SinglePhotonPi0Calo")
    pi0_calo_upgrade = fupgrade.Get("SinglePhotonPi0Calo")


    pi_calo_ratio = pi0_calo_upgrade.Clone()
    pi_calo_ratio.Divide(pi0_calo_nominal)
    pi_calo_ratio.SetLineColor(ROOT.kRed)
    pi_calo_ratio.SetTitle("Background Ratio, Reco. Energy")
    # pi_calo_ratio.SetMinimum(0.5)
    # pi_calo_ratio.SetMaximum(4.0)
    pi_calo_ratio.Draw("")

    nueFromMuon_nominal = fnominal.Get("NueFromMuonDecayCalo")
    nueFromMuon_upgrade = fupgrade.Get("NueFromMuonDecayCalo")
    nueFromK0_nominal = fnominal.Get("NueFromK0DecayCalo")
    nueFromK0_upgrade = fupgrade.Get("NueFromK0DecayCalo")
    nueFromKPlus_nominal = fnominal.Get("NueFromKPlusDecayCalo")
    nueFromKPlus_upgrade = fupgrade.Get("NueFromKPlusDecayCalo")

    nueTotal_nominal = nueFromMuon_nominal.Clone()
    nueTotal_nominal.Add(nueFromK0_nominal)
    nueTotal_nominal.Add(nueFromKPlus_nominal)

    nueTotal_upgrade = nueFromMuon_upgrade.Clone()
    nueTotal_upgrade.Add(nueFromK0_upgrade)
    nueTotal_upgrade.Add(nueFromKPlus_upgrade)

    nueTotal_upgrade.Divide(nueTotal_nominal)
    nueTotal_upgrade.SetLineColor(ROOT.kGreen)
    nueTotal_upgrade.Draw("same")


    leg = TLegend(0.2,0.75,0.4,0.9)
    leg.AddEntry(pi0_calo_upgrade, "Single Photon","l")
    leg.AddEntry(nueTotal_upgrade, "Intrinsic #nu_{e}","l")
    leg.SetFillStyle(0);
    leg.SetFillColor(0);
    leg.SetBorderSize(0);
    # leg.SetTextSize(7)
    leg.Draw()

    c.Print(outputDir+"upgradeRatioCalo.pdf","pdf")
    c.Print(outputDir+"upgradeRatioCalo.png","png")


    return




def plotUpgradeRatiosTrue(file1,file2):

    c = TCanvas()

    pi0_nominal_true = file1.Get("SinglePhotonPi0")
    pi0_upgrade_true = file2.Get("SinglePhotonPi0")


    pi0_upgrade_ratio = pi0_upgrade_true.Clone()
    pi0_upgrade_ratio.Divide(pi0_nominal_true)
    pi0_upgrade_ratio.SetLineColor(ROOT.kRed)
    pi0_upgrade_ratio.SetTitle("Background Ratio, True Energy")
    # pi0_upgrade_ratio.SetMinimum(0.5)
    # pi0_upgrade_ratio.SetMaximum(4.0)
    pi0_upgrade_ratio.Draw("")

    nueFromMuon_nominal_true = file1.Get("NueFromMuonDecay")
    nueFromMuon_upgrade_true = file2.Get("NueFromMuonDecay")
    nueFromK0_nominal_true = file1.Get("NueFromK0Decay")
    nueFromK0_upgrade_true = file2.Get("NueFromK0Decay")
    nueFromKPlus_nominal_true = file1.Get("NueFromKPlusDecay")
    nueFromKPlus_upgrade_true = file2.Get("NueFromKPlusDecay")

    nueFromMuon_nominal_true.Add(nueFromK0_nominal_true)
    nueFromMuon_nominal_true.Add(nueFromKPlus_nominal_true)
    nueFromMuon_upgrade_true.Add(nueFromK0_upgrade_true)

    nueFromMuon_upgrade_true.Add(nueFromKPlus_upgrade_true)
    nueFromMuon_upgrade_true.Divide(nueFromMuon_nominal_true)
    nueFromMuon_upgrade_true.SetLineColor(ROOT.kGreen)

    nueFromMuon_upgrade_true.Draw("same")

    leg2 = TLegend(0.2,0.75,0.4,0.9)
    leg2.AddEntry(pi0_upgrade_ratio, "Single Photon","l")
    leg2.AddEntry(nueFromMuon_upgrade_true, "Intrinsic #nu_{e}","l")
    leg2.SetFillStyle(0);
    leg2.SetFillColor(0);
    leg2.SetBorderSize(0);
    leg2.Draw()

    c.Print(outputDir+"upgradeRatioTrue.pdf","pdf")
    c.Print(outputDir+"upgradeRatioTrue.png","png")

    return 

def plotNueToPi0Ratio(fnominal,fupgrade):

    # Need the ratios of nue to pi0 before and after upgrade:

    pi0_calo_nominal = fnominal.Get("SinglePhotonPi0Calo")
    pi0_calo_upgrade = fupgrade.Get("SinglePhotonPi0Calo")


    nueFromMuon_nominal = fnominal.Get("NueFromMuonDecayCalo")
    nueFromMuon_upgrade = fupgrade.Get("NueFromMuonDecayCalo")
    nueFromK0_nominal = fnominal.Get("NueFromK0DecayCalo")
    nueFromK0_upgrade = fupgrade.Get("NueFromK0DecayCalo")
    nueFromKPlus_nominal = fnominal.Get("NueFromKPlusDecayCalo")
    nueFromKPlus_upgrade = fupgrade.Get("NueFromKPlusDecayCalo")

    nueTotal_nominal = nueFromMuon_nominal.Clone()
    nueTotal_nominal.Add(nueFromK0_nominal)
    nueTotal_nominal.Add(nueFromKPlus_nominal)

    nueTotal_upgrade = nueFromMuon_upgrade.Clone()
    nueTotal_upgrade.Add(nueFromK0_upgrade)
    nueTotal_upgrade.Add(nueFromKPlus_upgrade)

    nueToPi0Ratio_nominal = pi0_calo_nominal.Clone()
    nueToPi0Ratio_nominal.Divide(nueTotal_nominal)
    nueToPi0Ratio_nominal.SetLineColor(ROOT.kBlue)


    nueToPi0Ratio_upgrade = pi0_calo_upgrade.Clone()
    nueToPi0Ratio_upgrade.Divide(nueTotal_upgrade)
    nueToPi0Ratio_upgrade.SetLineColor(ROOT.kOrange)

    c = TCanvas("canv3","canv3",1600,800)
    c.SetTitle("Ratio of intrinsic to Single Photon")
    nueToPi0Ratio_upgrade.SetTitle("Ratio of intrinsic to Single Photon")
    nueToPi0Ratio_upgrade.Draw("")
    nueToPi0Ratio_nominal.Draw("same")

    leg3 = TLegend(0.5,0.75,0.8,0.9)
    leg3.AddEntry(nueToPi0Ratio_nominal, "#pi^{0} to #nu_{e}, Nominal","l")
    leg3.AddEntry(nueToPi0Ratio_upgrade, "#pi^{0} to #nu_{e}, Upgrade","l")
    leg3.SetFillStyle(0);
    leg3.SetFillColor(0);
    leg3.SetBorderSize(0);
    leg3.Draw()

    c.Print(outputDir+"intrinsicToPhotonRatio.pdf","pdf")
    c.Print(outputDir+"intrinsicToPhotonRatio.png","png")



    return c

if __name__ == '__main__':
  
    outputDir="/home/cadams/Dropbox/Talks/BeamUpgradeMeeting/HornUpgradePlots3/"

    file1="/data_linux/nominal_ntuples/combined_ntuple_470m_nu_processed_nue.root"
    file2="/data_linux/horn_upgrade_repeat/combined_ntuple_470m_nu_processed_nue.root"

    f1 = TFile(file1)
    f2 = TFile(file2)

    ROOT.gStyle.SetOptStat(0)

    plotUpgradeRatiosCalo(f1,f2)
    plotUpgradeRatiosTrue(f1,f2)
    plotNueToPi0Ratio(f1,f2)


    # nd = input("Waiting for enter ...")