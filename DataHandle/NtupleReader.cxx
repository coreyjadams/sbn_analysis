#ifndef NTUPLEREADER_CXX
#define NTUPLEREADER_CXX

#include "NtupleReader.h"
#include "TMath.h"
#include "TRandom.h"

namespace sbn {

NtupleReader::NtupleReader(readConfig r) {
  this->config = r;
  return;
}

bool NtupleReader::UpToDate(const char * sourceFile, const char * testFile) const {
  if (utils.fileExists(sourceFile)) {
    if (utils.fileExists(testFile)) {
      struct stat attrib;
      stat(testFile, &attrib);
      int timediff = attrib.st_mtime;
      stat(sourceFile, &attrib);
      timediff -= attrib.st_mtime;
      if (timediff < 0) {
        // std::cout << "\tFile " << testFile << " is up to date." << std::endl;
        return true;
      }
      else {
        // std::cout << "\tFile" << testFile << " is not up to date." << std::endl;
        return false;
      }
    }
    else {return false;}
  }
  else {return false;}
}

bool NtupleReader::buildFileNames() {
  //Need to make the names of the files, nue and numu:
  fileNameSource = "combined_ntuple_";
  fileNameSource += baselMap[config.basel];
  fileNameSource += "_";     //"combined_ntuple_470m_"
  fileNameSource += modeMap[config.mode]; //"combined_ntuple_470m_nu"
  fileNameSourceOsc = fileNameSource;
  if (config.signal == kNue)
    fileNameSourceOsc += "_fosc";
  fileNameSource += "_processed_";
  fileNameSource += signalMap[config.signal];
  fileNameSource += ".root";
  fileNameSourceOsc += "_processed_";
  if (config.signal == kNue ) fileNameSourceOsc += "fosc";
  else if (config.signal == kNumu) fileNameSourceOsc += "numu";
  fileNameSourceOsc += ".root";

  // std::cout << "fileNameSource is " << fileNameSource << std::endl;
  // std::cout << "fileNameSourceOsc is " << fileNameSourceOsc << std::endl;

  TString absPath = config.path;
  absPath += fileNameSource;
  // Verify these source files exist:
  if ( !utils.fileExists(absPath.Data() ) ) {
    std::cout << absPath << " does not exist.\n" << std::endl;
    return false;
  }
  if (config.includeOsc) {
    absPath = config.path;
    absPath += fileNameSourceOsc;
    if (! utils.fileExists(absPath.Data() ) ) {
      std::cout << absPath << " does not exist.\n" << std::endl;
      return false;
    }
  }
  // If it reached here, the source files exist (both if needed)

  // From here, build the hist file names and set the variable to
  // recreate them or not.

  char tempstring[100];
  //path to the file that holds histos:
  fileNameHists = "hists/";
  fileNameHists += fileNameSource;
  fileNameHistsOsc = "hists_osc/";
  fileNameHistsOsc += fileNameSourceOsc;
  fileNameHists += "_";
  if (config.signal == kNue) {
    if (config.showerContainmentDist != -999) {
      sprintf(tempstring, "cont%g", config.showerContainmentDist);
      fileNameHists += tempstring;
      fileNameHists += "_";
      fileNameHistsOsc += "_";
      fileNameHistsOsc += tempstring;
    }
    if (config.minDistanceToStart != -999) {
      sprintf(tempstring, "dist%g", config.minDistanceToStart);
      fileNameHists += tempstring;
      fileNameHists += "_";
    }
    if (config.minVertexEnergyPhoton != 10000) {
      sprintf(tempstring, "vePhot%g", config.minVertexEnergyPhoton);
      fileNameHists += tempstring;
      fileNameHists += "_";
    }
    if (config.minShowerGap != 10000) {
      sprintf(tempstring, "sg%g", config.minShowerGap);
      fileNameHists += tempstring;
      fileNameHists += "_";
    }
    if (config.minVertexEnergySignal != 0) {
      sprintf(tempstring, "veSig%g", config.minVertexEnergySignal);
      fileNameHists += tempstring;
      fileNameHists += "_";
      fileNameHistsOsc += "_";
      fileNameHistsOsc += tempstring;
    }
  }

  if (config.usingMultiWeights) {
    sprintf(tempstring, "%dw_%d", config.nWeights,
            config.multiWeightSource);
    fileNameHists += tempstring;
    if (config.absolute_MWSource)
      fileNameHists += "_abs_";
  }
  fileNameHists += energyMap[config.energy];
  fileNameHists += "_hists.root";

  sprintf(tempstring, "_%dp_", config.npoints);
  fileNameHistsOsc += tempstring;

  fileNameHistsOsc += energyMap[config.energy];;
  fileNameHistsOsc += "_hists.root";

  // std::cout << "fileNameHists is " << fileNameHists << std::endl;
  // std::cout << "fileNameHistsOsc is " << fileNameHistsOsc << std::endl;


  // Check if these source files exist:
  absPath = config.path;
  if ( utils.fileExists(absPath + fileNameHists) ) {
    if (  UpToDate( (absPath + fileNameSource).Data(),
                    (absPath + fileNameHists ).Data() )
          || config.forceRemake )
    {
      // std::cout << fileNameHists << " not up to date.\n" << std::endl;
      remakeHist = true;
    }
    else {
      remakeHist = false;
    }
  }
  else {
    std::cout << "Message: File " << absPath + fileNameHists << " does not exist, remaking.\n";
    remakeHist = true;
  }

  if (config.includeOsc) {
    if ( utils.fileExists(absPath + fileNameHistsOsc) ) {
      absPath = config.path;
      if ( UpToDate( (absPath + fileNameSourceOsc).Data(),
                     (absPath + fileNameHistsOsc ).Data() )
           || config.forceRemake )
      {
        // std::cout << fileNameHistsOsc << " not up to date.\n" << std::endl;
        remakeHistOsc = true;
      }
      else {
        remakeHistOsc = false;
      }
    }
    else {
      std::cout << "Message: File " << absPath + fileNameHists << " does not exist, remaking.\n";
      remakeHistOsc = true;
    }
  }

  // // For debugging, printing out stuff:
  // std::cout << std::endl;
  // std::cout << "fileNameSource " << fileNameSource << std::endl;
  // std::cout << "fileNameSourceOsc " << fileNameSourceOsc << std::endl;
  // std::cout << "fileNameHists " << fileNameHists << std::endl;
  // std::cout << "fileNameHistsOsc " << fileNameHistsOsc << std::endl;

  // std::cout << "\n\n\nUp to date information: \n";
  // std::cout << fileNameHists << " is ";
  // if ( UpToDate((absPath + fileNameHists).Data(),
  //               (absPath + fileNameSource).Data() )){
  //   std::cout << "up to date compared to " << fileNameSource << "\n";
  // }
  // else{
  //   std::cout << "NOT up to date compared to " << fileNameSource << "\n";
  // }

  return true;
}

bool NtupleReader::init(readConfig r) {
  this -> config = r;
  double emin = 0.0;
  double emax = 3.0;
  double nbins = 120.0;
  double binSize = (emax - emin) / nbins;
  defaultBins.resize(nbins + 1);
  for (double i = 0; i <= nbins; i++) {
    defaultBins.at(i) = emin + i * binSize;
  }


  bool result = buildFileNames();
  if (! result ) return result;


  return result;
}

void NtupleReader::getData(std::vector<float> &               output,
                           std::vector<std::vector<float> > & nominalDataByType)
{

  // Resize the returning vectors and get them ready:

  output.resize(defaultBins.size() - 1);
  switch (config.signal)
  {
  case kNue:
    nominalDataByType.resize(kNnuebkg);
    break;
  case kNumu:
    nominalDataByType.resize(kNnumubkg);
    break;
  default:
    nominalDataByType.resize(kNothbkg);
  }
  for (auto & vec : nominalDataByType) vec.resize(defaultBins.size() - 1);


  if (remakeHist) LoopData(output, nominalDataByType);
  else  ReadData(output, nominalDataByType);



  if (config.signal == kNue) {
    // If this is the nue sample, have to add the dirt and cosmics:
    std::vector<float> cosmics = GetCosmicsFromFile();
    std::vector<float> dirt    = GetDirtFromFile();

    if (config.includeCosmics){
      nominalDataByType[kCosmics] = cosmics;
      for (size_t i = 0; i < output.size(); i++)
        output[i] += cosmics[i];
    }

    if (config.includeCosmics){
      nominalDataByType[kDirt] = dirt;
      for (size_t i = 0; i < output.size(); i++)
        output[i] += dirt[i];
    }

  }

  return;

}

void NtupleReader::getDataOsc(std::vector<std::vector<float> > & outputOsc)
{
  //resize and prepare the vector:
  outputOsc.resize(config.npoints + 1);
  for (auto & vec : outputOsc) vec.resize(defaultBins.size() - 1);
  if (remakeHistOsc) LoopDataOsc(outputOsc);
  else  ReadDataOsc(outputOsc);
  return;
}

void NtupleReader::LoopData(std::vector<float> &               output,
                            std::vector<std::vector<float> > & nominalDataByType) {

  // This is the loop function, which is the heart of the class.
  TString dummytarget = config.path + fileNameSource;
  TChain *c = new TChain("EventsTot");
  TChain *d = new TChain("EventsTot");
  double fill_energy, true_energy, Elep, wgt;
  double checking_wgt;
  int inno, isCC, mode, ibkg;
  double ShowerContainedDist;
  double ShowerDistToStart;
  double showerGap;
  double vertexEnergy;
  int nuchan;


  int nbytes = 0, nb = 0;
  int ievt, ientry;

  c->Reset();
  c->Add(dummytarget);
  if (c == 0) exit(5);

  d->Reset();
  d->Add(dummytarget);
  if (d == 0) exit(5);


  const int nentries1 = int(c->GetEntries());

  // std::cout<<"Input filename:\t" << dummytarget << std::endl;
  std::cout << "Number of entries at L= " << baselMap[config.basel] << ":\t" << nentries1 << std::endl;
  c->SetBranchAddress("enugen", &true_energy);
  c->SetBranchAddress("inno", &inno);
  c->SetBranchAddress("nuchan", &nuchan);         // NUANCE interaction codes
  c->SetBranchAddress("mode", &mode);
  c->SetBranchAddress("isCC", &isCC);
  c->SetBranchAddress("wgt", &wgt);
  c->SetBranchAddress("ibkg", &ibkg);
  c->SetBranchAddress("ShowerContainedDistance", &ShowerContainedDist);
  c->SetBranchAddress("ShowerDistanceToStart"  , &ShowerDistToStart);
  c->SetBranchAddress("VertexEnergy"  , &vertexEnergy);
  c->SetBranchAddress("showerGap"  , &showerGap);
  c->SetBranchAddress("LepE", &Elep);
  switch (config.energy) {
  case kElep:
    c->SetBranchAddress("LepE", &fill_energy);
    break;
  case kTrue:
    c->SetBranchAddress("enugen", &fill_energy);
    break;
  case kCCQE:
    c->SetBranchAddress("Eccqe", &fill_energy);
    break;
  case kCaloWithNeutrals:
    c->SetBranchAddress("Ecalo1", &fill_energy);
    break;
  case kCaloNoNeutrals:
    c->SetBranchAddress("Ecalo2", &fill_energy);
    break;
  case kNEnergy:
    break;
  default:
    break;
  }

///MW
  // std::vector<std::vector<float> > *MultiWeight;
  // MultiWeight = 0;
  // if (config.usingMultiWeights)
  //   c->SetBranchAddress("MultiWeight",&MultiWeight);

  d->SetBranchAddress("wgt", &checking_wgt);

  // At this point, set up the histograms that root will need:
  TH1F * nominalHisto;                // the nominal background plot
  TH1F * MCStatsHisto;                // the mc stats plot, for computing simulation error
  std::vector<TH1F*> histosByType;    // vec of histos by bkg
///MW
  // TH1F ** nominalHistoMultiWeight;
  // if (config.usingMultiWeights)
  //   nominalHistoMultiWeight = new TH1F*[config.nWeights];

  // For this analysis, since the number of backgrounds by type is arbitrary,
  // work around it by using a pointer to vector and point it, just once,
  // to the right list.  Then can call on this everywhere.

  const std::vector<std::string> * ptrToTypeMap;
  switch (config.signal) {
  case kNue:
    ptrToTypeMap = &(nueMap);
    break;
  case kNumu:
    ptrToTypeMap = &(numuMap);
    break;
  default:
    ptrToTypeMap = new std::vector<std::string>(0);
  }
  histosByType.resize(ptrToTypeMap->size());

  // Initialize all the necessary histograms
  // Using sprintf to create all the names based off of the config object
  // The hope is this is easily reproducable at read in, and prevent root screw ups
  // static int rootNameCounter=0;
  char nameHolder[100];
  int nbins = defaultBins.size() - 1;

  sprintf(nameHolder, "nominalHisto_%s_%s",  signalMap[config.signal].c_str(),
          baselMap[config.basel].c_str());
  nominalHisto = new TH1F(nameHolder, nameHolder, nbins, &(defaultBins[0]));

  sprintf(nameHolder, "MCStatsHisto_%s_%s",  signalMap[config.signal].c_str(),
          baselMap[config.basel].c_str());
  MCStatsHisto = new TH1F(nameHolder, nameHolder, nbins, &(defaultBins[0]));

  // intialize the histograms for the by-type histos
  for (unsigned int i = 0; i < histosByType.size(); i ++) {
    sprintf(nameHolder, "histoByType_%s_%s_%s",  signalMap[config.signal].c_str(),
            baselMap[config.basel].c_str(),
            (*ptrToTypeMap).at(i).c_str());
    histosByType[i] = new TH1F(nameHolder, nameHolder, nbins, &(defaultBins[0]));
  }


  // Transform histograms into vectors to save to file and report back
///MW
  // std::vector<std::vector<float > > edistrnueMultiWeightVec;
  // if (config.usingMultiWeights)
  //   edistrnueMultiWeightVec.resize(config.nWeights);


  // Now the histograms are set up and ready to go.
  // Loop over the ntuple:
  std::cout << "Creating nominal histograms for " << signalMap[config.signal]
            << " at L = " << baselMap[config.basel] << " in "
            << modeMap[config.mode] << " mode with energy "
            << energyMap[config.energy] << ": " << std::endl;
  for (ievt = 0; ievt < nentries1; ievt++)
  {
    ientry = d->LoadTree(ievt); if (ientry < 0) break; nb = d->GetEntry(ievt);
    if (checking_wgt == 0) {
      continue;
    }

    ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
    // if (ievt == 0) {
    //   std::cout << "This is the first event, multiweight information:\n";
    //   std::cout << "  mutliweight.size(): " << MultiWeight -> size() << std::endl;
    //   std::cout << "  mutliweight[0].size(): " << (*MultiWeight)[0].size()<< std::endl;
    // }
    // if ( (ievt%100000) == 0) {
    //   std::cout << "On event " << ievt << " of " << nentries1 << std::endl;
    // }
    loadBar(ievt, nentries1, 20000, 30);


    // here, do a check on the signal to see which route to take:
    if (config.signal == kNue) {
      if ( config.energy == kCCQE && nuchan != 1 &&
           (ibkg == kNueFromNueCC_muon       ||
            ibkg == kNueFromNueCC_chargeKaon ||
            ibkg == kNueFromNueCC_neutKaon)  )
      {
        if (inno == 12 || inno == -12)
          continue;
      }

      if (Elep < 0.200) continue;

      if (ibkg == kNueFromNueCC_muon       ||
          ibkg == kNueFromNueCC_chargeKaon ||
          ibkg == kNueFromNueCC_neutKaon   ||
          ibkg == kNueFromEScatter         ||
          ibkg == kNueFromNC_pi0           ||
          ibkg == kNueFromNC_delta0
         ) {
        if (ShowerContainedDist < config.showerContainmentDist) {
          continue;
        }
      }
      if (ShowerDistToStart < config.minDistanceToStart) {
        continue;
      }
      if (vertexEnergy < config.minVertexEnergySignal) {
        continue;
      }
      if ( showerGap    > config.minShowerGap &&
           vertexEnergy > config.minVertexEnergyPhoton) {
        continue;
      }


      // if (!absolute_MWSource) edistrnue->Fill(fill_energy,weight);
      if (inno == 12 || inno == -12) MCStatsHisto->Fill(fill_energy);

      nominalHisto->Fill(fill_energy, wgt);
      histosByType[ibkg] -> Fill(fill_energy, wgt);

///MW
      // if (config.usingMultiWeights){
      //   for (int i = 0; i < nWeights; ++i)
      //   {
      //     double mult_wgt = (*MultiWeight)[multiWeightSource][i];
      //     if (absolute_MWSource){
      //       if (mult_wgt != 1.0){
      //         edistrnueMultiWeight[i]->Fill(fill_energy,wgt*mult_wgt);
      //         if (i == 1)
      //           edistrnue->Fill(fill_energy,weight);
      //       }
      //     }
      //     else{
      //       if (i == 0) edistrnue->Fill(fill_energy,weight);
      //       edistrnueMultiWeight[i]->Fill(fill_energy,wgt*mult_wgt);
      //     }
      //   }
      // }

    }
    else if (config.signal == kNumu) {
      nominalHisto->Fill(fill_energy, wgt);
      histosByType[ibkg] -> Fill(fill_energy, wgt);
      if (inno == 14 || inno == -14) MCStatsHisto->Fill(fill_energy);
    }

  } //end of loop
  std::cout << "DONE." << std::endl;

  // Get the cosmic and dirt stuff:
  //
  // Get the cosmics data:

  // Get the dirt data:
  //



  // Now move the data from the histograms into the objects that are returned
  //fill the vectors with the results from filling the histograms;

  for (unsigned int i = 0; i < histosByType.size(); i ++) {
    for (int bin = 0; bin < nbins;  bin++) {
      nominalDataByType[i][bin] = histosByType[i] -> GetBinContent(bin + 1);
    }
  }
  for (int bin = 0; bin < nbins;  bin++) {
    output[bin] = nominalHisto -> GetBinContent(bin + 1);
  }

///MW
  // if (usingMultiWeights){
  //   for (int i = 0; i < nWeights; ++i)
  //   {
  //     for (int bin = 0; bin < nbins; ++bin)
  //     {
  //       edistrnueMultiWeightVec[i][bin] = edistrnueMultiWeight[i]->GetBinContent(bin+1);
  //       multiWeightData[i][bin] = edistrnueMultiWeight[i]->GetBinContent(bin+1);
  //     }
  //   }
  // }


  //Write these histograms to a file!

  TFile * f = TFile::Open(config.path + fileNameHists, "RECREATE");
  if (!f) {
    std::cout << "Could not open file to write." << std::endl;
    exit(-1);
  }

  TTree *t;
  if (config.signal == kNue)
    t = new TTree("tvecnue", "Tree with vectors");
  else if (config.signal == kNumu)
    t = new TTree("tvec", "Tree with vectors");

  if (config.signal == kNue)
    t-> Branch("edistrnueVec", &output);
  if (config.signal == kNumu)
    t-> Branch("nominalOutputVec", &output);

  for (unsigned int i = 0; i < histosByType.size(); i ++) {
    char tempName[100];
    sprintf(tempName, "%sVec", (*ptrToTypeMap).at(i).c_str());
    t -> Branch(tempName, &nominalDataByType[i]);
  }

  // Save the config object to the file too!
  t -> Branch("configuration", &config);

///MW
  // if (usingMultiWeights){
  //   for (int i = 0; i < nWeights; i ++){
  //     char  name[100];
  //     sprintf(name, "edistrnueMultiWeightVec_%i",i);
  //     t -> Branch(name, &(edistrnueMultiWeightVec[i]));
  //   }
  // }


  t->Fill();

///MW
  // if (usingMultiWeights){
  //   for (int i = 0; i < nWeights; ++i)
  //   {
  //     edistrnueMultiWeight[i]->Write();
  //   }
  // }
  nominalHisto -> Write();
  MCStatsHisto -> Write();
  for (unsigned int i = 0; i < histosByType.size(); i ++) {
    histosByType[i] -> Write();
  }

  if (config.signal == kNue)
    t -> Write("tvecnue");
  else if (config.signal == kNumu)
    t -> Write("tvec");

  delete t;
  f -> Close();


}
void NtupleReader::LoopDataOsc(std::vector<std::vector<float> > & outputOsc) {

// This is the loop function, which is the heart of the class.
  TString dummytarget = config.path + fileNameSourceOsc;
  std::cout << "dummytarget is " << dummytarget << std::endl;
  TChain *c = new TChain("EventsTot");
  TChain *d = new TChain("EventsTot");
  double fill_energy, true_energy, Elep, wgt, nuleng;
  double checking_wgt;
  int inno, isCC, mode, ibkg;
  double ShowerContainedDist;
  double ShowerDistToStart;
  double showerGap;
  double vertexEnergy;
  int nuchan;


  int nbytes = 0, nb = 0;
  int ievt, ientry;

  c->Reset();
  c->Add(dummytarget);
  if (c == 0) exit(5);

  d->Reset();
  d->Add(dummytarget);
  if (d == 0) exit(5);


  const int nentries1 = int(c->GetEntries());

  // std::cout<<"Input filename:\t" << dummytarget << std::endl;
  std::cout << "Number of entries at L= " << baselMap[config.basel] << ":\t" << nentries1 << std::endl;
  c->SetBranchAddress("enugen", &true_energy);
  c->SetBranchAddress("inno", &inno);
  c->SetBranchAddress("nuchan", &nuchan);         // NUANCE interaction codes
  c->SetBranchAddress("mode", &mode);
  c->SetBranchAddress("isCC", &isCC);
  c->SetBranchAddress("wgt", &wgt);
  c->SetBranchAddress("ibkg", &ibkg);
  c->SetBranchAddress("ShowerContainedDistance", &ShowerContainedDist);
  c->SetBranchAddress("ShowerDistanceToStart"  , &ShowerDistToStart);
  c->SetBranchAddress("VertexEnergy"  , &vertexEnergy);
  c->SetBranchAddress("showerGap"  , &showerGap);
  c->SetBranchAddress("LepE", &Elep);
  c->SetBranchAddress("nuleng", &nuleng);
  switch (config.energy) {
  case kElep:
    c->SetBranchAddress("LepE", &fill_energy);
    break;
  case kTrue:
    c->SetBranchAddress("enugen", &fill_energy);
    break;
  case kCCQE:
    c->SetBranchAddress("Eccqe", &fill_energy);
    break;
  case kCaloWithNeutrals:
    c->SetBranchAddress("Ecalo1", &fill_energy);
    break;
  case kCaloNoNeutrals:
    c->SetBranchAddress("Ecalo2", &fill_energy);
    break;
  case kNEnergy:
    break;
  default:
    break;
  }

  d->SetBranchAddress("wgt", &checking_wgt);

  const Int_t nentries2 = Int_t(c->GetEntries());

  // The histogram used for binning:
  std::vector<TH1F *> eventsOsc;
  eventsOsc.resize(config.npoints + 1);
  std::vector<float> dm2Array;
  dm2Array.resize(config.npoints + 1);
  //Fill the array of dm2 points:

  char nameHolder[100];
  int nbins = defaultBins.size() - 1;

  for (int i = 0; i <= config.npoints; i++) //loop over dm2 points
  {
    dm2Array[i] = pow(10., ( TMath::Log10(config.dm2min)
                             + (i * 1. / config.npoints)
                             * TMath::Log10(config.dm2max / config.dm2min)) );
    sprintf(nameHolder, "oscllatedEvents%s_%s_%d",
            signalMap[config.signal].c_str(),
            baselMap[config.basel].c_str(),
            i);
    //While looping, set up the histograms:
    eventsOsc[i]
      = new TH1F(nameHolder, "Oscillated event distribution no dm2,",
                 nbins, &(defaultBins[0]));
  }

  std::cout << "Creating oscillated histograms for " << signalMap[config.signal]
            << " at L = " << baselMap[config.basel] << " in "
            << modeMap[config.mode] << " mode with energy "
            << energyMap[config.energy] << ": " << std::endl;
  for (ievt = 0; ievt < nentries2; ievt++)
  {
    // if ( (ievt%100000) == 0) {
    //   std::cout << "On event " << ievt << " of " << nentries2 << std::endl;
    // }
    loadBar(ievt, nentries2, 20000, 30);


    ientry = d->LoadTree(ievt); if (ientry < 0) break; nb = d->GetEntry(ievt);
    if (checking_wgt == 0) {
      // std::cout << "Skipping this event because the weight is zero." << std::endl;
      continue;
    }

    ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;




    if (config.signal == kNue) {
      if (config.energy == kCCQE && nuchan != 1) {
        if (inno == 12 || inno == -12)
          continue;
      }

      if (Elep < 0.200) continue;

      if (ShowerContainedDist < config.showerContainmentDist) {
        // std::cout << "Skipping this fosc event in the containment cut.\n";
        continue;
      }
      if (ShowerDistToStart < config.minDistanceToStart) {
        continue;
      }
      if (vertexEnergy < config.minVertexEnergySignal) {
        continue;
      }
    }
    // if (config.signal == kNumu){

    // }

    //Need to loop over dm2 points...
    for (int i = 0; i <= config.npoints; i++) {
      double weight = wgt * pow((TMath::Sin(1.27 * dm2Array[i] * (nuleng / 100.) / (true_energy * 1000.))), 2);
      eventsOsc[i]->Fill(fill_energy, weight);
    }

  }//end loop over events

  // Convert to vector, store in tree:
  // std::vector<std::vector<float> > edistrOscVec(config.npoints+1, std::vector<float>(nbins,0) );

  for (int point = 0; point <= config.npoints; point ++) {
    for (int i = 0; i < nbins; i++) {
      outputOsc[point][i] = eventsOsc[point]  -> GetBinContent(i + 1);
    }
  }

//write to file...
  TString fileLoc = config.path + fileNameHistsOsc;
  TFile f(fileLoc, "RECREATE");

  TTree *t1 = new TTree("tvecosc", "Tree with oscillation signals");
  for (int point = 0; point <= config.npoints; point ++) {
    char  name[100];
    if (config.signal == kNue){
      sprintf(name, "edistrnueoscVec_%i", point);
    }
    else {
      sprintf(name, "edistrOscVec_%i", point);
    }
    t1 -> Branch(name, &(outputOsc[point]));
    eventsOsc[point] -> Write();
  }

  t1 -> Fill();
  t1 -> Write();

  // std::cout << "Successfully wrote histograms to file." << std::endl;

  f.Close();

  return;
}

void NtupleReader::ReadData(std::vector<float> &               output,
                            std::vector<std::vector<float> > & nominalDataByType) {

  TChain *c;
  if (config.signal == kNue) {
    c = new TChain("tvecnue");
  }
  else if (config.signal == kNumu) {
    c = new TChain("tvec");
  }
  c -> Add(config.path + fileNameHists);
  std::vector<float> * readInVector = 0;
  std::vector<std::vector<float>*> readInByType;
  readInByType.resize(nominalDataByType.size());
///MW
  // std::vector<std::vector<float> * > multiWeightReadInVector;
  // if (usingMultiWeights) multiWeightReadInVector.resize(nWeights);
  if (config.signal == kNue)
    c->SetBranchAddress("edistrnueVec", &readInVector);
  if (config.signal == kNumu)
    c->SetBranchAddress("nominalOutputVec", &readInVector);
  // std::cout << readInVector -> size() << std::endl;
///MW
  // if (usingMultiWeights){
  //   for (int N_weight = 0; N_weight < nWeights; ++N_weight)
  //   {
  //     multiWeightReadInVector[N_weight] = 0;
  //     char  name[100];
  //     sprintf(name, "edistrnueMultiWeightVec_%i",N_weight);
  //     c->SetBranchAddress(name,&(multiWeightReadInVector[N_weight]));
  //   }
  // }
  // std::cout << "Reading in nue data, this may take a minute or two...." << std::endl;
  const std::vector<std::string> * ptrToTypeMap;
  switch (config.signal) {
  case kNue:
    ptrToTypeMap = &(nueMap);
    break;
  case kNumu:
    ptrToTypeMap = &(numuMap);
    break;
  default:
    ptrToTypeMap = new std::vector<std::string>(0);
  }
  for (unsigned int i = 0; i < ptrToTypeMap->size(); i ++) {
    if ( (*ptrToTypeMap).at(i) == "Dirt")
      continue;
    if ( (*ptrToTypeMap).at(i) == "Cosmics")
      continue;
    readInByType[i] = 0;
    char tempName[100];
    sprintf(tempName, "%sVec", (*ptrToTypeMap).at(i).c_str());
    c -> SetBranchAddress(tempName, &readInByType[i]);
  }

  c-> GetEntry(0);
  for (unsigned int i = 0; i < ptrToTypeMap->size(); i ++) {
    if ( (*ptrToTypeMap).at(i) == "Dirt")
      continue;
    if ( (*ptrToTypeMap).at(i) == "Cosmics")
      continue;
    nominalDataByType[i] = (*readInByType[i]);
    delete readInByType[i];
  }
  // output = nominalDataByType;
  output = *(readInVector);

///MW
  // if (usingMultiWeights){
  //   for (int N_weight = 0; N_weight < nWeights; ++N_weight)
  //   {
  //     multiWeightData[N_weight] = *(multiWeightReadInVector[N_weight]);
  //     delete multiWeightReadInVector[N_weight];
  //   }
  // }
  delete readInVector;
  delete c;
  return;
}
void NtupleReader::ReadDataOsc(std::vector<std::vector<float> > & outputOsc) {

  TChain *c = new TChain("tvecosc");
  c -> Add(config.path + fileNameHistsOsc);
  std::vector<std::vector<float> * > readInOscVec;
  readInOscVec.resize(outputOsc.size());
  for (unsigned int i = 0; i < outputOsc.size(); i ++) {
    readInOscVec[i] = 0;
    char tempName[100];
    if (config.signal == kNue)
      sprintf(tempName, "edistrnueoscVec_%i", i);
    else
      sprintf(tempName, "edistrOscVec_%i", i);

    c -> SetBranchAddress(tempName, &readInOscVec[i]);
  }
  c -> GetEntry(0);
  for (unsigned int i = 0; i < outputOsc.size(); i ++) {
    outputOsc[i] = *readInOscVec[i];
    delete readInOscVec[i];
  }

  return;
}

// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
void NtupleReader::loadBar(int x, int n, int r, int w)
{
  // Only update r times.
  if ( x % (n / r + 1) != 0 ) return;

  // Calculuate the ratio of complete-to-incomplete.
  float ratio = x / (float)n;
  int   c     = ratio * w;

  // Show the percentage complete.
  printf("%3d%% [", (int)(ratio * 100) );

  // Show the load bar.
  for (int x = 0; x < c; x++)
    printf("=");

  for (int x = c; x < w; x++)
    printf(" ");

  // ANSI Control codes to go back to the
  // previous line and clear it.
  printf("]\n\033[F\033[J");
}


std::vector<float> NtupleReader::GetDirtFromFile() {

  std::vector<float> result;
  result.resize(defaultBins.size() - 1);

  if (config.signal != kNue)
    return result;

  // Just hardcoding the names because they suck:
  TString fileName;
  TString histName;
  double scale = 1.0;
  if (config.basel == kNearDet) {
    fileName = config.path + "LAr1ND_dirt.root";
    histName = "LAr1ND_dirt";
    // scale = 1.118;
  }
  if (config.basel == kUboone) {
    fileName = config.path + "MicroBooNE_dirt.root";
    histName = "MicroBooNE_dirt";
  }
  if (config.basel == kT600) {
    fileName = config.path + "ICARUS_dirt.root";
    histName = "ICARUS_dirt";
  }

  // Verfiy the file exists:
  if (!utils.fileExists(fileName)) {
    std::cerr << "\n\nThe dirt file (" << fileName
              << ") requested does not exist, returning an"
              << " empty vector instead.\n\n";
    return result;
  }

  // otherwise, get the histogram from the file and read it into the vector:

  TFile * file = TFile::Open((TString)fileName, "READ");
  TH1F * histogram = (TH1F*)file -> Get(histName.Data());
  TH1F * smeared_hist = (TH1F*) histogram -> Clone();
  smeared_hist -> Reset();

  double boost = 1000;
  double wgt = 1.0 / boost;
  double center = 0;
  double entries = 0;

  TRandom smear(0);

  double wgt_prime = 0;

  for (int bin = 1; bin <= histogram->GetNbinsX(); bin ++) {

    center =  histogram->GetBinCenter(bin);
    entries = boost * (histogram->GetBinContent(bin));
    if (entries != 0) {
      wgt_prime = wgt * (boost * (histogram->GetBinContent(bin))) / (entries);
    }
    else { wgt_prime = 0; }

    for (int i = 0; i < entries; i++) {

      smeared_hist->Fill(smear.Gaus(center, center * 0.15 / sqrt(center)), wgt);

    }

  }

  // It's nasty but here is the spot to apply efficiency:
  for (unsigned int i = 0; i < defaultBins.size() - 1; i ++) {
    result[i] = scale * smeared_hist -> GetBinContent(i + 1);
  }

  delete file;

  return result;

}

std::vector<float> NtupleReader::GetCosmicsFromFile() {

  std::vector<float> result;
  result.resize(defaultBins.size() - 1);

  if (config.signal != kNue)
    return result;

  // Just hardcoding the names because they suck:
  TString fileName;
  TString histName;
  double scale = 1.0;
  if (config.basel == kNearDet) {
    fileName = config.path + "LAr1ND_cosmics.root";
    histName = "LAr1ND_cosmics";
    // scale = 1.118;
  }
  if (config.basel == kUboone) {
    fileName = config.path + "MicroBooNE_cosmics.root";
    histName = "MicroBooNE_cosmics";
    // scale = 1.22;
  }
  if (config.basel == kT600) {
    fileName = config.path + "ICARUS_cosmics.root";
    histName = "ICARUS_cosmics";
    // scale = 1.16;
  }



  // Verfiy the file exists:
  if (!utils.fileExists(fileName)) {
    std::cerr << "\n\nThe cosmics file (" << fileName
              << ") requested does not exist, returning an"
              << " empty vector instead.\n\n";
    return result;
  }

  // otherwise, get the histogram from the file and read it into the vector:

  TFile * file = TFile::Open((TString)fileName, "READ");
  TH1F * histogram = (TH1F*)file -> Get(histName.Data());
  TH1F * smeared_hist = (TH1F*) histogram -> Clone();
  smeared_hist -> Reset();

  double boost = 1000;
  // double wgt = 1.0 / boost;
  double center = 0;
  double entries = 0;

  TRandom smear(0);

  double wgt_prime = 0;

  for (int bin = 1; bin <= histogram->GetNbinsX(); bin ++) {

    center =  histogram->GetBinCenter(bin);
    entries = boost * (histogram->GetBinContent(bin));
    if (entries != 0) {
      wgt_prime = (histogram->GetBinContent(bin)) / (entries);
    }
    else { wgt_prime = 0; }

    for (int i = 0; i < entries; i++) {

      smeared_hist->Fill(smear.Gaus(center, center * 0.15 / sqrt(center)), wgt_prime);

    }

  }

  // It's nasty but here is the spot to apply efficiency:
  for (unsigned int i = 0; i < defaultBins.size() - 1; i ++) {
    result[i] = scale * smeared_hist -> GetBinContent(i + 1);
  }
  delete file;

  return result;

}




} // sbn




#endif
