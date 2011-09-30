
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
  Double_t absEtaBins[] = { 0.0, 1.1, 1.7, 2.3, 5.0 };
  const Int_t numAbsEtaBins = 4;
  
  Double_t mcToDataCorr_dijet[]              = { 1.04,  1.02,  0.91,  1.04  };
  Double_t mcToDataCorrStatErr_dijet[]       = { 0.012, 0.029, 0.044, 0.060 };
  Double_t mcToDataCorrSysErrUp_dijet[]      = { 0.052, 0.070, 0.085, 0.161 };
  Double_t mcToDataCorrSysErrDown_dijet[]    = { 0.049, 0.068, 0.081, 0.153 };

  Double_t mcToDataCorr_gammajet[]           = { 1.07,  1.10,  1.07,  1.18  };
  Double_t mcToDataCorrStatErr_gammajet[]    = { 0.020, 0.031, 0.048, 0.062 };
  Double_t mcToDataCorrSysErrUp_gammajet[]   = { 0.014, 0.009, 0.035, 0.017 };
  Double_t mcToDataCorrSysErrDown_gammajet[] = { 0.026, 0.023, 0.029, 0.057 };

  TH2D* pfJetResolutionMCtoDataCorrLUT = new TH2D("pfJetResolutionMCtoDataCorrLUT", 
						  "pfJetResolutionMCtoDataCorrLUT", numAbsEtaBins, absEtaBins, 1, -0.01, 1.e+6);

  for ( Int_t iAbsEtaBin = 0; iAbsEtaBin < numAbsEtaBins; ++iAbsEtaBin ) {
    Double_t mcToDataCorrSysErr_dijet = ( mcToDataCorr_dijet[numAbsEtaBins] < mcToDataCorr_gammajet[numAbsEtaBins] ) ?
      mcToDataCorrSysErrUp_dijet[iAbsEtaBin] : mcToDataCorrSysErrDown_dijet[iAbsEtaBin];
    Double_t mcToDataCorrTotErr2_dijet = square(mcToDataCorrStatErr_dijet[iAbsEtaBin]) + square(mcToDataCorrSysErr_dijet);
    Double_t weight_dijet = 1./mcToDataCorrTotErr2_dijet;

    Double_t mcToDataCorrSysErr_gammajet = ( mcToDataCorr_gammajet[numAbsEtaBins] < mcToDataCorr_dijet[numAbsEtaBins] ) ?
      mcToDataCorrSysErrUp_gammajet[iAbsEtaBin] : mcToDataCorrSysErrDown_gammajet[iAbsEtaBin];
    Double_t mcToDataCorrTotErr2_gammajet = square(mcToDataCorrStatErr_gammajet[iAbsEtaBin]) + square(mcToDataCorrSysErr_gammajet);
    Double_t weight_gammajet = 1./mcToDataCorrTotErr2_gammajet;

    Double_t mcToDataCorr = (weight_dijet*mcToDataCorr_dijet[iAbsEtaBin] + weight_gammajet*mcToDataCorr_gammajet[iAbsEtaBin])/
                            (weight_dijet + weight_gammajet);
    Double_t mcToDataCorrTotErr = 1./(weight_dijet + weight_gammajet);
    std::cout << "eta = " << absEtaBins[iAbsEtaBin] << "-" << absEtaBins[iAbsEtaBin + 1] << ":"
	      << " MC/Data correction = " << mcToDataCorr << " +/- " << mcToDataCorrTotErr << std::endl;

    Int_t binIndex = pfJetResolutionMCtoDataCorrLUT->FindBin(0.5*(absEtaBins[iAbsEtaBin] + absEtaBins[iAbsEtaBin + 1]), 1.);
    pfJetResolutionMCtoDataCorrLUT->SetBinContent(binIndex, mcToDataCorr);
    pfJetResolutionMCtoDataCorrLUT->SetBinError(binIndex, mcToDataCorrTotErr);
  }

  TFile* outputFile = new TFile("../data/pfJetResolutionMCtoDataCorrLUT.root", "RECREATE");
  pfJetResolutionMCtoDataCorrLUT->Write();
  delete outputFile;
}
