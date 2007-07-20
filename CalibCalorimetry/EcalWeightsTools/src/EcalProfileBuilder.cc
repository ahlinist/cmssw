/*=================================================================
*	EcalProfileBuilder.cc
*	David Wardrope - Imperial College London
*	13/07/07
*	Version - 4.2
*
*	Program makes smooth profiles of the signal shape from the ECAL
*	FE as input for the EcalWeightsBuilder. As few as 1000 events
*	per crystal are needed to build a profile of sufficient quality.
*================================================================*/
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalProfileBuilder.h"


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

  //Input weights from 2004 ECAL H4 Test beam
  weights_ = pSet.getUntrackedParameter<std::vector<double> >("AmplitudeWeights");

  //SPECIFY WHICH SUPERMODULE
  SuperModule_          = pSet.getUntrackedParameter<unsigned int>("SuperModule", 0);

  //Name of output file
  Shapefile_            = pSet.getUntrackedParameter<std::string>("Shapefile","profile.txt");

  //XTAL CHOSEN
  xtalchosen_           = pSet.getUntrackedParameter<int>("xtalchosen", -1);
  if(xtalchosen_ == -1)
    std::cout << "NO CRYSTAL CHOSEN, WILL TAKE CRYSTAL IN BEAM" << std::endl;
  else std::cout << "CRYSTAL CHOSEN =" << xtalchosen_ << std::endl;

  //Minimum number of entries per crystals
  nentry_               = pSet.getUntrackedParameter<int>("nentry", 0);

  //SCAN MODE
  //Building profile for all crystals readout.
  scanmode_             = pSet.getUntrackedParameter<bool>("scanmode", false);
  if(scanmode_) std::cout << "YOU ARE IN SCAN MODE, BUILDING PROFILE FOR ALL CRYSTALS IN BEAM" << std::endl;

  //------------------------------Initializing Counters-------------------------------------------------
  evt_cnt_ = 0;
  maxIdRef_ = 999999;

  //------------------------CHECKING WEIGHTS---------------------------------------
  std::cout << "=========================WEIGHTS==============================" << std::endl;
  for(int i=0; i < 250; ++i)
  {
    if((i%10)==0) std::cout << std::endl;
    std::cout << weights_[i] << " ";
  }
  std::cout << std::endl;
  std::cout << "==============================================================" << std::endl;
//   weightsin.close();
}//End of constructor

void EcalProfileBuilder::beginJob(edm::EventSetup const&)
{
  std::cout << "beginJob()" << std::endl ;
}//beginJob

void EcalProfileBuilder::endJob()
{
	std::cout << "Beginning of endJob(), with " << xtalIncluded_.size() << " crystals to process." << std::endl ;
  //Determine pedestals for each signal pulse, and subtract them
  for(Xtal_= Samples.begin(); Xtal_ !=Samples.end(); Xtal_++)
  {
  		std::cout << "Determining pedestal values for events in crystal " << Xtal_->first << std::endl ;
      int nsamp = 0;
		int goodshape = 0;
		double ped = 0. ;
      std::vector<double> vSamps = Xtal_->second ;
		std::vector<double> pedestal ;
      for(std::vector<double>::iterator sIter = vSamps.begin(); sIter != vSamps.end(); sIter++)
      {
			if(nsamp%11 >0 && nsamp%11 <4) ped += *sIter ;
			if(nsamp%11 == 10)
			{
				pedestal.push_back(ped/3.) ;
				ped = 0.;
			}
			nsamp++;
		}
      std::cout << "Size of vSamps before replacement :  " << vSamps.size() << std::endl ;

      for(std::vector<double>::iterator sIter = vSamps.begin(); sIter != vSamps.end(); sIter++)
      {
			if(nsamp%11 != 0)
			{
	  			double newSamp = *sIter - pedestal[goodshape] ;
	  			sIter = vSamps.erase(sIter) ;
	  			vSamps.insert(sIter, newSamp) ;
			}
			nsamp++ ;
			if(nsamp%11 == 10) goodshape++ ;
      }
      Samples.erase(Xtal_->first) ;
		Samples.insert(std::make_pair(Xtal_->first, vSamps)) ;
		std::cout << "Size of vSamps after replacement :  " << vSamps.size() << std::endl ;
  }

  //Determine Profile Amplitudes
  for(Xtal_= Samples.begin(); Xtal_ !=Samples.end(); Xtal_++)
  {
      int nsamp = 0;
      double TDCoffset = - 1000.;
      double amp = 0. ;
      std::vector<double> Franzen ;
      std::vector<double> vSamps = Xtal_->second ;

      for(std::vector<double>::iterator sIter = vSamps.begin(); sIter != vSamps.end(); sIter++)
      {
	if(nsamp%11 == 0) TDCoffset = *sIter ;
	else{
	  double sigheight = *sIter ;
	  amp += sigheight * weights_[10*int(25.*TDCoffset)+((nsamp%11)-1)] ;
	}
	if(nsamp%11 == 10)
	{
     Franzen.push_back(1./amp) ;
	  amp = 0. ;
	}
	nsamp++ ;
      }	//end of loop over profile information
   Corrections_.insert(std::make_pair(Xtal_->first, Franzen)) ;
	std::cout << "Generated Corrections for Crystal " << Xtal_->first << std::endl ;
  }//end of loop over Xtals

  //Apply the correction factors to the samples
  for(Xtal_= Samples.begin(); Xtal_ !=Samples.end(); Xtal_++)
  {
    int nsamp = 0;
    int goodshape = -1;
    double TDCoffset = - 1000.;
    std::vector<double> vSamps = Xtal_->second ;
    std::map<int, std::vector<double> >::iterator corIt = Corrections_.find(Xtal_->first);
    std::vector<double> correction = corIt->second ;
    p_TProfIter = p_Shape.find(Xtal_->first) ;
    if(p_TProfIter == p_Shape.end())
    {
      TProfile* p_SigShape = new TProfile(Form("Signal_Shape_%d", Xtal_->first),Form("Corrected Shape for Crystal %d", Xtal_->first),275,0,275,"");
      p_Shape.insert(std::make_pair(Xtal_->first, p_SigShape)) ;
      std::cout << "Created Profile for " << Xtal_->first << std::endl ;
      p_TProfIter = p_Shape.find(Xtal_->first) ;
    }
    for(std::vector<double>::iterator sIter = vSamps.begin(); sIter != vSamps.end(); sIter++)
    {
      if(nsamp%11 == 0)
      {
	TDCoffset = *sIter ;
	goodshape++ ;
      }
      else{
	double sigheight = *sIter ;
	double corry = correction[goodshape];
	sigheight *= corry ;
	p_TProfIter->second->Fill(int(25.*((nsamp%11)+TDCoffset-1.)), sigheight) ;
      }
      nsamp++ ;
    }//End loop over samples
	std::cout << "Applied Corrections for Crystal " << Xtal_->first << std::endl ;
  }//End loop over crystals
  std::cout << "Samples rescaled" << std::endl ;
	gStyle->SetOptStat(0);
  //Make a fit to a region of the profile to remove TDC offset dependent bias
  for(p_TProfIter = p_Shape.begin(); p_TProfIter != p_Shape.end(); p_TProfIter++)
    {
      double x[250] ;
      double xerr[250] ;
      double y[250] ;
      double yerr[250] ;
      double Unbias[25] ;
      double ysmooth[250] ;
      for(int i =0; i< 250; i++)
      {
	x[i] = 0.;
	xerr[i] = 0.;
	y[i] = 0.;
	yerr[i] = 0.;
      }
      std::vector<double> vProf ;
      std::vector<double> vProfErr ;
      for(int bin = 1; bin <251 ; bin++)
      {
	double value, error ;
	x[bin-1] = ((double(bin-1)+0.5)/double(250))*10.;
	value = p_TProfIter->second->GetBinContent(bin) ;
	if(value == 1.) value = 0.999999 ;
	y[bin-1] = value ;
	error = p_TProfIter->second->GetBinError(bin);
	if(bin <= 145 || bin > 180) error *= 15. ;
	yerr[bin] = error ;
      }
      //fit to profile
      TGraphErrors* Graph1 = new TGraphErrors(250, x, y, xerr, yerr) ;
      double TMaxShape = 5.5;
      double Tfit_min  = TMaxShape - 1.;	//changed from -1.0
      double Tfit_max  = TMaxShape + 3.5;  //changed from +3.0

      TF1 *myFit =new TF1("myFit","[3]*(((x-[0])/[1])^[2])*exp(-[2]*((x-[0]-[1])/[1]))",Tfit_min,Tfit_max);
      myFit->SetParName(0,"Toff");
      myFit->SetParName(1,"Tpeak");
      myFit->SetParName(2,"Alpha");
      myFit->SetParName(3,"AmpMax");
      myFit->SetParameter(0,3.0);
      myFit->SetParameter(1,2.0);
      myFit->SetParameter(2,1.3);
      myFit->SetParameter(3,1.0);
		myFit->SetLineColor(2);

      Graph1->Fit(myFit,"r");

      double param[4];
      myFit->GetParameters(param);

      double param_err[4];

      for(int i=0; i<4; ++i) param_err[i] = myFit->GetParError(i);

      //Evaluate function in area of good fit - 150-175ns
      for(int j = 150; j < 175; j++)
      {
	  double f = myFit->Eval(((double(j)+0.5)/double(250))*10.) ;
	  std::cout << ((double(j)+0.5)/double(250))*10. <<"\t"<< f << std::endl ;
	  Unbias[j%25] = f/y[j] ;
      }

      //Correct function for TDC dependent bias
      for(int j = 0; j<250; j++)	y[j] *= Unbias[j%25] ;

      //Smoothing
      for(int loc_bin=0; loc_bin < 250; ++loc_bin)
      {
	if(loc_bin < 3 || loc_bin > 247)	  ysmooth[loc_bin] = y[loc_bin];
	else{
	  double loc_val[5];
	  double loc_err[5];
	  double loc_x[5];
	  double loc_xerr[5];

	  std::cout << loc_bin << std::endl;

	  for(int i=0; i<5; ++i) {loc_x[i]=0.0; loc_xerr[i]=0.0;}
	  loc_x[0] = loc_bin-2;
	  loc_x[1] = loc_bin-1;
	  loc_x[2] = loc_bin;
	  loc_x[3] = loc_bin+1;
	  loc_x[4] = loc_bin+2;

	  loc_val[0] = y[loc_bin-2];
	  loc_val[1] = y[loc_bin-1];
	  loc_val[2] = y[loc_bin];
	  loc_val[3] = y[loc_bin+1];
	  loc_val[4] = y[loc_bin+2];

	  loc_err[0] = yerr[loc_bin-2];
	  loc_err[1] = yerr[loc_bin-1];
	  loc_err[2] = yerr[loc_bin];
	  loc_err[3] = yerr[loc_bin+1];
	  loc_err[4] = yerr[loc_bin+2];

	  TGraphErrors* loc_graph = new TGraphErrors(5, loc_x, loc_val, loc_xerr, loc_err);
	  TF1 *f1 = new TF1("f1","pol2",loc_x[0],loc_x[4]);
	  loc_graph->Fit(f1,"r");

	  std::cout << f1->Eval(loc_x[2]) << std::endl;
	  ysmooth[loc_bin] = f1->Eval(loc_x[2]);
	}//smoothing
      }//loop bins
	double binmax = -9999. ;

   for(int j = 0; j<250; j++)	if(ysmooth[j] > binmax) binmax = ysmooth[j] ;

   for(int j = 0; j<250; j++)
   {
		vProf.push_back(ysmooth[j]/binmax) ;
		vProfErr.push_back(yerr[j]/binmax) ;
   }

      Profiles_.insert(std::make_pair(p_TProfIter->first, vProf)) ;
      ProfileErrors_.insert(std::make_pair(p_TProfIter->first, vProfErr)) ;
   }

  //WRITING ASCII FILE
  std::cout << "Writing ASCII files" << std::endl ;
  std::ofstream outProfile(Shapefile_.c_str());
  outProfile << xtalIncluded_.size() << std::endl;
  for(Xtal_ = Profiles_.begin(); Xtal_ != Profiles_.end(); Xtal_++)
  {
      outProfile << Xtal_->first << std::endl ;
      std::vector<double> vProf = Xtal_->second ;
      std::map<int, std::vector<double> >::iterator errIt = ProfileErrors_.find(Xtal_->first);
      std::vector<double> vErr = errIt->second ;
      for(int bin = 0; bin<250; bin++)
	outProfile << bin+1 << "\t" << vProf[bin] << "\t" << vErr[bin] <<std::endl ;
  }
  outProfile.close();
  std::cout << "endJob() complete." << std::endl ;
}//endJob

EcalProfileBuilder::~EcalProfileBuilder()
{
  std::cout << "EcalProfileBuilder has completed" << std::endl ;
}//End of Destructor

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
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << eventHeaderProducer_.c_str() << std::endl;
  }//header
  //checking header only if needed
  if(!evtHeader) {
    std::cout << "THERE IS NO HEADER! skipping this event" << std::endl;
    return;
  }//checking header

  //GET XTAL IN BEAM
  xtalInBeam_ = EBDetId(1,evtHeader->crystalInBeam(),EBDetId::SMCRYSTALMODE);
  if (xtalInBeam_.ic() != maxIdRef_) {
    std::cout << "NEW XTAL IN BEAM=" << xtalInBeam_.ic() << std::endl;
    maxIdRef_ = xtalInBeam_.ic();
  }//xtalinbeam
  if(xtalInBeam_.ic() != xtalchosen_ && !scanmode_) return;

  //TABLE MOVING?
  if (evtHeader->tableIsMoving()) return;

//Confirm that crystal in the beam is the maximum containment crystal
  EBDetId maxHitId(0);
  double  maxHit = -999999.;
  for (EBDigiCollection::const_iterator digiItr = digis->begin(); digiItr != digis->end(); ++digiItr)
  {
      double eMax = 0.;
      for (int sample = 0; sample < digiItr->size(); ++sample)
      {
          double analogSample  = digiItr->sample(sample).adc();
          if ( eMax < analogSample ) eMax = analogSample;
      }//loop samples
      if(eMax > maxHit)
      {
	  maxHit = eMax;
	  maxHitId = digiItr->id();
      }
  }//loop digis
  if(maxHitId.ic() != xtalInBeam_.ic()) return;

  //Using this identity, find the TDC Offset and ADC samples and add to vector, and then map.
  EBDigiCollection::const_iterator digiItr = digis->find(xtalInBeam_);
  {
    int gain_tot=0 ;
    double eMax = 0.;
    std::vector<double> shapeInfo;
    shapeInfo.push_back(recTDC->offset());

    for (int sample = 0; sample < digiItr->size(); ++sample)
    {
      double analogSample  = digiItr->sample(sample).adc();
      gain_tot            += digiItr->sample(sample).gainId();
      shapeInfo.push_back(analogSample);
      if(int(25.*(recTDC->offset()+sample)) > 250) std::cout << "***********Out of bin range!" << std::endl ;
      if( eMax < analogSample )	eMax = analogSample;
    }

	 //std::cout << ">>>>>>>>>>>>>>GAIN TOTAL = " << gain_tot << std::endl ;
    //Check Profile is above noise threshold
    if(eMax > 300. && shapeInfo.size() == 11 && recTDC->offset() > 0. && recTDC->offset() < 1. && (gain_tot == 10 || gain_tot == 20 ))
    {
      //Find correct map key and append sample information to the end
      if(find(xtalIncluded_.begin(), xtalIncluded_.end(), xtalInBeam_.ic()) == xtalIncluded_.end())						xtalIncluded_.push_back(xtalInBeam_.ic()) ;
      std::map<int, std::vector<double> >::iterator XtalInst;
      XtalInst = Samples.find(xtalInBeam_.ic()) ;
      if(XtalInst == Samples.end()) Samples.insert(std::make_pair(xtalInBeam_.ic(), shapeInfo)) ;
      else{
	for(std::vector<double>::iterator it = shapeInfo.begin(); it != shapeInfo.end(); it++)
	{
	  double amps = *it ;
	  (*XtalInst).second.push_back(amps);
	}
      }
    }
  }
}//End of EcalProfileBuilder::BuildProfile
