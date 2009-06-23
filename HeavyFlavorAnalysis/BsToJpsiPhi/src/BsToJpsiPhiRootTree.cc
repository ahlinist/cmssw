#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiRootTree.h"

BsToJpsiPhiRootTree::BsToJpsiPhiRootTree(const std::string filename)
{
	//reset all values

	// data particles
	bs_.reset();
	phi_.reset();
	jpsi_.reset();
	kplus_.reset();
	kminus_.reset();
	muplus_.reset();
	muminus_.reset();
	
	// sim particles
	bsSim_.reset();
	phiSim_.reset();
	jpsiSim_.reset();
	kplusSim_.reset();
	kminusSim_.reset();
	muplusSim_.reset();
	muminusSim_.reset();
	
	// vertices
	pv_.reset();
	sv_.reset();

	// sim vertices
	pvSim_.reset();
	svSim_.reset();
	
	// decay length and decay time
	l2d_.reset();
	l3d_.reset();
	tau2d_.reset();
	tau3d_.reset();

	// sim decay length and sim decay time
	l2dSim_.reset();
	l3dSim_.reset();
	tau2dSim_.reset();
	tau3dSim_.reset();
	
	chi2_ = 0;
	ndof_ = 0;
	prob_ = 0;
	angleBsDecayLength_ = 0;

        MuMuInvMass_ =0;
        KKInvMass_ =0;
        JpsiPhiInvMass_ =0;
        angle_costheta_ =0;
        angle_phi_ =0;
        angle_cospsi_ =0;

	triggerbit_HLTmu3_ =0;
	triggerbit_HLTmu5_ =0;
	triggerbit_HLTdoubleIsoMu3_ =0;
	triggerbit_HLTdoubleMu3_ =0;
	triggerbit_HLTdoubleMu3_JPsi_ =0;
	
	flagKstar_ =-10;
	flagKs_ =-10;
	flagPhi_ =-10;

	isMatched_ = false;
	
	// open root file
  	bsFile_ = new TFile (filename.c_str(), "RECREATE" );
	int bufsize = 64000;
	// create tree structure
	bsTree_ = new TTree("BsTree","BsTree",bufsize);
	
	// particles
	bsTree_->Branch("Bs", &bs_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("phi", &phi_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("Jpsi", &jpsi_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("muplus", &muplus_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("muminus", &muminus_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("Kplus", &kplus_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("Kminus", &kminus_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	
	// sim particles
	bsTree_->Branch("sim_Bs", &bsSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("sim_phi", &phiSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("sim_Jpsi", &jpsiSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("sim_muplus", &muplusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("sim_muminus", &muminusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("sim_Kplus", &kplusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("sim_Kminus", &kminusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	
	// vertices
	bsTree_->Branch("PV", &pv_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");
	bsTree_->Branch("SV", &sv_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");

	// sim vertices
	bsTree_->Branch("sim_PV", &pvSim_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");
	bsTree_->Branch("sim_SV", &svSim_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");
	
	// decay lenth and time
	bsTree_->Branch("l2d", &l2d_, "l/D:dl/D");
	bsTree_->Branch("l3d", &l3d_, "l/D:dl/D");
	bsTree_->Branch("tau2d", &tau2d_, "time/D:dtime/D");
	bsTree_->Branch("tau3d", &tau3d_, "time/D:dtime/D");

	// sim decay length and sim decay time
	bsTree_->Branch("sim_l2d", &l2dSim_, "l/D:dl/D");
	bsTree_->Branch("sim_l3d", &l3dSim_, "l/D:dl/D");
	bsTree_->Branch("sim_tau2d", &tau2dSim_, "time/D:dtime/D");
	bsTree_->Branch("sim_tau3d", &tau3dSim_, "time/D:dtime/D");
	
	bsTree_->Branch("chi2", &chi2_, "chi2/D");
	bsTree_->Branch("ndof", &ndof_, "ndof/I");
	bsTree_->Branch("prob", &prob_, "prob/D");
	bsTree_->Branch("angleBsDecayLength", &angleBsDecayLength_, "angleBsDecayLength/D");
	bsTree_->Branch("isMatched", &isMatched_, "isMatched/B");
	bsTree_->Branch("MuMuInvMass", &MuMuInvMass_, "MuMuInvMass/D");
	bsTree_->Branch("KKInvMass", &KKInvMass_, "KKInvMass/D");
	bsTree_->Branch("JpsiPhiInvMass", &JpsiPhiInvMass_, "JpsiPhiInvMass/D");
	bsTree_->Branch("costheta", &angle_costheta_, "angle_costheta/D");
	bsTree_->Branch("phi", &angle_phi_, "angle_phi/D");
	bsTree_->Branch("cospsi", &angle_cospsi_, "angle_cospsi/D");
	bsTree_->Branch("triggerbit_HLTmu3", &triggerbit_HLTmu3_, "triggerbit_HLTmu3/I");
	bsTree_->Branch("triggerbit_HLTmu5", &triggerbit_HLTmu5_, "triggerbit_HLTmu5/I");
	bsTree_->Branch("triggerbit_HLTdoubleIsoMu3", &triggerbit_HLTdoubleIsoMu3_, "triggerbit_HLTdoubleIsoMu3/I");
	bsTree_->Branch("triggerbit_HLTdoubleMu3", &triggerbit_HLTdoubleMu3_, "triggerbit_HLTdoubleMu3/I");
	bsTree_->Branch("triggerbit_HLTdoubleMu3_JPsi", &triggerbit_HLTdoubleMu3_JPsi_, "triggerbit_HLTdoubleMu3_JPsi/I");
	bsTree_->Branch("flagKstar", &flagKstar_, "flagKstar/I");
	bsTree_->Branch("flagKs", &flagKs_, "flagKs/I");
	bsTree_->Branch("flagPhi", &flagPhi_, "flagPhi/I");

}

BsToJpsiPhiRootTree::~BsToJpsiPhiRootTree()
{
  bsFile_->Write();
  bsFile_->Close();
  
  // 	delete bsTree_;
  // 	delete bsFile_;
}

void BsToJpsiPhiRootTree::resetEntries()
{
  // data particles
  bs_.reset();
  phi_.reset();
  jpsi_.reset();
  kplus_.reset();
  kminus_.reset();
  muplus_.reset();
  muminus_.reset();
  
  // sim particles
  bsSim_.reset();
  phiSim_.reset();
  jpsiSim_.reset();
  kplusSim_.reset();
  kminusSim_.reset();
  muplusSim_.reset();
  muminusSim_.reset();
  
  // vertices
  pv_.reset();
  pvSim_.reset();
  sv_.reset();
  svSim_.reset();
	
  // decay length and time
  l2d_.reset();
  l3d_.reset();
  tau2d_.reset();
  tau3d_.reset();
  l2dSim_.reset();
  l3dSim_.reset();
  tau2dSim_.reset();
  tau3dSim_.reset();
  
  chi2_ = 0;
  ndof_ = 0;
  prob_ = 0;
  angleBsDecayLength_ = 0;
  MuMuInvMass_ =0;
  KKInvMass_ =0;
  JpsiPhiInvMass_ =0;

  angle_costheta_ = 0;
  angle_phi_ = 0;
  angle_cospsi_ = 0;
  
  triggerbit_HLTmu3_ =0;
  triggerbit_HLTmu5_ =0;
  triggerbit_HLTdoubleIsoMu3_ =0;
  triggerbit_HLTdoubleMu3_ =0;
  triggerbit_HLTdoubleMu3_JPsi_ =0;

  flagKstar_ =-10;
  flagKs_ =-10;
  flagPhi_ =-10;
  
  isMatched_ = false;
} 

//
// adds reconstucted Bs decay to the tree
//
void BsToJpsiPhiRootTree::addDecay(const RefCountedKinematicTree& myBsTree, const reco::Vertex * myPV)
{
  
  myBsTree->movePointerToTheTop();
  
  //We are now at the top of the decay tree getting the B_s reconstructed KinematicPartlcle
  RefCountedKinematicParticle b_s = myBsTree->currentParticle();
	
  // set fit properties
  chi2_=b_s->chiSquared();
  ndof_=(int)b_s->degreesOfFreedom();
  prob_ = TMath::Prob(b_s->chiSquared(), (int)b_s->degreesOfFreedom());
  
  // set the bs properties (px,py,pz,e,pt,eta,phi,m)
  bs_.set(b_s);
  
  // bs decay vertex
  RefCountedKinematicVertex bVertex = myBsTree->currentDecayVertex();

  // set the bs decay vertex values (x,y,z,dxx,dyy,dzz,dxy,dyz,dzx)
  sv_.set(bVertex);

  // get pv and calculate decay length and time
  if(myPV != 0){
    // set primary vertex values (x,y,z,dxx,dyy,dzz,dxy,dyz,dzx)
    pv_.set(*myPV);

    VertexDistance3D vdist3d;
    Measurement1D dist3d = vdist3d.distance(bVertex->vertexState(), *myPV);
    // 3D proper decay length (l,dl)
    l3d_.set(dist3d);

    VertexDistanceXY vdistXY;
    Measurement1D distXY = vdistXY.distance(bVertex->vertexState(), *myPV);
    // 2D proper decay length (l,dl)
    l2d_.set(distXY);

    // 3D proper decay time
    tau3d_.set(l3d_, bs_.m, sqrt(bs_.pt * bs_.pt + bs_.pz * bs_.pz));
    // 2D proper decay time
    tau2d_.set(l2d_, bs_.m, bs_.pt);

  } else { 
    cout << "no primary vertex found!" << endl; 
  }

  TLorentzVector pbs;
  pbs.SetXYZM(bs_.px,bs_.py,bs_.pz,bs_.m);
  
  // Fill all particles
  vector< RefCountedKinematicParticle > bs_children = myBsTree->finalStateParticles();
  if(bs_children.size() == 4){
    muplus_.set(bs_children[0]);
    muminus_.set(bs_children[1]);
    kplus_.set(bs_children[2]);
    kminus_.set(bs_children[3]);	
    jpsi_ = muplus_ + muminus_;
    phi_ = kplus_ + kminus_;
  } else { 
    cout << "bs had " << bs_children.size() << " children" << endl; 
  }

  // Angular Variables

  TLorentzVector pmuplus;
  pmuplus.SetXYZM(muplus_.px,muplus_.py,muplus_.pz,muplus_.m);

  TLorentzVector pmuminus;
  pmuminus.SetXYZM(muminus_.px,muminus_.py,muminus_.pz,muminus_.m);

  TLorentzVector ptotmm = pmuplus + pmuminus;
  MuMuInvMass_ = ptotmm.M();

  TLorentzVector pkplus;
  pkplus.SetXYZM(kplus_.px,kplus_.py,kplus_.pz,kplus_.m);

  TLorentzVector pkminus;
  pkminus.SetXYZM(kminus_.px,kminus_.py,kminus_.pz,kminus_.m);

  TLorentzVector ptotkk = pkplus + pkminus;
  KKInvMass_ = ptotkk.M();

  TLorentzVector ptotjp = pmuplus + pmuminus + pkplus + pkminus;
  JpsiPhiInvMass_ = ptotjp.M();

  // boosting in JPsi restframe
  TLorentzVector pjpsi;                                                                                                           
  pjpsi = pmuplus + pmuminus;

  TLorentzVector pphi;
  pphi = pkplus + pkminus;

  // the betas for the boost
  TVector3 p3_JPsi;
  p3_JPsi = pjpsi.Vect();
  p3_JPsi *= -1./pjpsi.E();

  // the boost matrix
  TLorentzRotation boost_jpsi(p3_JPsi);
  TLorentzVector p_JPsi_JPsi;
  p_JPsi_JPsi = boost_jpsi.VectorMultiplication(pjpsi);

  // the different momenta in the new frame                                                                                                       
  TLorentzVector p_JPsi_muplus;
  TLorentzVector p_JPsi_muminus; 
  TLorentzVector p_JPsi_Kplus;
  TLorentzVector p_JPsi_Kminus;
  TLorentzVector p_JPsi_phi;                                                                       
  TLorentzVector p_JPsi_Bs;   

  p_JPsi_muplus = boost_jpsi.VectorMultiplication(pmuplus);                                                                      
  p_JPsi_muminus = boost_jpsi.VectorMultiplication(pmuminus);                                                                           
  p_JPsi_Kplus = boost_jpsi.VectorMultiplication(pkplus);                                                                                              
  p_JPsi_Kminus = boost_jpsi.VectorMultiplication(pkminus);                                                                                     
  p_JPsi_phi = boost_jpsi.VectorMultiplication(pphi);
  p_JPsi_Bs = boost_jpsi.VectorMultiplication(pbs);

  // the 3-momenta

  TVector3 p3_JPsi_muplus;
  p3_JPsi_muplus = p_JPsi_muplus.Vect();
  TVector3 p3_JPsi_muminus;
  p3_JPsi_muminus = p_JPsi_muminus.Vect();

  TVector3 p3_JPsi_Kplus;
  p3_JPsi_Kplus = p_JPsi_Kplus.Vect();
  TVector3 p3_JPsi_Kminus;
  p3_JPsi_Kminus = p_JPsi_Kminus.Vect();

  TVector3 p3_JPsi_phi;
  p3_JPsi_phi = p_JPsi_phi.Vect();
  TVector3 p3_JPsi_Bs;
  p3_JPsi_Bs = p_JPsi_Bs.Vect();
  
  // coordinate system

  TVector3 x,y,z;
  x = p3_JPsi_phi.Unit();
  y = p3_JPsi_Kplus.Unit() - (x * (x * p3_JPsi_Kplus.Unit()));
  y = y.Unit();
  z = x.Cross(y);

  // Transversity Basis

  angle_costheta_ = p3_JPsi_muplus.Unit() * z;

  double cos_phi = p3_JPsi_muplus.Unit() * x / TMath::Sqrt(1 - angle_costheta_*angle_costheta_);
  double sin_phi = p3_JPsi_muplus.Unit() * y / TMath::Sqrt(1 - angle_costheta_*angle_costheta_);
  angle_phi_ = TMath::ACos(cos_phi);
  if (sin_phi < 0){
    angle_phi_ =  -angle_phi_;
  }

  // boosting in phi restframe                                                                                                          
  // the betas for the boost
  
  TVector3 p3_phi;
  p3_phi = pphi.Vect();
  p3_phi *= -1./pphi.E();
  
  // the boost matrix
  
  TLorentzRotation boost_phi(p3_phi);
  TLorentzVector p_phi_phi;
  p_phi_phi = boost_phi.VectorMultiplication(pphi);
  
  // the different momenta in the new frame
  
  TLorentzVector p_phi_Kplus;
  TLorentzVector p_phi_JPsi;
  TLorentzVector p_phi_Bs;
  
  p_phi_Kplus = boost_phi.VectorMultiplication(pkplus);
  p_phi_JPsi = boost_phi.VectorMultiplication(pjpsi);
  p_phi_Bs = boost_phi.VectorMultiplication(pbs);
  
  // the 3-momenta
  
  TVector3 p3_phi_Kplus;
  p3_phi_Kplus = p_phi_Kplus.Vect();
  TVector3 p3_phi_JPsi;
  p3_phi_JPsi = p_phi_JPsi.Vect();
  TVector3 p3_phi_Bs;
  p3_phi_Bs = p_phi_Bs.Vect();
  
  angle_cospsi_ = -1 * p3_phi_Kplus.Unit() * p3_phi_JPsi.Unit();

  
  // set cos of angle between bs momentum and decay length
  angleBsDecayLength_ = ((sv_.x - pv_.x) * bs_.px + (sv_.y - pv_.y) * bs_.py + (sv_.z - pv_.z) * bs_.pz) / sqrt( ((sv_.x - pv_.x) * (sv_.x - pv_.x) + (sv_.y - pv_.y) * (sv_.y - pv_.y) + (sv_.z - pv_.z) * (sv_.z - pv_.z)) * (bs_.px *  bs_.px + bs_.py *  bs_.py + bs_.pz *  bs_.pz));
}

//
// adds simulated Bs decay to the tree
//	
void BsToJpsiPhiRootTree::addSimDecay(const TrackingVertex * simVertex, const TrackingVertex * pVertex)
{
  //fill muons an kaons
  for (TrackingVertex::tp_iterator dt = simVertex->daughterTracks_begin(); dt!= simVertex->daughterTracks_end(); ++dt) {
    if((**dt).pdgId() == -13){
      muplusSim_.set(**dt);
    }
    if((**dt).pdgId() == 13){
      muminusSim_.set(**dt);
    }
    if((**dt).pdgId() == 321){
      kplusSim_.set(**dt);
    }
    if((**dt).pdgId() == -321){
      kminusSim_.set(**dt);
    }
  }
			
  // fill the mother particles
  jpsiSim_ = muplusSim_ + muminusSim_;
  phiSim_ = kplusSim_ + kminusSim_;
  bsSim_ = jpsiSim_ + phiSim_;
  
  // fill the vertices and the decay length and time
  pvSim_.set(pVertex->position());
  svSim_.set(simVertex->position());
  double l2d = sqrt( (pVertex->position().x() - simVertex->position().x()) * (pVertex->position().x() - simVertex->position().x()) + (pVertex->position().y() - simVertex->position().y()) * (pVertex->position().y() - simVertex->position().y()) );
  double l3d = sqrt( (pVertex->position().x() - simVertex->position().x()) * (pVertex->position().x() - simVertex->position().x()) + (pVertex->position().y() - simVertex->position().y()) * (pVertex->position().y() - simVertex->position().y()) + (pVertex->position().z() - simVertex->position().z()) * (pVertex->position().z() - simVertex->position().z()) );
  l2dSim_.set(l2d, 0);
  l3dSim_.set(l3d, 0);
  tau2dSim_.set(l2dSim_,bs_.m,bs_.pt);
  tau3dSim_.set(l3dSim_,bs_.m,bs_.pt*bs_.pt + bs_.pz*bs_.pz);		
  
}

//
// is the reconstructed decay consistent with a simulated?
// 
void BsToJpsiPhiRootTree::setAssociation(const bool association)
{
  cout << "is matched: " << association << endl;
  isMatched_ = association;
}

bool BsToJpsiPhiRootTree::getAssociation()
{
  return isMatched_;
}

void BsToJpsiPhiRootTree::getTrigBit(const int flag_1, const int flag_2, const int flag_3, const int flag_4, const int flag_5)
{
  triggerbit_HLTmu3_ = flag_1;
  triggerbit_HLTmu5_ = flag_2;
  triggerbit_HLTdoubleIsoMu3_ = flag_3;
  triggerbit_HLTdoubleMu3_ = flag_4;
  triggerbit_HLTdoubleMu3_JPsi_ = flag_5;

}

void BsToJpsiPhiRootTree::getBdFlags(const int bdjpsikstar, const int bdjpsiks, const int bdjpsiphi)
{
  cout << "bdjpsiphi flag while filling: " << bdjpsiphi << endl;
  cout << "bdjpsikstar flag while filling: " << bdjpsikstar << endl;
  cout << "bdjpsiks flag while filling: " << bdjpsiks << endl;
  flagPhi_ = bdjpsiphi;
  flagKstar_ = bdjpsikstar;
  flagKs_ = bdjpsiks;
}

void BsToJpsiPhiRootTree::fill()
{
  bsTree_->Fill();
}
