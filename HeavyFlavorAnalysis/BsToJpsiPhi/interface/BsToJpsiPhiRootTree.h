#ifndef HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiRootTree_h
#define HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiRootTree_h

#include <string>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <vector>

class BsToJpsiPhiRootTree {
public:
  
	BsToJpsiPhiRootTree();
	
	~BsToJpsiPhiRootTree();
	
	void resetEntries(); 
	void writeFile();
	void createTree(const std::string filename);

	// read tree from single file
	void readTree(const std::string filename); 

	// read tree as chain from multiple files
	void readTree(std::vector<std::string> filenames);

 
	void getAngles(const double aa, const double bb, const double cc, const double dd);
	void getVtx(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff, const double gg, 
		    const double hh, const double ii);


	void getDeDx(const double f1, const double f2, const int f3);
  
	void fill();  

	void setBranchAddresses();
public:

        int triggerbit_HLTmu3_;
        int triggerbit_HLTmu5_;
        int triggerbit_HLTmu9_;
        int triggerbit_HLTdoubleIsoMu3_;
        int triggerbit_HLTdoubleMu3_;
        int triggerbit_HLTdoubleMu3_JPsi_;
	int triggerbit_HLTdoubleMu0_;
	int triggerbit_HLTL1DoubleMuOpen_;
	int triggerbit_HLTL1MuOpen_;

	double	BSx_ ;
	double	BSy_ ;
	double	BSz_ ;
	double	PVx_ ;
	double	PVy_ ;
	double	PVz_ ;
	double	PVerrx_ ;
	double	PVerry_ ;
	double	PVerrz_ ;

	double PVx_refit_;
	double PVy_refit_;
	double PVz_refit_;
	double PVerrx_refit_;
	double PVerry_refit_;
	double PVerrz_refit_;

        double JpsiVtxProb_;
    
        double JpsiM_alone_;
        double JpsiPhi_alone_;
        double JpsiEta_alone_;
        double JpsiPt_alone_;
        double JpsiMu1Pt_alone_;
        double JpsiMu2Pt_alone_;
        double JpsiMu1Phi_alone_;
        double JpsiMu2Phi_alone_;
        double JpsiMu1Eta_alone_;
        double JpsiMu2Eta_alone_;
        int    JpsiMuon1Cat_alone_;
        int    JpsiMuon2Cat_alone_;


        double BsFitChi2_;
        int    BsFitNdof_;

        double BsFitVtxProb_;

        double BsFitM_;
    	double BsFitEta_;
	double BsFitPt_;
	double BsFitPz_;
	double BsFitPhi_;

	double BsFitVtx_x_;
	double BsFitVtx_y_;
	double BsFitVtx_z_;

        double BsM_nofit_;
        double BsPhi_nofit_;
        double BsEta_nofit_;
        double BsPt_nofit_;
        double BsPz_nofit_;

        double JpsiM_nofit_;
        double JpsiPhi_nofit_;
        double JpsiEta_nofit_;
        double JpsiPt_nofit_;
        double JpsiPz_nofit_;

        double PhiM_nofit_;
        double PhiPhi_nofit_;
        double PhiEta_nofit_;
        double PhiPt_nofit_;
        double PhiPz_nofit_;


        double  K1Pt_nofit_;
        double  K1Pz_nofit_;
        double  K1Eta_nofit_;
        double  K1Phi_nofit_;
	int     K1Key_nofit_;
        double  K2Eta_nofit_;
        double  K2Pt_nofit_;
        double  K2Pz_nofit_;
        double  K2Phi_nofit_;
	int     K2Key_nofit_;

        double  K1Chi2_;
        int     K1nHits_;
        double  K2Chi2_;
        int     K2nHits_;
        int     K1pixH_;
        int     K1trkH_;
        int     K2pixH_;
        int     K2trkH_;

        double  Mu1Chi2_;
        int     Mu1nHits_;
        double  Mu2Chi2_;
        int     Mu2nHits_;
        int     Mu1pixH_;
        int     Mu1trkH_;
        int     Mu2pixH_;
        int     Mu2trkH_;
	
	double costheta_;
	double phi_;
	double cospsi_;
	double AngleBsDecayLength_;

	int isMatched_;
	int isMatchedBd_;

	double BsLxy_;
	double BsLxyErr_;

	double BsCt_;
	double BsCtErr_;

	double BsErrX_;
	double BsErrY_;
	double BsErrXY_;

	int BsNumberOfCandidates_;


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
     

	double BsDist3d_;
	double BsDist3dErr_;
	double BsTime3d_;
	double BsTime3dErr_;
	double BsDist2d_;
	double BsDist2dErr_;
	double BsTime2d_;
	double BsTime2dErr_;

	double dedxTrk_;
	double errdedxTrk_;
	int numdedxTrk_;

	int iPassedCutIdent_;
	int iPassedCutIdentBd_;


        double K1Fit_par_[7];
        double K2Fit_par_[7];
        double K1Fit_sigX_;
        double K1Fit_sigY_;
        double K1Fit_sigZ_;
        double K2Fit_sigX_;
        double K2Fit_sigY_;
        double K2Fit_sigZ_;
        double K1Fit_sigPX_;
        double K1Fit_sigPY_;
        double K1Fit_sigPZ_;
        double K2Fit_sigPX_;
        double K2Fit_sigPY_;
        double K2Fit_sigPZ_;

         
	int GenNumberOfBdecays_;
	int BmesonsId_[10];
	int BDauIdMC_[10][15];
	int BDauDauIdMC_[10][15][10];
    	int GenNumberOfDaughters_[10];
	int GenNumberOfDaughtersDaughters_[10][15];

	double BDauMMC_[10][15];
	double BDauPtMC_[10][15];
	double BDauPzMC_[10][15];
	double BDauEtaMC_[10][15];
	double BDauPhiMC_[10][15];

	double BDauDauMMC_[10][15][10];
	double BDauDauPtMC_[10][15][10];
	double BDauDauPzMC_[10][15][10];
	double BDauDauEtaMC_[10][15][10];
	double BDauDauPhiMC_[10][15][10];

	double BMMC_[10];
	double BPtMC_[10];
	double BPzMC_[10];
	double BEtaMC_[10];
	double BPhiMC_[10];

	double BVtxMC_x_[10];
	double BVtxMC_y_[10];
	double BVtxMC_z_[10];
	double BSVtxMC_x_[10];
	double BSVtxMC_y_[10];
	double BSVtxMC_z_[10];
	double BLxy_MC_[10];
	double BCt_MC_[10];

        double genBsVtx_z_, genBsVtx_y_, genBsVtx_x_ ;
        double genBsSVtx_z_, genBsSVtx_y_, genBsSVtx_x_ ;

	int isGenJpsiEvent_;


	// for the Bd->Kstar analysis
	double BdFitChi2_Hyp1_;
        int    BdFitNdof_Hyp1_;

        double BdFitVtxProb_Hyp1_;
   
	int BdNumberOfCandidates_;

	double BdPVx_refit_   ;
	double BdPVy_refit_   ;
	double BdPVz_refit_   ;
	double BdPVerrx_refit_;
	double BdPVerry_refit_;
	double BdPVerrz_refit_;

        double BdFitM_Hyp1_;
  	double BdFitEta_Hyp1_;
	double BdFitPt_Hyp1_;
	double BdFitPz_Hyp1_;
	double BdFitPhi_Hyp1_;

	double BdFitVtx_x_Hyp1_;
	double BdFitVtx_y_Hyp1_;
	double BdFitVtx_z_Hyp1_;

        double BdM_nofit_;
        double BdPhi_nofit_;
        double BdEta_nofit_;
        double BdPt_nofit_;
        double BdPz_nofit_;

	double KstarMass_nofit_Hyp1_ ;
	double KstarMass_nofit_Hyp2_ ; 

	double BdK1_kpi_par_Hyp1_[7];
	double BdK2_kpi_par_Hyp1_[7];
	double BdK1_kpi_sigX_Hyp1_;
        double BdK1_kpi_sigY_Hyp1_;
        double BdK1_kpi_sigZ_Hyp1_;
	double BdK2_kpi_sigX_Hyp1_;
        double BdK2_kpi_sigY_Hyp1_;
        double BdK2_kpi_sigZ_Hyp1_;
	double BdK1_kpi_sigPX_Hyp1_;
	double BdK1_kpi_sigPY_Hyp1_;
        double BdK1_kpi_sigPZ_Hyp1_;
	double BdK2_kpi_sigPX_Hyp1_;
        double BdK2_kpi_sigPY_Hyp1_;
        double BdK2_kpi_sigPZ_Hyp1_;

	double BdFitChi2_Hyp2_;
        int    BdFitNdof_Hyp2_;

        double BdFitVtxProb_Hyp2_;
   

        double BdFitM_Hyp2_;
  	double BdFitEta_Hyp2_;
	double BdFitPt_Hyp2_;
	double BdFitPz_Hyp2_;
	double BdFitPhi_Hyp2_;

	double BdFitVtx_x_Hyp2_;
	double BdFitVtx_y_Hyp2_;
	double BdFitVtx_z_Hyp2_;

    
	double BdK1_kpi_par_Hyp2_[7];
	double BdK2_kpi_par_Hyp2_[7];
	double BdK1_kpi_sigX_Hyp2_;
        double BdK1_kpi_sigY_Hyp2_;
        double BdK1_kpi_sigZ_Hyp2_;
	double BdK2_kpi_sigX_Hyp2_;
        double BdK2_kpi_sigY_Hyp2_;
        double BdK2_kpi_sigZ_Hyp2_;
	double BdK1_kpi_sigPX_Hyp2_;
	double BdK1_kpi_sigPY_Hyp2_;
        double BdK1_kpi_sigPZ_Hyp2_;
	double BdK2_kpi_sigPX_Hyp2_;
        double BdK2_kpi_sigPY_Hyp2_;
        double BdK2_kpi_sigPZ_Hyp2_;


	double BdK1Pt_nofit_  ; 
	double BdK1Pz_nofit_  ; 
	double BdK1Eta_nofit_ ; 
	double BdK1Phi_nofit_ ; 
	int BdK1Key_nofit_  ; 
	double BdK2Pt_nofit_  ; 
	double BdK2Pz_nofit_  ; 
	double BdK2Eta_nofit_ ; 
	double BdK2Phi_nofit_ ; 
	int BdK2Key_nofit_  ; 

	double BdLxy_;
	double BdLxyErr_;
	double BdErrX_;
	double BdErrY_;
	double BdErrXY_;
	double BdCt_;
	double BdCtErr_;

	double BdDist3d_;
	double BdDist3dErr_;
	double BdTime3d_;
	double BdTime3dErr_;
	double BdDist2d_;
	double BdDist2dErr_;
	double BdTime2d_;
	double BdTime2dErr_;

	int BdK1mcId_;
        int BdK1momId_;
        int BdK1gmomId_;
        int BdK2mcId_;
        int BdK2momId_;
        int BdK2gmomId_;
        int BdMu1mcId_;
        int BdMu1momId_;
        int BdMu1gmomId_;
        int BdMu2mcId_;
        int BdMu2momId_;
        int BdMu2gmomId_;
       

	TFile* bsFile_;
	TTree* bsTree_; 
};

#endif

