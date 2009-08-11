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
	
	void getTrigBit(const int flag_1, const int flag_2, const int flag_3, const int flag_4, const int flag_5, const int flag_6);
	void getBdFlags(const int bdjpsikstar, const int bdjpsiks, const int bdjpsiphi);
	void getParams(const int nBsDau, const int momID, const int dau_1_ID, const int dau_2_ID);
	void getPar(const double aa, const double bb, const double cc);
	void getMCmatch(const int aa);
        void getJpsiAlone(const double aa, const int bb, const int cc, const double dd, const double ee,
                     const double ff, const double gg, const double hh, const double ii, const double ll,
                     const double mm, const double nn, const double oo, const int pp, const int qq);
        void getBs(const double aa, const int bb, const double cc, const double dd, const double ee, const double ff, const double gg, const double hh);
	void getJpsi(const double aa, const double bb, const double cc, const double dd);
	void getPhi(const double aa, const double bb, const double cc, const double dd);
	void getK1(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
		   const int hh, const int ii, const int ll, const int mm);
	void getK2(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
		   const int hh, const int ii, const int ll, const int mm);
	void getMu1(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
		    const int hh, const int ii, const int ll, const int mm);
	void getMu2(const double aa, const double bb, const double cc, const double dd, const int ee, const int ff, const int gg,
		    const int hh, const int ii, const int ll, const int mm);
	void getAngles(const double aa, const double bb, const double cc, const double dd);
	void getVtx(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg, 
		    const double hh, const double ii);
	void getLXY(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg);
	void getBdPar(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg, 
		      const double hh, const double ii);
	void getInfoK1(const int aa, const int bb, const int cc, const int dd);
	void getInfoK2(const int aa, const int bb, const int cc, const int dd);
	void getInfoMu1(const int aa, const int bb, const int cc, const int dd);
	void getInfoMu2(const int aa, const int bb, const int cc, const int dd);
	void get3d(const double aa, const double bb, const double cc, const double dd);
	void get1d(const double aa, const double bb, const double cc, const double dd);
	void getRef(const reco::TrackRef& aa);
	void getDeDx(const double f1, const double f2, const int f3);
	void fill();  //!< copy the information from memory to Ntuple

private:

        int triggerbit_HLTmu3_;
        int triggerbit_HLTmu5_;
        int triggerbit_HLTmu9_;
        int triggerbit_HLTdoubleIsoMu3_;
        int triggerbit_HLTdoubleMu3_;
        int triggerbit_HLTdoubleMu3_JPsi_;
	
	int flagKstar_;
	int flagKs_;
	int flagPhi_;
	int momID_;
	int nBsDau_;
	int dau_1_ID_;
	int dau_2_ID_;

	double	BSx_ ;
	double	BSy_ ;
	double	BSz_ ;
	double	PVx_ ;
	double	PVy_ ;
	double	PVz_ ;
	double	PVerrx_ ;
	double	PVerry_ ;
	double	PVerrz_ ;

        double JpsiVtxProb_;
        int MuCounter1_;
        int MuCounter2_;
        double JpsiM_alone_;
        double JpsiPhi_alone_;
        double JpsiEta_alone_;
        double JpsiPt_alone_;
        double JpsiMu1Pt_;
        double JpsiMu2Pt_;
        double JpsiMu1Phi_;
        double JpsiMu2Phi_;
        double JpsiMu1Eta_;
        double JpsiMu2Eta_;
        int JpsiMuon1Cat_;
        int JpsiMuon2Cat_;

        double BsMass_before_;
        double JpsiMass_before_;
        double PhiMass_before_;
        double chi2_Bs_;
        int ndof_Bs_;
        double BsVtxProb_;
        double JpsiM_;
        double JpsiPhi_;
        double JpsiEta_;
        double JpsiPt_;
        double PhiM_;
        double PhiPhi_;
        double PhiEta_;
        double PhiPt_;
        double BsM_;
        double BsPhi_;
        double BsEta_;
        double BsPt_;

        double  K1Pt_;
        double  K2Pt_;
        double  K1Eta_;
        double  K2Eta_;
        double  K1Phi_;
        double  K2Phi_;
        double  K1Chi2_;
        int     K1nHits_;
        double  K2Chi2_;
        int     K2nHits_;
        double  K1pixH_;
        int     K1trkH_;
        int     K2pixH_;
        int     K2trkH_;

        double  Mu1Pt_;
        double  Mu2Pt_;
        double  Mu1Eta_;
        double  Mu2Eta_;
        double  Mu1Phi_;
        double  Mu2Phi_;
        double  Mu1Chi2_;
        int     Mu1nHits_;
        double  Mu2Chi2_;
        int     Mu2nHits_;
        double  Mu1pixH_;
        int     Mu1trkH_;
        int     Mu2pixH_;
        int     Mu2trkH_;
	
	double costheta_;
	double phi_;
	double cospsi_;
	double AngleBsDecayLength_;

	int isMatched_;

	double BmassC_;
	double BLxy_;
	double BLxy2_;
	double BerrX_;
	double BerrY_;
	double BerrXY_;
	double Bsct1_;
	double Bsct2_;

        double BdM_;
        double BdmassC_;
        double KstarM_;
        double BdVtxProb_;
        double BdPhi_;
        double BdEta_;
        double BdPt_;
	double JpsiM_Bd_;
	double AngleBdDecayLength_;

        int     K1trkLay_;
        int     K1muDTh_;
        int     K1muCSCh_;
        int     K1muRPCh_;
        int     K2trkLay_;
        int     K2muDTh_;
        int     K2muCSCh_;
        int     K2muRPCh_;
        int     Mu1trkLay_;
        int     Mu1muDTh_;
        int     Mu1muCSCh_;
        int     Mu1muRPCh_;
        int     Mu2trkLay_;
        int     Mu2muDTh_;
        int     Mu2muCSCh_;
        int     Mu2muRPCh_;

        int K1mcId_;
        int K1momId_;
        int K1gmomId_;
        int K2mcId_;
        int K2momId_;
        int K2gmomId_;
        int Mu1mcId_;
        int Mu1momId_;
        int Mu1gmomId_;
        int Mu2mcId_;
        int Mu2momId_;
        int Mu2gmomId_;
        int K1Truth_;
        int K2Truth_;
        int Mu1Truth_;
        int Mu2Truth_;

	double Dist3d_;
	double dDist3d_;
	double Time3d_;
	double dTime3d_;
	double Dist_;
	double dDist_;
	double Time_;
	double dTime_;

	double dedx_;
	double errdedx_;
	int numdedx_;

	TFile* bsFile_;
	TTree* bsTree_; 
};

#endif
