
//-------------------------------------------------------------------------------
// This macro computes Data-to-MC correction factors for the PFJet resolution
// in different bins of |eta|.
//
// The correction factors are computed by averaging the Data/MC differences
// in PFJet resolution measured via:
//  o Pt-balance in QCD di-jet events
//  o Pt-balance between photon and highest Pt ("leading") jet in gamma + jet events
// selected in 2010 data.
//
// The measurements are documented in the JME-10-014 PAS
//  ( http://cms-physics.web.cern.ch/cms-physics/public/JME-10-014-pas.pdf )
//-------------------------------------------------------------------------------

Double_t square(Double_t x)
{
  return x*x;
}

void makePFJetResolutionMCtoDataCorrLUT()
{
  Double_t absEtaBins[] = { -0.1, 1.1, 1.7, 2.3, 9.9 };
  const Int_t numAbsEtaBins = 4;
  
  // numbers from JME-10-014 PAS
  Double_t mcToDataCorr_dijet2010[]              = { 1.04,  1.02,  0.91,  1.04  };
  Double_t mcToDataCorrStatErr_dijet2010[]       = { 0.012, 0.029, 0.044, 0.060 };
  Double_t mcToDataCorrSysErrUp_dijet2010[]      = { 0.052, 0.070, 0.085, 0.161 };
  Double_t mcToDataCorrSysErrDown_dijet2010[]    = { 0.049, 0.068, 0.081, 0.153 };

  Double_t mcToDataCorr_gammajet2010[]           = { 1.07,  1.10,  1.07,  1.18  };
  Double_t mcToDataCorrStatErr_gammajet2010[]    = { 0.020, 0.031, 0.048, 0.062 };
  Double_t mcToDataCorrSysErrUp_gammajet2010[]   = { 0.014, 0.009, 0.035, 0.017 };
  Double_t mcToDataCorrSysErrDown_gammajet2010[] = { 0.026, 0.023, 0.029, 0.057 };

  // numbers from AN-11/330
  Double_t mcToDataCorr_dijet2011[]              = { 1.055, 1.096, 1.134, 1.288 };
  Double_t mcToDataCorrStatErr_dijet2011[]       = { 0.012, 0.017, 0.035, 0.127 };
  Double_t mcToDataCorrSysErrUp_dijet2011[]      = { 0.061, 0.068, 0.097, 0.183 };
  Double_t mcToDataCorrSysErrDown_dijet2011[]    = { 0.060, 0.067, 0.095, 0.181 };

  TH2D* pfJetResolutionMCtoDataCorrLUT = new TH2D("pfJetResolutionMCtoDataCorrLUT", 
						  "pfJetResolutionMCtoDataCorrLUT", numAbsEtaBins, absEtaBins, 1, -0.01, 1.e+6);

  for ( Int_t iAbsEtaBin = 0; iAbsEtaBin < numAbsEtaBins; ++iAbsEtaBin ) {
    Double_t mcToDataCorrSysErr_dijet2010 = ( mcToDataCorr_dijet2010[numAbsEtaBins] < mcToDataCorr_gammajet2010[numAbsEtaBins] ) ?
      mcToDataCorrSysErrUp_dijet2010[iAbsEtaBin] : mcToDataCorrSysErrDown_dijet2010[iAbsEtaBin];
    Double_t mcToDataCorrTotErr2_dijet2010 = 
      square(mcToDataCorrStatErr_dijet2010[iAbsEtaBin]) + square(mcToDataCorrSysErr_dijet2010);
    Double_t weight_dijet2010 = 1./mcToDataCorrTotErr2_dijet2010;

    Double_t mcToDataCorrSysErr_gammajet2010 = ( mcToDataCorr_gammajet2010[numAbsEtaBins] < mcToDataCorr_dijet2010[numAbsEtaBins] ) ?
      mcToDataCorrSysErrUp_gammajet2010[iAbsEtaBin] : mcToDataCorrSysErrDown_gammajet2010[iAbsEtaBin];
    Double_t mcToDataCorrTotErr2_gammajet2010 = 
      square(mcToDataCorrStatErr_gammajet2010[iAbsEtaBin]) + square(mcToDataCorrSysErr_gammajet2010);
    Double_t weight_gammajet2010 = 1./mcToDataCorrTotErr2_gammajet2010;

    Double_t mcToDataCorr_2010 = 
      (weight_dijet2010*mcToDataCorr_dijet2010[iAbsEtaBin] + weight_gammajet2010*mcToDataCorr_gammajet2010[iAbsEtaBin])/
      (weight_dijet2010 + weight_gammajet2010);
    Double_t mcToDataCorrTotErr_2010 = TMath::Sqrt(1./(weight_dijet2010 + weight_gammajet2010));
    std::cout << "eta = " << absEtaBins[iAbsEtaBin] << "-" << absEtaBins[iAbsEtaBin + 1] << ":"
	      << " MC/Data correction = " << mcToDataCorr_2010 << " +/- " << mcToDataCorrTotErr_2010 << std::endl;

    Int_t binIndex = pfJetResolutionMCtoDataCorrLUT->FindBin(0.5*(absEtaBins[iAbsEtaBin] + absEtaBins[iAbsEtaBin + 1]), 1.);
    pfJetResolutionMCtoDataCorrLUT->SetBinContent(binIndex, mcToDataCorr_2010);
    pfJetResolutionMCtoDataCorrLUT->SetBinError(binIndex, mcToDataCorrTotErr_2010);    
  }
/*
  for ( Int_t iAbsEtaBin = 0; iAbsEtaBin < numAbsEtaBins; ++iAbsEtaBin ) {
    Double_t mcToDataCorrSysErr_dijet2010 = ( mcToDataCorr_dijet2010[numAbsEtaBins] < mcToDataCorr_dijet2011[numAbsEtaBins] ) ?
      mcToDataCorrSysErrUp_dijet[iAbsEtaBin] : mcToDataCorrSysErrDown_dijet2010[iAbsEtaBin];
    Double_t mcToDataCorrTotErr2_dijet2010 = 
      square(mcToDataCorrStatErr_dijet2010[iAbsEtaBin]) + square(mcToDataCorrSysErr_dijet2010);

    Double_t mcToDataCorrSysErr_dijet2011 = ( mcToDataCorr_dijet2011[numAbsEtaBins] < mcToDataCorr_dijet2010[numAbsEtaBins] ) ?
      mcToDataCorrSysErrUp_dijet2011[iAbsEtaBin] : mcToDataCorrSysErrDown_dijet2011[iAbsEtaBin];
    Double_t mcToDataCorrTotErr2_dijet2011 = 
      square(mcToDataCorrStatErr_dijet2011[iAbsEtaBin]) + square(mcToDataCorrSysErr_dijet2011);

    Double_t ratio_dijet2011to2010 = (mcToDataCorr_dijet2011[iAbsEtaBin]/mcToDataCorr_dijet2010[iAbsEtaBin]);
    Double_t ratioErr_dijet2011to2010 = 
      ratio_dijet2011to2010*TMath::Sqrt(mcToDataCorrTotErr2_dijet2010/square(mcToDataCorr_dijet2011[iAbsEtaBin])
				      + mcToDataCorrTotErr2_dijet2011/square(mcToDataCorr_dijet2010[iAbsEtaBin]));
    std::cout << "eta = " << absEtaBins[iAbsEtaBin] << "-" << absEtaBins[iAbsEtaBin + 1] << ":"
	      << " dijet2011/dijet2010 correction = " << ratio_dijet2011to2010
	      << " +/- " << ratioErr_dijet2011to2010 << std::endl;
  }
 */
  TFile* outputFile = new TFile("../data/pfJetResolutionMCtoDataCorrLUT.root", "RECREATE");
  pfJetResolutionMCtoDataCorrLUT->Write();
  delete outputFile;
}
