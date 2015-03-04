#ifndef NTUPLEREADER_CXX
#define NTUPLEREADER_CXX

#include "NtupleReader.h"

namespace sbn {

  NtupleReader::NtupleReader(readConfig r){
    this->config = r;
    return;
  }

  bool NtupleReader::fileExists(const char *filename) const{
    std::ifstream ifile(filename);
    return ifile.is_open();
  }

  bool NtupleReader::UpToDate(const char * sourceFile, const char * testFile) const{
    if (fileExists(sourceFile)){
      if (fileExists(testFile)){
        struct stat attrib;
        stat(testFile, &attrib);
        int timediff = attrib.st_mtime;
        stat(sourceFile, &attrib);
        timediff -= attrib.st_mtime;
        if (timediff > 0) {
          std::cout << "\tFile " << testFile << " is up to date." << std::endl;
          return true;
        }
        else {
          std::cout << "\tFile" << testFile << " is not up to date." << std::endl;
          return false;
        }
      }
      else {return false;}
    }
    else {return false;}
  }

  bool NtupleReader::buildFileNames(){
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
    if ( !fileExists(absPath.Data() ) ){
      std::cout << absPath << " does not exist.\n" << std::endl;
      return false;
    }
    if (config.includeOsc){
      absPath = config.path;
      absPath += fileNameSourceOsc;
      if (! fileExists(absPath.Data() ) ){
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
    if (config.signal == kNue){
      if(config.showerContainmentDist != -999){
        sprintf(tempstring, "cont%g",config.showerContainmentDist);
        fileNameHists += tempstring;
        fileNameHists += "_";
        fileNameHistsOsc += "_";
        fileNameHistsOsc += tempstring;
      }
      if(config.minDistanceToStart != -999){
        sprintf(tempstring, "dist%g",config.minDistanceToStart);
        fileNameHists += tempstring;
        fileNameHists += "_";
      }
      if (config.minVertexEnergyPhoton != 10000){
        sprintf(tempstring, "vePhot%g",config.minVertexEnergyPhoton);
        fileNameHists += tempstring;
        fileNameHists += "_";
      }
      if (config.minShowerGap != 10000){
        sprintf(tempstring, "sg%g",config.minShowerGap);
        fileNameHists += tempstring;
        fileNameHists += "_";
      }
      if (config.minVertexEnergySignal != 0){
        sprintf(tempstring, "veSig%g",config.minVertexEnergySignal);
        fileNameHists += tempstring;
        fileNameHists += "_";
        fileNameHistsOsc += "_";
        fileNameHistsOsc += tempstring;
      }
    }

    if (config.usingMultiWeights){
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
    absPath += fileNameHists;
    if ( ! UpToDate( (absPath + fileNameSource).Data(),
                     (absPath + fileNameHists ).Data() ) 
        || config.forceRemake )
    {
      std::cout << absPath << " not up to date.\n" << std::endl;
      remakeHist = true;
    }
    else{
      remakeHist = false;
    }
    if (config.includeOsc){
      absPath = config.path;
      absPath += fileNameHistsOsc;
      if ( ! UpToDate( (absPath + fileNameSourceOsc).Data(),
                       (absPath + fileNameHistsOsc ).Data() ) 
          || config.forceRemake )
      {
        std::cout << absPath << " not up to date.\n" << std::endl;
        remakeHistOsc = true;
      }
      else{
        remakeHistOsc = false;
      }
    }

    return true;
  }

  bool NtupleReader::init(readConfig r){
    this -> config = r;
    double emin = 0.0;
    double emax = 3.0;
    double nbins = 120.0;
    double binSize = (emax - emin)/nbins;
    defaultBins.resize(nbins+1);
    for (double i = 0; i <= nbins; i++) {
      defaultBins.at(i) = emin + i*binSize;
    }
    

    bool result = buildFileNames();
    if (! result ) return result;

    return result;
  }

  void NtupleReader::readData(std::vector<float> &               output,
                              std::vector<std::vector<float> > & nominalDataByType)
  {

    // Resize the returning vectors and get them ready:

    output.resize(defaultBins.size()-1);
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
    for (auto & vec : nominalDataByType) vec.resize(defaultBins.size()-1);


    if (remakeHist) Loop(output, nominalDataByType);
    else  Read(output, nominalDataByType);

    return;

  }
  void NtupleReader::readDataOsc(std::vector<std::vector<float> > & outputOsc)
  {
    //resize and prepare the vector:
    outputOsc.resize(config.npoints);
    for (auto & vec : outputOsc) vec.resize(defaultBins.size()-1);
    if (remakeHistOsc) LoopOsc(outputOsc);
    else  ReadOsc(outputOsc);
    return;
  }

  void NtupleReader::Loop(std::vector<float> &               output,
                          std::vector<std::vector<float> > & nominalDataByType){

    // This is the loop function, which is the heart of the class.

    TString dummytarget = config.path+fileNameSource;
    TChain *c = new TChain("EventsTot");
    TChain *d = new TChain("EventsTot");
    double fill_energy,true_energy,Elep,wgt;
    double checking_wgt;
    int inno,isCC,mode, ibkg;
    double ShowerContainedDist;
    double ShowerDistToStart;
    double showerGap;
    double vertexEnergy;
    int nuchan;


    int nbytes = 0,nb = 0;
    int ievt,ientry;

    c->Reset();
    c->Add(dummytarget);
    if (c==0) exit(5);
    
    d->Reset();
    d->Add(dummytarget);
    if (d==0) exit(5);
    

    const int nentries1 = int(c->GetEntries());

    // std::cout<<"Input filename:\t" << dummytarget << std::endl;
    std::cout << "Number of entries at L= " << baselMap[config.basel] << ":\t" << nentries1 << std::endl;
    c->SetBranchAddress("enugen",&true_energy);
    c->SetBranchAddress("inno",&inno);
    c->SetBranchAddress("nuchan", &nuchan);         // NUANCE interaction codes
    c->SetBranchAddress("mode",&mode);
    c->SetBranchAddress("isCC",&isCC);
    c->SetBranchAddress("wgt",&wgt);
    c->SetBranchAddress("ibkg",&ibkg);
    c->SetBranchAddress("ShowerContainedDistance",&ShowerContainedDist);
    c->SetBranchAddress("ShowerDistanceToStart"  ,&ShowerDistToStart);
    c->SetBranchAddress("VertexEnergy"  ,&vertexEnergy);
    c->SetBranchAddress("showerGap"  ,&showerGap);
    c->SetBranchAddress("LepE", &Elep);
    switch (config.energy){
      case kElep:
        c->SetBranchAddress("LepE", &fill_energy);
        break;
      case kTrue:
        c->SetBranchAddress("enugen",&fill_energy);
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
    }

///MW
    // std::vector<std::vector<float> > *MultiWeight;
    // MultiWeight = 0;
    // if (config.usingMultiWeights) 
    //   c->SetBranchAddress("MultiWeight",&MultiWeight);

    d->SetBranchAddress("wgt",&checking_wgt);

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
    switch (config.signal){
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
    int nbins=defaultBins.size()-1;

    sprintf(nameHolder,"nominalHisto_%s_%s",  signalMap[config.signal].c_str(),
                                              baselMap[config.basel].c_str());
    nominalHisto = new TH1F(nameHolder,nameHolder,nbins, &(defaultBins[0]));
    
    sprintf(nameHolder,"MCStatsHisto_%s_%s",  signalMap[config.signal].c_str(),
                                              baselMap[config.basel].c_str());
    MCStatsHisto = new TH1F(nameHolder,nameHolder,nbins, &(defaultBins[0]));

    // intialize the histograms for the by-type histos
    for (unsigned int i = 0; i < histosByType.size(); i ++){
      sprintf(nameHolder,"histoByType_%s_%s_%s",  signalMap[config.signal].c_str(),
                                                  baselMap[config.basel].c_str(),
                                                  (*ptrToTypeMap).at(i).c_str());
      histosByType[i] = new TH1F(nameHolder,nameHolder,nbins, &(defaultBins[0]));
    }

    std::cout << "histosByType.size() " << histosByType.size() << std::endl;

    // Transform histograms into vectors to save to file and report back
///MW
    // std::vector<std::vector<float > > edistrnueMultiWeightVec;
    // if (config.usingMultiWeights)
    //   edistrnueMultiWeightVec.resize(config.nWeights);

    
    // Now the histograms are set up and ready to go.
    // Loop over the ntuple:
    for (ievt=0; ievt<nentries1; ievt++)
    {
      ientry = d->LoadTree(ievt); if (ientry < 0) break; nb = d->GetEntry(ievt);
      if (checking_wgt == 0){
        continue;
      }

      ientry = c->LoadTree(ievt); if (ientry < 0) break; nb = c->GetEntry(ievt); nbytes += nb;
      // if (ievt == 0) {
      //   std::cout << "This is the first event, multiweight information:\n";
      //   std::cout << "  mutliweight.size(): " << MultiWeight -> size() << std::endl;
      //   std::cout << "  mutliweight[0].size(): " << (*MultiWeight)[0].size()<< std::endl;
      // }
      if ( (ievt%100000) == 0) {
        std::cout << "On event " << ievt << " of " << nentries1 << std::endl;
      }

      // here, do a check on the signal to see which route to take:
      if (config.signal == kNue){
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
           ){
          if(ShowerContainedDist < config.showerContainmentDist){
            continue;
          }
        }
        if(ShowerDistToStart < config.minDistanceToStart){
            continue;
        }  
        if (vertexEnergy < config.minVertexEnergySignal){
          continue;
        }
        if( showerGap    > config.minShowerGap && 
            vertexEnergy > config.minVertexEnergyPhoton){
          continue;
        }


        // if (!absolute_MWSource) edistrnue->Fill(fill_energy,weight);
        if (inno == 12 || inno == -12) MCStatsHisto->Fill(fill_energy);

        nominalHisto->Fill(fill_energy,wgt);
        histosByType[ibkg] -> Fill(fill_energy,wgt);

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
      else if (config.signal == kNumu){

      }

    } //end of loop

    // Get the cosmic and dirt stuff:
    // 
    // Get the cosmics data:
    
    // Get the dirt data:
    // 



    // Now move the data from the histograms into the objects that are returned
       //fill the vectors with the results from filling the histograms;
    
    for (unsigned int i = 0; i < histosByType.size(); i ++){
      for (int bin = 0; bin < nbins;  bin++){
        nominalDataByType[i][bin] = histosByType[i] -> GetBinContent(bin+1);
      }
    }
    for (int bin = 0; bin < nbins;  bin++){
      output[bin] = nominalHisto -> GetBinContent(bin+1);
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
    
    TFile * f = TFile::Open(config.path+fileNameHists, "RECREATE");
    if (!f) {
      std::cout << "Could not open file to write." << std::endl;
      exit(-1);
    }

    TTree *t = new TTree("tvecnue", "Tree with vectors");

    t-> Branch("nominalOutputVec", &output);
    for (unsigned int i = 0; i < histosByType.size(); i ++){
      char tempName[100];
      sprintf(tempName,"%sVec",(*ptrToTypeMap).at(i).c_str());
      t -> Branch(tempName,&nominalDataByType[i]);
    }

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
    for (unsigned int i = 0; i < histosByType.size(); i ++){
      histosByType[i] -> Write();
    }

    t -> Write("tvec");
    delete t;
    f -> Close();


  }
  void NtupleReader::LoopOsc(std::vector<std::vector<float> > & outputOsc){
    return;
  }
  
  void NtupleReader::Read(std::vector<float> &               output,
                          std::vector<std::vector<float> > & nominalDataByType){
    return;
  }
  void NtupleReader::ReadOsc(std::vector<std::vector<float> > & outputOsc){
    return;
  }

} // sbn

#endif
