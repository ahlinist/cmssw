/**
 * 
 * \file EcalProfileBuilder.cc 
 * Determination of channel signal pulse profile to be used
 * in the calculation of optimized weights for Test Beam Anlysis
 *
 * NOTE: this version can only run on Laser data!!
 *
 * $Date: 2007/07/13 14:43:14 $
 * $Revision: 1.3 $
 * Alexandre Zabi - Imperial College
*/

#include "CalibCalorimetry/EcalWeightsTools/interface/EcalProfileBuilder.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//INFO
#include "TBDataFormats/EcalTBObjects/interface/EcalTBTDCRecInfo.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBEventHeader.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBHodoscopeRecInfo.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"

//To be done
// determine pedestal and check other code

EcalProfileBuilder::EcalProfileBuilder(edm::ParameterSet const& pSet)
{
  //verbosity
  verbosity_            = pSet.getUntrackedParameter("verbosity", 1U);

  //Prints out 
  debug_                = pSet.getUntrackedParameter<bool>("debug", false);

  //Digi Producer
  digiProducer_         = pSet.getUntrackedParameter<std::string>("digiProducer");

  //TDC Info 
  tdcRecInfoCollection_ = pSet.getUntrackedParameter<std::string>("tdcRecInfoCollection");
  tdcRecInfoProducer_   = pSet.getUntrackedParameter<std::string>("tdcRecInfoProducer");
  
  //Event Header
  eventHeaderCollection_ = pSet.getUntrackedParameter<std::string>("eventHeaderCollection");
  eventHeaderProducer_   = pSet.getUntrackedParameter<std::string>("eventHeaderProducer");

  //Hodoscope Info
  hodoRecInfoCollection_ = pSet.getUntrackedParameter<std::string>("hodoRecInfoCollection");
  hodoRecInfoProducer_   = pSet.getUntrackedParameter<std::string>("hodoRecInfoProducer");

  //SPECIFY WHICH SUPERMODULE
  SuperModule_          = pSet.getUntrackedParameter<unsigned int>("SuperModule", 0);

  //Name of output file
  rootfile_             = pSet.getUntrackedParameter<std::string>("rootfile","profile.root");
  Shapefile_            = pSet.getUntrackedParameter<std::string>("Shapefile","profile.txt");

  //XTAL CHOSEN
  //Only building profile for the crystal selected.
  //the "scanmode" must be set to false to use this option 
  xtalchosen_           = pSet.getUntrackedParameter<int>("xtalchosen", -1);
  if(xtalchosen_ == -1)
    std::cout << "NO XTAL CHOSEN, WILL TAKE XTAL IN BEAM" << std::endl;
  else std::cout << "XTAL CHOSEN =" << xtalchosen_ << std::endl;

  //Minimum number of entries per crystals
  nentry_               = pSet.getUntrackedParameter<int>("nentry", 0);

  //SCAN MODE
  //Building profile for all crystals readout.
  scanmode_             = pSet.getUntrackedParameter<bool>("scanmode", false);
  if(scanmode_) std::cout << "YOU ARE IN SCAN MODE, BUILDING PROFILE FOR ALL CRYSTALS IN BEAM" << std::endl;

  //HODOSCOPE WINDOW CONSIDERED
  HodoWindow_           = pSet.getUntrackedParameter<double>("HodoWindow", 0.0);
  //The hodoscope cuts depend on the run number
  run_number_          = pSet.getUntrackedParameter<int>("run_number", 0);

  //NUMBER OF ITERATIONS
  nIter_                = pSet.getUntrackedParameter<int>("nIter", 1);

  //INIT
  //initializing number of shapes produced and number of entries
  nShape_  = 1700;
  for(int y=0; y<1700; ++y) nEntries_[y]=0;
  maxIdRef_ = 999999;
  evt_cnt_ = 0;

  //RETRIEVING HODOSCOPE POSITION FROM A FILE
  std::ifstream hodoin("../test/hodoscope.out");
  std::cout << "LOOKING FOR HODOSCOPE CUTS IN RUN NUMBER=" << run_number_ << std::endl;
  double centerHodoX[1700];
  double centerHodoY[1700];
  for(int xt=0; xt<1700; ++xt)
    {
      centerHodoX[xt] = 99999.9; 
      centerHodoY[xt] = 99999.9;
      XlowerCut_[xt]  = -50.0;
      XupperCut_[xt]  = 50.0;
      YlowerCut_[xt]  = -50.0;
      YupperCut_[xt]  = 50.0;
    }//loop xtal

  int nXtalIn=0;
  hodoin >> nXtalIn;
  for(int xt=0; xt < nXtalIn; ++xt)
    {
      int runNb;
      int xtal;
      double centx;
      double centy;
      int entrieshodo;
      hodoin >> runNb >> xtal >> centx >> centy >> entrieshodo;
      if(runNb == run_number_){
	centerHodoX[xtal-1] = centx;
	centerHodoY[xtal-1] = centy;
	XlowerCut_[xtal-1] = centerHodoX[xtal-1] - HodoWindow_;
	XupperCut_[xtal-1] = centerHodoX[xtal-1] + HodoWindow_;
	YlowerCut_[xtal-1] = centerHodoY[xtal-1] - HodoWindow_;
	YupperCut_[xtal-1] = centerHodoY[xtal-1] + HodoWindow_;
      }//checking for correct run number
    }//loop xtal
  hodoin.close();
  std::cout << "HODOSCOPE WINDOW CONSIDERED is +/-" << HodoWindow_ << " mm" << std::endl; 

}//CONSTRUCTOR

EcalProfileBuilder::~EcalProfileBuilder()
{
  //ouptut file
  TFile* fOut = new TFile(rootfile_.c_str(), "recreate");

  std::cout << "STARTING BUILDING PROFILES" << std::endl;

  //HOW MANY CRYSTALS:
  std::vector<int> XtalIN;
  bool first = true;
  for (unsigned int Xtal = 0; Xtal < xtalInfo_.size(); ++Xtal) {
    //only taking crystals with a certain number of entries
    if(nEntries_[xtalInfo_[Xtal]-1] < nentry_) continue;

    //std::cout << "XTAL=" << xtalInfo_[Xtal] << std::endl;
    if(first) {XtalIN.push_back(xtalInfo_[Xtal]); first = false;}
    else 
      { bool takethisone = true;
	for(unsigned int i=0; i < XtalIN.size(); ++i)
	  if(xtalInfo_[Xtal] == XtalIN[i]) takethisone = false;
	if(takethisone) XtalIN.push_back(xtalInfo_[Xtal]);
      }
  }//loop crystals
  nShape_ = XtalIN.size();
  std::cout << "Number of crystals in that file = " <<  nShape_ << std::endl;
  for(unsigned int i=0; i < XtalIN.size(); ++i)
    std::cout << XtalIN[i] << " " << nEntries_[XtalIN[i]-1] << std::endl;

  //PROFILE OR TH2
  TH2F** h_Shape_TT   = new TH2F*[nShape_];
  //TProfile** h_Shape_TT = new TProfile*[nShape_];

  //DECLARING PROFILES
  for (unsigned int Xtal = 0; Xtal < XtalIN.size(); Xtal++) {
    std::string hTitle = Form("Shape Xtal%d", XtalIN[Xtal]);
    h_Shape_TT[Xtal] = new TH2F(Form("hShapeXtal_%d", XtalIN[Xtal]), hTitle.c_str(), 250,0,10,350,0,3500);
    //h_Shape_TT[Xtal] = new TProfile(Form("hShapeXtal_%d", XtalIN[Xtal]), hTitle.c_str(), 250,0,10," ");
  }//loop xtal
  
  //PEDESTALS
  //double *pedestals = new double[nShape_];
  //for(int i=0; i<nShape_;++i) pedestals[i]=0.0;
  std::vector<double> pedestals(nShape_,0.0);
  int start = 0;
  int end   = 11;
  for (unsigned int Xtal = 0; Xtal < xtalInfo_.size(); ++Xtal) {
    for(int i=start+1; i < end-7; ++i){
      for (unsigned int ch = 0; ch < XtalIN.size(); ++ch)
	if(XtalIN[ch] == xtalInfo_[Xtal])
	  pedestals[ch]+= shapeData_[i];
    }//loop info
    start+= 11;
    end  += 11;
  }//loop xtal

  for (unsigned int ch = 0; ch < XtalIN.size(); ++ch){
    pedestals[ch] /= double(nEntries_[XtalIN[ch]-1]*3.0);
    //std::cout << "XTAL=" << XtalIN[ch] << " PEDESTAL=" << pedestals[ch] 
    //	      << " " << double(nEntries_[XtalIN[ch]-1]) << std::endl;
  }//loop channels  

  //PROFILE FEATURES
  std::vector<double> profileMean(nShape_*250,0.0);
  std::vector<double> profileMax(nShape_,0.0);
  std::vector<int>    binMax(nShape_,9999999);
  std::vector<double> profileRms(nShape_*250,0.0);
  std::vector<double> profileData(nShape_*250,0.0);
  std::vector<int>    profileEntries(nShape_*250,0);
  std::vector<double> profileMeanSaved(nShape_*250,0.0);
  std::vector<double> profileRmsSaved(nShape_*250,0.0);

  double nRMS = 2;
  std::cout << "NUMBERS OF ITERATIONS=" << nIter_ << std::endl;
  std::cout << "INTERVAL CHOSEN = +/-"  << nRMS   << " RMS" << std::endl; 
  //Starting Iteration
  for(int iter=0; iter < nIter_; ++iter) {
    if(iter > 1) nRMS = 1.5;
    std::cout << "THIS IS ITERATION " << iter << std::endl;

    //Filling profiles
    for(int i=0; i<(nShape_*250);++i) profileData[i]=0.0;
    for(int i=0; i<(nShape_*250);++i) profileEntries[i]=0; 
    start = 0;
    end   = 11;
    for (unsigned int Xtal = 0; Xtal < xtalInfo_.size(); ++Xtal) {
      double tdc  = 0.0;
      int    indS = 0;
      for(int i=start; i < end; ++i){
	if(i == start){
	  tdc = shapeData_[i];
	}//tdc
	else {
	  for (unsigned int ch = 0; ch < XtalIN.size(); ch++) 
	    if(XtalIN[ch] == xtalInfo_[Xtal]){ 
	      double dataP = shapeData_[i]-pedestals[ch]; 
	      if(iter == 0) {
		profileData[ch*250+indS*25+int(tdc*25.0)] += dataP;
		profileEntries[ch*250+indS*25+int(tdc*25.0)]++; 
		h_Shape_TT[ch]->Fill(double(indS)+tdc,dataP);
	      }
	      else {
		if(dataP >= (profileMean[ch*250+indS*25+int(tdc*25.0)] - nRMS*profileRms[ch*250+indS*25+int(tdc*25.0)]) 
		   && dataP <= (profileMean[ch*250+indS*25+int(tdc*25.0)] + nRMS*profileRms[ch*250+indS*25+int(tdc*25.0)]))
		  {
		    profileData[ch*250+indS*25+int(tdc*25.0)] += dataP;
		    profileEntries[ch*250+indS*25+int(tdc*25.0)]++;
		    //h_Shape_TT[ch]->Fill(double(indS)+tdc,dataP);
		  }//
	      }//removing point far away from mean value
	    }//xtal select
	  indS++;
	}//save shape
      }//loop info
      //std::cout << std::endl;
      start+= 11;
      end  += 11;
    }//loop xtal
    
    //DETERMINE MEAN AND MAX
    for(int i=0; i<(nShape_*250);++i) profileMeanSaved[i]=0.0;
    for(int i=0; i<(nShape_*250);++i) profileMeanSaved[i]=profileMean[i];

    for(int i=0; i<(nShape_*250);++i) profileMean[i]=0.0;
    for(int i=0; i<nShape_; ++i)      profileMax[i]=0.0;
    for(int i=0; i<nShape_; ++i)      binMax[i]=9999999;
    for (unsigned int ch = 0; ch < XtalIN.size(); ++ch)
      {
	for(unsigned int bin=0; bin < 250; ++bin){
	  if(profileEntries[ch*250+bin] !=0) {
	    profileMean[ch*250+bin] = profileData[ch*250+bin]/double(profileEntries[ch*250+bin]);
	    if(profileMean[ch*250+bin] > profileMax[ch]) {
	      profileMax[ch] = profileMean[ch*250+bin];
	      binMax[ch]     = bin;
	    }//max
	  }//numbers of entries
	}//loop bins
      }//loop xtal

    //DETERMINE RMS
    for(int i=0; i<(nShape_*250);++i) profileRmsSaved[i]=0.0;
    for(int i=0; i<(nShape_*250);++i) profileRmsSaved[i]=profileRms[i];

    for(int i=0; i<(nShape_*250);++i) profileRms[i]=0.0;
    start = 0;
    end   = 11;
    for (unsigned int Xtal = 0; Xtal < xtalInfo_.size(); ++Xtal) {
      double tdc  = 0.0;
      int    indS = 0;
      for(int i=start; i < end; ++i){
	if(i == start){
	  tdc = shapeData_[i];
	}
	else{
	  for (unsigned int ch = 0; ch < XtalIN.size(); ch++) 
	    if(XtalIN[ch] == xtalInfo_[Xtal]){ 
	      double dataP = shapeData_[i]-pedestals[ch]; 
	      if(iter == 0) {
		profileRms[ch*250+indS*25+int(tdc*25.0)] += (dataP - profileMean[ch*250+indS*25+int(tdc*25.0)])
                  * (dataP - profileMean[ch*250+indS*25+int(tdc*25.0)]);
		//profileRms[ch*250+indS*25+int(tdc*25.0)] += (dataP/profileMax[ch] - profileMean[ch*250+indS*25+int(tdc*25.0)]/profileMax[ch])
		//  * (dataP/profileMax[ch] - profileMean[ch*250+indS*25+int(tdc*25.0)]/profileMax[ch]); 
	      }//first iteration
	      else
		{
		  if(dataP >= (profileMeanSaved[ch*250+indS*25+int(tdc*25.0)] - nRMS*profileRmsSaved[ch*250+indS*25+int(tdc*25.0)])
		     && dataP <= (profileMeanSaved[ch*250+indS*25+int(tdc*25.0)] + nRMS*profileRmsSaved[ch*250+indS*25+int(tdc*25.0)]))
		    {
		      if(iter == nIter_-1){
			profileRms[ch*250+indS*25+int(tdc*25.0)] += (dataP/profileMax[ch] - profileMean[ch*250+indS*25+int(tdc*25.0)]/profileMax[ch])
			  * (dataP/profileMax[ch] - profileMean[ch*250+indS*25+int(tdc*25.0)]/profileMax[ch]); 

			//modif-beg
			//profileRms[ch*250+indS*25+int(tdc*25.0)] += (dataP - profileMean[ch*250+indS*25+int(tdc*25.0)])
			//  * (dataP - profileMean[ch*250+indS*25+int(tdc*25.0)]); 
			//modif-end

			//h_Shape_TT[ch]->Fill(double(indS)+tdc,dataP/profileMax[ch]);
			//h_Shape_TT[ch]->Fill(double(indS)+tdc,dataP);
		      }//normalize
		      else
			profileRms[ch*250+indS*25+int(tdc*25.0)] += (dataP - profileMean[ch*250+indS*25+int(tdc*25.0)])
			  * (dataP - profileMean[ch*250+indS*25+int(tdc*25.0)]);
		    }//throw away points that degrade the RMS
		}//iter
	    }//xtal select
	  indS++;
	}//data
      }//loop info
      start+= 11;
      end  += 11;
    }//loop xtal

    for (unsigned int ch = 0; ch < XtalIN.size(); ++ch)
      for(unsigned int bin=0; bin < 250; ++bin)
	if(profileEntries[ch*250+bin] !=0)
	  profileRms[ch*250+bin] = sqrt(profileRms[ch*250+bin])/double(profileEntries[ch*250+bin]);

  }//iteration

  for (unsigned int ch = 0; ch < XtalIN.size(); ++ch)
    std::cout << ch << " " << binMax[ch] << " " << profileMax[ch] << std::endl;

  //WRITING ASCII FILE
  std::ofstream outProfile(Shapefile_.c_str());
  outProfile << XtalIN.size() << std::endl;
  for (unsigned int ch = 0; ch < XtalIN.size(); ++ch)
    {
      outProfile << XtalIN[ch] << std::endl;//" " << double(binMax[ch])/25.0 << std::endl;//modif
      for(unsigned int bin=0; bin < 250; ++bin)
	outProfile << bin+1 << " " << profileMean[ch*250+bin]/profileMax[ch] << " " << profileRms[ch*250+bin] << std::endl;
      //outProfile << bin+1 << " " << profileMean[ch*250+bin] << " " << profileRms[ch*250+bin] << std::endl;//modif
    }//loop xtal
  outProfile.close();
  
  fOut->Write();
  fOut->Close(); 
  std::cout << "BUILDING PROFILE IS DONE" << std::endl;
}//DESTRUCTOR

void EcalProfileBuilder::analyze(const edm::Event& evt, const edm::EventSetup& evtSetup) 
{
  evt_cnt_++;
  if(evt_cnt_%5000==0) std::cout << "Read " << evt_cnt_ << " events" << std::endl;

  //GET DIGIS
  edm::Handle<EBDigiCollection> digis;
  try {
    evt.getByLabel( digiProducer_ , digis);
  } catch ( std::exception& ex ) {
    edm::LogError("EcalRecHitsTaskError") << "Error! can't get the Digis " << std::endl;
    //std::cout << "EcalProfileBuilder: ERROR: Can't get the Digi Producer " << digiProducer_ << std::endl;
  }//digis

  //GET TDC
  edm::Handle<EcalTBTDCRecInfo> pTDC;
  const EcalTBTDCRecInfo* recTDC=0;
  try {
    evt.getByLabel( tdcRecInfoProducer_, tdcRecInfoCollection_, pTDC);
    recTDC = pTDC.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << tdcRecInfoCollection_.c_str() << std::endl;
  }//tdc
  //checking tdc
  if (!recTDC){
    std::cout << "THERE IS NO TDC! skipping this event" << std::endl;
    return;
  }//checking tdc

  //GET HEADER
  edm::Handle<EcalTBEventHeader> pEventHeader;
  const EcalTBEventHeader* evtHeader=0;
  try {
    evt.getByLabel( eventHeaderProducer_ , pEventHeader );
    evtHeader = pEventHeader.product(); // get a ptr to the product
    //std::cout << "Taken EventHeader " << std::endl;
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << eventHeaderProducer_.c_str() << std::endl;
  }//header
  //checking header only if needed
  if(!evtHeader) {
    std::cout << "THERE IS NO HEADER! skipping this event" << std::endl;
    return;
  }//checking header
  //std::cout << "RUN NUMBER =" << evtHeader->runNumber() << std::endl;

  //GET XTAL IN BEAM
  xtalInBeam_ = EBDetId(1,evtHeader->crystalInBeam(),EBDetId::SMCRYSTALMODE);
  if (xtalInBeam_.ic() != maxIdRef_) {
    std::cout << "NEW XTAL IN BEAM=" << xtalInBeam_.ic() << std::endl;
    maxIdRef_ = xtalInBeam_.ic();
    std::cout << XlowerCut_[maxIdRef_-1] << " " << XupperCut_[maxIdRef_-1] << " " 
	      << YlowerCut_[maxIdRef_-1] << " " << YupperCut_[maxIdRef_-1] << std::endl;
  }//xtalinbeam
  if(xtalInBeam_.ic() != xtalchosen_ && !scanmode_) return;

  //TABLE MOVING?
  if (evtHeader->tableIsMoving()) return;

  //GET HODOSCOPE
  edm::Handle<EcalTBHodoscopeRecInfo> pHodo;
  const EcalTBHodoscopeRecInfo* recHodo=0;
  try {
    evt.getByLabel( hodoRecInfoProducer_, hodoRecInfoCollection_, pHodo);
    recHodo = pHodo.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << hodoRecInfoCollection_.c_str() << std::endl;
  }//Hodoscope
  //checking hodoscope
  if (!recHodo) {
    std::cout << "THERE IS NO HODO! skipping this event" << std::endl;
    return;
  }//checking hodo

  //CENTRAL EVENTS
  double xHodo = recHodo->posX();
  double yHodo = recHodo->posY();
  bool   central_event = false;
  int    xtalic = xtalInBeam_.ic();
  if((xHodo > XlowerCut_[xtalic-1])&& (xHodo < XupperCut_[xtalic-1]) 
    && (yHodo > YlowerCut_[xtalic-1]) && (yHodo < YupperCut_[xtalic-1]))
      central_event = true;
  if(!central_event) return;

  //CHECKING MAX HIT == XTAL IN BEAM
  // Loop over Ecal digis
  EBDetId maxHitId(0);
  double  maxHit = -999999.;
  for (EBDigiCollection::const_iterator digiItr = digis->begin(); digiItr != digis->end(); ++digiItr)
    {
      double eMax = 0.;
      for (int sample = 0; sample < digiItr->size(); ++sample)
        {
          double analogSample  = digiItr->sample(sample).adc();
          if ( eMax < analogSample )
	    eMax = analogSample;
        }//loop samples
      
      if(eMax > maxHit) 
	{
	  maxHit = eMax;
	  maxHitId = digiItr->id();
	}
    }//loop digis
  if(maxHitId.ic() != xtalInBeam_.ic()) return;

  //Search for particular digis
  EBDigiCollection::const_iterator digiItr = digis->find(xtalInBeam_); 
    {
      double samples_save[10]; for(int i=0; i < 10; ++i) samples_save[i]= 0.0;
      int    gain_tot=0;
      int    sMax = -1;
      double eMax = 0.;
      std::vector<double> shapeInfo;
      shapeInfo.push_back(recTDC->offset());

      for (int sample = 0; sample < digiItr->size(); ++sample)
	{
	  double analogSample  = digiItr->sample(sample).adc();
	  gain_tot            += digiItr->sample(sample).gainId();

	  samples_save[sample] = analogSample;
	  shapeInfo.push_back(analogSample);
	  
//  	  std::cout << analogSample << " ";
//  	  std::cout << gainSample << " ";
	  if ( eMax < analogSample )
	    {
	      eMax = analogSample;
	      sMax = sample;
	    }
	}//loop samples
//       std::cout << std::endl;

      //FILLING PROFILE HISTOGRAM INFO
      //only take crystals with energy > noise
      if(eMax > 300){
	for (int Xtal = 1; Xtal < 1701; Xtal++) {
	  if(Xtal == xtalInBeam_.ic()){ 
// 	    std::cout << "FOR XTAL=" << Xtal << std::endl;
	    if(shapeInfo.size() == 11 
 	       && (recTDC->offset() >= 0) 
 	       && (recTDC->offset() <= 1)
	       && gain_tot == 10){
	      nEntries_[Xtal-1]++;
	      xtalInfo_.push_back(Xtal); //save xtal number
	      for(unsigned int indice=0; indice < shapeInfo.size(); ++indice)
		{
// 		  std::cout << shapeInfo[indice] << " ";
		  shapeData_.push_back(shapeInfo[indice]);
		}//loop indice
// 	      std::cout << std::endl;
	    }//if correct size
	  }//crystal
	}//loop xtal
      }//Noise cut

    }//loop digis
}//ANALYZE
