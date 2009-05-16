#ifndef HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiRootTree_h
#define HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiRootTree_h

#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include "TROOT.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TLorentzRotation.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicVertex.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicTree.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
#include "DataFormats/Math/interface/LorentzVector.h"

class BsToJpsiPhiRootTree {
public:
  
	BsToJpsiPhiRootTree(const std::string filename = "bsTree.root");
	
	~BsToJpsiPhiRootTree();
	
	void resetEntries(); 
	
	// Fill the particles	
	void addDecay(const RefCountedKinematicTree& myBsTree, const reco::Vertex * myPV);
	void addSimDecay(const TrackingVertex * simVertex, const TrackingVertex * pVertex);
	void setAssociation(const bool association);
	void getTrigBit(const int flag_1, const int flag_2, const int flag_3, const int flag_4, const int flag_5);
	bool getAssociation();
	void fill();  //!< copy the information from memory to Ntuple

private:

	struct MyParticle {
		double px;
		double py;
		double pz;
		double e;
		double pt;
		double eta;
		double phi;
		double m;
		void reset(){px=0; py=0; pz=0; pt=0; eta=0; phi=0; m=0;}
		void set(RefCountedKinematicParticle const & kinParticle){
			px=kinParticle->currentState().globalMomentum().x(); 
			py=kinParticle->currentState().globalMomentum().y(); 
			pz=kinParticle->currentState().globalMomentum().z();
			pt=kinParticle->currentState().globalMomentum().perp(); 
			eta=kinParticle->currentState().globalMomentum().eta(); 
			phi=kinParticle->currentState().globalMomentum().phi(); 
			m=kinParticle->currentState().mass();
			e=sqrt(px*px + py*py + pz*pz + m*m); 
		}
		void set(TrackingParticle const & tp){
			px=tp.px(); 
			py=tp.py(); 
			pz=tp.pz();
			pt=tp.pt(); 
			eta=tp.eta(); 
			phi=tp.phi(); 
			m=tp.mass();
			e=tp.energy(); 
		}
		const MyParticle operator+(MyParticle const & rhs){
			MyParticle tmp;
			tmp.px = this->px + rhs.px;
			tmp.py = this->py + rhs.py;
			tmp.pz = this->pz + rhs.pz;
			tmp.e = this->e + rhs.e;
			tmp.pt = sqrt(tmp.px*tmp.px + tmp.py*tmp.py);
			tmp.eta = - log(tan(0.5 * atan2(tmp.pt, tmp.pz)));
			tmp.phi = atan2(tmp.py, tmp.px);
			tmp.m = sqrt(tmp.e*tmp.e -(tmp.px*tmp.px + tmp.py*tmp.py+ tmp.pz*tmp.pz));;
			return tmp;
		}
	};
	
	struct MyVertex {
		double x;
		double y;
		double z;
		double dxx;
		double dyy;
		double dzz;
		double dxy;
		double dyz;
		double dzx;
		void reset(){x=0; y=0; z=0; dxx=0; dyy=0; dzz=0; dxy=0; dyz=0; dzx=0;}
		void set(RefCountedKinematicVertex const & kinVertex){
			x=kinVertex->position().x();
			y=kinVertex->position().y();
			z=kinVertex->position().z();
			dxx=kinVertex->error().cxx();
			dyy=kinVertex->error().cyy();
			dzz=kinVertex->error().czz();
			dxy=kinVertex->error().cyx();
			dyz=kinVertex->error().czy();
			dzx=kinVertex->error().czx();
		}
		void set(reco::Vertex const & recoVertex){
			x=recoVertex.position().x();
			y=recoVertex.position().y();
			z=recoVertex.position().z();
			dxx=recoVertex.covariance(0,0);
			dyy=recoVertex.covariance(1,1);
			dzz=recoVertex.covariance(2,2);
			dxy=recoVertex.covariance(0,1);
			dyz=recoVertex.covariance(1,2);
			dzx=recoVertex.covariance(2,0);
		}
		void set(math::XYZTLorentzVectorD const & vertex)
		{
			x=vertex.x();
			y=vertex.y();
			z=vertex.z();
			dxx=0;
			dyy=0;
			dzz=0;
			dxy=0;
			dyz=0;
			dzx=0;
		}
	};
	
	struct MyLength {
		double length;
		double dLength;
		void reset(){length=0; dLength=0;}
		void set(const Measurement1D & dist){
			length = dist.value();
			dLength = dist.error();
		}
		void set(const double & l, const double & dl){
			length = l;
			dLength = dl;
		}
	};
	
	
	struct MyTime {
		double time;
		double dTime;
		void reset(){time=0; dTime=0;}
		//time is given in ps
		void set(const MyLength & l, const double & m, const double & p){
			time = l.length * m / p *100. /3.;
			dTime = l.dLength * m / p * 100. /3.;
		}
	};

	// data particles
	MyParticle bs_;
	MyParticle phi_;
	MyParticle jpsi_;
	MyParticle kplus_;
	MyParticle kminus_;
	MyParticle muplus_;
	MyParticle muminus_;
	
	// sim particles
	MyParticle bsSim_;
	MyParticle phiSim_;
	MyParticle jpsiSim_;
	MyParticle kplusSim_;
	MyParticle kminusSim_;
	MyParticle muplusSim_;
	MyParticle muminusSim_;
	
	// vertices
	MyVertex pv_;
	MyVertex pvSim_;
	MyVertex sv_;
	MyVertex svSim_;
	
	
	// decay length and time
	MyLength l2d_;
	MyLength l3d_;
	MyTime tau2d_;
	MyTime tau3d_;
	MyLength l2dSim_;
	MyLength l3dSim_;
	MyTime tau2dSim_;
	MyTime tau3dSim_;
	
	bool isMatched_;
	double chi2_;
	int ndof_;
	double prob_;
	double angleBsDecayLength_;
	double MuMuInvMass_;
	double KKInvMass_;
	double JpsiPhiInvMass_;
	double angle_costheta_;
	double angle_phi_;
	double angle_cospsi_;

        int triggerbit_HLTmu3_;
        int triggerbit_HLTmu5_;
        int triggerbit_HLTdoubleIsoMu3_;
        int triggerbit_HLTdoubleMu3_;
        int triggerbit_HLTdoubleMu3_JPsi_;
	
	TFile* bsFile_;
	TTree* bsTree_; 
};

#endif
