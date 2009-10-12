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

	void getMCmatch(const int aa);

 
	void getAngles(const double aa, const double bb, const double cc, const double dd);
	void getVtx(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg, 
		    const double hh, const double ii);
	void getLXY(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg);
	void getInfoK1(const int aa, const int bb, const int cc, const int dd);
	void getInfoK2(const int aa, const int bb, const int cc, const int dd);
	void getInfoMu1(const int aa, const int bb, const int cc, const int dd);
	void getInfoMu2(const int aa, const int bb, const int cc, const int dd);
	void get3d(const double aa, const double bb, const double cc, const double dd);
	void get1d(const double aa, const double bb, const double cc, const double dd);
	void getDeDx(const double f1, const double f2, const int f3);
        void setFitParKK(RefCountedKinematicTree& myTree);
        void setFitParKpi(RefCountedKinematicTree& myTree);
        void setFitParpipi(RefCountedKinematicTree& myTree);
	void fill();  //!< copy the information from memory to Ntuple

public:

        int triggerbit_HLTmu3_;
        int triggerbit_HLTmu5_;
        int triggerbit_HLTmu9_;
        int triggerbit_HLTdoubleIsoMu3_;
        int triggerbit_HLTdoubleMu3_;
        int triggerbit_HLTdoubleMu3_JPsi_;
	
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
        double BsPhi_before_;
        double BsEta_before_;
        double BsPt_before_;
        double BsPz_before_;

        double JpsiMass_before_;
        double JpsiPhi_before_;
        double JpsiEta_before_;
        double JpsiPt_before_;
        double JpsiPz_before_;

        double PhiMass_before_;
        double PhiPhi_before_;
        double PhiEta_before_;
        double PhiPt_before_;
        double PhiPz_before_;

        double  K1Pt_before_;
        double  K1Pz_before_;
        double  K1Eta_before_;
        double  K1Phi_before_;
        double  K2Eta_before_;
        double  K2Pt_before_;
        double  K2Pz_before_;
        double  K2Phi_before_;

        double chi2_Bs_;
        int ndof_Bs_;

        double BsVtxProb_;
        double BsVtxProbKpi_;
        double BsVtxProbpipi_;

        double BfitM_KK_;
        double BfitM_Kpi_;
        double BfitM_pipi_;

	double BsVtx_x_;
	double BsVtx_y_;
	double BsVtx_z_;

        double BsMass_after_;
        double BsPhi_after_;
        double BsEta_after_;
        double BsPt_after_;
        double BsPz_after_;

        double JpsiMass_after_;
        double JpsiPhi_after_;
        double JpsiEta_after_;
        double JpsiPt_after_;
        double JpsiPz_after_;

        double PhiMass_after_;
        double PhiPhi_after_;
        double PhiEta_after_;
        double PhiPt_after_;
        double PhiPz_after_;

        double  K1Pt_after_;
        double  K1Pz_after_;
        double  K1Eta_after_;
        double  K1Phi_after_;
        double  K2Eta_after_;
        double  K2Pt_after_;
        double  K2Pz_after_;
        double  K2Phi_after_;

        double  K1Chi2_;
        int     K1nHits_;
        double  K2Chi2_;
        int     K2nHits_;
        double  K1pixH_;
        int     K1trkH_;
        int     K2pixH_;
        int     K2trkH_;

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

	double BLxy_;
	double BLxy2_;
	double BerrX_;
	double BerrY_;
	double BerrXY_;
	double Bsct1_;
	double Bsct2_;

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

	double dedxTrk_;
	double errdedxTrk_;
	int numdedxTrk_;

	int iPassedCutIdent_;

	//MC info
	int isGenBsEvent_;
	int GenNumberOfBsDaughters_;
	int isGenBsJpsiPhiEvent_;
	int isGenBsJpsiPhiMuMuKKEvent_;
	int isGenBdEvent_;
	int GenNumberOfBdDaughters_;
	int isGenBdJpsiKstarEvent_;
	int isGenBdJpsiKstarMuMuKpiEvent_;

	double genBsM_, genBsPt_, genBsEta_, genBsPhi_;
	double genJpsiM_, genJpsiPt_, genJpsiEta_, genJpsiPhi_;
	double genPhiM_, genPhiPt_, genPhiEta_, genPhiPhi_;
	double genMu1M_, genMu1Pt_, genMu1Eta_, genMu1Phi_;
	double genMu2M_, genMu2Pt_, genMu2Eta_, genMu2Phi_;
	double genK1M_, genK1Pt_, genK1Eta_, genK1Phi_;	
	double genK2M_, genK2Pt_, genK2Eta_, genK2Phi_;	
	double genBsPz_, genJpsiPz_, genPhiPz_, genMu1Pz_, genMu2Pz_, genK1Pz_, genK2Pz_;

        double K1_kk_par0_;
        double K1_kk_par1_;
        double K1_kk_par2_;
        double K1_kk_par3_;
        double K1_kk_par4_;
        double K1_kk_par5_;
        double K1_kk_par6_;

        double K2_kk_par0_;
        double K2_kk_par1_;
        double K2_kk_par2_;
        double K2_kk_par3_;
        double K2_kk_par4_;
        double K2_kk_par5_;
        double K2_kk_par6_;

        double K1_kpi_par0_;
        double K1_kpi_par1_;
        double K1_kpi_par2_;
        double K1_kpi_par3_;
        double K1_kpi_par4_;
        double K1_kpi_par5_;
        double K1_kpi_par6_;

	double K2_kpi_par0_;
        double K2_kpi_par1_;
        double K2_kpi_par2_;
        double K2_kpi_par3_;
        double K2_kpi_par4_;
        double K2_kpi_par5_;
        double K2_kpi_par6_;

        double K1_pipi_par0_;
        double K1_pipi_par1_;
        double K1_pipi_par2_;
        double K1_pipi_par3_;
        double K1_pipi_par4_;
        double K1_pipi_par5_;
        double K1_pipi_par6_;

        double K2_pipi_par0_;
        double K2_pipi_par1_;
        double K2_pipi_par2_;
        double K2_pipi_par3_;
        double K2_pipi_par4_;
        double K2_pipi_par5_;
        double K2_pipi_par6_;

        double K1_kk_sigX_;
        double K1_kk_sigY_;
        double K1_kk_sigZ_;

        double K1_kpi_sigX_;
        double K1_kpi_sigY_;
        double K1_kpi_sigZ_;

        double K1_pipi_sigX_;
        double K1_pipi_sigY_;
        double K1_pipi_sigZ_;

        double K2_kk_sigX_;
        double K2_kk_sigY_;
        double K2_kk_sigZ_;

        double K2_kpi_sigX_;
        double K2_kpi_sigY_;
        double K2_kpi_sigZ_;

        double K2_pipi_sigX_;
        double K2_pipi_sigY_;
        double K2_pipi_sigZ_;

        double K1_kk_sigPX_;
        double K1_kk_sigPY_;
        double K1_kk_sigPZ_;

        double K1_kpi_sigPX_;
        double K1_kpi_sigPY_;
        double K1_kpi_sigPZ_;

        double K1_pipi_sigPX_;
        double K1_pipi_sigPY_;
        double K1_pipi_sigPZ_;

        double K2_kk_sigPX_;
        double K2_kk_sigPY_;
        double K2_kk_sigPZ_;

        double K2_kpi_sigPX_;
        double K2_kpi_sigPY_;
        double K2_kpi_sigPZ_;

        double K2_pipi_sigPX_;
        double K2_pipi_sigPY_;
        double K2_pipi_sigPZ_;

	double K1Pt_error_;
	double K2Pt_error_;

	int isGenBsJpsiKKEvent_;
	int BdDausId_;

	TFile* bsFile_;
	TTree* bsTree_; 
};

#endif

