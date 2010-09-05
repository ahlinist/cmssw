inline double square(double x)
{
  return x*x;	
}

inline double cube(double x)
{
  return x*x*x;	
}

inline double fourth(double x)
{
  return x*x*x*x;	
}

double smearedKinematicDistribution(double x, double M, double s) 
{
  double num_first_term = TMath::Exp(-0.5*square(x)/square(s))
                         *8*s*(fourth(M) + 2*square(M)*(2*square(s) + square(x)) + 6*(square(s) + square(x))*(8*square(s) + square(x)));

  double num_second_term = TMath::Exp(-square(M - 2*x)/(8*square(s)))
                          *s*(15*fourth(M) + 14*cube(M) + 48*(square(s)+square(x))*(8*square(s)+square(x)) 
                             + 4*square(M)*(20*square(s) + 7*square(x)) + 24*M*(7*square(s)*x + cube(x)));

  double num_third_term = 4*TMath::Sqrt(TMath::TwoPi())
                         *x*(fourth(M) + 6*square(M)*square(s) + 90*fourth(s) + 2*(square(M) + 30*square(s))*square(x) + 6*fourth(x))
                         *(TMath::Erf((M - 2*x)/(2*TMath::Sqrt2()*s)) + TMath::Erf(x/(TMath::Sqrt2()*s)));
  double num_factor = 1/(2*TMath::Sqrt(TMath::TwoPi()));
  double numerator = num_factor*(num_first_term - num_second_term + num_third_term);
 
  // now compute normalization factor
  double den_first_term = (2*TMath::Sqrt(1.0/TMath::PiOver2()) 
                         *TMath::Exp(-square(M)/(8*square(s)))
                         *M*s*(11*fourth(M) + 44*square(M)*square(s) + 240*fourth(s)));
  double den_second_term = TMath::Erf(M/(2*TMath::Sqrt2()*s))
                          *(11*fourth(M)*square(M) - 32*fourth(M)*square(s) - 96*square(M)*fourth(s) - 960*fourth(s)*square(s));
  double denominator = (1./16)*(den_first_term + den_second_term);

  return numerator/denominator;
}

double movingTauLeptonPtPDF(double tauPt, double diTauMass)
{
/*
  return 1.; // ONLY FOR TESTING
 */
  double smearNorm = 0.52 + 0.000658*diTauMass;
  double smearWidth = 1.8 + 0.018*diTauMass;
  double M = 2.3 + 1.04*diTauMass;
  double gammaScale = 6.74 + 0.020*diTauMass;
  double gammaShape = 2.2 + 0.0364*diTauMass;
  
  return smearNorm*smearedKinematicDistribution(tauPt, M, smearWidth) 
        + (1 - smearNorm)*TMath::GammaDist(tauPt, gammaShape, 0., gammaScale);
}

double prob(double theta)
{
/*
  return 1.; // ONLY FOR TESTING
 */
  return TMath::Sin(theta);
}

double prob(double mTau, double mVis, double mNuNu)
{
/*
  return 1.; // ONLY FOR TESTING
 */
  double p1 = mNuNu/2.;
  double p3 = TMath::Sqrt((square(mTau) - square(mNuNu + mVis))*(square(mTau) - square(mNuNu - mVis)))/(2.*mTau);
  return p1*p3;
}

void draw(TCanvas* canvas, double ptVis, const std::string& outputFileName)
{
  std::cout << "<draw>:" << std::endl;
  std::cout << " ptVis = " << ptVis << std::endl;

  TH1* histoMethod01 = new TH1D("histoMethod01", "cos(theta) Distribution", 100, -1., +1);
  histoMethod01->Sumw2();
  TH1* histoMethod02 = new TH1D("histoMethod02", "cos(theta) Distribution", 100, -1., +1);
  histoMethod02->Sumw2();
  
  TRandom3 rnd;

  double mZ = 91.19;
  double mTau = 1.78;
  double mVis = 0.11;

  double thetaLab = 0.25*TMath::Pi();

  unsigned numToys = 100000;
  for ( unsigned iToy = 0; iToy < numToys; ++iToy ) {
    if ( (iToy % 10000) == 0 ) std::cout << "iToy = " << iToy << std::endl;

    double theta = rnd.Rndm()*TMath::Pi();
    double mNuNu = rnd.Rndm()*(mTau - mVis);

    double E = (1./(2.*mTau))*(square(mTau) + square(mVis) - square(mNuNu));
    //std::cout << "E = " << E << std::endl;
    double p = TMath::Sqrt(square(E) - square(mVis));
    //std::cout << "p = " << p << std::endl;
/*
    double ptTau01 = E*mTau*ptVis 
      - TMath::Sqrt(square(mTau*p*ptVis*TMath::Cos(theta)) - square(E*mTau*p*TMath::Cos(theta)*TMath::Sin(thetaLab)) 
		   + square(mTau)*fourth(p*TMath::Cos(theta))*square(TMath::Sin(thetaLab)))/
       (square(E) - square(p*TMath::Cos(theta)));
 */
    double ptTau01 = (mTau*ptVis)/(E + p*TMath::Cos(theta));
    //std::cout << "ptTau01 = " << ptTau01 << std::endl;
    
    //std::cout << "TMath::Sin(theta) = " << TMath::Sin(theta) << std::endl;
    //std::cout << "prob(mTau, mVis, mNuNu) = " << prob(mTau, mVis, mNuNu) << std::endl;
    //std::cout << "movingTauLeptonPtPDF(ptTau01, mZ) = " << movingTauLeptonPtPDF(ptTau01, mZ) << std::endl; 

    double weight01 = prob(theta)*prob(mTau, mVis, mNuNu)*movingTauLeptonPtPDF(ptTau01, mZ);
    //std::cout << "weight01 = " << weight01 << std::endl;
    histoMethod01->Fill(TMath::Cos(theta), weight01);

    double ptTau02 = rnd.Rndm()*mZ;
    double weight02 = prob(theta)*prob(mTau, mVis, mNuNu)*movingTauLeptonPtPDF(ptTau02, mZ);

    double x = (1./mTau)*(E + TMath::Sqrt(1 - square(mTau*TMath::Sin(thetaLab)/ptTau02))*p*TMath::Cos(theta));
    //std::cout << "x = " << x << std::endl;
    double ptVis02 = x*ptTau02;
    if ( TMath::Abs(ptVis02 - ptVis) < 1. ) {
      //std::cout << "weight02 = " << weight02 << std::endl;
      histoMethod02->Fill(TMath::Cos(theta), weight02);
    }
  }
  
  histoMethod01->Scale(1./histoMethod01->Integral());
  histoMethod01->SetLineColor(2);
  histoMethod01->SetMarkerColor(2);
  histoMethod01->SetStats(false);
  histoMethod01->Draw("e1p");
  histoMethod02->Scale(1./histoMethod02->Integral());
  histoMethod02->SetLineColor(4);
  histoMethod02->SetMarkerColor(4);
  histoMethod02->SetStats(false);
  histoMethod02->Draw("e1psame");

  canvas->Update();
  canvas->Print(outputFileName.data());
}

void toyMCsculptingEffect()
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  draw(canvas, 15., "toyMCsculptingEffect_ptVis15.png");
  draw(canvas, 25., "toyMCsculptingEffect_ptVis25.png");
  draw(canvas, 35., "toyMCsculptingEffect_ptVis35.png");
    
  delete canvas;
}
