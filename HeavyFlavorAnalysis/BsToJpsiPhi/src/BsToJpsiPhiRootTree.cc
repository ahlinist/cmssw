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
	bsTree_->Branch("simBs", &bsSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("simphi", &phiSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("simJpsi", &jpsiSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("simmuplus", &muplusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("simmuminus", &muminusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("simKplus", &kplusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	bsTree_->Branch("simKminus", &kminusSim_, "px/D:py/D:pz/D:e/D:pt/D:eta/D:phi/D:m/D");
	
	// vertices
	bsTree_->Branch("PV", &pv_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");
	bsTree_->Branch("simPV", &pvSim_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");
	bsTree_->Branch("SV", &sv_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");
	bsTree_->Branch("simSV", &svSim_, "x/D:y/D:z/D:dxx/D:dyy/D:dzz/D:dxy/D:dyz/D:dzx/D");
	
	// decay lenth and time
	bsTree_->Branch("l2d", &l2d_, "l/D:dl/D");
	bsTree_->Branch("l3d", &l3d_, "l/D:dl/D");
	bsTree_->Branch("tau2d", &tau2d_, "time/D:dtime/D");
	bsTree_->Branch("tau3d", &tau3d_, "time/D:dtime/D");
	bsTree_->Branch("siml2d", &l2dSim_, "l/D:dl/D");
	bsTree_->Branch("siml3d", &l3dSim_, "l/D:dl/D");
	bsTree_->Branch("simtau2d", &tau2dSim_, "time/D:dtime/D");
	bsTree_->Branch("simtau3d", &tau3dSim_, "time/D:dtime/D");
	
	bsTree_->Branch("chi2", &chi2_, "chi2/D");
	bsTree_->Branch("ndof", &ndof_, "ndof/I");
	bsTree_->Branch("prob", &prob_, "prob/D");
	bsTree_->Branch("angleBsDecayLength", &angleBsDecayLength_, "angleBsDecayLength/D");
	bsTree_->Branch("isMatched", &isMatched_, "isMatched/B");
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
	
	// set the bs properties
	bs_.set(b_s);
	
	// The B_s decay vertex
	RefCountedKinematicVertex bVertex = myBsTree->currentDecayVertex();
	sv_.set(bVertex);
	// get pv and calculate decay length and time
	if(myPV != 0){
		pv_.set(*myPV);
		VertexDistance3D vdist3d;
		Measurement1D dist3d = vdist3d.distance(bVertex->vertexState(), *myPV);
		l3d_.set(dist3d);
		VertexDistanceXY vdistXY;
		Measurement1D distXY = vdistXY.distance(bVertex->vertexState(), *myPV);
		l2d_.set(distXY);
		tau2d_.set(l2d_, bs_.m, bs_.pt);
		tau3d_.set(l3d_, bs_.m, sqrt(bs_.pt * bs_.pt + bs_.pz * bs_.pz));
	} else { cout << "no primary vertex found!" << endl; }
	
	// Fill all particles
	vector< RefCountedKinematicParticle > bs_children = myBsTree->finalStateParticles();
	if(bs_children.size() == 4){
		muplus_.set(bs_children[0]);
		muminus_.set(bs_children[1]);
		kplus_.set(bs_children[2]);
		kminus_.set(bs_children[3]);	
		jpsi_ = muplus_ + muminus_;
		phi_ = kplus_ + kminus_;
	} else { cout << "bs had " << bs_children.size() << " children" << endl; }
	
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

void BsToJpsiPhiRootTree::fill()
{
	bsTree_->Fill();
}
