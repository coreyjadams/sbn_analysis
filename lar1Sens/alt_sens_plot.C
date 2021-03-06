
#include "TString.h"
#include "TBox.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFile.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include <vector>
#include "TROOT.h"

#include <iostream>

void alt_sens_plot(){

  gSystem->Load("liblar1Sens.so");


  // TString fileSource = "/home/cadams/nominal_ntuples/sens_plots/";
  // TString fileSource = "/media/cadams/data_linux/stats_comp/";
  TString fileSource = "/data_linux/comparison/";

  std::vector<TString> names;

  // names.push_back(fileSource+"nominal_ntuples/sens_plots/altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_dirt_cos_.root");
  // names.push_back(fileSource+"altSensPlot_ND_100m_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_dirt_cos_.root");
  // names.push_back(fileSource+"altSensPlot_ND_100m_uB_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_dirt_cos_.root");


  // names.push_back(fileSource+"altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_fullCosmics_xsec_0_flux_6_dirt_cos_.root");
  // names.push_back(fileSource+"altSensPlot_ND_100m_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_fullCosmics_xsec_0_flux_6_dirt_cos_.root");
  // names.push_back(fileSource+"altSensPlot_ND_100m_uB_nue_appearance_ecalo2_nu_vePhot0.05_gap3_fullCosmics_xsec_0_flux_6_dirt_cos_.root");

  names.push_back(fileSource+"/Nominal/altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_dirt_cos_.root");
  // names.push_back(fileSource+"NoCosmic/altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_dirt_.root");
  // names.push_back(fileSource+"NominalNoCosmicNoDirt/altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_.root");
  // names.push_back(fileSource+"PlotsTo100MeV/altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_xsec_0_flux_6_.root");
  names.push_back(fileSource+"/horn_upgrade/altSensPlot_ND_100m_uB_T600_onaxis_nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_3years_xsec_0_flux_6_dirt_cos_.root");

  // names.push_back(fileSource+"nominal_stats.root");
  // names.push_back(fileSource+"double_stats.root");
  // names.push_back(fileSource+"half_extra_stats.root");
  // names.push_back(fileSource+"half_stats.root");


  std::vector<int> colors;
  colors.push_back(kRed-3);
  colors.push_back(1);
  colors.push_back(4);
  colors.push_back(kGreen-3);

  std::vector<TString> legEntry;
  // legEntry.push_back("Full SBN Program");
  // legEntry.push_back("LAr1-ND, T600");
  // legEntry.push_back("LAr1-ND, MicroBooNE");

  // legEntry.push_back("SBN, Topological Cosmic ID Only");
  // legEntry.push_back("80% Efficiency (Nominal)");
  // legEntry.push_back("70% Efficiency");
  // legEntry.push_back("60% Efficiency");
  // legEntry.push_back("50% Efficiency");
  legEntry.push_back("Nominal SBN");
  legEntry.push_back("No Cosmics");
  legEntry.push_back("No Cosmics No Dirt");
  legEntry.push_back("100 MeV Threshold");
  // legEntry.push_back("1.5x SBN Statistics");
  // legEntry.push_back("0.5x SBN Statistics");


  std::vector <int> index;
  index.push_back(0);
  // index.push_back(1);
  // index.push_back(2);
  index.push_back(3);



  TFile * base = new TFile(names.front(), "OPEN");




  TGraph * fiveSigmaLine = (TGraph*) base -> Get("fiveSigmaLine");
  TGraph * threeSigmaLine = (TGraph*) base -> Get("threeSigmaLine");
  TGraph * ninetyCLLine = (TGraph*) base -> Get("ninetyCLLine");
  // fiveSigmaLine -> SetLineStyle(2);
  // fiveSigmaLine -> SetLineWidth(2);


  fiveSigmaLine -> SetLineColor(1);
  fiveSigmaLine -> SetLineWidth(2);
  threeSigmaLine -> SetLineColor(1);
  threeSigmaLine -> SetLineWidth(2);
  ninetyCLLine -> SetLineColor(1);
  ninetyCLLine -> SetLineWidth(2);

  fiveSigmaLine->SetMinimum(0.0);
  fiveSigmaLine->SetMaximum(20);
  gStyle->SetPadBottomMargin(0.16);

  TCanvas * canv2 = new TCanvas("c2","Sensitivity along bottom of LSND region",2*600,600);
  fiveSigmaLine -> SetTitle("");
  fiveSigmaLine -> GetXaxis() -> SetTitle("#Delta m^{2} (eV^{2})");
  fiveSigmaLine -> GetYaxis() -> SetTitle("Significance #sqrt{ #Delta #chi^{2}} ");
  fiveSigmaLine -> GetXaxis() -> SetTitleSize(.06);
  fiveSigmaLine -> GetYaxis() -> SetTitleSize(.06);
  fiveSigmaLine -> GetXaxis() -> CenterTitle();
  fiveSigmaLine -> GetYaxis() -> CenterTitle();

  fiveSigmaLine -> GetXaxis() -> SetTitleOffset(0.85);
  fiveSigmaLine -> GetYaxis() -> SetTitleOffset(0.55);

  canv2 -> SetGrid();
  canv2 -> SetLogx();

  fiveSigmaLine -> GetXaxis() -> SetLimits(0.025,109);

  fiveSigmaLine -> Draw();

  TBox * slab1 = new TBox(3.37287,0.0,5.44503,18);
  TBox * slab2 = new TBox(7.72681,0.0,9.63829,18);

  slab1->SetFillStyle(3001);
  slab1->SetFillColor(kGray);
  slab1->Draw();
  slab2->SetFillStyle(3001);
  slab2->SetFillColor(kGray);
  slab2->Draw();

  fiveSigmaLine -> Draw("same");
  threeSigmaLine -> Draw("same");
  ninetyCLLine -> Draw("same");

  double legAddition = 0.02 + 0.052*index.size();

  TLegend * legend = new TLegend(0.12,0.74 - legAddition,0.47,0.74);
  // legend -> SetFillStyle(0);
  // legend -> SetFillColor(0);
  // legend -> SetBorderSize(0);
  legend -> SetMargin(0.1);
  legend -> SetTextSize(0.045);

  for ( auto i = index.rbegin(); i != index.rend(); i++){
    std::cout << "\nIndex is  " << *i << std::endl;
    std::cout << "\nName is  " << names[*i] << std::endl;
    TFile * temp = new TFile(names[*i],"OPEN");
    TGraph * graph1 = (TGraph*) temp -> Get("graph_0");
    TGraph * graph2 = (TGraph*) temp -> Get("graph_1");
    TGraph * graph3 = (TGraph*) temp -> Get("graph_2");

    graph1 -> SetLineWidth(3);
    graph2 -> SetLineWidth(3);
    graph3 -> SetLineWidth(3);

    // if (*i != 0){
    //   graph1 -> SetLineStyle(2);
    //   graph2 -> SetLineStyle(2);
    //   graph3 -> SetLineStyle(2);
    //   graph1 -> SetLineWidth(2);
    //   graph2 -> SetLineWidth(2);
    //   graph3 -> SetLineWidth(2);
    // }


    graph1 -> SetLineColor(colors[*i]);
    graph2 -> SetLineColor(colors[*i]);
    graph3 -> SetLineColor(colors[*i]);
    graph1 -> Draw("same");
    graph2 -> Draw("same");
    graph3 -> Draw("same");


  }

  for ( auto j = index.begin(); j != index.end(); j++){
    TFile * temp = new TFile(names[*j],"OPEN");
    TGraph * graph1 = (TGraph*) temp -> Get("graph_0");
    graph1 -> SetLineWidth(3);
    graph1 -> SetLineColor(colors[*j]);
    // if(*j != 0){
    //   graph1 -> SetLineWidth(2);
    //   graph1 -> SetLineStyle(2);
    // }

    legend -> AddEntry(graph1,legEntry[*j],"l");
  }


  lar1::PlotUtils putils;
  
  TBox * slab3 = new TBox(0.03,18,10,19.9);
  slab3->SetFillColor(kWhite);
  slab3->Draw();

  putils.add_plot_label("Sensitivity to 3+1 #nu signal along the LSND 99% CL",0.12,0.86, 0.05, 1, 62, 12);
  putils.add_plot_label("5 #sigma",.905, .34 , 0.029, 1, 62, 12);
  putils.add_plot_label("3 #sigma",.905, .263, 0.029, 1, 62, 12);
  putils.add_plot_label("90% CL"  ,.905, .206, 0.029, 1, 62, 12);


  // legend -> AddEntry(fiveSigmaLine,"5 #sigma Sensitivity","l");
  // legend -> AddEntry(threeSigmaLine,"3 #sigma Sensitivity","l");
  // legend -> AddEntry(ninetyCLLine,"90% CL Sensitivity","l");
  legend -> Draw();

}