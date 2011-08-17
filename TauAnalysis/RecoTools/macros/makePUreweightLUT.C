TH1* getHistogram(const TString& histogramName, const TString& histogramLabel, TObjArray inputFileNames)
{
  TH1* retVal = 0;

  for ( int iInputFile = 0; iInputFile < inputFileNames.GetEntries(); ++iInputFile ) {
    TObjString* inputFileName = dynamic_cast<TObjString*>(inputFileNames.At(iInputFile));
    assert(inputFileName);

    TFile* inputFile = TFile::Open(inputFileName->GetString());
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

    delete inputFile;
  }

  return retVal;
}

TH1* makePUreweightHistogram(const TString& histogramReweightName, TH1* histogramData, TH1* histogramMC, 
			     double minReweight = 1.e-1, double maxReweight = 1.e+1)
{
  assert(histoData->GetDimension() == histoMC->GetDimension() &&
	 histoData->GetNbinsX()    == histoMC->GetNbinsX()    &&
	 histoData->GetNbinsY()    == histoMC->GetNbinsY()    );

  TH1* retVal = histogramData->Clone(histogramReweightName.Data());
  retVal->Reset();

  TAxis* xAxis = histoData->GetXaxis();
  TAxis* yAxis = histoData->GetYaxis();

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
  TString histoLUTname = "";

  TObjArray inputFileNamesData;
  inputFileNamesData.Add(new TObjString(""));
  inputFileNamesData.Add(new TObjString(""));
  TH1* histoData = getHistogram(histoLUTname, "Data", inputFileNamesData); 

  TObjArray inputFileNamesMC;
  inputFileNamesMC.Add(new TObjString(""));
  TH1* histoMC = getHistogram(histoLUTname, "MC", inputFileNamesData); 

  TH1* histoPUreweight = makePUreweightHistogram("histoReweight", histoData, histoMC, 1.e-1, 1.e+1);

  TFile* outputFile = new TFile("../data/vertexMultiplicityVsRhoPFNeutralReweight.root", "RECREATE");
  histoData->Write();
  histoMC->Write();
  histoPUreweight->Write();
  delete outputFile;
}
