#include "TH1F.h"
#include "TCanvas.h"

void effPlotter(){
  lar1::EffCalc a;

  lar1::PlotUtils utils;

  // a.setPath("/media/cadams/data_linux/nominal_text/");
  // a.setDetector("470m");
  // a.getNominalData();

  TString path = "/media/cadams/data_linux/";
  TString file2 = "nominal_ntuples/text/";
  TString file1 = "nominal_tables/";

  TString fileName="nue_appearance_ecalo2_nu_vePhot0.05_gap3_lessCosmics_600m_onaxis_nu_globBF.txt";

  utils.set_plot_style();

  std::vector<int> types;
  types.push_back(2); //nue from mu
  types.push_back(3); //nue from k+
  types.push_back(4); //nue from k0
  types.push_back(5); //nuscat
  types.push_back(6); //pi0
  types.push_back(7); //gamma
  types.push_back(9); //numuCC
  types.push_back(9); //dirt
  types.push_back(10);//cosmic
  types.push_back(11);//other 
  types.push_back(12);  //signal

  // TString title = "Ratio of Simulated Signal, Two Horns / One Horn";
  TString title = "Ratio of #nu_{e} from K decay, Two Horns / One Horn";
  // TString title = "Ratio of #nu_{e} intrinsics, Two Horns / One Horn";

  // TCanvas * events;
  TCanvas * c1 = new TCanvas("stupidNue","stupidNue",800,800);
  // c1->Divide(2);

  TH1F * eff = a.makeNueRatio(path+file2+fileName, path+file1+fileName, types, title, 2);
  eff -> SetLineWidth(3);
  eff -> Draw();

//   double minVertexEnergySignal = 0.015;
//   int background = 6;
//   while (minVertexEnergySignal < 0.505){
//     c1 -> cd();
//     // TH1F * temp = a.makeEfficiency(background,3,minVertexEnergySignal,minVertexEnergySignal);
//     TH1F * tempNue  = a.makeNueEfficiency(3,minVertexEnergySignal,minVertexEnergySignal);
//     // TH1F * tempPhot = a.makePhotonEfficiency(3,minVertexEnergySignal,minVertexEnergySignal);
//     tempNue -> Draw();
//     utils.add_plot_label(Form("VE: %g MeV",1000*minVertexEnergySignal),.6,.2,0.04,1,62,12);
//     // utils.add_plot_label(Form("VE: %g MeV",1000*minVertexEnergySignal),.6,.2,0.04,1,62,12);
//     c1 -> Print(Form("eff_%g.png",minVertexEnergySignal));
//     events = a.createBackground(3,minVertexEnergySignal,minVertexEnergySignal);

//     events -> Print(Form("events_norm_%g.png",minVertexEnergySignal));

//     // events = a.createBackground(3,minVertexEnergySignal,minVertexEnergySignal,false);
//     // events -> Print(Form("events_unnorm_%g.png",minVertexEnergySignal));
//     minVertexEnergySignal += 0.005;
//   }
}