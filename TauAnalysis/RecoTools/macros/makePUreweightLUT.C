TH1* getHistogram(const TString& histogramName, const TString& histogramLabel, const TString& inputFilePath, TObjArray inputFileNames)
{
  TH1* retVal = 0;

  for ( int iInputFile = 0; iInputFile < inputFileNames.GetEntries(); ++iInputFile ) {
    TObjString* inputFileName = dynamic_cast<TObjString*>(inputFileNames.At(iInputFile));
    assert(inputFileName);

    TString inputFileName_full = TString(inputFilePath).Append(inputFileName->GetString());

    TFile* inputFile = TFile::Open(inputFileName_full.Data());
    if ( !inputFile ) {
      std::cerr << "Failed to open inputFile = " << inputFileName->GetString() 
		<< " --> skipping !!" << std::endl;
      continue;
    }

    TH1* histogram = dynamic_cast<TH1*>(inputFile->Get(histogramName));
    if ( !histogram ) {
      std::cerr << "Failed to load histogram = " << histogramName << " from inputFile = " << inputFileName->GetString() 
		<< " --> skipping !!" << std::endl;
      continue;
    }

    if ( !retVal ) {
      retVal = (TH1*)histogram->Clone(TString(histogramName).Append("_").Append(histogramLabel).Data());
      if ( !retVal->GetSumw2N() ) retVal->Sumw2();
    } else {
      retVal->Add(histogram);
    }
  }

  return retVal;
}

TH1* makePUreweightHistogram(const TString& histogramReweightName, TH1* histogramData, TH1* histogramMC, 
			     double minReweight = 1.e-1, double maxReweight = 1.e+1)
{
  assert(histogramData->GetDimension() == histogramMC->GetDimension() &&
	 histogramData->GetNbinsX()    == histogramMC->GetNbinsX()    &&
	 histogramData->GetNbinsY()    == histogramMC->GetNbinsY()    );

  TH1* retVal = histogramData->Clone(histogramReweightName.Data());
  retVal->Reset();

  TAxis* xAxis = histogramData->GetXaxis();
  TAxis* yAxis = histogramData->GetYaxis();

  int numBinsX = xAxis->GetNbins();
  for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
    int numBinsY = yAxis->GetNbins();
    for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
      
      double binContentData = histogramData->GetBinContent(iBinX, iBinY);
      double binContentMC   = histogramMC->GetBinContent(iBinX, iBinY);

      double reweight = 1.0;
      if ( binContentMC > 0. ) reweight = (binContentData/binContentMC);
      if ( reweight < minReweight ) reweight = minReweight;
      if ( reweight > maxReweight ) reweight = maxReweight;

      double binErrorData = histogramData->GetBinError(iBinX, iBinY);
      double binErrorMC   = histogramMC->GetBinError(iBinX, iBinY);

      double reweightErr2 = 0.;
      if ( binContentData > 0. ) reweightErr2 += TMath::Power(binErrorData/binContentData, 2.);
      if ( binContentMC   > 0. ) reweightErr2 += TMath::Power(binErrorMC/binContentMC, 2.);

      retVal->SetBinContent(iBinX, iBinY, reweight);
      retVal->SetBinError(iBinX, iBinY, TMath::Sqrt(reweightErr2));
    }
  }

  return retVal;
}

makePUreweightLUT()
{
  TString inputFilePath = "rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/PATtuples/ZllRecoilCorrection/";

  TString histoLUTname = "DQMData/producePUreweightHistograms/PFNeutralRhoVsVtxMultiplicity";

  TObjArray inputFileNamesData;
  inputFileNamesData.Add(new TObjString("ZllRecoilCorrection_Data_runs160329to163869_v1_layer_2_job_0_30a.root"));
  inputFileNamesData.Add(new TObjString("ZllRecoilCorrection_Data_runs165071to167913_v1_layer_2_job_0_d39.root"));
  TH1* histogramData = getHistogram(histoLUTname, "Data", inputFilePath, inputFileNamesData); 

  TObjArray inputFileNamesMC;
  inputFileNamesMC.Add(new TObjString("ZllRecoilCorrection_simDYtoMuMu_v1_layer_2_job_0_cf4.root"));
  TH1* histogramMC = getHistogram(histoLUTname, "MC", inputFilePath, inputFileNamesMC); 

  TH1* histogramPUreweight = makePUreweightHistogram("histoReweight", histogramData, histogramMC, 1.e-1, 1.e+1);

  TFile* outputFile = new TFile("../data/vertexMultiplicityVsRhoPFNeutralReweight.root", "RECREATE");
  histogramData->Write();
  histogramMC->Write();
  histogramPUreweight->Write();
  delete outputFile;
}
