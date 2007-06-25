// -*- C++ -*-
//
// Package:    EcalLaserShapeTools
// Class:      EcalLaserShapeTools
// 
/**\class EcalLaserShapeTools EcalLaserShapeTools.cc CalibCalorimetry/EcalLaserShapeTools/src/EcalLaserShapeTools.cc

 Description: <Aplha and Beta for Ecal channels from Laser events>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Patrick Jarry
//         Created:  Sun May 21 18:42:33 CEST 2006
// $Id: EcalLaserShapeTools.h,v 1.7 2006/08/15 00:05:11 franzoni Exp $
//
//


#include <memory>
#include <iostream>
#include <fstream>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/EDProduct.h" 
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "CalibCalorimetry/EcalPedestalOffsets/interface/TPedValues.h"
#include "CalibCalorimetry/EcalPedestalOffsets/interface/TPedResult.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include <DataFormats/EcalDigi/interface/EBDataFrame.h>
#include <DataFormats/EcalDetId/interface/EBDetId.h>
#include <DataFormats/EcalDigi/interface/EcalMGPASample.h>

#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/RunTag.h"
#include "OnlineDB/EcalCondDB/interface/RunDat.h"
#include "OnlineDB/EcalCondDB/interface/MonRunDat.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "OnlineDB/EcalCondDB/interface/LMFRunIOV.h"
#include "OnlineDB/EcalCondDB/interface/LMFRunTag.h"

#include "OnlineDB/EcalCondDB/interface/LMFLaserBlueShapeDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserIRedShapeDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserBlueCoeffDat.h"
#include "OnlineDB/EcalCondDB/interface/LMFLaserIRedCoeffDat.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TDirectory.h"

#define N_samples 10 
#define Nevt_num 600 

// gio: to be replaced by uncalRecHit (renamed for PN)
// gio: to be replaced by uncalRecHit (renamed for PN)
struct PNfit {
  double max;
  double t0;
};


struct matrice
{
  int nb_lignes ;
  int nb_colonnes ;
  double **coeff ;
};
typedef struct matrice matrice ;

using namespace cms;
using namespace edm;
using namespace std;

//
// class decleration
//

class EcalLaserShapeTools : public edm::EDAnalyzer {
 public:
  explicit EcalLaserShapeTools(const edm::ParameterSet&);
  ~EcalLaserShapeTools();
  
 protected:
  void beginJob (EventSetup const& eventSetup) ;
  void endJob (void) ;

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
 private:

  void beginRunDb( void  );
  string  dbName_;
  string  dbHostName_;
  int     dbHostPort_;
  string  dbUserName_;
  string  dbPassword_;
  string  location_;
  time_t current_time_;
  time_t last_time_;

  RunIOV runiov_;
  LMFRunIOV LMFiov_;
  int run_;

  // ----------member data ---------------------------

  PNfit                    maxPNDiode(const EcalPnDiodeDigi& pndigi);
  std::pair<int,int>  pnFromTT(const EcalTrigTowerDetId& tt) ;
  void                     initPNTTMap(void);

  unsigned long evtNum_;
  string rootFile_ ;
  string txtFile_ ;
  string histFile_ ;
  string hitCollection_ ;
  string hitProducer_ ;
  string digiCollection_ ;
  string digiProducer_ ;
  string pndiodeProducer_;

  
  int event_;
  int iphi_, jeta_;
  int ism_;
  int laser_color_;
  int  pn_amp_;
  
  // map of PN diodes as a function of Trig towers
  std::map<int, std::pair<int,int> > pnFromTTMap_;
  
  // a vector of vectors, each containing data from a crystal
  std::vector< std::vector <EBDataFrame >   >  blueData;
  std::vector< std::vector <EBDataFrame >   >  irData;
  std::vector< std::vector <float >   >        irPnData;
  std::vector< std::vector <float >   >        bluePnData;
  std::vector< TTree    >       treesToWrite;
  //  std::vector< TTree*    >       treesToWrite;


 



 matrice cree_mat(int,int) ;
 matrice cree_mat_prod(matrice,matrice) ;
 void fill_mat(matrice,matrice) ;
 matrice fill_mat_int(matrice,matrice,matrice) ;
  
  double fitpj(double *,int, const std::vector <EBDataFrame > &, const std::vector<float> &, std::vector < pair <float,float> > &) ; 
  double fitpj(double *,int) ; 
  
  void produit_mat(matrice,matrice,matrice) ;
  void produit_mat_int(matrice,matrice,matrice) ;
 void diff_mat(matrice,matrice,matrice) ;
 void somme_mat_int(matrice,matrice) ;
 void somme_mat_int_scale(matrice,matrice,double) ;
 void print_mat_nk(matrice,int) ;
 void print_mat(matrice) ;
 void transpose_mat(matrice,matrice) ;
 void inverse_mat(matrice,matrice) ;
 void copie_colonne_mat(matrice,matrice,int) ;
 char name_mat[10] ;
 void zero_mat(matrice) ;
 void zero_mat_nk(matrice,int) ;
 double parab(double *,Int_t,Int_t,double *) ;
 double inverpj(int,double g[N_samples][N_samples],double ginv[N_samples][N_samples]);
 double inv3x3(double a[3][3] , double b[3][3] ) ;
 double pulseShapepj( Double_t *, Double_t * ) ;
  // void FitGaus(int, TH1F* ,double * ,double *, double *) ;
 double ComputePulseWidth( double alpha_here , double beta_here ) ;


  // not sure all this is actually needed GIO fixme
   double adcval[Nevt_num][N_samples] ;
   double db_i[Nevt_num][2] ;
   double adc_read[1701][Nevt_num][10] ;
   int IPRINT ;
   int ns ; // number of samples
   int nb_iter ; // number of iterations to measure alpha,beta
   int nsmin ; // beginning of fit
   int nsmax ; // end of fit
   double a1ini ; // value of alpha at starting point
   double a2ini ; // value of alpha_prim/beta at starting point
   double a3ini ; // value of beta/alpha_prim at starting point
  
  double      alpha_[4][1700] ;
  double      beta_[4][1700] ;
  double      chi2_[4][1700] ;
  double      width_[4][1700] ;
  
   double sigma_rec ;
   int METHODE ;
 




};

//
// constants, enums and typedefs


//
// static data member definitions
//
