#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiRootTree.h"

BsToJpsiPhiRootTree::BsToJpsiPhiRootTree()
{
  resetEntries();
  bsTree_ = 0;
  bsFile_ = 0;

}


void BsToJpsiPhiRootTree::createTree(const std::string filename)
{

  // open root file
  bsFile_ = new TFile (filename.c_str(), "RECREATE" );
  int bufsize = 64000;
  // create tree structure
  bsTree_ = new TTree("BsTree","BsTree",bufsize);
  


bsTree_->Branch(  "triggerbit_HLTmu3"             , &triggerbit_HLTmu3_,                "triggerbit_HLTmu3/I");                                         
bsTree_->Branch(  "triggerbit_HLTmu5"		  , &triggerbit_HLTmu5_,                "triggerbit_HLTmu5/I");                                      
bsTree_->Branch(  "triggerbit_HLTmu9"		  , &triggerbit_HLTmu9_,                "triggerbit_HLTmu9/I");                                      
bsTree_->Branch(  "triggerbit_HLTdoubleIsoMu3"	  , &triggerbit_HLTdoubleIsoMu3_,       "triggerbit_HLTdoubleIsoMu3/I");                                
bsTree_->Branch(  "triggerbit_HLTdoubleMu3"	  , &triggerbit_HLTdoubleMu3_,          "triggerbit_HLTdoubleMu3/I");        
bsTree_->Branch(  "triggerbit_HLTdoubleMu0"	  , &triggerbit_HLTdoubleMu0_,          "triggerbit_HLTdoubleMu0/I");                             
bsTree_->Branch(  "triggerbit_HLTdoubleMu3_JPsi"  , &triggerbit_HLTdoubleMu3_JPsi_,     "triggerbit_HLTdoubleMu3_JPsi/I");                              
bsTree_->Branch(  "BSx"				  , &BSx_,                              "BSx/D");                                                     
bsTree_->Branch(  "BSy"				  , &BSy_,                              "BSy/D");                                                    
bsTree_->Branch(  "BSz"				  , &BSz_,                              "BSz/D");                                                       
bsTree_->Branch(  "PVx"				  , &PVx_,                              "PVx/D");                                                       
bsTree_->Branch(  "PVy"				  , &PVy_,                              "PVy/D");                                                       
bsTree_->Branch(  "PVz"				  , &PVz_,                              "PVz/D");                                                       
bsTree_->Branch(  "PVerrx"			  , &PVerrx_,                           "PVerrx/D");                                                    
bsTree_->Branch(  "PVerry"			  , &PVerry_,                           "PVerry/D");                                                    
bsTree_->Branch(  "PVerrz"			  , &PVerrz_,                           "PVerrz/D"); 

bsTree_->Branch( "PVx_refit"   ,         &PVx_refit_   ,         "PVx_refit/D"   ); 
bsTree_->Branch( "PVy_refit"   ,	 &PVy_refit_   ,	 "PVy_refit/D"   );
bsTree_->Branch( "PVz_refit"   ,	 &PVz_refit_   ,	 "PVz_refit/D"   );
bsTree_->Branch( "PVerrx_refit",	 &PVerrx_refit_,	 "PVerrx_refit/D");
bsTree_->Branch( "PVerry_refit",	 &PVerry_refit_,	 "PVerry_refit/D");
bsTree_->Branch( "PVerrz_refit",	 &PVerrz_refit_,	 "PVerrz_refit/D");

                                                   
bsTree_->Branch(  "JpsiVtxProb"			  , &JpsiVtxProb_,                      "JpsiVtxProb/D");                                               
bsTree_->Branch(  "JpsiM_alone"			  , &JpsiM_alone_,                      "JpsiM_alone/D");                                               
bsTree_->Branch(  "JpsiPhi_alone"		  , &JpsiPhi_alone_,                    "JpsiPhi_alone/D");                                             
bsTree_->Branch(  "JpsiEta_alone"		  , &JpsiEta_alone_,                    "JpsiEta_alone/D");                                             
bsTree_->Branch(  "JpsiPt_alone"		  , &JpsiPt_alone_,                     "JpsiPt_alone/D");                                              
bsTree_->Branch(  "JpsiMu1Pt_alone"		  , &JpsiMu1Pt_alone_,                  "JpsiMu1Pt_alone/D");                                           
bsTree_->Branch(  "JpsiMu2Pt_alone"		  , &JpsiMu2Pt_alone_,                  "JpsiMu2Pt_alone/D");                                           
bsTree_->Branch(  "JpsiMu1Phi_alone"		  , &JpsiMu1Phi_alone_,                 "JpsiMu1Phi_alone/D");                                          
bsTree_->Branch(  "JpsiMu2Phi_alone"		  , &JpsiMu2Phi_alone_,                 "JpsiMu2Phi_alone/D");                                          
bsTree_->Branch(  "JpsiMu1Eta_alone"		  , &JpsiMu1Eta_alone_,                 "JpsiMu1Eta_alone/D");                                          
bsTree_->Branch(  "JpsiMu2Eta_alone"		  , &JpsiMu2Eta_alone_,                 "JpsiMu2Eta_alone/D");                                          
bsTree_->Branch(  "JpsiMuon1Cat_alone"		  , &JpsiMuon1Cat_alone_,               "JpsiMuon1Cat_alone/I");                                     
bsTree_->Branch(  "JpsiMuon2Cat_alone"		  , &JpsiMuon2Cat_alone_,               "JpsiMuon2Cat_alone/I");                                     
bsTree_->Branch(  "BsFitChi2"			  , &BsFitChi2_,                        "BsFitChi2/D");                                                 
bsTree_->Branch(  "BsFitNdof"			  , &BsFitNdof_,                        "BsFitNdof/I");                                                
bsTree_->Branch(  "BsFitVtxProb"		  , &BsFitVtxProb_,                     "BsFitVtxProb/D");                                              
        
bsTree_->Branch(  "BsFitM"			  , &BsFitM_,                           "BsFitM/D");                                                 
bsTree_->Branch(  "BsFitEta"			  , &BsFitEta_,                         "BsFitEta/D");                                                  
bsTree_->Branch(  "BsFitPt"			  , &BsFitPt_,                          "BsFitPt/D");                                                   
bsTree_->Branch(  "BsFitPz"			  , &BsFitPz_,                          "BsFitPz/D");                                                   
bsTree_->Branch(  "BsFitPhi"			  , &BsFitPhi_,                         "BsFitPhi/D");                                                  
bsTree_->Branch(  "BsFitVtx_x"			  , &BsFitVtx_x_,                       "BsFitVtx_x/D");                                                
bsTree_->Branch(  "BsFitVtx_y"			  , &BsFitVtx_y_,                       "BsFitVtx_y/D");                                                
bsTree_->Branch(  "BsFitVtx_z"			  , &BsFitVtx_z_,                       "BsFitVtx_z/D");                                                
bsTree_->Branch(  "BsM_nofit"			  , &BsM_nofit_,                        "BsM_nofit/D");                                                 
bsTree_->Branch(  "BsPhi_nofit"			  , &BsPhi_nofit_,                      "BsPhi_nofit/D");                                               
bsTree_->Branch(  "BsEta_nofit"			  , &BsEta_nofit_,                      "BsEta_nofit/D");                                               
bsTree_->Branch(  "BsPt_nofit"			  , &BsPt_nofit_,                       "BsPt_nofit/D");                                                
bsTree_->Branch(  "BsPz_nofit"			  , &BsPz_nofit_,                       "BsPz_nofit/D");                                                
bsTree_->Branch(  "JpsiM_nofit"			  , &JpsiM_nofit_,                      "JpsiM_nofit/D");                                               
bsTree_->Branch(  "JpsiPhi_nofit"		  , &JpsiPhi_nofit_,                    "JpsiPhi_nofit/D");                                             
bsTree_->Branch(  "JpsiEta_nofit"		  , &JpsiEta_nofit_,                    "JpsiEta_nofit/D");                                             
bsTree_->Branch(  "JpsiPt_nofit"		  , &JpsiPt_nofit_,                     "JpsiPt_nofit/D");                                              
bsTree_->Branch(  "JpsiPz_nofit"		  , &JpsiPz_nofit_,                     "JpsiPz_nofit/D");                                              
bsTree_->Branch(  "PhiM_nofit"			  , &PhiM_nofit_,                       "PhiM_nofit/D");                                                
bsTree_->Branch(  "PhiPhi_nofit"		  , &PhiPhi_nofit_,                     "PhiPhi_nofit/D");                                              
bsTree_->Branch(  "PhiEta_nofit"		  , &PhiEta_nofit_,                     "PhiEta_nofit/D");                                              
bsTree_->Branch(  "PhiPt_nofit"			  , &PhiPt_nofit_,                      "PhiPt_nofit/D");                                               
bsTree_->Branch(  "PhiPz_nofit"			  , &PhiPz_nofit_,                      "PhiPz_nofit/D");                                               
bsTree_->Branch(  "K1Pt_nofit"			  , &K1Pt_nofit_,                       "K1Pt_nofit/D");                                                
bsTree_->Branch(  "K1Pz_nofit"			  , &K1Pz_nofit_,                       "K1Pz_nofit/D");                                                
bsTree_->Branch(  "K1Eta_nofit"			  , &K1Eta_nofit_,                      "K1Eta_nofit/D");                                               
bsTree_->Branch(  "K1Phi_nofit"			  , &K1Phi_nofit_,                      "K1Phi_nofit/D");                                               
bsTree_->Branch(  "K1Key_nofit"			  , &K1Key_nofit_,                       "K1Key_nofit/I");                                                
bsTree_->Branch(  "K2Eta_nofit"			  , &K2Eta_nofit_,                      "K2Eta_nofit/D");                                               
bsTree_->Branch(  "K2Pt_nofit"			  , &K2Pt_nofit_,                       "K2Pt_nofit/D");                                                
bsTree_->Branch(  "K2Pz_nofit"			  , &K2Pz_nofit_,                       "K2Pz_nofit/D");                                                
bsTree_->Branch(  "K2Phi_nofit"			  , &K2Phi_nofit_,                      "K2Phi_nofit/D"); 
bsTree_->Branch(  "K2Key_nofit"			  , &K2Key_nofit_,                       "K2Key_nofit/I");                                              
bsTree_->Branch(  "K1Chi2"			  , &K1Chi2_,                           "K1Chi2/D");                                                    
bsTree_->Branch(  "K1nHits"			  , &K1nHits_,                          "K1nHits/I");                                                
bsTree_->Branch(  "K2Chi2"			  , &K2Chi2_,                           "K2Chi2/D");                                                    
bsTree_->Branch(  "K2nHits"			  , &K2nHits_,                          "K2nHits/I");                                                   
bsTree_->Branch(  "K1pixH"			  , &K1pixH_,                           "K1pixH/I");                                                  
bsTree_->Branch(  "K1trkH"			  , &K1trkH_,                           "K1trkH/I");                                                 
bsTree_->Branch(  "K2pixH"			  , &K2pixH_,                           "K2pixH/I");                                                 
bsTree_->Branch(  "K2trkH"			  , &K2trkH_,                           "K2trkH/I");                                                 
bsTree_->Branch(  "Mu1Chi2"			  , &Mu1Chi2_,                          "Mu1Chi2/D");                                                   
bsTree_->Branch(  "Mu1nHits"			  , &Mu1nHits_,                         "Mu1nHits/I");                                                 
bsTree_->Branch(  "Mu2Chi2"			  , &Mu2Chi2_,                          "Mu2Chi2/D");                                                   
bsTree_->Branch(  "Mu2nHits"			  , &Mu2nHits_,                         "Mu2nHits/I");                                               
bsTree_->Branch(  "Mu1pixH"			  , &Mu1pixH_,                          "Mu1pixH/I");                                               
bsTree_->Branch(  "Mu1trkH"			  , &Mu1trkH_,                          "Mu1trkH/I");                                                
bsTree_->Branch(  "Mu2pixH"			  , &Mu2pixH_,                          "Mu2pixH/I");                                                
bsTree_->Branch(  "Mu2trkH"			  , &Mu2trkH_,                          "Mu2trkH/I");                                                
bsTree_->Branch(  "costheta"			  , &costheta_,                         "costheta/D");                                                  
bsTree_->Branch(  "phi"				  , &phi_,                              "phi/D");                                                       
bsTree_->Branch(  "cospsi"			  , &cospsi_,                           "cospsi/D");                                                    
bsTree_->Branch(  "AngleBsDecayLength"		  , &AngleBsDecayLength_,               "AngleBsDecayLength/D");                                        
bsTree_->Branch(  "isMatched"			  , &isMatched_,                        "isMatched/I");                                                
bsTree_->Branch(  "isMatchedBd"			  , &isMatchedBd_,                      "isMatchedBd/I");                                              
bsTree_->Branch(  "BsLxy"			  , &BsLxy_,                            "BsLxy/D");                                                     
bsTree_->Branch(  "BsCt"			  , &BsCt_,                             "BsCt/D");         
bsTree_->Branch(  "BsLxyErr"			  , &BsLxyErr_,                            "BsLxyErr/D");                                                     
bsTree_->Branch(  "BsCtErr"			  , &BsCtErr_,                             "BsCtErr/D");         
 bsTree_->Branch(  "BsNumberOfCandidates"          , &BsNumberOfCandidates_,            "BsNumberOfCandidates/I");

bsTree_->Branch(  "BsErrX"			  , &BsErrX_,                           "BsErrX/D");                                                    
bsTree_->Branch(  "BsErrY"			  , &BsErrY_,                           "BsErrY/D");                                                    
bsTree_->Branch(  "BsErrXY"			  , &BsErrXY_,                          "BsErrXY/D");                                                   
                                             
bsTree_->Branch(  "K1trkLay"			  , &K1trkLay_,                         "K1trkLay/I");                                                  
bsTree_->Branch(  "K1muDTh"			  , &K1muDTh_,                          "K1muDTh/I");                                               
bsTree_->Branch(  "K1muCSCh"			  , &K1muCSCh_,                         "K1muCSCh/I");                                                
bsTree_->Branch(  "K1muRPCh"			  , &K1muRPCh_,                         "K1muRPCh/I");                                               
bsTree_->Branch(  "K2trkLay"			  , &K2trkLay_,                         "K2trkLay/I");                                               
bsTree_->Branch(  "K2muDTh"			  , &K2muDTh_,                          "K2muDTh/I");                                               
bsTree_->Branch(  "K2muCSCh"			  , &K2muCSCh_,                         "K2muCSCh/I");                                                
bsTree_->Branch(  "K2muRPCh"			  , &K2muRPCh_,                         "K2muRPCh/I");                                               
bsTree_->Branch(  "Mu1trkLay"			  , &Mu1trkLay_,                        "Mu1trkLay/I");                                               
bsTree_->Branch(  "Mu1muDTh"			  , &Mu1muDTh_,                         "Mu1muDTh/I");                                              
bsTree_->Branch(  "Mu1muCSCh"			  , &Mu1muCSCh_,                        "Mu1muCSCh/I");                                               
bsTree_->Branch(  "Mu1muRPCh"			  , &Mu1muRPCh_,                        "Mu1muRPCh/I");                                              
bsTree_->Branch(  "Mu2trkLay"			  , &Mu2trkLay_,                        "Mu2trkLay/I");                                              
bsTree_->Branch(  "Mu2muDTh"			  , &Mu2muDTh_,                         "Mu2muDTh/I");                                              
bsTree_->Branch(  "Mu2muCSCh"			  , &Mu2muCSCh_,                        "Mu2muCSCh/I");                                               
bsTree_->Branch(  "Mu2muRPCh"			  , &Mu2muRPCh_,                        "Mu2muRPCh/I");                                              
bsTree_->Branch(  "K1mcId"			  , &K1mcId_,                           "K1mcId/I");                                                  
bsTree_->Branch(  "K1momId"			  , &K1momId_,                          "K1momId/I");                                                 
bsTree_->Branch(  "K1gmomId"			  , &K1gmomId_,                         "K1gmomId/I");                                                
bsTree_->Branch(  "K2mcId"			  , &K2mcId_,                           "K2mcId/I");                                               
bsTree_->Branch(  "K2momId"			  , &K2momId_,                          "K2momId/I");                                                 
bsTree_->Branch(  "K2gmomId"			  , &K2gmomId_,                         "K2gmomId/I");                                                
bsTree_->Branch(  "Mu1mcId"			  , &Mu1mcId_,                          "Mu1mcId/I");                                               
bsTree_->Branch(  "Mu1momId"			  , &Mu1momId_,                         "Mu1momId/I");                                                
bsTree_->Branch(  "Mu1gmomId"			  , &Mu1gmomId_,                        "Mu1gmomId/I");                                               
bsTree_->Branch(  "Mu2mcId"			  , &Mu2mcId_,                          "Mu2mcId/I");                                              
bsTree_->Branch(  "Mu2momId"			  , &Mu2momId_,                         "Mu2momId/I");                                                
bsTree_->Branch(  "Mu2gmomId"			  , &Mu2gmomId_,                        "Mu2gmomId/I");                                               
              
bsTree_->Branch(  "BsDist3d"			  , &BsDist3d_,                         "BsDist3d/D");                                                  
bsTree_->Branch(  "BsDist3dErr"			  , &BsDist3dErr_,                      "BsDist3dErr/D");                                               
bsTree_->Branch(  "BsTime3d"			  , &BsTime3d_,                         "BsTime3d/D");                                                  
bsTree_->Branch(  "BsTime3dErr"			  , &BsTime3dErr_,                      "BsTime3dErr/D");                                               
bsTree_->Branch(  "BsDist2d"			  , &BsDist2d_,                         "BsDist2d/D");                                                  
bsTree_->Branch(  "BsDist2dErr"			  , &BsDist2dErr_,                      "BsDist2dErr/D");                                               
bsTree_->Branch(  "BsTime2d"			  , &BsTime2d_,                         "BsTime2d/D");                                                  
bsTree_->Branch(  "BsTime2dErr"			  , &BsTime2dErr_,                      "BsTime2dErr/D");                                               
bsTree_->Branch(  "dedxTrk"			  , &dedxTrk_,                          "dedxTrk/D");                                                   
bsTree_->Branch(  "errdedxTrk"			  , &errdedxTrk_,                       "errdedxTrk/D");                                                
bsTree_->Branch(  "numdedxTrk"			  , &numdedxTrk_,                       "numdedxTrk/I");                                              
bsTree_->Branch(  "iPassedCutIdent"		  , &iPassedCutIdent_,                  "iPassedCutIdent/I");                                         
bsTree_->Branch(  "iPassedCutIdentBd"		  , &iPassedCutIdentBd_,                "iPassedCutIdentBd/I");                                        
bsTree_->Branch(  "K1Fit_par"			  , K1Fit_par_,                         "K1Fit_par[7]/D");                                          
bsTree_->Branch(  "K2Fit_par"			  , K2Fit_par_,                         "K2Fit_par[7]/D");                                      
bsTree_->Branch(  "K1Fit_sigX"			  , &K1Fit_sigX_,                       "K1Fit_sigX/D");                                                
bsTree_->Branch(  "K1Fit_sigY"			  , &K1Fit_sigY_,                       "K1Fit_sigY/D");                                                
bsTree_->Branch(  "K1Fit_sigZ"			  , &K1Fit_sigZ_,                       "K1Fit_sigZ/D");                                                
bsTree_->Branch(  "K2Fit_sigX"			  , &K2Fit_sigX_,                       "K2Fit_sigX/D");                                                
bsTree_->Branch(  "K2Fit_sigY"			  , &K2Fit_sigY_,                       "K2Fit_sigY/D");                                                
bsTree_->Branch(  "K2Fit_sigZ"			  , &K2Fit_sigZ_,                       "K2Fit_sigZ/D");                                                
bsTree_->Branch(  "K1Fit_sigPX"			  , &K1Fit_sigPX_,                      "K1Fit_sigPX/D");                                               
bsTree_->Branch(  "K1Fit_sigPY"			  , &K1Fit_sigPY_,                      "K1Fit_sigPY/D");                                               
bsTree_->Branch(  "K1Fit_sigPZ"			  , &K1Fit_sigPZ_,                      "K1Fit_sigPZ/D");                                               
bsTree_->Branch(  "K2Fit_sigPX"			  , &K2Fit_sigPX_,                      "K2Fit_sigPX/D");                                               
bsTree_->Branch(  "K2Fit_sigPY"			  , &K2Fit_sigPY_,                      "K2Fit_sigPY/D");                                               
bsTree_->Branch(  "K2Fit_sigPZ"			  , &K2Fit_sigPZ_,                      "K2Fit_sigPZ/D");                                               
          
bsTree_->Branch(  "GenNumberOfBdecays"		  , &GenNumberOfBdecays_,               "GenNumberOfBdecays/I");                                        
bsTree_->Branch(  "BmesonsId"			  , BmesonsId_,                         "BmesonsId[10]/I");                                
bsTree_->Branch(  "BDauIdMC"			  , BDauIdMC_,                          "BDauIdMC[10][15]/I");                                 
bsTree_->Branch(  "BDauDauIdMC"			  , BDauDauIdMC_,                     	"BDauDauIdMC[10][15][10]/I");                        
bsTree_->Branch(  "GenNumberOfDaughters"	  , GenNumberOfDaughters_,              "GenNumberOfDaughters[10]/I");                           
bsTree_->Branch(  "GenNumberOfDaughtersDaughters" , GenNumberOfDaughtersDaughters_,     "GenNumberOfDaughtersDaughters[10][15]/I");            
bsTree_->Branch(  "BDauMMC"			  , BDauMMC_,                           "BDauMMC[10][15]/D");         
bsTree_->Branch(  "BDauPtMC"			  , BDauPtMC_,                          "BDauPtMC[10][15]/D");                                 
bsTree_->Branch(  "BDauPzMC"			  , BDauPzMC_,                          "BDauPzMC[10][15]/D");                             
bsTree_->Branch(  "BDauEtaMC"			  , BDauEtaMC_,                         "BDauEtaMC[10][15]/D");                             
bsTree_->Branch(  "BDauPhiMC"			  , BDauPhiMC_,                         "BDauPhiMC[10][15]/D");                            
bsTree_->Branch(  "BDauDauMMC"			  , BDauDauMMC_,                      	"BDauDauMMC[10][15][10]/D");                         
bsTree_->Branch(  "BDauDauPtMC"			  , BDauDauPtMC_,                     	"BDauDauPtMC[10][15][10]/D");                     
bsTree_->Branch(  "BDauDauPzMC"			  , BDauDauPzMC_,                     	"BDauDauPzMC[10][15][10]/D");                    
bsTree_->Branch(  "BDauDauEtaMC"		  , BDauDauEtaMC_,                    	"BDauDauEtaMC[10][15][10]/D");                    
bsTree_->Branch(  "BDauDauPhiMC"		  , BDauDauPhiMC_,                    	"BDauDauPhiMC[10][15][10]/D");                   
bsTree_->Branch(  "BMMC"			  , BMMC_,                              "BMMC[10]/D");                         
bsTree_->Branch(  "BPtMC"			  , BPtMC_,                             "BPtMC[10]/D");                                         
bsTree_->Branch(  "BPzMC"			  , BPzMC_,                             "BPzMC[10]/D");                                        
bsTree_->Branch(  "BEtaMC"			  , BEtaMC_,                            "BEtaMC[10]/D");                                        
bsTree_->Branch(  "BPhiMC"			  , BPhiMC_,                            "BPhiMC[10]/D");    

bsTree_->Branch(  "BVtxMC_x" ,   BVtxMC_x_  , "BVtxMC_x[10]/D" );
bsTree_->Branch(  "BVtxMC_y" ,	 BVtxMC_y_  , "BVtxMC_y[10]/D" );
bsTree_->Branch(  "BVtxMC_z" ,	 BVtxMC_z_  , "BVtxMC_z[10]/D" );
bsTree_->Branch(  "BSVtxMC_x",	 BSVtxMC_x_ , "BSVtxMC_x[10]/D");
bsTree_->Branch(  "BSVtxMC_y",	 BSVtxMC_y_ , "BSVtxMC_y[10]/D");
bsTree_->Branch(  "BSVtxMC_z",	 BSVtxMC_z_ , "BSVtxMC_z[10]/D");
bsTree_->Branch(  "BLxy_MC"  ,	 BLxy_MC_   , "BLxy_MC[10]/D"  );
bsTree_->Branch(  "BCt_MC"   ,	 BCt_MC_    , "BCt_MC[10]/D"   );   
                                   
bsTree_->Branch(  "genBsVtx_z"			  , &genBsVtx_z_,                       "genBsVtx_z/D");                                                
bsTree_->Branch(  "genBsVtx_y"			  , &genBsVtx_y_,                       "genBsVtx_y/D");                                                
bsTree_->Branch(  "genBsVtx_x"			  , &genBsVtx_x_,                       "genBsVtx_x/D");                                                
bsTree_->Branch(  "genBsSVtx_z"			  , &genBsSVtx_z_,                      "genBsSVtx_z/D");                                               
bsTree_->Branch(  "genBsSVtx_y" 		  , &genBsSVtx_y_,                      "genBsSVtx_y/D");                                               
bsTree_->Branch(  "genBsSVtx_x"			  , &genBsSVtx_x_,                      "genBsSVtx_x/D");                                               
bsTree_->Branch(  "isGenJpsiEvent"		  , &isGenJpsiEvent_,                   "isGenJpsiEvent/I");                                           
bsTree_->Branch(  "BdFitChi2_Hyp1"		  , &BdFitChi2_Hyp1_,                   "BdFitChi2_Hyp1/D");                                            
bsTree_->Branch(  "BdFitNdof_Hyp1"		  , &BdFitNdof_Hyp1_,                   "BdFitNdof_Hyp1/I");                                           
bsTree_->Branch(  "BdFitVtxProb_Hyp1"		  , &BdFitVtxProb_Hyp1_,                "BdFitVtxProb_Hyp1/D");                                         
bsTree_->Branch(  "BdFitM_Hyp1"			  , &BdFitM_Hyp1_,                      "BdFitM_Hyp1/D");                                               
bsTree_->Branch(  "BdFitEta_Hyp1"		  , &BdFitEta_Hyp1_,                    "BdFitEta_Hyp1/D");                                             
bsTree_->Branch(  "BdFitPt_Hyp1"		  , &BdFitPt_Hyp1_,                     "BdFitPt_Hyp1/D");                                              
bsTree_->Branch(  "BdFitPz_Hyp1"		  , &BdFitPz_Hyp1_,                     "BdFitPz_Hyp1/D");                                              
bsTree_->Branch(  "BdFitPhi_Hyp1"		  , &BdFitPhi_Hyp1_,                    "BdFitPhi_Hyp1/D");                                             
bsTree_->Branch(  "BdFitVtx_x_Hyp1"		  , &BdFitVtx_x_Hyp1_,                  "BdFitVtx_x_Hyp1/D");                                           
bsTree_->Branch(  "BdFitVtx_y_Hyp1"		  , &BdFitVtx_y_Hyp1_,                  "BdFitVtx_y_Hyp1/D");                                           
bsTree_->Branch(  "BdFitVtx_z_Hyp1"		  , &BdFitVtx_z_Hyp1_,                  "BdFitVtx_z_Hyp1/D");                                           
bsTree_->Branch(  "BdM_nofit"		  , &BdM_nofit_,                   "BdM_nofit/D");                                            
bsTree_->Branch(  "BdPhi_nofit"		  , &BdPhi_nofit_,                 "BdPhi_nofit/D");                                          
bsTree_->Branch(  "BdEta_nofit"		  , &BdEta_nofit_,                 "BdEta_nofit/D");                                          
bsTree_->Branch(  "BdPt_nofit"		  , &BdPt_nofit_,                  "BdPt_nofit/D");                                           
bsTree_->Branch(  "BdPz_nofit"		  , &BdPz_nofit_,                  "BdPz_nofit/D");                                           
bsTree_->Branch(  "KstarMass_nofit_Hyp1"	  , &KstarMass_nofit_Hyp1_,             "KstarMass_nofit_Hyp1/D"); 
bsTree_->Branch(  "KstarMass_nofit_Hyp2"	  , &KstarMass_nofit_Hyp2_,             "KstarMass_nofit_Hyp2/D");                                      
bsTree_->Branch(  "BdK1_kpi_par_Hyp1"		  , BdK1_kpi_par_Hyp1_,                 "BdK1_kpi_par_Hyp1[7]/D");                                  
bsTree_->Branch(  "BdK2_kpi_par_Hyp1"		  , BdK2_kpi_par_Hyp1_,                 "BdK2_kpi_par_Hyp1[7]/D");                              
bsTree_->Branch(  "BdK1_kpi_sigX_Hyp1"		  , &BdK1_kpi_sigX_Hyp1_,               "BdK1_kpi_sigX_Hyp1/D");                                        
bsTree_->Branch(  "BdK1_kpi_sigY_Hyp1"		  , &BdK1_kpi_sigY_Hyp1_,               "BdK1_kpi_sigY_Hyp1/D");                                        
bsTree_->Branch(  "BdK1_kpi_sigZ_Hyp1"		  , &BdK1_kpi_sigZ_Hyp1_,               "BdK1_kpi_sigZ_Hyp1/D");                                        
bsTree_->Branch(  "BdK2_kpi_sigX_Hyp1"		  , &BdK2_kpi_sigX_Hyp1_,               "BdK2_kpi_sigX_Hyp1/D");                                        
bsTree_->Branch(  "BdK2_kpi_sigY_Hyp1"		  , &BdK2_kpi_sigY_Hyp1_,               "BdK2_kpi_sigY_Hyp1/D");                                        
bsTree_->Branch(  "BdK2_kpi_sigZ_Hyp1"		  , &BdK2_kpi_sigZ_Hyp1_,               "BdK2_kpi_sigZ_Hyp1/D");                                        
bsTree_->Branch(  "BdK1_kpi_sigPX_Hyp1"		  , &BdK1_kpi_sigPX_Hyp1_,              "BdK1_kpi_sigPX_Hyp1/D");                                       
bsTree_->Branch(  "BdK1_kpi_sigPY_Hyp1"		  , &BdK1_kpi_sigPY_Hyp1_,              "BdK1_kpi_sigPY_Hyp1/D");                                       
bsTree_->Branch(  "BdK1_kpi_sigPZ_Hyp1"		  , &BdK1_kpi_sigPZ_Hyp1_,              "BdK1_kpi_sigPZ_Hyp1/D");                                       
bsTree_->Branch(  "BdK2_kpi_sigPX_Hyp1"		  , &BdK2_kpi_sigPX_Hyp1_,              "BdK2_kpi_sigPX_Hyp1/D");                                       
bsTree_->Branch(  "BdK2_kpi_sigPY_Hyp1"		  , &BdK2_kpi_sigPY_Hyp1_,              "BdK2_kpi_sigPY_Hyp1/D");                                       
bsTree_->Branch(  "BdK2_kpi_sigPZ_Hyp1"		  , &BdK2_kpi_sigPZ_Hyp1_,              "BdK2_kpi_sigPZ_Hyp1/D");                                       
bsTree_->Branch(  "BdFitChi2_Hyp2"		  , &BdFitChi2_Hyp2_,                   "BdFitChi2_Hyp2/D");                                            
bsTree_->Branch(  "BdFitNdof_Hyp2"		  , &BdFitNdof_Hyp2_,                   "BdFitNdof_Hyp2/I");                                           
bsTree_->Branch(  "BdFitVtxProb_Hyp2"		  , &BdFitVtxProb_Hyp2_,                "BdFitVtxProb_Hyp2/D");                                         
bsTree_->Branch(  "BdFitM_Hyp2"			  , &BdFitM_Hyp2_,                      "BdFitM_Hyp2/D");                                               
bsTree_->Branch(  "BdFitEta_Hyp2"		  , &BdFitEta_Hyp2_,                    "BdFitEta_Hyp2/D");                                             
bsTree_->Branch(  "BdFitPt_Hyp2"		  , &BdFitPt_Hyp2_,                     "BdFitPt_Hyp2/D");                                              
bsTree_->Branch(  "BdFitPz_Hyp2"		  , &BdFitPz_Hyp2_,                     "BdFitPz_Hyp2/D");                                              
bsTree_->Branch(  "BdFitPhi_Hyp2"		  , &BdFitPhi_Hyp2_,                    "BdFitPhi_Hyp2/D");                                             
bsTree_->Branch(  "BdFitVtx_x_Hyp2"		  , &BdFitVtx_x_Hyp2_,                  "BdFitVtx_x_Hyp2/D");                                           
bsTree_->Branch(  "BdFitVtx_y_Hyp2"		  , &BdFitVtx_y_Hyp2_,                  "BdFitVtx_y_Hyp2/D");                                           
bsTree_->Branch(  "BdFitVtx_z_Hyp2"		  , &BdFitVtx_z_Hyp2_,                  "BdFitVtx_z_Hyp2/D");                                           
bsTree_->Branch(  "BdNumberOfCandidates"          , &BdNumberOfCandidates_,            "BdNumberOfCandidates/I");
bsTree_->Branch(  "BdK1_kpi_par_Hyp2"		  , BdK1_kpi_par_Hyp2_,                 "BdK1_kpi_par_Hyp2[7]/D");                                  
bsTree_->Branch(  "BdK2_kpi_par_Hyp2"		  , BdK2_kpi_par_Hyp2_,                 "BdK2_kpi_par_Hyp2[7]/D");                              
bsTree_->Branch(  "BdK1_kpi_sigX_Hyp2"		  , &BdK1_kpi_sigX_Hyp2_,               "BdK1_kpi_sigX_Hyp2/D");                                        
bsTree_->Branch(  "BdK1_kpi_sigY_Hyp2"		  , &BdK1_kpi_sigY_Hyp2_,               "BdK1_kpi_sigY_Hyp2/D");                                        
bsTree_->Branch(  "BdK1_kpi_sigZ_Hyp2"		  , &BdK1_kpi_sigZ_Hyp2_,               "BdK1_kpi_sigZ_Hyp2/D");                                        
bsTree_->Branch(  "BdK2_kpi_sigX_Hyp2"		  , &BdK2_kpi_sigX_Hyp2_,               "BdK2_kpi_sigX_Hyp2/D");                                        
bsTree_->Branch(  "BdK2_kpi_sigY_Hyp2"		  , &BdK2_kpi_sigY_Hyp2_,               "BdK2_kpi_sigY_Hyp2/D");                                        
bsTree_->Branch(  "BdK2_kpi_sigZ_Hyp2"		  , &BdK2_kpi_sigZ_Hyp2_,               "BdK2_kpi_sigZ_Hyp2/D");                                        
bsTree_->Branch(  "BdK1_kpi_sigPX_Hyp2"		  , &BdK1_kpi_sigPX_Hyp2_,              "BdK1_kpi_sigPX_Hyp2/D");                                       
bsTree_->Branch(  "BdK1_kpi_sigPY_Hyp2"		  , &BdK1_kpi_sigPY_Hyp2_,              "BdK1_kpi_sigPY_Hyp2/D");                                       
bsTree_->Branch(  "BdK1_kpi_sigPZ_Hyp2"		  , &BdK1_kpi_sigPZ_Hyp2_,              "BdK1_kpi_sigPZ_Hyp2/D");                                       
bsTree_->Branch(  "BdK2_kpi_sigPX_Hyp2"		  , &BdK2_kpi_sigPX_Hyp2_,              "BdK2_kpi_sigPX_Hyp2/D");                                       
bsTree_->Branch(  "BdK2_kpi_sigPY_Hyp2"		  , &BdK2_kpi_sigPY_Hyp2_,              "BdK2_kpi_sigPY_Hyp2/D");                                       
bsTree_->Branch(  "BdK2_kpi_sigPZ_Hyp2"		  , &BdK2_kpi_sigPZ_Hyp2_,              "BdK2_kpi_sigPZ_Hyp2/D");                                       
bsTree_->Branch(  "BdK1Pt_nofit" 		  , &BdK1Pt_nofit_,                     "BdK1Pt_nofit/D");                                              
bsTree_->Branch(  "BdK1Pz_nofit" 		  , &BdK1Pz_nofit_,                     "BdK1Pz_nofit/D");                                              
bsTree_->Branch(  "BdK1Eta_nofit" 		  , &BdK1Eta_nofit_,                    "BdK1Eta_nofit/D");                                             
bsTree_->Branch(  "BdK1Phi_nofit" 		  , &BdK1Phi_nofit_,                    "BdK1Phi_nofit/D");                                             
bsTree_->Branch(  "BdK1Key_nofit" 		  , &BdK1Key_nofit_,                     "BdK1Key_nofit/I");                                              
bsTree_->Branch(  "BdK2Pt_nofit" 		  , &BdK2Pt_nofit_,                     "BdK2Pt_nofit/D");                                              
bsTree_->Branch(  "BdK2Pz_nofit" 		  , &BdK2Pz_nofit_,                     "BdK2Pz_nofit/D");                                              
bsTree_->Branch(  "BdK2Eta_nofit" 		  , &BdK2Eta_nofit_,                    "BdK2Eta_nofit/D");                                             
bsTree_->Branch(  "BdK2Phi_nofit" 		  , &BdK2Phi_nofit_,                    "BdK2Phi_nofit/D");                                             
bsTree_->Branch(  "BdK2Key_nofit" 		  , &BdK2Key_nofit_,                     "BdK2Key_nofit/I"); 

bsTree_->Branch(  "BdPVx_refit" ,  &BdPVx_refit_ ,  "BdPVx_refit/D" );    
bsTree_->Branch(  "BdPVy_refit" ,&BdPVy_refit_ ,    "BdPVy_refit/D");   
bsTree_->Branch(  "BdPVz_refit" ,  &BdPVz_refit_ ,  "BdPVz_refit/D" );              
bsTree_->Branch(  "BdPVerrx_refit" , &BdPVerrx_refit_ , "BdPVerrx_refit/D" ); 
bsTree_->Branch(  "BdPVerry_refit" ,&BdPVerry_refit_ ,"BdPVerry_refit/D" ); 
bsTree_->Branch(  "BdPVerrz_refit" , &BdPVerrz_refit_ , "BdPVerrz_refit/D" ); 
                                             
bsTree_->Branch(  "BdLxy"			  , &BdLxy_,                            "BdLxy/D");                                                     
bsTree_->Branch(  "BdLxyErr"			  , &BdLxyErr_,                            "BdLxyErr/D");                                                     
bsTree_->Branch(  "BdErrX"			  , &BdErrX_,                           "BdErrX/D");                                                    
bsTree_->Branch(  "BdErrY"			  , &BdErrY_,                           "BdErrY/D");                                                    
bsTree_->Branch(  "BdErrXY"			  , &BdErrXY_,                          "BdErrXY/D");                                                   
bsTree_->Branch(  "BdCt"			  , &BdCt_,                             "BdCt/D");                                                      
bsTree_->Branch(  "BdCtErr"			  , &BdCtErr_,                           "BdCtErr/D");                                                      
bsTree_->Branch(  "BdDist3d"			  , &BdDist3d_,                         "BdDist3d/D");                                                  
bsTree_->Branch(  "BdDist3dErr"			  , &BdDist3dErr_,                      "BdDist3dErr/D");                                               
bsTree_->Branch(  "BdTime3d"			  , &BdTime3d_,                         "BdTime3d/D");                                                  
bsTree_->Branch(  "BdTime3dErr"			  , &BdTime3dErr_,                      "BdTime3dErr/D");                                               
bsTree_->Branch(  "BdDist2d"			  , &BdDist2d_,                         "BdDist2d/D");                                                  
bsTree_->Branch(  "BdDist2dErr"			  , &BdDist2dErr_,                      "BdDist2dErr/D");                                               
bsTree_->Branch(  "BdTime2d"			  , &BdTime2d_,                         "BdTime2d/D");                                                  
bsTree_->Branch(  "BdTime2dErr"                   , &BdTime2dErr_,                      "BdTime2dErr/D");                                               
 bsTree_->Branch(  "BdK1mcId"     ,       &BdK1mcId_     ,  "BdK1mcId/I"    );
 bsTree_->Branch(  "BdK1momId"	  ,	  &BdK1momId_    ,  "BdK1momId/I"   );
 bsTree_->Branch(  "BdK1gmomId"	  ,	  &BdK1gmomId_   ,  "BdK1gmomId/I"  );
 bsTree_->Branch(  "BdK2mcId"	  ,	  &BdK2mcId_     ,  "BdK2mcId/I"    );
 bsTree_->Branch(  "BdK2momId"	  ,	  &BdK2momId_    ,  "BdK2momId/I"   );
 bsTree_->Branch(  "BdK2gmomId"	  ,	  &BdK2gmomId_   ,  "BdK2gmomId/I"  );
 bsTree_->Branch(  "BdMu1mcId"	  ,	  &BdMu1mcId_    ,  "BdMu1mcId/I"   );
 bsTree_->Branch(  "BdMu1momId"	  ,	  &BdMu1momId_   ,  "BdMu1momId/I"  );
 bsTree_->Branch(  "BdMu1gmomId"  ,	  &BdMu1gmomId_  ,  "BdMu1gmomId/I" );
 bsTree_->Branch(  "BdMu2mcId"	  ,	  &BdMu2mcId_    ,  "BdMu2mcId/I"   );
 bsTree_->Branch(  "BdMu2momId"	  ,	  &BdMu2momId_   ,  "BdMu2momId/I"  );
 bsTree_->Branch(  "BdMu2gmomId"  ,	  &BdMu2gmomId_  ,  "BdMu2gmomId/I" );

}

BsToJpsiPhiRootTree::~BsToJpsiPhiRootTree()
{}

void BsToJpsiPhiRootTree::writeFile()
{
  bsFile_->Write();
  bsFile_->Close();
  
}

void BsToJpsiPhiRootTree::resetEntries()
{


  triggerbit_HLTmu3_ = -9999999;
  triggerbit_HLTmu5_ = -9999999;
  triggerbit_HLTmu9_ = -9999999;
  triggerbit_HLTdoubleIsoMu3_ = -9999999;
  triggerbit_HLTdoubleMu3_ = -9999999;
  triggerbit_HLTdoubleMu0_ = -9999999;
  triggerbit_HLTdoubleMu3_JPsi_ = -9999999;
  BSx_ = -9999999;
  BSy_ = -9999999;
  BSz_ = -9999999;
  PVx_ = -9999999;
  PVy_ = -9999999;
  PVz_ = -9999999;
  PVerrx_ = -9999999;
  PVerry_ = -9999999;
  PVerrz_ = -9999999;

  PVx_refit_= -9999999; 
  PVy_refit_= -9999999;
  PVz_refit_= -9999999;   
  PVerrx_refit_= -9999999;
  PVerry_refit_= -9999999;
  PVerrz_refit_= -9999999;


  JpsiVtxProb_ = -9999999;
  JpsiM_alone_ = -9999999;
  JpsiPhi_alone_ = -9999999;
  JpsiEta_alone_ = -9999999;
  JpsiPt_alone_ = -9999999;
  JpsiMu1Pt_alone_ = -9999999;
  JpsiMu2Pt_alone_ = -9999999;
  JpsiMu1Phi_alone_ = -9999999;
  JpsiMu2Phi_alone_ = -9999999;
  JpsiMu1Eta_alone_ = -9999999;
  JpsiMu2Eta_alone_ = -9999999;
  JpsiMuon1Cat_alone_ = -9999999;
  JpsiMuon2Cat_alone_ = -9999999;
  BsFitChi2_ = -9999999;
  BsFitNdof_ = -9999999;
  BsFitVtxProb_ = -9999999;
  BsNumberOfCandidates_ =  0;
  BsFitM_ = -9999999;
  BsFitEta_ = -9999999;
  BsFitPt_ = -9999999;
  BsFitPz_ = -9999999;
  BsFitPhi_ = -9999999;
  BsFitVtx_x_ = -9999999;
  BsFitVtx_y_ = -9999999;
  BsFitVtx_z_ = -9999999;
  BsM_nofit_ = -9999999;
  BsPhi_nofit_ = -9999999;
  BsEta_nofit_ = -9999999;
  BsPt_nofit_ = -9999999;
  BsPz_nofit_ = -9999999;
  JpsiM_nofit_ = -9999999;
  JpsiPhi_nofit_ = -9999999;
  JpsiEta_nofit_ = -9999999;
  JpsiPt_nofit_ = -9999999;
  JpsiPz_nofit_ = -9999999;
  PhiM_nofit_ = -9999999;
  PhiPhi_nofit_ = -9999999;
  PhiEta_nofit_ = -9999999;
  PhiPt_nofit_ = -9999999;
  PhiPz_nofit_ = -9999999;
  K1Pt_nofit_ = -9999999;
  K1Pz_nofit_ = -9999999;
  K1Eta_nofit_ = -9999999;
  K1Phi_nofit_ = -9999999;
 K1Key_nofit_ = -9999999;
  K2Eta_nofit_ = -9999999;
  K2Pt_nofit_ = -9999999;
  K2Pz_nofit_ = -9999999;
  K2Phi_nofit_ = -9999999;
 K2Key_nofit_ = -9999999;
  K1Chi2_ = -9999999;
  K1nHits_ = -9999999;
  K2Chi2_ = -9999999;
  K2nHits_ = -9999999;
  K1pixH_ = -9999999;
  K1trkH_ = -9999999;
  K2pixH_ = -9999999;
  K2trkH_ = -9999999;
  Mu1Chi2_ = -9999999;
  Mu1nHits_ = -9999999;
  Mu2Chi2_ = -9999999;
  Mu2nHits_ = -9999999;
  Mu1pixH_ = -9999999;
  Mu1trkH_ = -9999999;
  Mu2pixH_ = -9999999;
  Mu2trkH_ = -9999999;
  costheta_ = -9999999;
  phi_ = -9999999;
  cospsi_ = -9999999;
  AngleBsDecayLength_ = -9999999;
  isMatched_ = -9999999;
  isMatchedBd_ = -9999999;
  BsLxy_ = -9999999;
  BsLxyErr_ = -9999999;
  BsErrX_ = -9999999;
  BsErrY_ = -9999999;
  BsErrXY_ = -9999999;
  BsCt_ = -9999999;
  BsCtErr_ = -9999999;
  K1trkLay_ = -9999999;
  K1muDTh_ = -9999999;
  K1muCSCh_ = -9999999;
  K1muRPCh_ = -9999999;
  K2trkLay_ = -9999999;
  K2muDTh_ = -9999999;
  K2muCSCh_ = -9999999;
  K2muRPCh_ = -9999999;
  Mu1trkLay_ = -9999999;
  Mu1muDTh_ = -9999999;
  Mu1muCSCh_ = -9999999;
  Mu1muRPCh_ = -9999999;
  Mu2trkLay_ = -9999999;
  Mu2muDTh_ = -9999999;
  Mu2muCSCh_ = -9999999;
  Mu2muRPCh_ = -9999999;
  K1mcId_ = -9999999;
  K1momId_ = -9999999;
  K1gmomId_ = -9999999;
  K2mcId_ = -9999999;
  K2momId_ = -9999999;
  K2gmomId_ = -9999999;
  Mu1mcId_ = -9999999;
  Mu1momId_ = -9999999;
  Mu1gmomId_ = -9999999;
  Mu2mcId_ = -9999999;
  Mu2momId_ = -9999999;
  Mu2gmomId_ = -9999999;

  BsDist3d_ = -9999999;
  BsDist3dErr_ = -9999999;
  BsTime3d_ = -9999999;
  BsTime3dErr_ = -9999999;
  BsDist2d_ = -9999999;
  BsDist2dErr_ = -9999999;
  BsTime2d_ = -9999999;
  BsTime2dErr_ = -9999999;
  dedxTrk_ = -9999999;
  errdedxTrk_ = -9999999;
  numdedxTrk_ = -9999999;
  iPassedCutIdent_ = -9999999;
  iPassedCutIdentBd_ = -9999999;
 
  K1Fit_sigX_ = -9999999;
  K1Fit_sigY_ = -9999999;
  K1Fit_sigZ_ = -9999999;
  K2Fit_sigX_ = -9999999;
  K2Fit_sigY_ = -9999999;
  K2Fit_sigZ_ = -9999999;
  K1Fit_sigPX_ = -9999999;
  K1Fit_sigPY_ = -9999999;
  K1Fit_sigPZ_ = -9999999;
  K2Fit_sigPX_ = -9999999;
  K2Fit_sigPY_ = -9999999;
  K2Fit_sigPZ_ = -9999999;
 
  GenNumberOfBdecays_ = -9999999;

  genBsVtx_z_ = -9999999;
  genBsVtx_y_ = -9999999;
  genBsVtx_x_ = -9999999;
  genBsSVtx_z_ = -9999999;
  genBsSVtx_y_ = -9999999; 
  genBsSVtx_x_ = -9999999;
  isGenJpsiEvent_ = -9999999;
  BdFitChi2_Hyp1_ = -9999999;
  BdFitNdof_Hyp1_ = -9999999;
  BdFitVtxProb_Hyp1_ = -9999999;
  BdFitM_Hyp1_ = -9999999;
  BdFitEta_Hyp1_ = -9999999;
  BdFitPt_Hyp1_ = -9999999;
  BdFitPz_Hyp1_ = -9999999;
  BdFitPhi_Hyp1_ = -9999999;
  BdFitVtx_x_Hyp1_ = -9999999;
  BdFitVtx_y_Hyp1_ = -9999999;
  BdFitVtx_z_Hyp1_ = -9999999;
  BdM_nofit_ = -9999999;
  BdPhi_nofit_ = -9999999;
  BdEta_nofit_ = -9999999;
  BdPt_nofit_ = -9999999;
  BdPz_nofit_ = -9999999;
  KstarMass_nofit_Hyp1_ = -9999999;
  KstarMass_nofit_Hyp2_ = -9999999;
  BdK1_kpi_sigX_Hyp1_ = -9999999;
  BdK1_kpi_sigY_Hyp1_ = -9999999;
  BdK1_kpi_sigZ_Hyp1_ = -9999999;
  BdK2_kpi_sigX_Hyp1_ = -9999999;
  BdK2_kpi_sigY_Hyp1_ = -9999999;
  BdK2_kpi_sigZ_Hyp1_ = -9999999;
  BdK1_kpi_sigPX_Hyp1_ = -9999999;
  BdK1_kpi_sigPY_Hyp1_ = -9999999;
  BdK1_kpi_sigPZ_Hyp1_ = -9999999;
  BdK2_kpi_sigPX_Hyp1_ = -9999999;
  BdK2_kpi_sigPY_Hyp1_ = -9999999;
  BdK2_kpi_sigPZ_Hyp1_ = -9999999;
  BdFitChi2_Hyp2_ = -9999999;
  BdFitNdof_Hyp2_ = -9999999;
  BdFitVtxProb_Hyp2_ = -9999999;
  BdFitM_Hyp2_ = -9999999;
  BdFitEta_Hyp2_ = -9999999;
  BdFitPt_Hyp2_ = -9999999;
  BdFitPz_Hyp2_ = -9999999;
  BdFitPhi_Hyp2_ = -9999999;
  BdFitVtx_x_Hyp2_ = -9999999;
  BdFitVtx_y_Hyp2_ = -9999999;
  BdFitVtx_z_Hyp2_ = -9999999;
  BdNumberOfCandidates_ =  0; 

BdPVx_refit_    = -9999999;
BdPVy_refit_    = -9999999;
BdPVz_refit_    = -9999999;
               
BdPVerrx_refit_ = -9999999;
BdPVerry_refit_ = -9999999;
BdPVerrz_refit_ = -9999999;

  BdK1_kpi_sigX_Hyp2_ = -9999999;
  BdK1_kpi_sigY_Hyp2_ = -9999999;
  BdK1_kpi_sigZ_Hyp2_ = -9999999;
  BdK2_kpi_sigX_Hyp2_ = -9999999;
  BdK2_kpi_sigY_Hyp2_ = -9999999;
  BdK2_kpi_sigZ_Hyp2_ = -9999999;
  BdK1_kpi_sigPX_Hyp2_ = -9999999;
  BdK1_kpi_sigPY_Hyp2_ = -9999999;
  BdK1_kpi_sigPZ_Hyp2_ = -9999999;
  BdK2_kpi_sigPX_Hyp2_ = -9999999;
  BdK2_kpi_sigPY_Hyp2_ = -9999999;
  BdK2_kpi_sigPZ_Hyp2_ = -9999999;
  BdK1Pt_nofit_ = -9999999; 
  BdK1Pz_nofit_ = -9999999; 
  BdK1Eta_nofit_ = -9999999; 
  BdK1Phi_nofit_ = -9999999; 
 BdK1Key_nofit_ = -9999999; 
  BdK2Pt_nofit_ = -9999999; 
  BdK2Pz_nofit_ = -9999999; 
  BdK2Eta_nofit_ = -9999999; 
  BdK2Phi_nofit_ = -9999999; 
  BdK2Key_nofit_ = -9999999; 
  BdLxy_ = -9999999;
  BdLxyErr_ = -9999999;
  BdErrX_ = -9999999;
  BdErrY_ = -9999999;
  BdErrXY_ = -9999999;
  BdCt_ = -9999999;
  BdCtErr_ = -9999999;
  BdDist3d_ = -9999999;
  BdDist3dErr_ = -9999999;
  BdTime3d_ = -9999999;
  BdTime3dErr_ = -9999999;
  BdDist2d_ = -9999999;
  BdDist2dErr_ = -9999999;
  BdTime2d_ = -9999999;
  BdTime2dErr_ = -9999999;                                    

BdK1mcId_    = -9999999;
BdK1momId_   = -9999999;
BdK1gmomId_  = -9999999;
BdK2mcId_    = -9999999;
BdK2momId_   = -9999999;
BdK2gmomId_  = -9999999;
BdMu1mcId_   = -9999999;
BdMu1momId_  = -9999999;
BdMu1gmomId_ = -9999999;
BdMu2mcId_   = -9999999;
BdMu2momId_  = -9999999;
BdMu2gmomId_ = -9999999;


  for(int i=0; i<7; i++){
    K1Fit_par_[i] = -9999999;
    K2Fit_par_[i] = -9999999;

    BdK1_kpi_par_Hyp1_[i] = -9999999;
    BdK2_kpi_par_Hyp1_[i] = -9999999;
    BdK1_kpi_par_Hyp2_[i] = -9999999;
    BdK2_kpi_par_Hyp2_[i] = -9999999;
  }




  for(int i=0; i<10; i++){ 
    BmesonsId_[i]  =  -9999999;
  
    BMMC_[i] =  -9999999;
    BPtMC_[i] =  -9999999;
    BPzMC_[i] =  -9999999;
    BEtaMC_[i] =  -9999999;
    BPhiMC_[i] =  -9999999;

    BVtxMC_x_[i] =  -9999999;
    BVtxMC_y_[i]  =  -9999999;
    BVtxMC_z_[i]  =  -9999999;
    BSVtxMC_x_[i] =  -9999999;
    BSVtxMC_y_[i] =  -9999999;
    BSVtxMC_z_[i] =  -9999999;
    BLxy_MC_[i]   =  -9999999;
    BCt_MC_[i]    =  -9999999;
    
    GenNumberOfDaughters_[i] =  -9999999;

    for(int j=0;j<15;j++){
      BDauIdMC_[i][j]= -9999999;       
	
      BDauMMC_[i][j]= -9999999;  
      BDauPtMC_[i][j]= -9999999; 
      BDauPzMC_[i][j]= -9999999; 
      BDauEtaMC_[i][j]= -9999999;
      BDauPhiMC_[i][j]= -9999999;
      
      GenNumberOfDaughtersDaughters_[i][j] =  -9999999;
      
      for(int k=0; k<10; k++){
	BDauDauIdMC_[i][j][k]= -9999999;
	
	BDauDauMMC_[i][j][k]= -9999999;  
	BDauDauPtMC_[i][j][k]= -9999999; 
	BDauDauPzMC_[i][j][k]= -9999999; 
	BDauDauEtaMC_[i][j][k]= -9999999;
	BDauDauPhiMC_[i][j][k]= -9999999;
      }
    }
  }

 
 
    
  
} 

void BsToJpsiPhiRootTree::getDeDx(const double f1, const double f2, const int f3)
{
  dedxTrk_ = f1;
  errdedxTrk_ = f2;
  numdedxTrk_ = f3;
}





void BsToJpsiPhiRootTree::getVtx(const double aa, const double bb, const double cc, const double dd, const double ee, const double ff,
				 const double gg, const double hh, const double ii)
{
  BSx_ = aa;
  BSy_ = bb;
  BSz_ = cc;
  PVx_ = dd;
  PVy_ = ee;
  PVz_ = ff;
  PVerrx_ = gg;
  PVerry_ = hh;
  PVerrz_ = ii;
}





void BsToJpsiPhiRootTree::getAngles(const double aa, const double bb, const double cc, const double dd)
{
  costheta_ = aa;
  phi_ = bb;
  cospsi_ = cc;
  AngleBsDecayLength_ = dd;
}







void BsToJpsiPhiRootTree::fill()
{
  bsTree_->Fill();
}






void BsToJpsiPhiRootTree::readTree(const std::string filename)
{

  // open root file
  bsFile_ = new TFile (filename.c_str(), "READ" );
 
  // create tree structure
  bsTree_ =  (TTree*) bsFile_->Get("BsTree");
  
  setBranchAddresses();
}

void BsToJpsiPhiRootTree::readTree(std::vector<std::string> filenames){
  TChain * myChain = new TChain("BsTree");
  for(std::vector<std::string>::iterator it = filenames.begin(); it!=filenames.end(); it++){
    myChain->Add( (*it).c_str());
  }

  bsTree_ = myChain;
  setBranchAddresses();
}

void BsToJpsiPhiRootTree::setBranchAddresses(){


bsTree_->SetBranchAddress(  "triggerbit_HLTmu3"             , &triggerbit_HLTmu3_  );                    
bsTree_->SetBranchAddress(  "triggerbit_HLTmu5"		  , &triggerbit_HLTmu5_  );                                   
bsTree_->SetBranchAddress(  "triggerbit_HLTmu9"		  , &triggerbit_HLTmu9_  );                                 
bsTree_->SetBranchAddress(  "triggerbit_HLTdoubleIsoMu3"	  , &triggerbit_HLTdoubleIsoMu3_  );                           
bsTree_->SetBranchAddress(  "triggerbit_HLTdoubleMu3"	  , &triggerbit_HLTdoubleMu3_  );                 
bsTree_->SetBranchAddress(  "triggerbit_HLTdoubleMu0"	  , &triggerbit_HLTdoubleMu0_  );                        
bsTree_->SetBranchAddress(  "triggerbit_HLTdoubleMu3_JPsi"  , &triggerbit_HLTdoubleMu3_JPsi_  );                         
bsTree_->SetBranchAddress(  "BSx"				  , &BSx_  );                                                
bsTree_->SetBranchAddress(  "BSy"				  , &BSy_  );                                               
bsTree_->SetBranchAddress(  "BSz"				  , &BSz_  );                                                  
bsTree_->SetBranchAddress(  "PVx"				  , &PVx_  );                                                  
bsTree_->SetBranchAddress(  "PVy"				  , &PVy_  );                                                  
bsTree_->SetBranchAddress(  "PVz"				  , &PVz_  );                                                  
bsTree_->SetBranchAddress(  "PVerrx"			  , &PVerrx_  );                                               
bsTree_->SetBranchAddress(  "PVerry"			  , &PVerry_  );                                               
bsTree_->SetBranchAddress(  "PVerrz"			  , &PVerrz_  ); 

bsTree_->SetBranchAddress( "PVx_refit"   ,         &PVx_refit_      ); 
bsTree_->SetBranchAddress( "PVy_refit"   ,	 &PVy_refit_        );
bsTree_->SetBranchAddress( "PVz_refit"   ,	 &PVz_refit_        );
bsTree_->SetBranchAddress( "PVerrx_refit",	 &PVerrx_refit_     );
bsTree_->SetBranchAddress( "PVerry_refit",	 &PVerry_refit_     );
bsTree_->SetBranchAddress( "PVerrz_refit",	 &PVerrz_refit_     );
                                              
bsTree_->SetBranchAddress(  "JpsiVtxProb"			  , &JpsiVtxProb_  );                                          
bsTree_->SetBranchAddress(  "JpsiM_alone"			  , &JpsiM_alone_  );                                          
bsTree_->SetBranchAddress(  "JpsiPhi_alone"		  , &JpsiPhi_alone_  );                                        
bsTree_->SetBranchAddress(  "JpsiEta_alone"		  , &JpsiEta_alone_  );                                        
bsTree_->SetBranchAddress(  "JpsiPt_alone"		  , &JpsiPt_alone_  );                                         
bsTree_->SetBranchAddress(  "JpsiMu1Pt_alone"		  , &JpsiMu1Pt_alone_  );                                      
bsTree_->SetBranchAddress(  "JpsiMu2Pt_alone"		  , &JpsiMu2Pt_alone_  );                                      
bsTree_->SetBranchAddress(  "JpsiMu1Phi_alone"		  , &JpsiMu1Phi_alone_  );                                     
bsTree_->SetBranchAddress(  "JpsiMu2Phi_alone"		  , &JpsiMu2Phi_alone_  );                                     
bsTree_->SetBranchAddress(  "JpsiMu1Eta_alone"		  , &JpsiMu1Eta_alone_  );                                     
bsTree_->SetBranchAddress(  "JpsiMu2Eta_alone"		  , &JpsiMu2Eta_alone_  );                                     
bsTree_->SetBranchAddress(  "JpsiMuon1Cat_alone"		  , &JpsiMuon1Cat_alone_  );                                
bsTree_->SetBranchAddress(  "JpsiMuon2Cat_alone"		  , &JpsiMuon2Cat_alone_  );                                
bsTree_->SetBranchAddress(  "BsFitChi2"			  , &BsFitChi2_  );                                            
bsTree_->SetBranchAddress(  "BsFitNdof"			  , &BsFitNdof_  );                                           
bsTree_->SetBranchAddress(  "BsFitVtxProb"		  , &BsFitVtxProb_  );                                         
bsTree_->SetBranchAddress(  "BsNumberOfCandidates"        , &BsNumberOfCandidates_);

bsTree_->SetBranchAddress(  "BsFitM"			  , &BsFitM_  );                                            
bsTree_->SetBranchAddress(  "BsFitEta"			  , &BsFitEta_  );                                             
bsTree_->SetBranchAddress(  "BsFitPt"			  , &BsFitPt_  );                                              
bsTree_->SetBranchAddress(  "BsFitPz"			  , &BsFitPz_  );                                              
bsTree_->SetBranchAddress(  "BsFitPhi"			  , &BsFitPhi_  );                                             
bsTree_->SetBranchAddress(  "BsFitVtx_x"			  , &BsFitVtx_x_  );                                           
bsTree_->SetBranchAddress(  "BsFitVtx_y"			  , &BsFitVtx_y_  );                                           
bsTree_->SetBranchAddress(  "BsFitVtx_z"			  , &BsFitVtx_z_  );                                           
bsTree_->SetBranchAddress(  "BsM_nofit"			  , &BsM_nofit_  );                                            
bsTree_->SetBranchAddress(  "BsPhi_nofit"			  , &BsPhi_nofit_  );                                          
bsTree_->SetBranchAddress(  "BsEta_nofit"			  , &BsEta_nofit_  );                                          
bsTree_->SetBranchAddress(  "BsPt_nofit"			  , &BsPt_nofit_  );                                           
bsTree_->SetBranchAddress(  "BsPz_nofit"			  , &BsPz_nofit_  );                                           
bsTree_->SetBranchAddress(  "JpsiM_nofit"			  , &JpsiM_nofit_  );                                          
bsTree_->SetBranchAddress(  "JpsiPhi_nofit"		  , &JpsiPhi_nofit_  );                                        
bsTree_->SetBranchAddress(  "JpsiEta_nofit"		  , &JpsiEta_nofit_  );                                        
bsTree_->SetBranchAddress(  "JpsiPt_nofit"		  , &JpsiPt_nofit_  );                                         
bsTree_->SetBranchAddress(  "JpsiPz_nofit"		  , &JpsiPz_nofit_  );                                         
bsTree_->SetBranchAddress(  "PhiM_nofit"			  , &PhiM_nofit_  );                                           
bsTree_->SetBranchAddress(  "PhiPhi_nofit"		  , &PhiPhi_nofit_  );                                         
bsTree_->SetBranchAddress(  "PhiEta_nofit"		  , &PhiEta_nofit_  );                                         
bsTree_->SetBranchAddress(  "PhiPt_nofit"			  , &PhiPt_nofit_  );                                          
bsTree_->SetBranchAddress(  "PhiPz_nofit"			  , &PhiPz_nofit_  );                                          
bsTree_->SetBranchAddress(  "K1Pt_nofit"			  , &K1Pt_nofit_  );                                           
bsTree_->SetBranchAddress(  "K1Pz_nofit"			  , &K1Pz_nofit_  );                                           
bsTree_->SetBranchAddress(  "K1Eta_nofit"			  , &K1Eta_nofit_  );                                          
bsTree_->SetBranchAddress(  "K1Phi_nofit"			  , &K1Phi_nofit_  );                                          
bsTree_->SetBranchAddress(  "K1Key_nofit"			  , &K1Key_nofit_  );                                           
bsTree_->SetBranchAddress(  "K2Eta_nofit"			  , &K2Eta_nofit_  );                                          
bsTree_->SetBranchAddress(  "K2Pt_nofit"			  , &K2Pt_nofit_  );                                           
bsTree_->SetBranchAddress(  "K2Pz_nofit"			  , &K2Pz_nofit_  );                                           
bsTree_->SetBranchAddress(  "K2Phi_nofit"			  , &K2Phi_nofit_  );                                          
bsTree_->SetBranchAddress(  "K2Key_nofit"			  , &K2Key_nofit_  );                                           
bsTree_->SetBranchAddress(  "K1Chi2"			  , &K1Chi2_  );                                               
bsTree_->SetBranchAddress(  "K1nHits"			  , &K1nHits_  );                                           
bsTree_->SetBranchAddress(  "K2Chi2"			  , &K2Chi2_  );                                               
bsTree_->SetBranchAddress(  "K2nHits"			  , &K2nHits_  );                                              
bsTree_->SetBranchAddress(  "K1pixH"			  , &K1pixH_  );                                             
bsTree_->SetBranchAddress(  "K1trkH"			  , &K1trkH_  );                                            
bsTree_->SetBranchAddress(  "K2pixH"			  , &K2pixH_  );                                            
bsTree_->SetBranchAddress(  "K2trkH"			  , &K2trkH_  );                                            
bsTree_->SetBranchAddress(  "Mu1Chi2"			  , &Mu1Chi2_  );                                              
bsTree_->SetBranchAddress(  "Mu1nHits"			  , &Mu1nHits_  );                                            
bsTree_->SetBranchAddress(  "Mu2Chi2"			  , &Mu2Chi2_  );                                              
bsTree_->SetBranchAddress(  "Mu2nHits"			  , &Mu2nHits_  );                                          
bsTree_->SetBranchAddress(  "Mu1pixH"			  , &Mu1pixH_  );                                          
bsTree_->SetBranchAddress(  "Mu1trkH"			  , &Mu1trkH_  );                                           
bsTree_->SetBranchAddress(  "Mu2pixH"			  , &Mu2pixH_  );                                           
bsTree_->SetBranchAddress(  "Mu2trkH"			  , &Mu2trkH_  );                                           
bsTree_->SetBranchAddress(  "costheta"			  , &costheta_  );                                             
bsTree_->SetBranchAddress(  "phi"				  , &phi_  );                                                  
bsTree_->SetBranchAddress(  "cospsi"			  , &cospsi_  );                                               
bsTree_->SetBranchAddress(  "AngleBsDecayLength"		  , &AngleBsDecayLength_  );                                   
bsTree_->SetBranchAddress(  "isMatched"			  , &isMatched_  );                                           
bsTree_->SetBranchAddress(  "isMatchedBd"			  , &isMatchedBd_  );                                         
bsTree_->SetBranchAddress(  "BsLxy"			  , &BsLxy_  );                    
bsTree_->SetBranchAddress(  "BsLxyErr"			  , &BsLxyErr_  );                                                
bsTree_->SetBranchAddress(  "BsErrX"			  , &BsErrX_  );                                               
bsTree_->SetBranchAddress(  "BsErrY"			  , &BsErrY_  );                                               
bsTree_->SetBranchAddress(  "BsErrXY"			  , &BsErrXY_  );                                              
bsTree_->SetBranchAddress(  "BsCt"			  , &BsCt_  );                                                 
bsTree_->SetBranchAddress(  "BsCtErr"			  , &BsCtErr_  );                                                 
bsTree_->SetBranchAddress(  "K1trkLay"			  , &K1trkLay_  );                                             
bsTree_->SetBranchAddress(  "K1muDTh"			  , &K1muDTh_  );                                          
bsTree_->SetBranchAddress(  "K1muCSCh"			  , &K1muCSCh_  );                                           
bsTree_->SetBranchAddress(  "K1muRPCh"			  , &K1muRPCh_  );                                          
bsTree_->SetBranchAddress(  "K2trkLay"			  , &K2trkLay_  );                                          
bsTree_->SetBranchAddress(  "K2muDTh"			  , &K2muDTh_  );                                          
bsTree_->SetBranchAddress(  "K2muCSCh"			  , &K2muCSCh_  );                                           
bsTree_->SetBranchAddress(  "K2muRPCh"			  , &K2muRPCh_  );                                          
bsTree_->SetBranchAddress(  "Mu1trkLay"			  , &Mu1trkLay_  );                                          
bsTree_->SetBranchAddress(  "Mu1muDTh"			  , &Mu1muDTh_  );                                         
bsTree_->SetBranchAddress(  "Mu1muCSCh"			  , &Mu1muCSCh_  );                                          
bsTree_->SetBranchAddress(  "Mu1muRPCh"			  , &Mu1muRPCh_  );                                         
bsTree_->SetBranchAddress(  "Mu2trkLay"			  , &Mu2trkLay_  );                                         
bsTree_->SetBranchAddress(  "Mu2muDTh"			  , &Mu2muDTh_  );                                         
bsTree_->SetBranchAddress(  "Mu2muCSCh"			  , &Mu2muCSCh_  );                                          
bsTree_->SetBranchAddress(  "Mu2muRPCh"			  , &Mu2muRPCh_  );                                         
bsTree_->SetBranchAddress(  "K1mcId"			  , &K1mcId_  );                                             
bsTree_->SetBranchAddress(  "K1momId"			  , &K1momId_  );                                            
bsTree_->SetBranchAddress(  "K1gmomId"			  , &K1gmomId_  );                                           
bsTree_->SetBranchAddress(  "K2mcId"			  , &K2mcId_  );                                          
bsTree_->SetBranchAddress(  "K2momId"			  , &K2momId_  );                                            
bsTree_->SetBranchAddress(  "K2gmomId"			  , &K2gmomId_  );                                           
bsTree_->SetBranchAddress(  "Mu1mcId"			  , &Mu1mcId_  );                                          
bsTree_->SetBranchAddress(  "Mu1momId"			  , &Mu1momId_  );                                           
bsTree_->SetBranchAddress(  "Mu1gmomId"			  , &Mu1gmomId_  );                                          
bsTree_->SetBranchAddress(  "Mu2mcId"			  , &Mu2mcId_  );                                         
bsTree_->SetBranchAddress(  "Mu2momId"			  , &Mu2momId_  );                                           
bsTree_->SetBranchAddress(  "Mu2gmomId"			  , &Mu2gmomId_  );                                          
                    
bsTree_->SetBranchAddress(  "BsDist3d"			  , &BsDist3d_  );                                             
bsTree_->SetBranchAddress(  "BsDist3dErr"			  , &BsDist3dErr_  );                                          
bsTree_->SetBranchAddress(  "BsTime3d"			  , &BsTime3d_  );                                             
bsTree_->SetBranchAddress(  "BsTime3dErr"			  , &BsTime3dErr_  );                                          
bsTree_->SetBranchAddress(  "BsDist2d"			  , &BsDist2d_  );                                             
bsTree_->SetBranchAddress(  "BsDist2dErr"			  , &BsDist2dErr_  );                                          
bsTree_->SetBranchAddress(  "BsTime2d"			  , &BsTime2d_  );                                             
bsTree_->SetBranchAddress(  "BsTime2dErr"			  , &BsTime2dErr_  );                                          
bsTree_->SetBranchAddress(  "dedxTrk"			  , &dedxTrk_  );                                              
bsTree_->SetBranchAddress(  "errdedxTrk"			  , &errdedxTrk_  );                                           
bsTree_->SetBranchAddress(  "numdedxTrk"			  , &numdedxTrk_  );                                         
bsTree_->SetBranchAddress(  "iPassedCutIdent"		  , &iPassedCutIdent_  );                                    
bsTree_->SetBranchAddress(  "iPassedCutIdentBd"		  , &iPassedCutIdentBd_  );                                   
bsTree_->SetBranchAddress(  "K1Fit_par"			  , K1Fit_par_  );                                         
bsTree_->SetBranchAddress(  "K2Fit_par"			  , K2Fit_par_  );                                     
bsTree_->SetBranchAddress(  "K1Fit_sigX"			  , &K1Fit_sigX_  );                                           
bsTree_->SetBranchAddress(  "K1Fit_sigY"			  , &K1Fit_sigY_  );                                           
bsTree_->SetBranchAddress(  "K1Fit_sigZ"			  , &K1Fit_sigZ_  );                                           
bsTree_->SetBranchAddress(  "K2Fit_sigX"			  , &K2Fit_sigX_  );                                           
bsTree_->SetBranchAddress(  "K2Fit_sigY"			  , &K2Fit_sigY_  );                                           
bsTree_->SetBranchAddress(  "K2Fit_sigZ"			  , &K2Fit_sigZ_  );                                           
bsTree_->SetBranchAddress(  "K1Fit_sigPX"			  , &K1Fit_sigPX_  );                                          
bsTree_->SetBranchAddress(  "K1Fit_sigPY"			  , &K1Fit_sigPY_  );                                          
bsTree_->SetBranchAddress(  "K1Fit_sigPZ"			  , &K1Fit_sigPZ_  );                                          
bsTree_->SetBranchAddress(  "K2Fit_sigPX"			  , &K2Fit_sigPX_  );                                          
bsTree_->SetBranchAddress(  "K2Fit_sigPY"			  , &K2Fit_sigPY_  );                                          
bsTree_->SetBranchAddress(  "K2Fit_sigPZ"			  , &K2Fit_sigPZ_  );                                          
              
bsTree_->SetBranchAddress(  "GenNumberOfBdecays"		  , &GenNumberOfBdecays_  );                                   
bsTree_->SetBranchAddress(  "BmesonsId"			  , BmesonsId_  );                                
bsTree_->SetBranchAddress(  "BDauIdMC"			  , BDauIdMC_  );                                     
bsTree_->SetBranchAddress(  "BDauDauIdMC"			  , BDauDauIdMC_  );                                
bsTree_->SetBranchAddress(  "GenNumberOfDaughters"	  , GenNumberOfDaughters_  );                           
bsTree_->SetBranchAddress(  "GenNumberOfDaughtersDaughters" , GenNumberOfDaughtersDaughters_  );                
bsTree_->SetBranchAddress(  "BDauMMC"			  , BDauMMC_  );                          
bsTree_->SetBranchAddress(  "BDauPtMC"			  , BDauPtMC_  );                                     
bsTree_->SetBranchAddress(  "BDauPzMC"			  , BDauPzMC_  );                                 
bsTree_->SetBranchAddress(  "BDauEtaMC"			  , BDauEtaMC_  );                                 
bsTree_->SetBranchAddress(  "BDauPhiMC"			  , BDauPhiMC_  );                                
bsTree_->SetBranchAddress(  "BDauDauMMC"			  , BDauDauMMC_  );                                 
bsTree_->SetBranchAddress(  "BDauDauPtMC"			  , BDauDauPtMC_  );                             
bsTree_->SetBranchAddress(  "BDauDauPzMC"			  , BDauDauPzMC_  );                            
bsTree_->SetBranchAddress(  "BDauDauEtaMC"		  , BDauDauEtaMC_  );                            
bsTree_->SetBranchAddress(  "BDauDauPhiMC"		  , BDauDauPhiMC_  );                           
bsTree_->SetBranchAddress(  "BMMC"			  , BMMC_  );                             
bsTree_->SetBranchAddress(  "BPtMC"			  , BPtMC_  );                                         
bsTree_->SetBranchAddress(  "BPzMC"			  , BPzMC_  );                                        
bsTree_->SetBranchAddress(  "BEtaMC"			  , BEtaMC_  );                                        
bsTree_->SetBranchAddress(  "BPhiMC"			  , BPhiMC_  );        

bsTree_->SetBranchAddress(  "BVtxMC_x" , BVtxMC_x_); 
bsTree_->SetBranchAddress(  "BVtxMC_y" , BVtxMC_y_); 
bsTree_->SetBranchAddress(  "BVtxMC_z" , BVtxMC_z_); 
bsTree_->SetBranchAddress(  "BSVtxMC_x", BSVtxMC_x_);
bsTree_->SetBranchAddress(  "BSVtxMC_y", BSVtxMC_y_);
bsTree_->SetBranchAddress(  "BSVtxMC_z", BSVtxMC_z_);
bsTree_->SetBranchAddress(  "BLxy_MC"  , BLxy_MC_);  
bsTree_->SetBranchAddress(  "BCt_MC"   , BCt_MC_);   

                               
bsTree_->SetBranchAddress(  "genBsVtx_z"			  , &genBsVtx_z_  );                                           
bsTree_->SetBranchAddress(  "genBsVtx_y"			  , &genBsVtx_y_  );                                           
bsTree_->SetBranchAddress(  "genBsVtx_x"			  , &genBsVtx_x_  );                                           
bsTree_->SetBranchAddress(  "genBsSVtx_z"			  , &genBsSVtx_z_  );                                          
bsTree_->SetBranchAddress(  "genBsSVtx_y" 		  , &genBsSVtx_y_  );                                          
bsTree_->SetBranchAddress(  "genBsSVtx_x"			  , &genBsSVtx_x_  );                                          
bsTree_->SetBranchAddress(  "isGenJpsiEvent"		  , &isGenJpsiEvent_  );                                      
bsTree_->SetBranchAddress(  "BdFitChi2_Hyp1"		  , &BdFitChi2_Hyp1_  );                                       
bsTree_->SetBranchAddress(  "BdFitNdof_Hyp1"		  , &BdFitNdof_Hyp1_  );                                      
bsTree_->SetBranchAddress(  "BdFitVtxProb_Hyp1"		  , &BdFitVtxProb_Hyp1_  );                                    
bsTree_->SetBranchAddress(  "BdFitM_Hyp1"			  , &BdFitM_Hyp1_  );                                          
bsTree_->SetBranchAddress(  "BdFitEta_Hyp1"		  , &BdFitEta_Hyp1_  );                                        
bsTree_->SetBranchAddress(  "BdFitPt_Hyp1"		  , &BdFitPt_Hyp1_  );                                         
bsTree_->SetBranchAddress(  "BdFitPz_Hyp1"		  , &BdFitPz_Hyp1_  );                                         
bsTree_->SetBranchAddress(  "BdFitPhi_Hyp1"		  , &BdFitPhi_Hyp1_  );                                        
bsTree_->SetBranchAddress(  "BdFitVtx_x_Hyp1"		  , &BdFitVtx_x_Hyp1_  );                                      
bsTree_->SetBranchAddress(  "BdFitVtx_y_Hyp1"		  , &BdFitVtx_y_Hyp1_  );                                      
bsTree_->SetBranchAddress(  "BdFitVtx_z_Hyp1"		  , &BdFitVtx_z_Hyp1_  );                                      
bsTree_->SetBranchAddress(  "BdM_nofit"		  , &BdM_nofit_  );                                       
bsTree_->SetBranchAddress(  "BdPhi_nofit"		  , &BdPhi_nofit_  );                                     
bsTree_->SetBranchAddress(  "BdEta_nofit"		  , &BdEta_nofit_  );                                     
bsTree_->SetBranchAddress(  "BdPt_nofit"		  , &BdPt_nofit_  );                                      
bsTree_->SetBranchAddress(  "BdPz_nofit"		  , &BdPz_nofit_  );                                      
bsTree_->SetBranchAddress(  "KstarMass_nofit_Hyp1"	  , &KstarMass_nofit_Hyp1_  );  
bsTree_->SetBranchAddress(  "KstarMass_nofit_Hyp2"	  , &KstarMass_nofit_Hyp2_  );                                
bsTree_->SetBranchAddress(  "BdK1_kpi_par_Hyp1"		  , BdK1_kpi_par_Hyp1_  );                                 
bsTree_->SetBranchAddress(  "BdK2_kpi_par_Hyp1"		  , BdK2_kpi_par_Hyp1_  );                             
bsTree_->SetBranchAddress(  "BdK1_kpi_sigX_Hyp1"		  , &BdK1_kpi_sigX_Hyp1_  );                                   
bsTree_->SetBranchAddress(  "BdK1_kpi_sigY_Hyp1"		  , &BdK1_kpi_sigY_Hyp1_  );                                   
bsTree_->SetBranchAddress(  "BdK1_kpi_sigZ_Hyp1"		  , &BdK1_kpi_sigZ_Hyp1_  );                                   
bsTree_->SetBranchAddress(  "BdK2_kpi_sigX_Hyp1"		  , &BdK2_kpi_sigX_Hyp1_  );                                   
bsTree_->SetBranchAddress(  "BdK2_kpi_sigY_Hyp1"		  , &BdK2_kpi_sigY_Hyp1_  );                                   
bsTree_->SetBranchAddress(  "BdK2_kpi_sigZ_Hyp1"		  , &BdK2_kpi_sigZ_Hyp1_  );                                   
bsTree_->SetBranchAddress(  "BdK1_kpi_sigPX_Hyp1"		  , &BdK1_kpi_sigPX_Hyp1_  );                                  
bsTree_->SetBranchAddress(  "BdK1_kpi_sigPY_Hyp1"		  , &BdK1_kpi_sigPY_Hyp1_  );                                  
bsTree_->SetBranchAddress(  "BdK1_kpi_sigPZ_Hyp1"		  , &BdK1_kpi_sigPZ_Hyp1_  );                                  
bsTree_->SetBranchAddress(  "BdK2_kpi_sigPX_Hyp1"		  , &BdK2_kpi_sigPX_Hyp1_  );                                  
bsTree_->SetBranchAddress(  "BdK2_kpi_sigPY_Hyp1"		  , &BdK2_kpi_sigPY_Hyp1_  );                                  
bsTree_->SetBranchAddress(  "BdK2_kpi_sigPZ_Hyp1"		  , &BdK2_kpi_sigPZ_Hyp1_  );                                  
bsTree_->SetBranchAddress(  "BdFitChi2_Hyp2"		  , &BdFitChi2_Hyp2_  );                                       
bsTree_->SetBranchAddress(  "BdFitNdof_Hyp2"		  , &BdFitNdof_Hyp2_  );                                      
bsTree_->SetBranchAddress(  "BdFitVtxProb_Hyp2"		  , &BdFitVtxProb_Hyp2_  );                                    
bsTree_->SetBranchAddress(  "BdFitM_Hyp2"			  , &BdFitM_Hyp2_  );                                          
bsTree_->SetBranchAddress(  "BdFitEta_Hyp2"		  , &BdFitEta_Hyp2_  );                                        
bsTree_->SetBranchAddress(  "BdFitPt_Hyp2"		  , &BdFitPt_Hyp2_  );                                         
bsTree_->SetBranchAddress(  "BdFitPz_Hyp2"		  , &BdFitPz_Hyp2_  );                                         
bsTree_->SetBranchAddress(  "BdFitPhi_Hyp2"		  , &BdFitPhi_Hyp2_  );                                        
bsTree_->SetBranchAddress(  "BdFitVtx_x_Hyp2"		  , &BdFitVtx_x_Hyp2_  );                                      
bsTree_->SetBranchAddress(  "BdFitVtx_y_Hyp2"		  , &BdFitVtx_y_Hyp2_  );                                      
bsTree_->SetBranchAddress(  "BdFitVtx_z_Hyp2"		  , &BdFitVtx_z_Hyp2_  );                                      
                  
bsTree_->SetBranchAddress(  "BdPVx_refit",      &BdPVx_refit_   );     
bsTree_->SetBranchAddress(  "BdPVy_refit",      &BdPVy_refit_   );     
bsTree_->SetBranchAddress(  "BdPVz_refit",      &BdPVz_refit_   );     
bsTree_->SetBranchAddress(  "BdPVerrx_refit",   &BdPVerrx_refit_);     
bsTree_->SetBranchAddress(  "BdPVerry_refit",   &BdPVerry_refit_);     
bsTree_->SetBranchAddress(  "BdPVerrz_refit",   &BdPVerrz_refit_);     
bsTree_->SetBranchAddress(  "BdNumberOfCandidates"        , &BdNumberOfCandidates_);
bsTree_->SetBranchAddress(  "BdK1_kpi_par_Hyp2"		  , BdK1_kpi_par_Hyp2_  );                                 
bsTree_->SetBranchAddress(  "BdK2_kpi_par_Hyp2"		  , BdK2_kpi_par_Hyp2_  );                             
bsTree_->SetBranchAddress(  "BdK1_kpi_sigX_Hyp2"		  , &BdK1_kpi_sigX_Hyp2_  );                                   
bsTree_->SetBranchAddress(  "BdK1_kpi_sigY_Hyp2"		  , &BdK1_kpi_sigY_Hyp2_  );                                   
bsTree_->SetBranchAddress(  "BdK1_kpi_sigZ_Hyp2"		  , &BdK1_kpi_sigZ_Hyp2_  );                                   
bsTree_->SetBranchAddress(  "BdK2_kpi_sigX_Hyp2"		  , &BdK2_kpi_sigX_Hyp2_  );                                   
bsTree_->SetBranchAddress(  "BdK2_kpi_sigY_Hyp2"		  , &BdK2_kpi_sigY_Hyp2_  );                                   
bsTree_->SetBranchAddress(  "BdK2_kpi_sigZ_Hyp2"		  , &BdK2_kpi_sigZ_Hyp2_  );                                   
bsTree_->SetBranchAddress(  "BdK1_kpi_sigPX_Hyp2"		  , &BdK1_kpi_sigPX_Hyp2_  );                                  
bsTree_->SetBranchAddress(  "BdK1_kpi_sigPY_Hyp2"		  , &BdK1_kpi_sigPY_Hyp2_  );                                  
bsTree_->SetBranchAddress(  "BdK1_kpi_sigPZ_Hyp2"		  , &BdK1_kpi_sigPZ_Hyp2_  );                                  
bsTree_->SetBranchAddress(  "BdK2_kpi_sigPX_Hyp2"		  , &BdK2_kpi_sigPX_Hyp2_  );                                  
bsTree_->SetBranchAddress(  "BdK2_kpi_sigPY_Hyp2"		  , &BdK2_kpi_sigPY_Hyp2_  );                                  
bsTree_->SetBranchAddress(  "BdK2_kpi_sigPZ_Hyp2"		  , &BdK2_kpi_sigPZ_Hyp2_  );                                  
bsTree_->SetBranchAddress(  "BdK1Pt_nofit" 		  , &BdK1Pt_nofit_  );                                         
bsTree_->SetBranchAddress(  "BdK1Pz_nofit" 		  , &BdK1Pz_nofit_  );                                         
bsTree_->SetBranchAddress(  "BdK1Eta_nofit" 		  , &BdK1Eta_nofit_  );                                        
bsTree_->SetBranchAddress(  "BdK1Phi_nofit" 		  , &BdK1Phi_nofit_  );  
bsTree_->SetBranchAddress(  "BdK1Key_nofit" 		  , &BdK1Key_nofit_  );                                                  
bsTree_->SetBranchAddress(  "BdK2Pt_nofit" 		  , &BdK2Pt_nofit_  );                                         
bsTree_->SetBranchAddress(  "BdK2Pz_nofit" 		  , &BdK2Pz_nofit_  );                                         
bsTree_->SetBranchAddress(  "BdK2Eta_nofit" 		  , &BdK2Eta_nofit_  );                                        
bsTree_->SetBranchAddress(  "BdK2Phi_nofit" 		  , &BdK2Phi_nofit_  );                                        
bsTree_->SetBranchAddress(  "BdK2Key_nofit" 		  , &BdK2Key_nofit_  );                                                  
bsTree_->SetBranchAddress(  "BdLxy"			  , &BdLxy_  );                                                
bsTree_->SetBranchAddress(  "BdLxyErr"			  , &BdLxyErr_  );                                                
bsTree_->SetBranchAddress(  "BdErrX"			  , &BdErrX_  );                                               
bsTree_->SetBranchAddress(  "BdErrY"			  , &BdErrY_  );                                               
bsTree_->SetBranchAddress(  "BdErrXY"			  , &BdErrXY_  );                                              
bsTree_->SetBranchAddress(  "BdCt"			  , &BdCt_  );                                                 
bsTree_->SetBranchAddress(  "BdCtErr"			  , &BdCtErr_  );                                                 
bsTree_->SetBranchAddress(  "BdDist3d"			  , &BdDist3d_  );                                             
bsTree_->SetBranchAddress(  "BdDist3dErr"			  , &BdDist3dErr_  );                                          
bsTree_->SetBranchAddress(  "BdTime3d"			  , &BdTime3d_  );                                             
bsTree_->SetBranchAddress(  "BdTime3dErr"			  , &BdTime3dErr_  );                                          
bsTree_->SetBranchAddress(  "BdDist2d"			  , &BdDist2d_  );                                             
bsTree_->SetBranchAddress(  "BdDist2dErr"			  , &BdDist2dErr_  );                                          
bsTree_->SetBranchAddress(  "BdTime2d"			  , &BdTime2d_  );                                             
bsTree_->SetBranchAddress(  "BdTime2dErr"                   , &BdTime2dErr_  );                                             

 bsTree_->SetBranchAddress(  "BdK1mcId"           ,       &BdK1mcId_     );
 bsTree_->SetBranchAddress(  "BdK1momId"	  ,	  &BdK1momId_    );
 bsTree_->SetBranchAddress(  "BdK1gmomId"	  ,	  &BdK1gmomId_   );
 bsTree_->SetBranchAddress(  "BdK2mcId"	          ,	  &BdK2mcId_     );
 bsTree_->SetBranchAddress(  "BdK2momId"	  ,	  &BdK2momId_    );
 bsTree_->SetBranchAddress(  "BdK2gmomId"	  ,	  &BdK2gmomId_   );
 bsTree_->SetBranchAddress(  "BdMu1mcId"	  ,	  &BdMu1mcId_    );
 bsTree_->SetBranchAddress(  "BdMu1momId"	  ,	  &BdMu1momId_   );
 bsTree_->SetBranchAddress(  "BdMu1gmomId"        ,	  &BdMu1gmomId_  );
 bsTree_->SetBranchAddress(  "BdMu2mcId"	  ,	  &BdMu2mcId_    );
 bsTree_->SetBranchAddress(  "BdMu2momId"	  ,	  &BdMu2momId_   );
 bsTree_->SetBranchAddress(  "BdMu2gmomId"        ,	  &BdMu2gmomId_  );


}
