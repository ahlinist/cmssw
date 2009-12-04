#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <string>
#include <vector>
#include <iostream>

// global flag to include ppmumux histograms
const bool usePpMuMux = false;


// function to scale histograms to the normalized luminosity
void ScaleHistograms(TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, TH1F* BdJpsiK0, 
		     TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F* ppmumux);           


// function to add the various backgrounds into one histogram
TH1F* addBackgrounds(TString name, TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, 
		     TH1F* BdJpsiK0, TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F *ppmumux);      

// function to draw histograms in a standard way (normalized to one)
void drawStandardNormalized(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux);

// draw histograms scaled to correc lumi
void drawStandardLumiScaled(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux);


// ***************main function*****************************
void MakePlots() {


  TFile * BtoJPsiMuMuHistoFile = new TFile ( "BtoJPsiMuMuHistoFile.root" , "READ");
  TFile * JPsiMuMuHistoFile  = new TFile ( "JPsiMuMuHistoFile.root", "READ");
//   TFile * ppmuXHistoFile   = new TFile ("ppmuXHistoFile.root", "READ");
//  TFile * ppmumuXHistoFile = new TFile ("ppmumuXHistoFile.root", "READ");
  TFile * ppmumuXHistoFile = new TFile ("JPsiMuMuHistoFile.root", "READ");


TH1F * hJPsiMass_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsJPsiPhiSignal"                );
TH1F * hJPsiMass_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsJPsiKKSignal"                 );
TH1F * hJPsiMass_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdJPsiKstarSignal"              );
TH1F * hJPsiMass_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsOther"                        );
TH1F * hJPsiMass_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdOther"                        );
TH1F * hJPsiMass_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_Other"                          );
TH1F * hJPsiMass_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BsJpsiEta"                      );
TH1F * hJPsiMass_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdJpsiK10"                      );
TH1F * hJPsiMass_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BdJpsiK0"                       );
TH1F * hJPsiMass_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hJPsiMass_BpJpsiKp"                       );
TH1F * hJPsiMass_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hJPsiMass_Other"                          );
TH1F * hJPsiMass_ppmumux                       =(TH1F*) ppmumuXHistoFile->Get("hJPsiMass_Other"                          );


TH1F * hPhiMass_BsJPsiPhiSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsJPsiPhiSignal"                 );
TH1F * hPhiMass_BsJPsiKKSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsJPsiKKSignal"                  );
TH1F * hPhiMass_BdJPsiKstarSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdJPsiKstarSignal"               );
TH1F * hPhiMass_BsOther                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsOther"                         );
TH1F * hPhiMass_BdOther                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdOther"                         );
TH1F * hPhiMass_BOther                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_Other"                           );
TH1F * hPhiMass_BsJpsiEta                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BsJpsiEta"                       );
TH1F * hPhiMass_BdJpsiK10                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdJpsiK10"                       );
TH1F * hPhiMass_BdJpsiK0                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BdJpsiK0"                        );
TH1F * hPhiMass_BpJpsiKp                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMass_BpJpsiKp"                        );
TH1F * hPhiMass_JpsiOther                          =(TH1F*) JPsiMuMuHistoFile->Get("hPhiMass_Other"                           );
TH1F * hPhiMass_ppmumux                          =(TH1F*) ppmumuXHistoFile->Get("hPhiMass_Other"                           );


TH1F * hPhiMassFinal_BsJPsiPhiSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsJPsiPhiSignal"            );
TH1F * hPhiMassFinal_BsJPsiKKSignal            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsJPsiKKSignal"             );
TH1F * hPhiMassFinal_BdJPsiKstarSignal         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdJPsiKstarSignal"          );
TH1F * hPhiMassFinal_BsOther                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsOther"                    );
TH1F * hPhiMassFinal_BdOther                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdOther"                    );
TH1F * hPhiMassFinal_BOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_Other"                      );
TH1F * hPhiMassFinal_BsJpsiEta                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BsJpsiEta"                  );
TH1F * hPhiMassFinal_BdJpsiK10                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdJpsiK10"                  );
TH1F * hPhiMassFinal_BdJpsiK0                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BdJpsiK0"                   );
TH1F * hPhiMassFinal_BpJpsiKp                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPhiMassFinal_BpJpsiKp"                   );
TH1F * hPhiMassFinal_JpsiOther                     =(TH1F*) JPsiMuMuHistoFile->Get("hPhiMassFinal_Other"                      );
TH1F * hPhiMassFinal_ppmumux                     =(TH1F*) ppmumuXHistoFile->Get("hPhiMassFinal_Other"                      );


TH1F * hBsMassFinal_BsJPsiPhiSignal            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsJPsiPhiSignal"             );
TH1F * hBsMassFinal_BsJPsiKKSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsJPsiKKSignal"              );
TH1F * hBsMassFinal_BdJPsiKstarSignal          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdJPsiKstarSignal"           );
TH1F * hBsMassFinal_BsOther                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsOther"                     );
TH1F * hBsMassFinal_BdOther                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdOther"                     );
TH1F * hBsMassFinal_BOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_Other"                       );
TH1F * hBsMassFinal_BsJpsiEta                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BsJpsiEta"                   );
TH1F * hBsMassFinal_BdJpsiK10                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdJpsiK10"                   );
TH1F * hBsMassFinal_BdJpsiK0                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BdJpsiK0"                    );
TH1F * hBsMassFinal_BpJpsiKp                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinal_BpJpsiKp"                    );
TH1F * hBsMassFinal_JpsiOther                      =(TH1F*) JPsiMuMuHistoFile->Get("hBsMassFinal_Other"                       );
TH1F * hBsMassFinal_ppmumux                      =(TH1F*) ppmumuXHistoFile->Get("hBsMassFinal_Other"                       );

TH1F * hBsMassFinalAfterFit_BsJPsiPhiSignal    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJPsiPhiSignal"     );
TH1F * hBsMassFinalAfterFit_BsJPsiKKSignal     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJPsiKKSignal"      );
TH1F * hBsMassFinalAfterFit_BdJPsiKstarSignal  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdJPsiKstarSignal"   );
TH1F * hBsMassFinalAfterFit_BsOther            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsOther"             );
TH1F * hBsMassFinalAfterFit_BdOther            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdOther"             );
TH1F * hBsMassFinalAfterFit_BOther              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_Other"               );
TH1F * hBsMassFinalAfterFit_BsJpsiEta          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BsJpsiEta"           );
TH1F * hBsMassFinalAfterFit_BdJpsiK10          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdJpsiK10"           );
TH1F * hBsMassFinalAfterFit_BdJpsiK0           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BdJpsiK0"            );
TH1F * hBsMassFinalAfterFit_BpJpsiKp           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_BpJpsiKp"            );
TH1F * hBsMassFinalAfterFit_JpsiOther              =(TH1F*) JPsiMuMuHistoFile->Get("hBsMassFinalAfterFit_Other"               );
TH1F * hBsMassFinalAfterFit_ppmumux              =(TH1F*) ppmumuXHistoFile->Get("hBsMassFinalAfterFit_Other"               );

TH1F * hK1Pt_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsJPsiPhiSignal"                    );
TH1F * hK1Pt_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsJPsiKKSignal"                     );
TH1F * hK1Pt_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdJPsiKstarSignal"                  );
TH1F * hK1Pt_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsOther"                            );
TH1F * hK1Pt_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdOther"                            );
TH1F * hK1Pt_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_Other"                              );
TH1F * hK1Pt_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BsJpsiEta"                          );
TH1F * hK1Pt_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdJpsiK10"                          );
TH1F * hK1Pt_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BdJpsiK0"                           );
TH1F * hK1Pt_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK1Pt_BpJpsiKp"                           );
TH1F * hK1Pt_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hK1Pt_Other"                              );
TH1F * hK1Pt_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hK1Pt_Other"                              );

TH1F * hK2Pt_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsJPsiPhiSignal"                    );
TH1F * hK2Pt_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsJPsiKKSignal"                     );
TH1F * hK2Pt_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdJPsiKstarSignal"                  );
TH1F * hK2Pt_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsOther"                            );
TH1F * hK2Pt_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdOther"                            );
TH1F * hK2Pt_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_Other"                              );
TH1F * hK2Pt_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BsJpsiEta"                          );
TH1F * hK2Pt_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdJpsiK10"                          );
TH1F * hK2Pt_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BdJpsiK0"                           );
TH1F * hK2Pt_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hK2Pt_BpJpsiKp"                           );
TH1F * hK2Pt_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hK2Pt_Other"                              );
TH1F * hK2Pt_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hK2Pt_Other"                              );

TH1F * hDist3D_BsJPsiPhiSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsJPsiPhiSignal"                  );
TH1F * hDist3D_BsJPsiKKSignal                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsJPsiKKSignal"                   );
TH1F * hDist3D_BdJPsiKstarSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdJPsiKstarSignal"                );
TH1F * hDist3D_BsOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsOther"                          );
TH1F * hDist3D_BdOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdOther"                          );
TH1F * hDist3D_BOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_Other"                            );
TH1F * hDist3D_BsJpsiEta                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BsJpsiEta"                        );
TH1F * hDist3D_BdJpsiK10                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdJpsiK10"                        );
TH1F * hDist3D_BdJpsiK0                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BdJpsiK0"                         );
TH1F * hDist3D_BpJpsiKp                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDist3D_BpJpsiKp"                         );
TH1F * hDist3D_JpsiOther                           =(TH1F*) JPsiMuMuHistoFile->Get("hDist3D_Other"                            );
TH1F * hDist3D_ppmumux                           =(TH1F*) ppmumuXHistoFile->Get("hDist3D_Other"                            );

TH1F * hDistSign3D_BsJPsiPhiSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsJPsiPhiSignal"              );
TH1F * hDistSign3D_BsJPsiKKSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsJPsiKKSignal"               );
TH1F * hDistSign3D_BdJPsiKstarSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdJPsiKstarSignal"            );
TH1F * hDistSign3D_BsOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsOther"                      );
TH1F * hDistSign3D_BdOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdOther"                      );
TH1F * hDistSign3D_BOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_Other"                        );
TH1F * hDistSign3D_BsJpsiEta                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BsJpsiEta"                    );
TH1F * hDistSign3D_BdJpsiK10                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdJpsiK10"                    );
TH1F * hDistSign3D_BdJpsiK0                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BdJpsiK0"                     );
TH1F * hDistSign3D_BpJpsiKp                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign3D_BpJpsiKp"                     );
TH1F * hDistSign3D_JpsiOther                       =(TH1F*) JPsiMuMuHistoFile->Get("hDistSign3D_Other"                        );
TH1F * hDistSign3D_ppmumux                       =(TH1F*) ppmumuXHistoFile->Get("hDistSign3D_Other"                        );

TH1F * hDistSign1D_BsJPsiPhiSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsJPsiPhiSignal"              );
TH1F * hDistSign1D_BsJPsiKKSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsJPsiKKSignal"               );
TH1F * hDistSign1D_BdJPsiKstarSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdJPsiKstarSignal"            );
TH1F * hDistSign1D_BsOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsOther"                      );
TH1F * hDistSign1D_BdOther                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdOther"                      );
TH1F * hDistSign1D_BOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_Other"                        );
TH1F * hDistSign1D_BsJpsiEta                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BsJpsiEta"                    );
TH1F * hDistSign1D_BdJpsiK10                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdJpsiK10"                    );
TH1F * hDistSign1D_BdJpsiK0                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BdJpsiK0"                     );
TH1F * hDistSign1D_BpJpsiKp                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hDistSign1D_BpJpsiKp"                     );
TH1F * hDistSign1D_JpsiOther                       =(TH1F*) JPsiMuMuHistoFile->Get("hDistSign1D_Other"                        );
TH1F * hDistSign1D_ppmumux                       =(TH1F*) ppmumuXHistoFile->Get("hDistSign1D_Other"                        );

TH1F * hPointingAngle_BsJPsiPhiSignal          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsJPsiPhiSignal"           );
TH1F * hPointingAngle_BsJPsiKKSignal           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsJPsiKKSignal"            );
TH1F * hPointingAngle_BdJPsiKstarSignal        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdJPsiKstarSignal"         );
TH1F * hPointingAngle_BsOther                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsOther"                   );
TH1F * hPointingAngle_BdOther                  =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdOther"                   );
TH1F * hPointingAngle_BOther                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_Other"                     );
TH1F * hPointingAngle_BsJpsiEta                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BsJpsiEta"                 );
TH1F * hPointingAngle_BdJpsiK10                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdJpsiK10"                 );
TH1F * hPointingAngle_BdJpsiK0                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BdJpsiK0"                  );
TH1F * hPointingAngle_BpJpsiKp                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hPointingAngle_BpJpsiKp"                  );
TH1F * hPointingAngle_JpsiOther                    =(TH1F*) JPsiMuMuHistoFile->Get("hPointingAngle_Other"                     );
TH1F * hPointingAngle_ppmumux                    =(TH1F*) ppmumuXHistoFile->Get("hPointingAngle_Other"                     );

TH1F * hChi2_BsJPsiPhiSignal                   =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsJPsiPhiSignal"                    );
TH1F * hChi2_BsJPsiKKSignal                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsJPsiKKSignal"                     );
TH1F * hChi2_BdJPsiKstarSignal                 =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdJPsiKstarSignal"                  );
TH1F * hChi2_BsOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsOther"                            );
TH1F * hChi2_BdOther                           =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdOther"                            );
TH1F * hChi2_BOther                             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_Other"                              );
TH1F * hChi2_BsJpsiEta                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BsJpsiEta"                          );
TH1F * hChi2_BdJpsiK10                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdJpsiK10"                          );
TH1F * hChi2_BdJpsiK0                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BdJpsiK0"                           );
TH1F * hChi2_BpJpsiKp                          =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2_BpJpsiKp"                           );
TH1F * hChi2_JpsiOther                             =(TH1F*) JPsiMuMuHistoFile->Get("hChi2_Other"                              );
TH1F * hChi2_ppmumux                             =(TH1F*) ppmumuXHistoFile->Get("hChi2_Other"                              );

TH1F * hChi2Ndof_BsJPsiPhiSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsJPsiPhiSignal"                );
TH1F * hChi2Ndof_BsJPsiKKSignal                =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsJPsiKKSignal"                 );
TH1F * hChi2Ndof_BdJPsiKstarSignal             =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdJPsiKstarSignal"              );
TH1F * hChi2Ndof_BsOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsOther"                        );
TH1F * hChi2Ndof_BdOther                       =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdOther"                        );
TH1F * hChi2Ndof_BOther                         =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_Other"                          );
TH1F * hChi2Ndof_BsJpsiEta                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BsJpsiEta"                      );
TH1F * hChi2Ndof_BdJpsiK10                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdJpsiK10"                      );
TH1F * hChi2Ndof_BdJpsiK0                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BdJpsiK0"                       );
TH1F * hChi2Ndof_BpJpsiKp                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hChi2Ndof_BpJpsiKp"                       );
TH1F * hChi2Ndof_JpsiOther                         =(TH1F*) JPsiMuMuHistoFile->Get("hChi2Ndof_Other"                          );
TH1F * hChi2Ndof_ppmumux                         =(TH1F*) ppmumuXHistoFile->Get("hChi2Ndof_Other"                          );

TH1F * hBsVtxProb_BsJPsiPhiSignal              =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsJPsiPhiSignal"               );
TH1F * hBsVtxProb_BsJPsiKKSignal               =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsJPsiKKSignal"                );
TH1F * hBsVtxProb_BdJPsiKstarSignal            =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdJPsiKstarSignal"             );
TH1F * hBsVtxProb_BsOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsOther"                       );
TH1F * hBsVtxProb_BdOther                      =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdOther"                       );
TH1F * hBsVtxProb_BOther                        =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_Other"                         );
TH1F * hBsVtxProb_BsJpsiEta                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BsJpsiEta"                     );
TH1F * hBsVtxProb_BdJpsiK10                    =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdJpsiK10"                     );
TH1F * hBsVtxProb_BdJpsiK0                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BdJpsiK0"                      );
TH1F * hBsVtxProb_BpJpsiKp                     =(TH1F*) BtoJPsiMuMuHistoFile->Get("hBsVtxProb_BpJpsiKp"                    );
TH1F * hBsVtxProb_JpsiOther                        =(TH1F*) JPsiMuMuHistoFile->Get("hBsVtxProb_Other"                         );
TH1F * hBsVtxProb_ppmumux                        =(TH1F*) ppmumuXHistoFile->Get("hBsVtxProb_Other"                         );



 ScaleHistograms(
 hPhiMassFinal_BsJPsiPhiSignal  ,  
 hPhiMassFinal_BsJPsiKKSignal   ,  
 hPhiMassFinal_BdJPsiKstarSignal,  
 hPhiMassFinal_BsOther          ,  
 hPhiMassFinal_BdOther          ,  
 hPhiMassFinal_BOther           ,  
 hPhiMassFinal_BsJpsiEta        ,  
 hPhiMassFinal_BdJpsiK10        ,  
 hPhiMassFinal_BdJpsiK0         ,  
 hPhiMassFinal_BpJpsiKp         ,  
 hPhiMassFinal_JpsiOther        ,
 hPhiMassFinal_ppmumux          );  

 ScaleHistograms(
hJPsiMass_BsJPsiPhiSignal   ,            
hJPsiMass_BsJPsiKKSignal    ,            
hJPsiMass_BdJPsiKstarSignal ,            
hJPsiMass_BsOther           ,            
hJPsiMass_BdOther           ,            
hJPsiMass_BOther            ,            
hJPsiMass_BsJpsiEta         ,            
hJPsiMass_BdJpsiK10         ,            
hJPsiMass_BdJpsiK0          ,            
hJPsiMass_BpJpsiKp          ,            
hJPsiMass_JpsiOther         ,
hJPsiMass_ppmumux);           
              
 ScaleHistograms(                         
hPhiMass_BsJPsiPhiSignal    ,            
hPhiMass_BsJPsiKKSignal     ,            
hPhiMass_BdJPsiKstarSignal  ,            
hPhiMass_BsOther            ,            
hPhiMass_BdOther            ,            
hPhiMass_BOther             ,            
hPhiMass_BsJpsiEta          ,            
hPhiMass_BdJpsiK10          ,            
hPhiMass_BdJpsiK0           ,            
hPhiMass_BpJpsiKp           ,            
hPhiMass_JpsiOther,
hPhiMass_ppmumux          );           
                                       
    ScaleHistograms(                                    
hPhiMassFinal_BsJPsiPhiSignal    ,       
hPhiMassFinal_BsJPsiKKSignal     ,       
hPhiMassFinal_BdJPsiKstarSignal  ,       
hPhiMassFinal_BsOther            ,       
hPhiMassFinal_BdOther            ,       
hPhiMassFinal_BOther             ,       
hPhiMassFinal_BsJpsiEta          ,       
hPhiMassFinal_BdJpsiK10          ,       
hPhiMassFinal_BdJpsiK0           ,       
hPhiMassFinal_BpJpsiKp           ,       
hPhiMassFinal_JpsiOther,
hPhiMassFinal_ppmumux          );      
                                       
    ScaleHistograms(                                    
hBsMassFinal_BsJPsiPhiSignal     ,       
hBsMassFinal_BsJPsiKKSignal      ,       
hBsMassFinal_BdJPsiKstarSignal   ,       
hBsMassFinal_BsOther             ,       
hBsMassFinal_BdOther             ,       
hBsMassFinal_BOther              ,       
hBsMassFinal_BsJpsiEta           ,       
hBsMassFinal_BdJpsiK10           ,       
hBsMassFinal_BdJpsiK0            ,       
hBsMassFinal_BpJpsiKp            ,       
hBsMassFinal_JpsiOther,
hBsMassFinal_ppmumux           );      
       
 ScaleHistograms(                                
hBsMassFinalAfterFit_BsJPsiPhiSignal   , 
hBsMassFinalAfterFit_BsJPsiKKSignal    , 
hBsMassFinalAfterFit_BdJPsiKstarSignal , 
hBsMassFinalAfterFit_BsOther           , 
hBsMassFinalAfterFit_BdOther           , 
hBsMassFinalAfterFit_BOther            , 
hBsMassFinalAfterFit_BsJpsiEta         , 
hBsMassFinalAfterFit_BdJpsiK10         , 
hBsMassFinalAfterFit_BdJpsiK0          , 
hBsMassFinalAfterFit_BpJpsiKp          , 
hBsMassFinalAfterFit_JpsiOther,
hBsMassFinalAfterFit_ppmumux         );
     
 ScaleHistograms(                                  
hK1Pt_BsJPsiPhiSignal   ,                
hK1Pt_BsJPsiKKSignal    ,                
hK1Pt_BdJPsiKstarSignal ,                
hK1Pt_BsOther           ,                
hK1Pt_BdOther           ,                
hK1Pt_BOther            ,                
hK1Pt_BsJpsiEta         ,                
hK1Pt_BdJpsiK10         ,                
hK1Pt_BdJpsiK0          ,                
hK1Pt_BpJpsiKp          ,                
hK1Pt_JpsiOther ,
hK1Pt_ppmumux        );               
    
 ScaleHistograms(                                   
hK2Pt_BsJPsiPhiSignal    ,               
hK2Pt_BsJPsiKKSignal     ,               
hK2Pt_BdJPsiKstarSignal  ,               
hK2Pt_BsOther            ,               
hK2Pt_BdOther            ,               
hK2Pt_BOther             ,               
hK2Pt_BsJpsiEta          ,               
hK2Pt_BdJpsiK10          ,               
hK2Pt_BdJpsiK0           ,               
hK2Pt_BpJpsiKp           ,               
hK2Pt_JpsiOther,
hK2Pt_ppmumux          );              
   
 ScaleHistograms(                                    
hDist3D_BsJPsiPhiSignal   ,              
hDist3D_BsJPsiKKSignal    ,              
hDist3D_BdJPsiKstarSignal ,              
hDist3D_BsOther           ,              
hDist3D_BdOther           ,              
hDist3D_BOther            ,              
hDist3D_BsJpsiEta         ,              
hDist3D_BdJpsiK10         ,              
hDist3D_BdJpsiK0          ,              
hDist3D_BpJpsiKp          ,              
hDist3D_JpsiOther,
hDist3D_ppmumux         );             
  
 ScaleHistograms(                                     
hDistSign3D_BsJPsiPhiSignal    ,         
hDistSign3D_BsJPsiKKSignal     ,         
hDistSign3D_BdJPsiKstarSignal  ,         
hDistSign3D_BsOther            ,         
hDistSign3D_BdOther            ,         
hDistSign3D_BOther             ,         
hDistSign3D_BsJpsiEta          ,         
hDistSign3D_BdJpsiK10          ,         
hDistSign3D_BdJpsiK0           ,         
hDistSign3D_BpJpsiKp           ,         
hDistSign3D_JpsiOther ,
hDistSign3D_ppmumux         );        
  
 ScaleHistograms(                                     
hDistSign1D_BsJPsiPhiSignal   ,          
hDistSign1D_BsJPsiKKSignal    ,          
hDistSign1D_BdJPsiKstarSignal ,          
hDistSign1D_BsOther           ,          
hDistSign1D_BdOther           ,          
hDistSign1D_BOther            ,          
hDistSign1D_BsJpsiEta         ,          
hDistSign1D_BdJpsiK10         ,          
hDistSign1D_BdJpsiK0          ,          
hDistSign1D_BpJpsiKp          ,          
hDistSign1D_JpsiOther,
hDistSign1D_ppmumux        );         
          
 ScaleHistograms(                             
hPointingAngle_BsJPsiPhiSignal   ,       
hPointingAngle_BsJPsiKKSignal    ,       
hPointingAngle_BdJPsiKstarSignal ,       
hPointingAngle_BsOther           ,       
hPointingAngle_BdOther           ,       
hPointingAngle_BOther            ,       
hPointingAngle_BsJpsiEta         ,       
hPointingAngle_BdJpsiK10         ,       
hPointingAngle_BdJpsiK0          ,       
hPointingAngle_BpJpsiKp          ,       
hPointingAngle_JpsiOther ,
hPointingAngle_ppmumux        );      
   
 ScaleHistograms(                                    
hChi2_BsJPsiPhiSignal     ,              
hChi2_BsJPsiKKSignal      ,              
hChi2_BdJPsiKstarSignal   ,              
hChi2_BsOther             ,              
hChi2_BdOther             ,              
hChi2_BOther              ,              
hChi2_BsJpsiEta           ,              
hChi2_BdJpsiK10           ,              
hChi2_BdJpsiK0            ,              
hChi2_BpJpsiKp            ,              
hChi2_JpsiOther,
hChi2_ppmumux         );             
        
 ScaleHistograms(                               
hChi2Ndof_BsJPsiPhiSignal   ,            
hChi2Ndof_BsJPsiKKSignal    ,            
hChi2Ndof_BdJPsiKstarSignal ,            
hChi2Ndof_BsOther           ,            
hChi2Ndof_BdOther           ,            
hChi2Ndof_BOther            ,            
hChi2Ndof_BsJpsiEta         ,            
hChi2Ndof_BdJpsiK10         ,            
hChi2Ndof_BdJpsiK0          ,            
hChi2Ndof_BpJpsiKp          ,            
hChi2Ndof_JpsiOther,
hChi2Ndof_ppmumux         );           
     
 ScaleHistograms(                                  
hBsVtxProb_BsJPsiPhiSignal    ,          
hBsVtxProb_BsJPsiKKSignal     ,          
hBsVtxProb_BdJPsiKstarSignal  ,          
hBsVtxProb_BsOther            ,          
hBsVtxProb_BdOther            ,          
hBsVtxProb_BOther             ,          
hBsVtxProb_BsJpsiEta          ,          
hBsVtxProb_BdJpsiK10          ,          
hBsVtxProb_BdJpsiK0           ,          
hBsVtxProb_BpJpsiKp           ,          
hBsVtxProb_JpsiOther,
hBsVtxProb_ppmumux         );         


 
 //****************************************
 // create summed histograms for background



 TH1F* hJPsiMass_BBackground =  addBackgrounds("hJPsiMass_BBackground",
					       hJPsiMass_BsJPsiPhiSignal   ,            
					       hJPsiMass_BsJPsiKKSignal    ,            
					       hJPsiMass_BdJPsiKstarSignal ,            
					       hJPsiMass_BsOther           ,            
					       hJPsiMass_BdOther           ,            
					       hJPsiMass_BOther            ,            
					       hJPsiMass_BsJpsiEta         ,            
					       hJPsiMass_BdJpsiK10         ,            
					       hJPsiMass_BdJpsiK0          ,            
					       hJPsiMass_BpJpsiKp          ,            
					       hJPsiMass_JpsiOther,
					       hJPsiMass_ppmumux);           
              
 TH1F* hPhiMass_BBackground =  addBackgrounds("hPhiMass_BBackground",                        
					      hPhiMass_BsJPsiPhiSignal    ,            
					      hPhiMass_BsJPsiKKSignal     ,            
					      hPhiMass_BdJPsiKstarSignal  ,            
					      hPhiMass_BsOther            ,            
					      hPhiMass_BdOther            ,            
					      hPhiMass_BOther             ,            
					      hPhiMass_BsJpsiEta          ,            
					      hPhiMass_BdJpsiK10          ,            
					      hPhiMass_BdJpsiK0           ,            
					      hPhiMass_BpJpsiKp           ,            
					      hPhiMass_JpsiOther,
					      hPhiMass_ppmumux);           
                                       
  TH1F* hPhiMassFinal_BBackground =  addBackgrounds("hPhiMassFinal_BBackground",
						    hPhiMassFinal_BsJPsiPhiSignal    ,       
						    hPhiMassFinal_BsJPsiKKSignal     ,       
						    hPhiMassFinal_BdJPsiKstarSignal  ,       
						    hPhiMassFinal_BsOther            ,       
						    hPhiMassFinal_BdOther            ,       
						    hPhiMassFinal_BOther             ,       
						    hPhiMassFinal_BsJpsiEta          ,       
						    hPhiMassFinal_BdJpsiK10          ,       
						    hPhiMassFinal_BdJpsiK0           ,       
						    hPhiMassFinal_BpJpsiKp           ,       
						    hPhiMassFinal_JpsiOther ,
						    hPhiMassFinal_ppmumux);      
                                       
     TH1F* hBsMassFinal_BBackground =  addBackgrounds("hBsMassFinal_BBackground",                                    
						      hBsMassFinal_BsJPsiPhiSignal     ,       
						      hBsMassFinal_BsJPsiKKSignal      ,       
						      hBsMassFinal_BdJPsiKstarSignal   ,       
						      hBsMassFinal_BsOther             ,       
						      hBsMassFinal_BdOther             ,       
						      hBsMassFinal_BOther              ,       
						      hBsMassFinal_BsJpsiEta           ,       
						      hBsMassFinal_BdJpsiK10           ,       
						      hBsMassFinal_BdJpsiK0            ,       
						      hBsMassFinal_BpJpsiKp            ,       
						      hBsMassFinal_JpsiOther ,
						      hBsMassFinal_ppmumux);      
     

  TH1F* hBsMassFinalAfterFit_BBackground =  addBackgrounds("hBsMassFinalAfterFit_BBackground",                                
							   hBsMassFinalAfterFit_BsJPsiPhiSignal   , 
							   hBsMassFinalAfterFit_BsJPsiKKSignal    , 
							   hBsMassFinalAfterFit_BdJPsiKstarSignal , 
							   hBsMassFinalAfterFit_BsOther           , 
							   hBsMassFinalAfterFit_BdOther           , 
							   hBsMassFinalAfterFit_BOther            , 
							   hBsMassFinalAfterFit_BsJpsiEta         , 
							   hBsMassFinalAfterFit_BdJpsiK10         , 
							   hBsMassFinalAfterFit_BdJpsiK0          , 
							   hBsMassFinalAfterFit_BpJpsiKp          , 
							   hBsMassFinalAfterFit_JpsiOther ,
							   hBsMassFinalAfterFit_ppmumux);
  
  TH1F* hK1Pt_BBackground =  addBackgrounds("hK1Pt_BBackground",                                  
					    hK1Pt_BsJPsiPhiSignal   ,                
					    hK1Pt_BsJPsiKKSignal    ,                
					    hK1Pt_BdJPsiKstarSignal ,                
					    hK1Pt_BsOther           ,                
					    hK1Pt_BdOther           ,                
					    hK1Pt_BOther            ,                
					    hK1Pt_BsJpsiEta         ,                
					    hK1Pt_BdJpsiK10         ,                
					    hK1Pt_BdJpsiK0          ,                
					    hK1Pt_BpJpsiKp          ,                
					    hK1Pt_JpsiOther ,
					    hK1Pt_ppmumux);               
    

  TH1F* hK2Pt_BBackground =  addBackgrounds("hK2Pt_BBackground",                                   
					    hK2Pt_BsJPsiPhiSignal    ,               
					    hK2Pt_BsJPsiKKSignal     ,               
					    hK2Pt_BdJPsiKstarSignal  ,               
					    hK2Pt_BsOther            ,               
					    hK2Pt_BdOther            ,               
					    hK2Pt_BOther             ,               
					    hK2Pt_BsJpsiEta          ,               
					    hK2Pt_BdJpsiK10          ,               
					    hK2Pt_BdJpsiK0           ,               
					    hK2Pt_BpJpsiKp           ,               
					    hK2Pt_JpsiOther ,
					    hK2Pt_ppmumux);              
  

  TH1F* hDist3D_BBackground =  addBackgrounds("hDist3D_BBackground",                                    
					      hDist3D_BsJPsiPhiSignal   ,              
					      hDist3D_BsJPsiKKSignal    ,              
					      hDist3D_BdJPsiKstarSignal ,              
					      hDist3D_BsOther           ,              
					      hDist3D_BdOther           ,              
					      hDist3D_BOther            ,              
					      hDist3D_BsJpsiEta         ,              
					      hDist3D_BdJpsiK10         ,              
					      hDist3D_BdJpsiK0          ,              
					      hDist3D_BpJpsiKp          ,              
					      hDist3D_JpsiOther ,
					      hDist3D_ppmumux);             
  
  TH1F* hDistSign3D_BBackground =  addBackgrounds("hDistSign3D_BBackground",                                     
						  hDistSign3D_BsJPsiPhiSignal    ,         
						  hDistSign3D_BsJPsiKKSignal     ,         
						  hDistSign3D_BdJPsiKstarSignal  ,         
						  hDistSign3D_BsOther            ,         
						  hDistSign3D_BdOther            ,         
						  hDistSign3D_BOther             ,         
						  hDistSign3D_BsJpsiEta          ,         
						  hDistSign3D_BdJpsiK10          ,         
						  hDistSign3D_BdJpsiK0           ,         
						  hDistSign3D_BpJpsiKp           ,         
						  hDistSign3D_JpsiOther  ,
						  hDistSign3D_ppmumux);        
  
  TH1F* hDistSign1D_BBackground =  addBackgrounds("hDistSign1D_BBackground",                                     
						  hDistSign1D_BsJPsiPhiSignal   ,          
						  hDistSign1D_BsJPsiKKSignal    ,          
						  hDistSign1D_BdJPsiKstarSignal ,          
						  hDistSign1D_BsOther           ,          
						  hDistSign1D_BdOther           ,          
						  hDistSign1D_BOther            ,          
						  hDistSign1D_BsJpsiEta         ,          
						  hDistSign1D_BdJpsiK10         ,          
						  hDistSign1D_BdJpsiK0          ,          
						  hDistSign1D_BpJpsiKp          ,          
						  hDistSign1D_JpsiOther ,
						  hDistSign1D_ppmumux);         
          
  TH1F* hPointingAngle_BBackground =  addBackgrounds("hPointingAngle_BBackground",                             
						     hPointingAngle_BsJPsiPhiSignal   ,       
						     hPointingAngle_BsJPsiKKSignal    ,       
						     hPointingAngle_BdJPsiKstarSignal ,       
						     hPointingAngle_BsOther           ,       
						     hPointingAngle_BdOther           ,       
						     hPointingAngle_BOther            ,       
						     hPointingAngle_BsJpsiEta         ,       
						     hPointingAngle_BdJpsiK10         ,       
						     hPointingAngle_BdJpsiK0          ,       
						     hPointingAngle_BpJpsiKp          ,       
						     hPointingAngle_JpsiOther ,
						     hPointingAngle_ppmumux);      
   
  TH1F* hChi2_BBackground =  addBackgrounds("hChi2_BBackground",                                    
					    hChi2_BsJPsiPhiSignal     ,              
					    hChi2_BsJPsiKKSignal      ,              
					    hChi2_BdJPsiKstarSignal   ,              
					    hChi2_BsOther             ,              
					    hChi2_BdOther             ,              
					    hChi2_BOther              ,              
					    hChi2_BsJpsiEta           ,              
					    hChi2_BdJpsiK10           ,              
					    hChi2_BdJpsiK0            ,              
					    hChi2_BpJpsiKp            ,              
					    hChi2_JpsiOther ,
					    hChi2_ppmumux);             
  
  TH1F* hChi2Ndof_BBackground =  addBackgrounds("hChi2Ndof_BBackground",                               
						hChi2Ndof_BsJPsiPhiSignal   ,            
						hChi2Ndof_BsJPsiKKSignal    ,            
						hChi2Ndof_BdJPsiKstarSignal ,            
						hChi2Ndof_BsOther           ,            
						hChi2Ndof_BdOther           ,            
						hChi2Ndof_BOther            ,            
						hChi2Ndof_BsJpsiEta         ,            
						hChi2Ndof_BdJpsiK10         ,            
						hChi2Ndof_BdJpsiK0          ,            
						hChi2Ndof_BpJpsiKp          ,            
						hChi2Ndof_JpsiOther,
						hChi2Ndof_ppmumux);           
     
  TH1F* hBsVtxProb_BBackground =  addBackgrounds("hBsVtxProb_BBackground",                                  
						 hBsVtxProb_BsJPsiPhiSignal    ,          
						 hBsVtxProb_BsJPsiKKSignal     ,          
						 hBsVtxProb_BdJPsiKstarSignal  ,          
						 hBsVtxProb_BsOther            ,          
						 hBsVtxProb_BdOther            ,          
						 hBsVtxProb_BOther             ,          
						 hBsVtxProb_BsJpsiEta          ,          
						 hBsVtxProb_BdJpsiK10          ,          
						 hBsVtxProb_BdJpsiK0           ,          
						 hBsVtxProb_BpJpsiKp           ,          
						 hBsVtxProb_JpsiOther  ,
						 hBsVtxProb_ppmumux);         



  //*********************

  drawStandardNormalized("JPsiMass",
			 hJPsiMass_BsJPsiPhiSignal,
			 hJPsiMass_BdJPsiKstarSignal,
			 hJPsiMass_BBackground, 
			 hJPsiMass_JpsiOther,
			 hJPsiMass_ppmumux);      

  drawStandardNormalized("PhiMass",
			 hPhiMass_BsJPsiPhiSignal,              
			 hPhiMass_BdJPsiKstarSignal,            
			 hPhiMass_BBackground,                  
			 hPhiMass_JpsiOther,
			 hPhiMass_ppmumux);                  
    


  drawStandardNormalized("K1Pt",
			 hK1Pt_BsJPsiPhiSignal,                 
			 hK1Pt_BdJPsiKstarSignal,               
			 hK1Pt_BBackground,                     
			 hK1Pt_JpsiOther,
			 hK1Pt_ppmumux);                     
      
  drawStandardNormalized("K2Pt",
			 hK2Pt_BsJPsiPhiSignal,                 
			 hK2Pt_BdJPsiKstarSignal,               
			 hK2Pt_BBackground,                     
			 hK2Pt_JpsiOther,
			 hK2Pt_ppmumux);                     
       
  drawStandardNormalized("Dist3D",
			 hDist3D_BsJPsiPhiSignal,               
			 hDist3D_BdJPsiKstarSignal,             
			 hDist3D_BBackground,                   
			 hDist3D_JpsiOther,
			 hDist3D_ppmumux);                   

  drawStandardNormalized("DistSign3D",
			 hDistSign3D_BsJPsiPhiSignal,           
			 hDistSign3D_BdJPsiKstarSignal,         
			 hDistSign3D_BBackground,               
			 hDistSign3D_JpsiOther,
			 hDistSign3D_ppmumux);               

  drawStandardNormalized("DistSign1D",
			 hDistSign1D_BsJPsiPhiSignal,           
			 hDistSign1D_BdJPsiKstarSignal,         
			 hDistSign1D_BBackground,               
			 hDistSign1D_JpsiOther,
			 hDistSign1D_ppmumux);               

  drawStandardNormalized("PointingAngle",
			 hPointingAngle_BsJPsiPhiSignal,        
			 hPointingAngle_BdJPsiKstarSignal,      
			 hPointingAngle_BBackground,            
			 hPointingAngle_JpsiOther,
			 hPointingAngle_ppmumux);            

  drawStandardNormalized("Chi2",
			 hChi2_BsJPsiPhiSignal,                 
			 hChi2_BdJPsiKstarSignal,               
			 hChi2_BBackground,                     
			 hChi2_JpsiOther,
			 hChi2_ppmumux);                     
 
  drawStandardNormalized("Chi2Ndof", 
			 hChi2Ndof_BsJPsiPhiSignal,             
			 hChi2Ndof_BdJPsiKstarSignal,           
			 hChi2Ndof_BBackground,                 
			 hChi2Ndof_JpsiOther,
			 hChi2Ndof_ppmumux);                 
  
  drawStandardNormalized("BsVtxProb",
			 hBsVtxProb_BsJPsiPhiSignal,            
			 hBsVtxProb_BdJPsiKstarSignal,          
			 hBsVtxProb_BBackground,                
			 hBsVtxProb_JpsiOther,
			 hBsVtxProb_ppmumux);                
  


  //***********************
  // plots scaled to correct lumi
  drawStandardLumiScaled("PhiMassFinal",
			 hPhiMassFinal_BsJPsiPhiSignal,         
			 hPhiMassFinal_BdJPsiKstarSignal,       
			 hPhiMassFinal_BBackground,             
			 hPhiMassFinal_JpsiOther,
			 hPhiMassFinal_ppmumux);             


  drawStandardLumiScaled("BsMassFinal",
			 hBsMassFinal_BsJPsiPhiSignal,          
			 hBsMassFinal_BdJPsiKstarSignal,        
			 hBsMassFinal_BBackground,              
			 hBsMassFinal_JpsiOther,
			 hBsMassFinal_ppmumux);              
 
  drawStandardLumiScaled("BsMassFinalAfterFit",
			 hBsMassFinalAfterFit_BsJPsiPhiSignal,  
			 hBsMassFinalAfterFit_BdJPsiKstarSignal,
			 hBsMassFinalAfterFit_BBackground,      
			 hBsMassFinalAfterFit_JpsiOther,
			 hBsMassFinalAfterFit_ppmumux);      




 TCanvas *cv3 = new TCanvas("Bs mass before/after fit", "Bs mass before/after fit", 900, 600 );
 cv3->cd();
 cv3->SetFillColor(0);


 hBsMassFinal_BsJPsiPhiSignal             ->SetLineColor(1  );
 hBsMassFinalAfterFit_BsJPsiPhiSignal     ->SetLineColor(2  );

 hBsMassFinal_BsJPsiPhiSignal              ->SetLineWidth(3);
 hBsMassFinalAfterFit_BsJPsiPhiSignal     ->SetLineWidth(3);

 TLegend * leg3 = new TLegend(0.6,0.75,0.93,0.91);
 leg3->AddEntry(  hBsMassFinal_BsJPsiPhiSignal   , "Bs mass w/o fit" );
 leg3->AddEntry(  hBsMassFinalAfterFit_BsJPsiPhiSignal    , "Bs mass with fit"	);
 

hBsMassFinalAfterFit_BsJPsiPhiSignal   ->Draw();
hBsMassFinal_BsJPsiPhiSignal    ->Draw("SAME");


 leg3->Draw("SAME");


//  //---------------
//  TCanvas *cv4 = new TCanvas("BsVtxProb", "BsVtxProb", 900, 600 );
//  cv4->cd();
//  cv4->SetFillColor(0);


// hBsVtxProb_BsJPsiPhiSignal    ->SetLineColor(1  );
// hBsVtxProb_BsJPsiKKSignal     ->SetLineColor(2  );
// hBsVtxProb_BdJPsiKstarSignal  ->SetLineColor(3  );
// hBsVtxProb_BsOther            ->SetLineColor(4  );
// hBsVtxProb_BdOther            ->SetLineColor(5  );
// hBsVtxProb_BOther             ->SetLineColor(6  );
// hBsVtxProb_BsJpsiEta          ->SetLineColor(7  );
// hBsVtxProb_BdJpsiK10          ->SetLineColor(8  );
// hBsVtxProb_BdJpsiK0           ->SetLineColor(9  );
// hBsVtxProb_BpJpsiKp           ->SetLineColor(12 );
// hBsVtxProb_JpsiOther          ->SetLineColor(11 );

// hBsVtxProb_BsJPsiPhiSignal    ->SetLineWidth(3);
// hBsVtxProb_BsJPsiKKSignal     ->SetLineWidth(3);
// hBsVtxProb_BdJPsiKstarSignal  ->SetLineWidth(3);
// hBsVtxProb_BsOther            ->SetLineWidth(3);
// hBsVtxProb_BdOther            ->SetLineWidth(3);
// hBsVtxProb_BOther             ->SetLineWidth(3);
// hBsVtxProb_BsJpsiEta          ->SetLineWidth(3);
// hBsVtxProb_BdJpsiK10          ->SetLineWidth(3);
// hBsVtxProb_BdJpsiK0           ->SetLineWidth(3);
// hBsVtxProb_BpJpsiKp           ->SetLineWidth(3);
// hBsVtxProb_JpsiOther          ->SetLineWidth(3);

//  TLegend * leg4 = new TLegend(0.6,0.75,0.93,0.91);
//  leg4->AddEntry(  hBsVtxProb_BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
//  leg4->AddEntry(  hBsVtxProb_BsJPsiKKSignal    , "Bs -> JPsi KK"	);
//  leg4->AddEntry(  hBsVtxProb_BdJPsiKstarSignal , "Bd -> JPsi K*"	);
//  leg4->AddEntry(  hBsVtxProb_BsJpsiEta         , "Bs -> JPsi eta"	);
//  leg4->AddEntry(  hBsVtxProb_BdJpsiK10         , "Bd -> JPsi K10"	);
//  leg4->AddEntry(  hBsVtxProb_BdJpsiK0          , "Bd -> JPsi K0"	);
//  leg4->AddEntry(  hBsVtxProb_BpJpsiKp          , "B+ -> JPsi K+"	);
//  leg4->AddEntry(  hBsVtxProb_BsOther           , "other Bs"	);
//  leg4->AddEntry(  hBsVtxProb_BdOther           , "other Bd"	);
//  leg4->AddEntry(  hBsVtxProb_BOther            , "other"		);
//  leg4->AddEntry(  hBsVtxProb_JpsiOther         , "prompt JPsi"    );


// hBsVtxProb_BsJPsiPhiSignal   ->DrawNormalized();
// hBsVtxProb_BsJPsiKKSignal    ->DrawNormalized("SAME");
// hBsVtxProb_BdJPsiKstarSignal ->DrawNormalized("SAME");
// hBsVtxProb_BsOther           ->DrawNormalized("SAME");
// hBsVtxProb_BdOther           ->DrawNormalized("SAME");
// hBsVtxProb_BOther            ->DrawNormalized("SAME");
// hBsVtxProb_BsJpsiEta         ->DrawNormalized("SAME");
// hBsVtxProb_BdJpsiK10         ->DrawNormalized("SAME");
// hBsVtxProb_BdJpsiK0          ->DrawNormalized("SAME");
// hBsVtxProb_BpJpsiKp          ->DrawNormalized("SAME");
// hBsVtxProb_JpsiOther         ->DrawNormalized("SAME");


//  leg4->Draw("SAME");

//  //----------------------
//  TCanvas *cv31 = new TCanvas("PhiMass bg merged", "PhiMass bg merged", 900, 600 );
//  cv31->cd();
//  cv31->SetFillColor(0);



// hPhiMass_BsJPsiPhiSignal    ->SetLineColor(1  );
// hPhiMass_BdJPsiKstarSignal  ->SetLineColor(2  );
// hPhiMass_BBackground        ->SetLineColor(3  );
// hPhiMass_JpsiOther          ->SetLineColor(4 );

// hPhiMass_BsJPsiPhiSignal    ->SetLineWidth(3  );
// hPhiMass_BdJPsiKstarSignal  ->SetLineWidth(3  );
// hPhiMass_BBackground        ->SetLineWidth(3  );
// hPhiMass_JpsiOther          ->SetLineWidth(3 );


//  TLegend * leg31 = new TLegend(0.6,0.75,0.93,0.91);
//  leg31->AddEntry(  hPhiMass_BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
//  leg31->AddEntry(  hPhiMass_BdJPsiKstarSignal , "Bd -> JPsi K*"	);
//  leg31->AddEntry(  hPhiMass_BBackground       , "otherB -> JPsi X");
//  leg31->AddEntry(  hPhiMass_JpsiOther         , "prompt JPsi"    );


//  hPhiMass_BsJPsiPhiSignal   ->DrawNormalized();
//  hPhiMass_BdJPsiKstarSignal ->DrawNormalized("SAME");
//  hPhiMass_BBackground       ->DrawNormalized("SAME");
//  hPhiMass_JpsiOther         ->DrawNormalized("SAME");
 

//  leg31->Draw("SAME");



  }



//------------------------------------
void ScaleHistograms(TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, TH1F* BdJpsiK0, TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F *ppmumux){

BsJPsiPhiSignal      ->Scale(100./ 63. ); 
BsJPsiKKSignal       ->Scale(100./ 63. ); 
BdJPsiKstarSignal    ->Scale(100./ 80.);  
BsOther              ->Scale(100./ 63. ); 
BdOther              ->Scale(100./ 80. ); 
BOther               ->Scale(100./ 80.);  
BsJpsiEta            ->Scale(100./ 63.);  
BdJpsiK10            ->Scale(100./ 80. ); 
BdJpsiK0             ->Scale(100./ 80. ); 
BpJpsiKp             ->Scale(100./ 80.);  
JpsiOther            ->Scale(100./ 9.);   
 ppmumux              ->Scale(100./0.9);
}


//------------------------------------
TH1F* addBackgrounds(TString name, TH1F* BsJPsiPhiSignal,  TH1F* BsJPsiKKSignal, TH1F* BdJPsiKstarSignal, TH1F* BsOther,
		     TH1F* BdOther, TH1F* BOther, TH1F* BsJpsiEta, TH1F* BdJpsiK10, TH1F* BdJpsiK0, TH1F* BpJpsiKp, TH1F* JpsiOther, TH1F* ppmumux){

 TH1F* BBackground = (TH1F*) BsJPsiKKSignal->Clone(name);
 BBackground->Add( BsOther );
 BBackground->Add( BdOther );
 BBackground->Add( BOther );
 BBackground->Add( BsJpsiEta );
 BBackground->Add( BdJpsiK10 );
 BBackground->Add( BdJpsiK0 );
 BBackground->Add( BpJpsiKp );

 return BBackground;
}




//--------------------------------------
void drawStandardNormalized(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux){


 TCanvas *cv = new TCanvas(name, name, 900, 600 );
 cv->cd();
 cv->SetFillColor(0);


 BsJPsiPhiSignal    ->SetLineColor(1 );
 BdJPsiKstarSignal  ->SetLineColor(2 );
 BBackground        ->SetLineColor(3);
 JpsiOther          ->SetLineColor(4 );
 ppmumux            ->SetLineColor(5 );

 BsJPsiPhiSignal    ->SetLineWidth(3 );
 BdJPsiKstarSignal  ->SetLineWidth(3 );
 BBackground        ->SetLineWidth(3);
 JpsiOther          ->SetLineWidth(3 );
 ppmumux            ->SetLineWidth(3 );


 TLegend * leg3 = new TLegend(0.6,0.75,0.93,0.91);
 leg3->AddEntry(  BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
 leg3->AddEntry(  BdJPsiKstarSignal , "Bd -> JPsi K*"	);
 leg3->AddEntry(  BBackground       , "other B->JPsi X"	);
 leg3->AddEntry(  JpsiOther         , "prompt JPsi"    );
 if(usePpMuMux) leg3->AddEntry(  ppmumux         , "pp-> mu mu X"    );


 JpsiOther         ->DrawNormalized();
 BsJPsiPhiSignal   ->DrawNormalized("SAME");
 BdJPsiKstarSignal ->DrawNormalized("SAME");
 BBackground       ->DrawNormalized("SAME");
 if(usePpMuMux) ppmumux           ->DrawNormalized("SAME");
 

 leg3->Draw("SAME");

 }



void drawStandardLumiScaled(TString name, TH1F* BsJPsiPhiSignal, TH1F* BdJPsiKstarSignal, TH1F *BBackground, TH1F* JpsiOther, TH1F* ppmumux){
 TCanvas *cv = new TCanvas(name, name, 900, 600 );
 cv->cd();
 cv->SetFillColor(0);


 BsJPsiPhiSignal    ->SetLineColor(1 );
 BdJPsiKstarSignal  ->SetLineColor(2 );
 BBackground        ->SetLineColor(3);
 JpsiOther          ->SetLineColor(4 );
 ppmumux            ->SetLineColor( 5);
 
 BsJPsiPhiSignal    ->SetLineWidth(3 );
 BdJPsiKstarSignal  ->SetLineWidth(3 );
 BBackground        ->SetLineWidth(3);
 JpsiOther          ->SetLineWidth(3 );
 ppmumux            ->SetLineWidth(3);


 TLegend * leg3 = new TLegend(0.6,0.75,0.93,0.91);
 leg3->AddEntry(  BsJPsiPhiSignal   , "Bs -> JPsi Phi" );
 leg3->AddEntry(  BdJPsiKstarSignal , "Bd -> JPsi K*"	);
 leg3->AddEntry(  BBackground       , "other B->JPsi X"	);
 leg3->AddEntry(  JpsiOther         , "prompt JPsi"    );
 if(usePpMuMux) leg3->AddEntry(  ppmumux         , "pp-> mu mu X"    );



 BsJPsiPhiSignal   ->Draw();
 BdJPsiKstarSignal ->Draw("SAME");
 BBackground       ->Draw("SAME");
 JpsiOther         ->Draw("SAME"); 
 if(usePpMuMux) ppmumux           ->Draw("SAME");

 leg3->Draw("SAME");

}
