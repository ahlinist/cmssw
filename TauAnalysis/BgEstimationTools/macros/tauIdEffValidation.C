
void tauIdEffValidation()
{
  TFile* inputFile = TFile::Open("rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/bgEstPlots/ZtoMuTau_frSimple/10TeV/plotsZtoMuTau_ZtautauSum.root");

  // define colors used to plot efficiencies/fake-rates of different tau id. criteria
  // (definition of "rainbow" colors taken from TauAnalysis/DQMTools/python/plotterStyleDefinitions_cfi.py)
  TArrayI colors(7);
  colors[0] = 877; // violett
  colors[1] = 596; // dark blue
  colors[2] = 856; // light blue
  colors[3] = 817; // green
  colors[4] = 396; // yellow
  colors[5] = 797; // orange
  colors[6] = 628; // red

  TObjArray cutEffNames;
  cutEffNames.Add(new TObjString("ByStandardChain"));

  TObjArray meNames;
  meNames.Add(new TObjString("TauPt"));
  meNames.Add(new TObjString("TauEta"));
  meNames.Add(new TObjString("TauPhi"));
  meNames.Add(new TObjString("TauAssocJetPt"));
  meNames.Add(new TObjString("TauJetRadius"));

  //TString dqmDirectory = "zMuTauAnalyzer_frUnweighted/afterEvtSelTauLeadTrkPt_beforeEvtSelTauTrkIso/TauIdEffValidation";
  TString dqmDirectory = "zMuTauAnalyzer_frUnweighted/afterEvtSelDiMuPairZmumuHypothesisVeto/TauIdEffValidation";

  showTauIdEfficiency(inputFile, "Ztautau", dqmDirectory, meNames, cutEffNames, "effZTTsim", colors);
  //showTauIdEfficiency(inputFile, "qcdSum", dqmDirectory, meNames, cutEffNames, "frMuEnrichedQCDsim", colors);

  delete inputFile;
}

TH1* getMonitorElement(TFile* inputFile, const TString& dqmDirectoryName, const char* processName, const TString& meName)
{
  TString meName_full = TString("DQMData").Append("/").Append(processName).Append("/").Append(dqmDirectoryName).Append("/").Append(meName);
  std::cout << "meName_full = " << meName_full << std::endl;
  
  TH1* me = (TH1*)inputFile->Get(meName_full);
  std::cout << "me = " << me <<  std::endl;
  std::cout << "integral = " << me->Integral() << std::endl;
  
  return me;
}

void showTauIdEfficiency(TFile* inputFile, const char* processName, const TString& dqmDirectoryName, const TObjArray& meNames,
			 const TObjArray& cutEffNames, const char* effTypeLabel, const TArrayI& colors)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  unsigned numMonitorElements = meNames.GetEntries();
  for ( unsigned iMonitorElement = 0; iMonitorElement < numMonitorElements; ++iMonitorElement ) {
    const TString& meName = ((TObjString*)meNames.At(iMonitorElement))->GetString();

    TLegend legend(0.24, 0.69, 0.85, 0.89);
    legend.SetBorderSize(0);
    legend.SetFillColor(0);

    bool isFirst = true;

    unsigned numCutEfficiencies = cutEffNames.GetEntries();
    for ( unsigned iCutEff = 0; iCutEff < numCutEfficiencies; ++iCutEff ) {
      const TString& cutEffName = ((TObjString*)cutEffNames.At(iCutEff))->GetString();

      TString meName_numerator_cuts = TString("numerators").Append("/").Append(cutEffName).Append("/").Append(meName).Append("_cuts");
      TH1* me_numerator_cuts = getMonitorElement(inputFile, dqmDirectoryName, processName, meName_numerator_cuts);

      TString meName_numerator_weights = TString("numerators").Append("/").Append(cutEffName).Append("/").Append(meName).Append("_").Append(effTypeLabel);
      TH1* me_numerator_weights = getMonitorElement(inputFile, dqmDirectoryName, processName, meName_numerator_weights);

      TString meName_denominator = TString("denominator").Append("/").Append(meName).Append("_denominator");
      TH1* me_denominator = getMonitorElement(inputFile, dqmDirectoryName, processName, meName_denominator);

      TH1* me_efficiency_cuts = (TH1*)me_numerator_cuts->Clone();
      me_efficiency_cuts->Divide(me_numerator_cuts, me_denominator, 1., 1., "B");
      me_efficiency_cuts->SetLineColor(colors[iCutEff]);
      me_efficiency_cuts->SetMarkerColor(colors[iCutEff]);      
      me_efficiency_cuts->SetMarkerStyle(20);
      me_efficiency_cuts->SetMarkerSize(1);

      TH1* me_efficiency_weights = (TH1*)me_numerator_weights->Clone();
      me_efficiency_weights->Divide(me_numerator_weights, me_denominator, 1., 1., "B");
      me_efficiency_weights->SetLineColor(colors[iCutEff]);
      me_efficiency_weights->SetMarkerColor(colors[iCutEff]);
      me_efficiency_weights->SetMarkerStyle(24);
      me_efficiency_weights->SetMarkerSize(1);

      if ( isFirst ) {	
	me_efficiency_cuts->SetTitle(TString(meName).Append(" (").Append(processName).Append(")"));
	me_efficiency_cuts->SetStats(false);
	me_efficiency_cuts->SetMinimum(0.);
	me_efficiency_cuts->SetMaximum(1.4);
	me_efficiency_cuts->Draw("e1p");
      } else {
	me_efficiency_cuts->Draw("e1psame");	
      }
      
      me_efficiency_weights->Draw("e1psame");

      legend.AddEntry(me_efficiency_cuts, TString(cutEffName).Append(" (cuts)"), "p");
      legend.AddEntry(me_efficiency_weights, TString(cutEffName).Append(" (").Append(effTypeLabel).Append(")"), "p");
    }

    legend.Draw();

    TString outputFileName = TString("tauIdEffValidation").Append("_").Append(processName).Append("_").Append(meName).Append(".png");

    canvas->Update();
    canvas->Print(outputFileName);
  }

  delete canvas;
}
		     

