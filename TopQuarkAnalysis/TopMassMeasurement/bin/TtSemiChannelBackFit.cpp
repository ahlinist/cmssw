  //if asked for, recalculate channel background fit (for the moment, only with otherTt)
  if(constructBchannel){
    
    cout<<"*** Making channel background fit and probSignal estimate..."<<endl;
    hmtsign    	= new TH1F("hmtsign","Signal top mass contributions",nrMtHistBins/4,minMtHist,maxMtHist);
    hmtotherTT 	= new TH1F("hmtotherTT","otherTT backgr top mass contributions",nrMtHistBins/4,minMtHist,maxMtHist);
    hmtW4j 	= new TH1F("hmtW4j","W+4jets backgr top mass contributions",nrMtHistBins/4,minMtHist,maxMtHist);
    hmtWbb2j 	= new TH1F("hmtWbb2j","Wbb+2jets backgr top mass contributions",nrMtHistBins/4,minMtHist,maxMtHist);
    hmtWbb3j 	= new TH1F("hmtWbb3j","Wbb+3jets backgr top mass contributions",nrMtHistBins/4,minMtHist,maxMtHist);
    
    for (nr = 1; nr <= nrSignalFiles; nr++) {
      TString fileDir = Dir; fileDir += combBConstrSample;
      ttbarEventReader *events = new ttbarEventReader((char*) ((string)fileDir).c_str(), "TtSemiLepAnalysis_", nr);
      Bool_t ok = events->FileOK();
      if(ok){
        while (events->next()) {
          ++nrAnalysedSign;
          if(EventSelection((*events)(),combBTagFit)==1) { AnalyseEvent((*events)(),20); }
        }
        delete events; 
      }
    }
    for (nr = 1; nr <= nrotherTTFiles; nr++) {
      ttbarEventReader *events = new ttbarEventReader((char*) ((string)otherTTDir).c_str(), "TtSemiLepAnalysis_", nr); 
      Bool_t ok = events->FileOK();
      if(ok){
        while (events->next()) {
          ++nrAnalysedotherTT;
          if(EventSelection((*events)(),combBTagFit)==1) { AnalyseEvent((*events)(),21); }
        }
        delete events; 
      } 
    }
    for (nr = 1; nr <= nrW4jFiles; nr++) {
      ttbarEventReader *events = new ttbarEventReader((char*) ((string)W4jDir).c_str(), "TtSemiLepAnalysis_", nr); 
      Bool_t ok = events->FileOK();
      if(ok){
        while (events->next()) {
          ++nrAnalysedW4j;
          if(EventSelection((*events)(),combBTagFit)==1) { AnalyseEvent((*events)(),22); }
        }
        delete events; 
      } 
    }   
    for (nr = 1; nr <= nrWbb2jFiles; nr++) {
      ttbarEventReader *events = new ttbarEventReader((char*) ((string)Wbb2jDir).c_str(), "TtSemiLepAnalysis_", nr); 
      Bool_t ok = events->FileOK();
      if(ok){
        while (events->next()) {
          ++nrAnalysedWbb2j;
          if(EventSelection((*events)(),combBTagFit)==1) { AnalyseEvent((*events)(),23); }
        }
        delete events; 
      } 
    }   
    for (nr = 1; nr <= nrWbb3jFiles; nr++) {
      ttbarEventReader *events = new ttbarEventReader((char*) ((string)Wbb3jDir).c_str(), "TtSemiLepAnalysis_", nr); 
      Bool_t ok = events->FileOK();
      if(ok){
        while (events->next()) {
          ++nrAnalysedWbb3j;
          if(EventSelection((*events)(),combBTagFit)==1) { AnalyseEvent((*events)(),24); }
        }
        delete events; 
      } 
    }

    //Calculate probSignal
    Int_t neededotherTT  = (Int_t) (TtCrossSection*nrAnalysedSign*1.)/(TtCrossSection*1.);
    Int_t neededW4j     = (Int_t) (W4jCrossSection*nrAnalysedSign*1.)/(TtCrossSection*1.);
    Int_t neededWbb2j   = (Int_t) (Wbb2jCrossSection*nrAnalysedSign*1.)/(TtCrossSection*1.);
    Int_t neededWbb3j   = (Int_t) (Wbb3jCrossSection*nrAnalysedSign*1.)/(TtCrossSection*1.);
    Float_t otherTTCorr  = 0, W4jCorr  = 0, Wbb2jCorr  = 0, Wbb3jCorr  = 0;
    if(nrAnalysedotherTT>0) otherTTCorr = (neededotherTT*1.)/(nrAnalysedotherTT*1.);
    if(nrAnalysedW4j>0)    W4jCorr    = (neededW4j*1.)   /(nrAnalysedW4j*1.);
    if(nrAnalysedWbb2j>0)  Wbb2jCorr  = (neededWbb2j*1.) /(nrAnalysedWbb2j*1.);
    if(nrAnalysedWbb3j>0)  Wbb3jCorr  = (neededWbb3j*1.) /(nrAnalysedWbb3j*1.);
    
    //scaling of histos & fitting
    for(int nrb = 0; nrb<=nrMtHistBins; nrb++){
      hmtotherTT->SetBinContent(nrb,otherTTCorr*hmtotherTT->GetBinContent(nrb));
      hmtW4j->SetBinContent(nrb,W4jCorr*hmtW4j->GetBinContent(nrb));
      hmtWbb2j->SetBinContent(nrb,Wbb2jCorr*hmtWbb2j->GetBinContent(nrb));
      hmtWbb3j->SetBinContent(nrb,Wbb3jCorr*hmtWbb3j->GetBinContent(nrb));
    }
    
    theProbSignal = hmtsign->GetSumOfWeights()/(hmtsign   ->GetSumOfWeights()
    			                      + hmtotherTT->GetSumOfWeights()
    			                      + hmtW4j->GetSumOfWeights()
    			                      + hmtWbb2j->GetSumOfWeights()
    			                      + hmtWbb3j->GetSumOfWeights());
					      
					      
    cout<<"STATISTICS"<<endl;
    cout<<endl<<"*** Analysed: "<<endl;
    cout<<"    "<<nrAnalysedSign<<" signal events -> integrated luminosity = "<<(nrAnalysedSign*1.)/(1000.*TtCrossSection)<<"fb-1"<<endl<<endl;
    cout<<"    "<<nrAnalysedotherTT<<" otherTT events -> integrated luminosity = "<<(nrAnalysedotherTT*1.)/(1000.*TtCrossSection)<<"fb-1  (corr = 
"<<otherTTCorr<<")"<<endl;
    cout<<"    "<<nrAnalysedW4j<<" W4j events -> integrated luminosity = "<<(nrAnalysedW4j*1.)/(1000.*W4jCrossSection)<<"fb-1  (corr = 
"<<W4jCorr<<")"<<endl;
    cout<<"    "<<nrAnalysedWbb2j<<" Wbb2j events -> integrated luminosity = "<<(nrAnalysedWbb2j*1.)/(1000.*Wbb2jCrossSection)<<"fb-1  (corr = 
"<<Wbb2jCorr<<")"<<endl;
    cout<<"    "<<nrAnalysedWbb3j<<" Wbb3j events -> integrated luminosity = "<<(nrAnalysedWbb3j*1.)/(1000.*Wbb3jCrossSection)<<"fb-1  (corr = 
"<<Wbb3jCorr<<")"<<endl;
    cout<<endl;
    cout<<"*** After full selection: "<<endl;
    cout<<"    "<<hmtsign->GetSumOfWeights()<<" signal events "<<endl;
    cout<<"    "<<hmtotherTT->GetSumOfWeights()<<" otherTT events"<<endl; 
    cout<<"    "<<hmtW4j->GetSumOfWeights()<<" W4j events"<<endl; 
    cout<<"    "<<hmtWbb2j->GetSumOfWeights()<<" Wbb2j events"<<endl; 
    cout<<"    "<<hmtWbb3j->GetSumOfWeights()<<" Wbb3j events"<<endl; 
    cout<<endl;
    
    //scaling of histos & fitting
    for(int nrb = 0; nrb<=nrMtHistBins; nrb++){
      hmtotherTT->SetBinContent(nrb,otherTTCorr*hmtotherTT->GetBinContent(nrb));
      hmtW4j->SetBinContent(nrb,W4jCorr*hmtW4j->GetBinContent(nrb));
      hmtWbb2j->SetBinContent(nrb,Wbb2jCorr*hmtWbb2j->GetBinContent(nrb));
      hmtWbb3j->SetBinContent(nrb,Wbb3jCorr*hmtWbb3j->GetBinContent(nrb));
    }
    
    cout<<"*** After full selection (for 1 fb-1): "<<endl;
    cout<<"    "<<hmtsign->GetSumOfWeights()/((nrAnalysedSign*1.)/(1000.*TtCrossSection))<<" signal events "<<endl;
    cout<<"    "<<hmtotherTT->GetSumOfWeights()/((nrAnalysedSign*1.)/(1000.*TtCrossSection))<<" otherTT events"<<endl; 
    cout<<"    "<<hmtW4j->GetSumOfWeights()/((nrAnalysedSign*1.)/(1000.*TtCrossSection))<<" W4j events"<<endl; 
    cout<<"    "<<hmtWbb2j->GetSumOfWeights()/((nrAnalysedSign*1.)/(1000.*TtCrossSection))<<" Wbb2j events"<<endl; 
    cout<<"    "<<hmtWbb3j->GetSumOfWeights()/((nrAnalysedSign*1.)/(1000.*TtCrossSection))<<" Wbb3j events"<<endl; 
    cout<<endl;
    cout<<"*** probSignal  = "<<theProbSignal<<endl;
    
    
    hBtot = *hmtotherTT;
    //hBtot.Add(hmtW4j);
    //hBtot.Add(hmtWbb2j);
    //hBtot.Add(hmtWbb3j);
    hBtot.SetTitle("Total backgr top mass contributions");
    hBtot.SetName("hBtot");
    
    htot = *hmtsign;
    htot.Add(&hBtot);
    htot.SetName("hboth");
    htot.SetTitle("All top mass contributions");
    
    TFile * outfile = new TFile(channelBgrFileName, "RECREATE");
    TH1F *hsign     = new TH1F(*hmtsign);
    TH1F *hotherTT  = new TH1F(*hmtotherTT);
    TH1F *hW4j      = new TH1F(*hmtW4j);
    TH1F *hWbb2j    = new TH1F(*hmtWbb2j);
    TH1F *hWbb3j    = new TH1F(*hmtWbb3j);
    hmtBTot   	    = new TH1F(hBtot);
    TH1F *hmtboth   = new TH1F(htot);
    TFormula gauss("gauss", "gaus"); TFormula lan("lan", "landau");
    fmtBTot = new TF1("fmtBTot","gauss+lan",minMtHist,maxMtHist);
    fmtBTot->SetParameters(60,200,20,60,200,20);
    hmtBTot->Fit("fmtBTot","RQ");
    outfile->cd();
    outfile->Write();
    outfile->Close();
    delete outfile;
  }
