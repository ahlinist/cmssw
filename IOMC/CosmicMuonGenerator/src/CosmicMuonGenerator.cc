#include "IOMC/CosmicMuonGenerator/interface/CosmicMuonGenerator.h"

void CosmicMuonGenerator::runCMG(){
  initialize();
  for (unsigned int iGen=0; iGen<NumberOfEvents; ++iGen){ nextEvent(); }
  terminate();
}

void CosmicMuonGenerator::initialize(){
  checkIn();
  if (NumberOfEvents > 0){
    RanGen.SetSeed2(RanSeed,54321);
    // set up "surface geometry" dimensions
    Target3dRadius = sqrt(RadiusTarget*RadiusTarget + Z_DistTarget*Z_DistTarget) + MinStepSize;
    if (Debug) cout << "  radius of sphere  around  target = " << Target3dRadius << " mm" << endl;
    SurfaceRadius = (SurfaceOfEarth+RadiusTarget)*tan(MaxTheta) + Target3dRadius;
    if (Debug) cout << "  starting point radius at surface = " << SurfaceRadius << " mm" << endl;
    // initialize phase space regions
    int iv=0;
    int      nE = 15; // number of energy intervals
    double minE = MinE; double maxE = MinE;
    double incE = (MaxE-MinE)/pow(2.,nE);
    for (int iE=0; iE<nE; ++iE){// E
      minE = maxE;
      maxE = minE+incE*pow(2.,iE);
      //if (maxE > MaxE) maxE = MaxE;
      if (iE == nE-1) maxE = MaxE;
      int      nTheta = 2; // number of theta intervals
      double minTheta = MinTheta; double maxTheta = MinTheta;
      double incTheta = (MaxTheta-MinTheta)/double(nTheta);
      for (int iTheta=0; iTheta<nTheta; ++iTheta){// theta
        minTheta = maxTheta;
        maxTheta = minTheta + incTheta;
        if (iTheta == nTheta-1) maxTheta = MaxTheta;
        V[iv].setBorders(minE,maxE,minTheta,maxTheta);
        ++iv;
      }
    }
    for (int iv=0; iv<30; ++iv){
      V[iv].setIntAndMax(333);
      SumIntegrals += V[iv].integral();
      cout << "  setting up volume " << iv << endl;
      if (Debug) cout << "    E = " << V[iv].loEnergy() << "..." << V[iv].upEnergy()
		      << "  theta = " << V[iv].loTheta() << "..." << V[iv].upTheta() << endl;
      if (Debug) cout << "    --> maximum = " << V[iv].maximum() << "  integral = " << V[iv].integral() << endl;
    }
    cout << "  sum of all phase space integrals = " << SumIntegrals << endl;
    double lowerProb = 0.;
    double upperProb = 0.;
    for (int iv=0; iv<30; ++iv){
      lowerProb = upperProb;
      upperProb = lowerProb + V[iv].integral()/SumIntegrals;
      V[iv].setProbabilities(lowerProb,upperProb);
      if (Debug) cout << "  probability interval " << iv << ":    " << lowerProb << "..." << upperProb << endl;
    }
#if ROOT_INTERACTIVE
  // book histos
  TH1D* ene = new TH1D("ene","generated energy",210,0.,1050.);
  TH1D* the = new TH1D("the","generated theta",90,0.,90.);
  TH1D* phi = new TH1D("phi","generated phi",120,0.,360.);
  TH3F* ver = new TH3F("ver","Z-X-Y coordinates",50,-25.,25.,20,-10.,10.,20,-10.,10.);
#endif
    if (EventDisplay) initEvDis();
    cout << endl;
    cout << "  generating " << NumberOfEvents << " events with random seed " << RanSeed << endl;
    NotInitialized = false;
  }
}

void CosmicMuonGenerator::nextEvent(){
  double E = 0.; double Theta = 0.; double Phi = 0.; double RxzV = 0.; double PhiV = 0.;
  if (int(Nsel)%100 == 0) cout << "    generated " << int(Nsel) << " events" << endl;
  // generate momentum
  bool   notSelected = true;
  while (notSelected){
    double prob = RanGen.Rndm();
    for (int iv=0; iv<30; ++iv){
      if (prob >= V[iv].loProb() && prob < V[iv].upProb()){
	bool   badMomentumGenerated = true;
	while (badMomentumGenerated){
	  E = RanGen.Rndm()*(V[iv].upEnergy()-V[iv].loEnergy()) + V[iv].loEnergy();
	  Theta = RanGen.Rndm()*(V[iv].upTheta()-V[iv].loTheta()) + V[iv].loTheta();
	  double dN = dNdEdT(E,Theta);
	  if (RanGen.Rndm()*V[iv].maximum() < dN){
	    Ngen+=1.;
	    badMomentumGenerated = false;
	    Phi = RanGen.Rndm()*(MaxPhi-MinPhi) + MinPhi;
	  }
	}
      }
    }
    // generate vertex
    double Nver = 0.;
    bool   badVertexGenerated = true;
    while (badVertexGenerated){
      RxzV = sqrt(RanGen.Rndm())*SurfaceRadius;
      PhiV = RanGen.Rndm()*TwoPi;
      // check phi range (for a sphere with Target3dRadius around the target)
      double dPhi = Pi; if (RxzV > Target3dRadius) dPhi = asin(Target3dRadius/RxzV);
      double rotPhi = PhiV + Pi; if (rotPhi > TwoPi) rotPhi -= TwoPi;
      double disPhi = fabs(rotPhi - Phi); if (disPhi > Pi) disPhi = TwoPi - disPhi;
      if (disPhi < dPhi) badVertexGenerated = false;
      Nver+=1.;
    }
    Ngen += (Nver-1.); 
    // complete event at surface
    int                             id =  13; // mu-
    if (RanGen.Rndm() < ChargeFrac) id = -13; // mu+
    double absMom = sqrt(E*E - MuonMass*MuonMass);
    double verMom = absMom*cos(Theta);
    double horMom = absMom*sin(Theta);
    double Px = horMom*sin(Phi); // [GeV/c]
    double Py = -verMom;         // [GeV/c]
    double Pz = horMom*cos(Phi); // [GeV/c]
    double Vx = RxzV*sin(PhiV);  // [mm]
    double Vy = SurfaceOfEarth;  // [mm]
    double Vz = RxzV*cos(PhiV);  // [mm]
    double T0 = (RanGen.Rndm()*(MaxT0-MinT0) + MinT0)*SpeedOfLight; // [mm/c];
    OneMuoEvt.create(id, Px, Py, Pz, E, MuonMass, Vx, Vy, Vz, T0);      
    // if angles are ok, propagate to target
    if (goodOrientation()) OneMuoEvt.propagate(ElossScaleFactor);
    if (OneMuoEvt.hitTarget() && OneMuoEvt.e() > MinE){
      Nsel+=1.;
      notSelected = false;
    }
  }
  // plot variables of selected events
#if ROOT_INTERACTIVE
  ene->Fill(OneMuoEvt.e());
  the->Fill((OneMuoEvt.theta()*Rad2Deg));
  phi->Fill((OneMuoEvt.phi()*Rad2Deg));
  ver->Fill((OneMuoEvt.vz()/1000.),(OneMuoEvt.vx()/1000.),(OneMuoEvt.vy()/1000.));
#endif
  if (Debug){
    cout << "new event" << endl;
    cout << "  Px,Py,Pz,E,m = " << OneMuoEvt.px() << ", " << OneMuoEvt.py() << ", "
         << OneMuoEvt.pz() << ", " << OneMuoEvt.e() << ", " << OneMuoEvt.m() << " GeV" << endl;
    cout << "  Vx,Vy,Vz,t0  = " << OneMuoEvt.vx() << ", " << OneMuoEvt.vy() << ", " 
         << OneMuoEvt.vz() << ", " << OneMuoEvt.t0() << " mm" << endl;
  }
  if (EventDisplay) displayEv();
}

void CosmicMuonGenerator::terminate(){
  if (NumberOfEvents > 0){
    cout << endl;
    cout << "*********************************************************" << endl;
    cout << "*********************************************************" << endl;
    cout << "***                                                   ***" << endl;
    cout << "***    C O S M I C   M U O N   S T A T I S T I C S    ***" << endl;
    cout << "***                                                   ***" << endl;
    cout << "*********************************************************" << endl;
    cout << "*********************************************************" << endl;
    cout << endl;
    cout << "       number of generated events:  " << int(Nsel) << endl;
    double selEff = Nsel/Ngen; // selection efficiency
    cout << "       event selection efficiency:  " << selEff*100. << "%" << endl;
    cout << endl;
    cout << "       energy range:  " << MinE             << " ... " << MaxE << " GeV" << endl;
    cout << "       theta  range:  " << MinTheta*Rad2Deg << " ... " << MaxTheta*Rad2Deg << " deg" << endl;
    cout << "       phi    range:  " << MinPhi*Rad2Deg   << " ... " << MaxPhi*Rad2Deg << " deg" << endl;
    cout << "       time   range:  " << MinT0            << " ... " << MaxT0 << " ns" << endl;
    cout << "       energy  loss:  " << ElossScaleFactor*100. << "%" << endl;
    cout << endl;
    double area = 0.01*Pi*SurfaceRadius*SurfaceRadius; // area on surface [cm^2] 
    EventRate= SumIntegrals*(MaxPhi-MinPhi)*area*selEff; // rate
    double rateErr = EventRate/sqrt(Nsel);  // rate error
    cout << "       rate is " << EventRate << "+-" << rateErr << " muons per second" << endl;
    cout << "       number of events corresponds to " << Nsel/EventRate << " s" << endl;
    cout << endl;
    cout << "*********************************************************" << endl;
    cout << "*********************************************************" << endl;
  }
}

void CosmicMuonGenerator::checkIn(){
  if (MinE < 0.){ NumberOfEvents = 0;
    cout << "  CMG-ERR: min.energy is out of range (0 GeV ... inf]" << endl << endl; }
  if (MaxE < 0.){ NumberOfEvents = 0;
    cout << "  CMG-ERR: max.energy is out of range (0 GeV ... inf]" << endl << endl; }
  if (MaxE <= MinE){ NumberOfEvents = 0;
    cout << "  CMG-ERR: max.energy is not greater than min.energy" << endl << endl; }
  if (MinTheta < 0.){ NumberOfEvents = 0;
    cout << "  CMG-ERR: min.theta is out of range [0 deg ... 90 deg)" << endl << endl; }
  if (MaxTheta < 0.){ NumberOfEvents = 0;
    cout << "  CMG-ERR: max.theta is out of range [0 deg ... 90 deg)" << endl << endl; }
  if (MaxTheta <= MinTheta){ NumberOfEvents = 0;
    cout << "  CMG-ERR: max.theta is not greater than min.theta" << endl << endl; }
  if (MinPhi < 0.){ NumberOfEvents = 0;
    cout << "  CMG-ERR: min.phi is out of range [0 deg ... 360 deg]" << endl << endl; }
  if (MaxPhi < 0.){ NumberOfEvents = 0;
    cout << "  CMG-ERR: max.phi is out of range [0 deg ... 360 deg]" << endl << endl; }
  if (MaxPhi <= MinPhi){ NumberOfEvents = 0;
    cout << "  CMG-ERR: max.phi is not greater than min.phi" << endl << endl; }
  if (MaxT0 <= MinT0){ NumberOfEvents = 0;
    cout << "  CMG-ERR: max.t0 is not greater than min.t0" << endl << endl; }
  if (ElossScaleFactor < 0.){ NumberOfEvents = 0;
    cout << "  CMG-ERR: E-loss scale factor is out of range [0 ... inf)" << endl << endl; }
}

bool CosmicMuonGenerator::goodOrientation(){
  // check angular range (for a sphere with Target3dRadius around the target)
  bool goodAngles = false;
  bool phiaccepted = false;
  bool thetaaccepted = false;
  double RxzV = sqrt(OneMuoEvt.vx()*OneMuoEvt.vx() + OneMuoEvt.vz()*OneMuoEvt.vz());
  double rVY = sqrt(RxzV*RxzV + SurfaceOfEarth*SurfaceOfEarth);
  double Phi = OneMuoEvt.phi();
  double PhiV = atan2(OneMuoEvt.vx(),OneMuoEvt.vz()) + Pi; if (PhiV > TwoPi) PhiV -= TwoPi;
  double disPhi = fabs(PhiV - Phi); if (disPhi > Pi) disPhi = TwoPi - disPhi;
  double dPhi = Pi; if (RxzV > Target3dRadius) dPhi = asin(Target3dRadius/RxzV);
  if (disPhi < dPhi) phiaccepted = true;
  double Theta = OneMuoEvt.theta();
  double ThetaV = asin(RxzV/rVY);
  double dTheta = Pi; if (rVY > Target3dRadius) dTheta = asin(Target3dRadius/rVY);
  //cout << "    dPhi = " <<   dPhi << "  (" <<   Phi << " <p|V> " <<   PhiV << ")" << endl;
  //cout << "  dTheta = " << dTheta << "  (" << Theta << " <p|V> " << ThetaV << ")" << endl;
  if (fabs(Theta-ThetaV) < dTheta) thetaaccepted = true;
  if (phiaccepted && thetaaccepted) goodAngles = true;
  return goodAngles;
}

void CosmicMuonGenerator::initEvDis(){
#if ROOT_INTERACTIVE
  float rCMS = RadiusCMS/1000.;
  float zCMS = Z_DistCMS/1000.;
  TH2F* disXY = new TH2F("disXY","X-Y view",160,-rCMS,rCMS,160,-rCMS,rCMS);
  TH2F* disZY = new TH2F("disZY","Z-Y view",150,-zCMS,zCMS,160,-rCMS,rCMS);
  gStyle->SetPalette(1,0);
  gStyle->SetMarkerColor(1);
  gStyle->SetMarkerSize(1.5);
  TCanvas *disC = new TCanvas("disC","Cosmic Muon Event Display",0,0,800,410);
  disC->Divide(2,1);
  disC->cd(1);
  gPad->SetTicks(1,1);
  disXY->SetMinimum(log10(MinE));
  disXY->SetMaximum(log10(MaxE));
  disXY->GetXaxis()->SetLabelSize(0.05);
  disXY->GetXaxis()->SetTitleSize(0.05);
  disXY->GetXaxis()->SetTitleOffset(1.0);
  disXY->GetXaxis()->SetTitle("X [m]");
  disXY->GetYaxis()->SetLabelSize(0.05);
  disXY->GetYaxis()->SetTitleSize(0.05);
  disXY->GetYaxis()->SetTitleOffset(0.8);
  disXY->GetYaxis()->SetTitle("Y [m]");
  disC->cd(2);
  gPad->SetGrid(1,1);
  gPad->SetTicks(1,1);
  disZY->SetMinimum(log10(MinE));
  disZY->SetMaximum(log10(MaxE));
  disZY->GetXaxis()->SetLabelSize(0.05);
  disZY->GetXaxis()->SetTitleSize(0.05);
  disZY->GetXaxis()->SetTitleOffset(1.0);
  disZY->GetXaxis()->SetTitle("Z [m]");
  disZY->GetYaxis()->SetLabelSize(0.05);
  disZY->GetYaxis()->SetTitleSize(0.05);
  disZY->GetYaxis()->SetTitleOffset(0.8);
  disZY->GetYaxis()->SetTitle("Y [m]");
#endif
}

void CosmicMuonGenerator::displayEv(){
#if ROOT_INTERACTIVE
  disXY->Reset();
  disZY->Reset();
  TMarker* InteractionPoint = new TMarker(0.,0.,2);
  TArc* r8m = new TArc(0.,0.,(RadiusCMS/1000.));
  TLatex* logEaxis = new TLatex(); logEaxis->SetTextSize(0.05);
  float energy = float(OneMuoEvt.e());
  float verX = float(OneMuoEvt.vx()/1000.); // [m]
  float verY = float(OneMuoEvt.vy()/1000.); // [m]
  float verZ = float(OneMuoEvt.vz()/1000.); // [m]
  float dirX = float(OneMuoEvt.px())/fabs(OneMuoEvt.py());
  float dirY = float(OneMuoEvt.py())/fabs(OneMuoEvt.py());
  float dirZ = float(OneMuoEvt.pz())/fabs(OneMuoEvt.py());
  float yStep = disXY->GetYaxis()->GetBinWidth(1);
  int   NbinY = disXY->GetYaxis()->GetNbins();
  for (int iy=0; iy<NbinY; ++iy){
    verX += dirX*yStep;
    verY += dirY*yStep;
    verZ += dirZ*yStep;
    float rXY = sqrt(verX*verX + verY*verY)*1000.; // [mm]
    float absZ = fabs(verZ)*1000.;                 // [mm]
    if (rXY < RadiusCMS && absZ < Z_DistCMS){
      disXY->Fill(verX,verY,log10(energy));
      disZY->Fill(verZ,verY,log10(energy));
      disC->cd(1); disXY->Draw("COLZ"); InteractionPoint->Draw("SAME"); r8m->Draw("SAME");
      logEaxis->DrawLatex((0.65*RadiusCMS/1000.),(1.08*RadiusCMS/1000.),"log_{10}E(#mu^{#pm})");
      disC->cd(2); disZY->Draw("COL"); InteractionPoint->Draw("SAME");
      gPad->Update();
    }
  }
#endif
}

void CosmicMuonGenerator::setNumberOfEvents(unsigned int N){ if (NotInitialized) NumberOfEvents = N; }

void CosmicMuonGenerator::setRanSeed(unsigned int N){ if (NotInitialized) RanSeed = N; }

void CosmicMuonGenerator::setMinE(double E){ if (NotInitialized) MinE = E; };

void CosmicMuonGenerator::setMaxE(double E){ if (NotInitialized) MaxE = E; };

void CosmicMuonGenerator::setMinTheta(double Theta){ if (NotInitialized) MinTheta = Theta*Deg2Rad; };

void CosmicMuonGenerator::setMaxTheta(double Theta){ if (NotInitialized) MaxTheta = Theta*Deg2Rad; };

void CosmicMuonGenerator::setMinPhi(double Phi){ if (NotInitialized) MinPhi = Phi*Deg2Rad; };

void CosmicMuonGenerator::setMaxPhi(double Phi){ if (NotInitialized) MaxPhi = Phi*Deg2Rad; };

void CosmicMuonGenerator::setMinT0(double T0){ if (NotInitialized) MinT0 = T0; };

void CosmicMuonGenerator::setMaxT0(double T0){ if (NotInitialized) MaxT0 = T0; };

void CosmicMuonGenerator::setElossScaleFactor(double ElossScaleFact){ if (NotInitialized) ElossScaleFactor = ElossScaleFact; };

double CosmicMuonGenerator::getRate(){ return EventRate; }
