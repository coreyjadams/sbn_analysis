
{
  gSystem->Load("libsbn_analysis_Reprocessing.so");


  TString directory = "/data_linux/nominal_ntuples/";

  // // LAr1-ND @ 100 meters
  Reprocessing ntuple_100m_nu(directory + "combined_ntuple_100m_nu.root");           
  Reprocessing ntuple_100m_nu_fosc         (directory + "combined_ntuple_100m_nu_fosc.root");      
  // ntuple_100m_nu.UseTwoHornConfig();
  ntuple_100m_nu.Loop("nue",0,0);
  // ntuple_100m_nu.Loop("rates",0,0);
  // ntuple_100m_nu.Loop("numu",0,0);
  // ntuple_100m_nu_fosc.UseTwoHornConfig();
  ntuple_100m_nu_fosc.Loop("fosc",0,0);

  // // // // // MicroBooNE @ 470m
  Reprocessing ntuple_470m_nu              (directory + "combined_ntuple_470m_nu.root");           
  Reprocessing ntuple_470m_nu_fosc         (directory + "combined_ntuple_470m_nu_fosc.root");
  // ntuple_470m_nu.UseTwoHornConfig();
  ntuple_470m_nu.Loop("nue",1,4);
  // ntuple_470m_nu.Loop("numu",1,4);
  // ntuple_470m_nu.Loop("rates",1,4);
  // ntuple_470m_nu_fosc.UseTwoHornConfig();
  ntuple_470m_nu_fosc.Loop("fosc",1,4);

  // // //T600 @ 600m on axis
  Reprocessing ntuple_600m_onaxis_nu       (directory + "combined_ntuple_600m_onaxis_nu.root");           
  Reprocessing ntuple_600m_onaxis_nu_fosc  (directory + "combined_ntuple_600m_onaxis_nu_fosc.root");      
  // ntuple_600m_onaxis_nu.UseTwoHornConfig();
  ntuple_600m_onaxis_nu.Loop("nue",2,5);
  // // ntuple_600m_onaxis_nu.Loop("numu",2,5);
  // ntuple_600m_onaxis_nu.Loop("rates",2,5);
  // ntuple_600m_onaxis_nu_fosc.UseTwoHornConfig();
  ntuple_600m_onaxis_nu_fosc.Loop("fosc",2,5);


return;
}
  //;
