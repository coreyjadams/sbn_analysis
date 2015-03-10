#ifndef PLOTUTILS_CXX
#define PLOTUTILS_CXX

#include "PlotUtils.h"


namespace sbn {

  void PlotUtils::add_plot_label(char* label, double x, double y,
                                 double size , int color,
                                 int font, int align){

    TLatex *latex = new TLatex( x, y, label );
    latex->SetNDC();
    latex->SetTextSize(size);
    latex->SetTextColor(color);
    latex->SetTextFont(font);
    latex->SetTextAlign(align);
    latex->Draw();
  }


  void PlotUtils::set_plot_style()
  {
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    gStyle->SetOptStat(0);
    gStyle->SetPadRightMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadTopMargin(0.15);
    gStyle->SetPadBottomMargin(0.15);
  }

} // sbn

#endif
