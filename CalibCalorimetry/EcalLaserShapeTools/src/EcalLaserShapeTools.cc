/*
 * \file EcalLaserShapeTools.cc
 *
 * $Date: 2006/08/06 22:52:50 $
 * $Revision: 1.8 $
 * \author P. Jarry
 * \author G. Franzoni
 *
 */

#include <CalibCalorimetry/EcalLaserShapeTools/interface/EcalLaserShapeTools.h>


EcalLaserShapeTools::EcalLaserShapeTools(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  evtNum_ =0;
  rootFile_                  = iConfig.getUntrackedParameter<std::string>("rootOutFile");
  txtFile_                     = iConfig.getUntrackedParameter<std::string>("txtOutFile");
  histFile_                   = iConfig.getUntrackedParameter<std::string>("HistogramOutFile");
  digiProducer_         = iConfig.getParameter<std::string>("digiProducer");
  pndiodeProducer_  = iConfig.getParameter<std::string>("pndiodeProducer");
  hitCollection_          = iConfig.getParameter<std::string>("hitCollection");
  hitProducer_            = iConfig.getParameter<std::string>("hitProducer");

  
  dbName_ = iConfig.getUntrackedParameter<string>("dbName", "");
  dbHostName_ = iConfig.getUntrackedParameter<string>("dbHostName", "");
  dbHostPort_ = iConfig.getUntrackedParameter<int>("dbHostPort", 1521);
  dbUserName_ = iConfig.getUntrackedParameter<string>("dbUserName", "");
  dbPassword_ = iConfig.getUntrackedParameter<string>("dbPassword", "");
  
  location_ =  iConfig.getUntrackedParameter<string>("location", "H4");

  for (int wavelength=0; wavelength <4; wavelength++){
    for (int channel=0; channel <4; channel++){
      alpha_[wavelength][channel]  =0;
      beta_[wavelength][channel]  =0;
      chi2_[wavelength][channel]  =0;
      width_[wavelength][channel]  =0;
    }
  }
  

}

void EcalLaserShapeTools::beginJob (EventSetup const& eventSetup) 
{
  
  for (int channel=0; channel < 1700; channel++){
    std::vector <EBDataFrame > theBlueChannel;
    theBlueChannel.reserve(600);
    blueData.push_back(theBlueChannel);

    std::vector <EBDataFrame > theIrChannel;
    theIrChannel.reserve(600);
    irData.push_back(theIrChannel);

    std::vector <float > pnForOneBlueChannel;
    pnForOneBlueChannel.reserve(600);
    bluePnData.push_back(pnForOneBlueChannel);

    std::vector <float > pnForOneIrChannel;
    pnForOneIrChannel.reserve(600);
    irPnData.push_back(pnForOneIrChannel);

  }
  
}


EcalLaserShapeTools::~EcalLaserShapeTools()
{

}


void EcalLaserShapeTools::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  evtNum_++;
  LogInfo("EcalLaserShapeTools") << "\n\n\n Event: " << evtNum_ << endl;
  
  LogDebug("EcalLaserShapeTools") << "Fetching hitCollection: " << hitCollection_
				  << " produced by " << hitProducer_ << std::endl;
  
  // dcc headers - event info such as wavelenght
  edm::Handle<EcalRawDataCollection> DCCHeaders;
  EcalRawDataCollection::const_iterator headerItr ;
  try{ 
    iEvent.getByLabel(digiProducer_, DCCHeaders);
  } catch ( std::exception& ex ) {
    LogDebug("EcalLaserShapeTools") << "Cannot get product:  EcalRawDataCollection from " 
				    << digiProducer_ << " - returning." << endl;
    return;
  }
  if (DCCHeaders->size() !=1)
    {
      LogInfo("EcalLaserShapeTools") << "More than 1 DCCHeader found - not expected." << endl;
    }
  
  

  // laser wavelenght from DCC header
  for (headerItr= DCCHeaders->begin();headerItr != DCCHeaders->end(); ++headerItr ) {
    EcalDCCHeaderBlock::EcalDCCEventSettings settings = headerItr->getEventSettings();
    laser_color_ = (int) settings.wavelength;
    event_         = headerItr->getLV1();
    run_ = headerItr-> getRunNumber();
  }
  
  if(laser_color_  <0 || laser_color_  > 3){
    LogError("EcalLaserShapeTools") << "Wrong value for laser wavelength: "<< laser_color_  <<std::endl;
  }
  if(laser_color_  ==1 || laser_color_  == 2){
    LogError("EcalLaserShapeTools") << "Expected only blue (0) and ir (3), while color is "<< laser_color_ <<std::endl;
  }



  // accessing digis (event-wise). Storing them in vector of vectors
  // to be used channel-wise in endJob()
  edm::Handle<EBDigiCollection>  digis;
  try{ 
    iEvent.getByLabel(digiProducer_, digis);
    // gio: systematize logs!
    //    LogDebug("EcalLaserShapeTools") << "Got product:  EBDigiCollection from: " 
    LogDebug("EcalLaserShapeTools") << "Got product:  EBDigiCollection from: " 
				    << digiProducer_ << " - size: " 
				    << digis->size()    << endl;
    if (digis->size()){
      EBDigiCollection::const_iterator digiItr = digis->begin();
      ism_ =     EBDetId( (*digiItr).id()).ism();    //    EBDetId                theId(digiItr->id()) ;
    }
    

  } catch ( std::exception& ex ) {
    LogError("EcalLaserShapeTools") << "Cannot get product:  EBDigiCollection from: " 
				    << digiProducer_ << " - returning." << endl;
    return;
  }



  // gettin pndiodes digi
  Handle<EcalPnDiodeDigiCollection> pndiodes;
  try {
    iEvent.getByLabel( pndiodeProducer_,pndiodes);
    LogDebug("EcalLaserShapeTools") << "Got product: EcalPnDiodeDigiCollection  from: " 
				    << digiProducer_ << " - size: " 
				    << pndiodes->size()    << endl;
     
  } catch ( std::exception& ex ) {
    LogError("EcalLaserShapeTools") << "Cannot get product: EcalPnDiodeDigiCollection from: " 
				    << digiProducer_ << " - returning." << endl;
    return;
  }
  const EcalPnDiodeDigiCollection* p_pndiodes = 0;
  p_pndiodes = pndiodes.product() ;



  //  gio
  // protection needed in case of missing PN
  EcalPnDiodeDigiCollection::const_iterator ipnd ;
  std::vector<double> maxAmplPN;
  for(ipnd=pndiodes->begin(); ipnd!=pndiodes->end(); ++ipnd) {
     
    PNfit res  = maxPNDiode( *ipnd );
    maxAmplPN.push_back( res.max );
    
  }


  EBDigiCollection::const_iterator digiItr ;
  //  int adc_lu[20][85][10] ;
  //  int gain_lu ;
   
  for (digiItr = digis->begin(); digiItr != digis->end(); ++digiItr ) {
     
    EBDetId                theId(digiItr->id()) ;
    EBDataFrame        theFrame( theId );  


    // look at PN diodes :
    // normalized amplitude - use average of 2 PNdiodes for each group of TTs
    // FIXME: protect in case of lack of pn in digi because of integrity
    float pn_amp = 0.5*( maxAmplPN[ pnFromTT(theId.tower()).first ] + maxAmplPN[ pnFromTT(theId.tower()).second ]) ;
    
    // retrieving a data frame
    for ( int i=0; i< (*digiItr).size() ; ++i ) {
      // introducing here the normalization to PN amplitude: FIXME, to be done differently
      EcalMGPASample theSample(  (*digiItr).sample(i).adc()  , (*digiItr).sample(i).gainId()	 );

      if ((*digiItr).sample(i).gainId()!=1 )
	{
	  LogInfo("EcalLaserShapeTools") << " gain " << (*digiItr).sample(i).gainId() << " other than 1(= X12) found for channel: "
					 << theId.ism() << " - note: gain ratio not accounted for" << endl;
	}
      
      theFrame.setSample( i, theSample);
      
    }
        

    if (laser_color_ == 0) 
      {
	blueData[ (*digiItr).id().ic() -1 ].push_back(theFrame);
	bluePnData[ (*digiItr).id().ic() -1  ].push_back(pn_amp);
      }
    else if (laser_color_ == 3)
      {
	irData[ (*digiItr).id().ic() -1      ].push_back(theFrame);
	irPnData[ (*digiItr).id().ic() -1  ].push_back(pn_amp);
      }
    else
      {	
	LogDebug("EcalLaserShapeTools") << "Expected only blue (0) and ir (3), while color is "
					<< laser_color_ << " - event not considered" << std::endl;
      }
    
  }
  
  
}

void EcalLaserShapeTools::endJob (void) 
{       

  //gio
  //  TDirectory* basedir = gDirectory;
  //  basedir ->cd();
  //  TDirectory* treeDir = (TDirectory*) basedir->Get("reducedData_blue");
  //  treeDir->cd();

  // create the ROOT file
  //  TFile file(rootFile_.c_str(),"RECREATE");
  //  TFile file(rootFile_.c_str(),"UPDATE");

  TH1F alphaHb("alpha_b", "#alpha_{b}", 100,0,5);
  TH1F betaHb("beta_b", "#beta_{b}", 100,0,5);
  TH2F betaVsalphaHb("beta_VS_alpha_b", "#beta vs #alpha (blue)", 100,0,5, 100,0,5);
  TH1F chi2Hb("chi2_b", "#Chi^{2}_{b}", 100,0,5);
  TH1F widthHb("Width_b", "#Gamma_{b}", 100,0,5);
  
  TH1F alphaHir("alpha_ir", "#alpha_{ir}", 100,0,5);
  TH1F betaHir("beta_ir", "#beta_{ir}", 100,0,5);
  TH2F betaVsalphaHir("beta_VS_alpha_ir", "#beta vs #alpha (infrared)", 100,0,5, 100,0,5);
  TH1F chi2Hir("chi2_ir", "#Chi^{2}_{ir}", 100,0,5);
  TH1F widthHir("Width_ir", "#Gamma_{ir}", 100,0,5);

  
  // gio loop 1 /2 stasrts here
  int icolor;
  std::vector< std::vector <EBDataFrame >   > * pointerChannelsData;
  for (int colour =0; colour < 2; colour++)
    {
      nb_iter = 5;   // anyway iteration is stopped when chi2 is stable ...(3 iterations are usually enough)
      nsmin   = 1;   // number of samples to fit before the maximum
      nsmax  = 2;    // number of samples to fit after the maximum sample
      
      
      //    int IPRINT=0 ; // no results printed
      sigma_rec = 1.2 ; // noise per channel (pessimisitic , can be adjusted according to covariance matrix)
      //    sprintf(laser_choice,"blue LASER") ;
      //    if(icolor==1) sprintf(laser_choice,"red LASER") ;
      //    printf("====>>  Computing alpha-beta parameters for each channel with %s ... wait a few seconds ...\n",laser_choice) ;
      
      // analyze data and compute parameters alpha-beta for each crystal
      // initialize constants
      // looping on all crys
      
      if (colour ==0)
	{
	  pointerChannelsData = (&blueData);
	  icolor =1;
	}
      else 
	{
	  pointerChannelsData = (&irData);
	  icolor =3;
	}
            
      
      // begin loop on channels
      std::vector< std::vector  <EBDataFrame >  >::const_iterator channelDataIter;
      for (channelDataIter = (*pointerChannelsData).begin(); channelDataIter !=(*pointerChannelsData).end();  channelDataIter++)
	{
	  LogDebug("EcalLaserShapeTools") << " channel: " 
					 << ((channelDataIter -  (*pointerChannelsData).begin() ) +1)
					 << " # events: " << (*channelDataIter).size() << endl;
	  // gio go away
	  if (   (channelDataIter -  (*pointerChannelsData).begin() )== 1){
	    LogDebug("EcalLaserShapeTools") << " ch " << ((channelDataIter -  (*pointerChannelsData).begin() ) +1)  << ";" << endl;
	    std::vector  <EBDataFrame >::const_iterator evtIter;
	    for (evtIter = (*channelDataIter).begin(); evtIter !=(*channelDataIter).end();  evtIter++)
	      {
		for (int sample=0; sample<10; sample++)
		  {
		    LogDebug("EcalLaserShapeTools") << "(in end job)  ch " << ( (*evtIter).id().ic()) 
						   << " event  " << (evtIter - (*channelDataIter).begin())
						   << " sample " << sample << " " << (*evtIter).sample(sample).adc()  << ";" << endl;
		  }
	      }
	  }
	
	  // crystal number - corresponds to ic()-1
	  int icris = ((int) (channelDataIter -  (*pointerChannelsData).begin() ) );
	
	  double param_out[3] ;

	  int eventsNumber =0;
	  // this is the function which calculates pretty much all
	  LogDebug("EcalLaserShapeTools") << " now fitting " << endl;
	  std::vector< pair <float,float> >  reducedData;
	  reducedData.reserve(600);
	
	  bool doFit =false;
	  if (
	      (*channelDataIter).size()> 100 &&          // at least 100 events
	      (*channelDataIter).size() == bluePnData[icris].size()  // synk between ADP and PN data
	      )
	    {
	      doFit = true;
	      eventsNumber = (*channelDataIter).size();
	    }


	  float    chi2s =0;
	  if (doFit) {

	    LogDebug("EcalLaserShapeTools") << " fitting channel " << (icris+1) << endl;
	    LogDebug("EcalLaserShapeTools") << "[before launching fitpj] size APD: " << (*channelDataIter).size() 
					    << " size pn " << bluePnData[icris].size() << endl;
	    chi2s = fitpj(param_out, eventsNumber , (*channelDataIter), bluePnData[icris], reducedData) ;
	    //gio
	    LogDebug("EcalLaserShapeTools") << "[after launching fitpj] size APD: " << (*channelDataIter).size() 
					    << " size pn " << bluePnData[icris].size() << endl;

	  }
	  else
	    {
	      LogDebug("EcalLaserShapeTools") << "[EcalLaserShape] Fit not  performed for channel: " 
					      << (icris+1) << " because too few APD events available (" 
					      << (*channelDataIter).size() << ") or mismatch with PN event number (" 
					      << bluePnData[icris].size() << ")" << endl;
	      return;                         // skip this channel and move on to the next
	    }
	  

	  //gio to go away
	  LogDebug("EcalLaserShapeTools") << "[out of fitpj] alpha is: " << param_out[0]  << " beta " << param_out[1]  << endl;
	  alpha_[icolor][icris]  = param_out[0] ;
	  beta_[icolor][icris]   = param_out[1] ;
	  chi2_[icolor][icris]   = chi2s ;
	  // calculating widht
	  width_[icolor][icris]=ComputePulseWidth(alpha_[icolor][icris],beta_[icolor][icris]) ;
	

	
	  char nameTree[10];
	  if (colour ==0)
	    {
	      sprintf(nameTree,"ApdOpnBlue_%d", (int) ((channelDataIter -  (*pointerChannelsData).begin())+1) );
	    }
	  else
	    {
	      sprintf(nameTree,"ApdOpnIr_%d", (int) ((channelDataIter -  (*pointerChannelsData).begin())+1) );
	    }
	
	  // gio test
	  TFile file(rootFile_.c_str(),"UPDATE");
	  
	  TTree *oneChannelTree = new TTree(nameTree,nameTree);

	  float ampli=0;
	  float time=0;
	  oneChannelTree ->Branch("Ampli",&ampli,"ampli/F");
	  oneChannelTree ->Branch("Time",&time,"time/F");

	  // one tree with reduced data per channel 
	  std::vector  < pair <float,float>  >::const_iterator Iter;
	  for (Iter = reducedData.begin(); Iter !=reducedData.end(); Iter++)
	    {
	      ampli = (*Iter).first;
	      time  = (*Iter).second;

	      oneChannelTree-> Fill();
	      LogDebug("EcalLaserShapeTools") << "[filling tree] ampli: " << ampli << " time: " << time << endl;
	    }
	
	  oneChannelTree->Write();
	  delete oneChannelTree;

	  // gio test
	  file.Write();
	  file.Close();

	  if (colour ==0)
	    {
	      alphaHb.Fill(param_out[0]);
	      betaHb.Fill(param_out[1]);
	      betaVsalphaHb.Fill(param_out[0], param_out[1]);
	      chi2Hb.Fill(chi2s);
	      widthHb.Fill(width_[icolor][icris]);
	    }
	  else
	    {
	      alphaHir.Fill(param_out[0]);
	      betaHir.Fill(param_out[1]);
	      betaVsalphaHir.Fill(param_out[0], param_out[1]);
	      chi2Hir.Fill(chi2s);
	      widthHir.Fill(width_[icolor][icris]);
	    }
	  


	}
      // end loop on channels

    } // end loop on the two colors

  // gio test
  TFile file(rootFile_.c_str(),"UPDATE");

  LogDebug("EcalLaserShapeTools") << "saved all trees" << endl;

  alphaHb.Write();	betaHb.Write();
  chi2Hb.Write();      widthHb.Write();
  
  alphaHir.Write();	betaHir.Write();
  chi2Hir.Write();      widthHir.Write();
  
  
  file.Write();
  file.Close();
  
  // writing out text file with alpha and beta channel by channel. For Blue laser.
  std::ofstream txt_outfile;
  txt_outfile.open(txtFile_.c_str(),ios::out);
  for (int ch=1; ch<1701; ch++)
    {
      int sm=1;
      txt_outfile << sm << "\t" << ch 
		  << "\t" << alpha_[1][ch-1] 
		  << "\t" << beta_[1][ch-1] 
		  << "\t" << chi2_[1][ch-1] 
		  << "\t" << alpha_[3][ch-1] 
		  << "\t" << beta_[3][ch-1] 
		  << "\t" << chi2_[3][ch-1] 
		  << endl;
    }
  txt_outfile.close();
    
  beginRunDb( );
  

}
// end endJob()



void  EcalLaserShapeTools::beginRunDb( void )
{

  if ( dbName_.size() != 0 ) {
    cout << "[EcalLaserShapeTools][beginRunDb] DB output will go to"
         << " dbName = '" << dbName_ << "'"
         << " dbHostName = '" << dbHostName_ << "'"
         << " dbHostPort = '" << dbHostPort_ << "'"
         << " dbUserName = '" << dbUserName_ << "'" << endl;
  } else {
    cout << " DB output is disabled" << endl;
  }
  
  current_time_ = time(NULL);
  last_time_ = current_time_;
  
  EcalCondDBInterface* econn;
  econn =0;

  if ( dbName_.size() != 0 ) {
    try {
      cout << "[EcalLaserShapeTools][beginRunDb] Opening DB connection ..." << endl;
      econn = new EcalCondDBInterface(dbHostName_, dbName_, dbUserName_, dbPassword_, dbHostPort_);
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
    }
  }


  
  // create the objects necessary to identify a dataset
  
  LocationDef locdef;
  locdef.setLocation(location_);
  
  RunTypeDef rundef;
  rundef.setRunType( "LASER"  );
 
  RunTag runtag;
  runtag.setLocationDef(locdef);
  runtag.setRunTypeDef(rundef);

  runtag.setGeneralTag( "LASER" );


  // fetch the RunIOV from the DB
  bool foundRunIOV = false;
  if ( econn ) {
    try {
      cout << "[EcalLaserShapeTools][beginRunDb] Fetching RunIOV ... " << flush;
      runiov_ = econn->fetchRunIOV(&runtag, run_);
      //      runiov_ = econn->fetchRunIOV(location_, run_);
      cout << "done." << endl;
      foundRunIOV = true;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
      foundRunIOV = false;
    }
  }



  // begin - setup the RunIOV (on behalf of the DAQ, if needed)
  if ( ! foundRunIOV ) {

    Tm startRun;

    startRun.setToCurrentGMTime();

    runiov_.setRunNumber(run_);
    runiov_.setRunStart(startRun);
    runiov_.setRunTag(runtag);

    if ( econn ) {
      try {
        cout << "[EcalLaserShapeTools][beginRunDb] Inserting missing RunIOV ... " << flush;
        econn->insertRunIOV(&runiov_);
        cout << "done." << endl;
      } catch (runtime_error &e) {
        cerr << e.what() << endl;
      }
    }

  }
  // end - setup the RunIOV (on behalf of the DAQ)



  string st = runiov_.getRunTag().getRunTypeDef().getRunType();
  if ( st == "LASER" )
    {
      cout << "runtype from db: LASER" << endl; 
    }
  else
    {
      cout << "runtype from db is not LASER, rather "  << st << endl; 
    }


  cout << endl;
  cout << "=============RunIOV:" << endl;
  cout << "Run Number:         " << runiov_.getRunNumber() << endl;
  cout << "Run Start:          " << runiov_.getRunStart().str() << endl;
  cout << "Run End:            " << runiov_.getRunEnd().str() << endl;
  cout << "====================" << endl;
  cout << endl;
  cout << "=============RunTag:" << endl;
  cout << "GeneralTag:         " << runiov_.getRunTag().getGeneralTag() << endl;
  cout << "Location:           " << runiov_.getRunTag().getLocationDef().getLocation() << endl;
  cout << "Run Type:           " << runiov_.getRunTag().getRunTypeDef().getRunType() << endl;
  cout << "====================" << endl;
  cout << endl;



  // getting LMF run IOV ready
  LMFiov_.setRunIOV(runiov_);
  LMFRunTag lmftag;
  lmftag.setGeneralTag("test-tb");
  LMFiov_.setLMFRunTag( lmftag);


  EcalLogicID ecid;
  LMFLaserBlueShapeDat   c1;
  map<EcalLogicID, LMFLaserBlueShapeDat > dataset1;


  for ( int ie = 1; ie <= 85; ie++ ) {
    for ( int ip = 1; ip <= 20; ip++ ) {
      c1.setAlpha(1);
      c1.setAlphaRMS(1);
      c1.setBeta(1);
      c1.setBetaRMS(1);

      int ic = (ip-1) + 20*(ie-1) + 1;
      if ( econn ) {
	try {
	  ecid = econn->getEcalLogicID("EB_crystal_number", ism_, ic);
	  dataset1[ecid] = c1;
	} catch (runtime_error &e) {
	  cerr << e.what() << endl;
	}
      }
    }// end loop on phi
  }// end loop on eta
  
  if ( econn ) {
    try {
      cout << "Inserting LMFLaserBlueShapeDat ... " << flush;
      if ( dataset1.size() != 0 ) econn->insertDataSet(&dataset1, &LMFiov_);  
      cout << "done." << endl;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
    }
  }


  if ( econn ) {
    try {
      cout << "Closing DB connection ..." << endl;
      delete econn;
      econn = 0;
    } catch (runtime_error &e) {
      cerr << e.what() << endl;
    }
  }












}// end of beginRunDb





// fixme: has to be moved into a producer/Algo
PNfit  EcalLaserShapeTools::maxPNDiode(const EcalPnDiodeDigi& pndigi) {
  const int ns = 50;
  double sample[ns],ampl[ns];
  double sampmax=0 ;
  int imax=0 ;
  for(int is=0; is < ns ; ++is) {
    sample[is] = is;
    ampl[is] = pndigi.sample(is).adc();
    if(ampl[is]>sampmax) {
      sampmax=ampl[is] ;
      imax = is ;
    }
  }
  // compute maximum with parabolic adjustement 
  double dt=0.5 ;
  double denom=2.*ampl[imax]-ampl[imax-1]-ampl[imax+1];
  if(denom>0.) dt=0.5*(ampl[imax+1]-ampl[imax-1])/denom ;
  
  PNfit res;
  //res.max =ampl[imax]+(ampl[imax+1]-ampl[imax-1])*dt*0.25 ;
  //res.t0 =(double)imax+dt ;
  
  TGraph gpn(ns,sample,ampl);
  TF1  mypol3("mypol3","pol3",25,50);
  gpn.Fit("mypol3","QFR","",25,50);
  //TF1* pol3 = (TF1*) gpn.GetFunction("mypol3");
  
  res.max = mypol3.GetMaximum();
  res.t0    = mypol3.GetMaximumX();
  
  
  return res;
}


std::pair<int,int>  EcalLaserShapeTools::pnFromTT(const EcalTrigTowerDetId& tt) {
  return pnFromTTMap_[ tt.iTT() ];
  //return pnFromTTMap_[ 1 ];
}



void  EcalLaserShapeTools::initPNTTMap() {
  
  using namespace std;
  
  // pairs of PN diodes for groups of trigger towers
  pair<int,int> pair05,pair16,pair27,pair38,pair49;
  
  pair05.first=0;
  pair05.second=5;
  
  pair16.first=1;
  pair16.second=6;
  
  pair27.first=2;
  pair27.second=7;
  
  pair38.first=3;
  pair38.second=8;
  
  pair49.first=4;
  pair49.second=9;
  
  pnFromTTMap_[1] = pair05;
  pnFromTTMap_[2] = pair05;
  pnFromTTMap_[3] = pair05;
  pnFromTTMap_[4] = pair05;
  pnFromTTMap_[5] = pair16;
  pnFromTTMap_[6] = pair16;
  pnFromTTMap_[7] = pair16;
  pnFromTTMap_[8] = pair16;
  pnFromTTMap_[9] = pair16;
  pnFromTTMap_[10] = pair16;
  pnFromTTMap_[11] = pair16;
  pnFromTTMap_[12] = pair16;
  pnFromTTMap_[13] = pair16;
  pnFromTTMap_[14] = pair16;
  pnFromTTMap_[15] = pair16;
  pnFromTTMap_[16] = pair16;
  pnFromTTMap_[17] = pair16;
  pnFromTTMap_[18] = pair16;
  pnFromTTMap_[19] = pair16;
  pnFromTTMap_[20] = pair16;
  pnFromTTMap_[21] = pair27;
  pnFromTTMap_[22] = pair27; 
  pnFromTTMap_[23] = pair27;
  pnFromTTMap_[24] = pair27;
  pnFromTTMap_[25] = pair27;
  pnFromTTMap_[26] = pair27;
  pnFromTTMap_[27] = pair27;
  pnFromTTMap_[28] = pair27;
  pnFromTTMap_[29] = pair27;
  pnFromTTMap_[30] = pair27;
  pnFromTTMap_[31] = pair27;
  pnFromTTMap_[32] = pair27; 
  pnFromTTMap_[33] = pair27;
  pnFromTTMap_[34] = pair27;
  pnFromTTMap_[35] = pair27;
  pnFromTTMap_[36] = pair27;
  pnFromTTMap_[37] = pair38;
  pnFromTTMap_[38] = pair38;
  pnFromTTMap_[39] = pair38;
  pnFromTTMap_[40] = pair38;
  pnFromTTMap_[41] = pair38;
  pnFromTTMap_[42] = pair38; 
  pnFromTTMap_[43] = pair38;
  pnFromTTMap_[44] = pair38;
  pnFromTTMap_[45] = pair38;
  pnFromTTMap_[46] = pair38;
  pnFromTTMap_[47] = pair38;
  pnFromTTMap_[48] = pair38;
  pnFromTTMap_[49] = pair38;
  pnFromTTMap_[50] = pair38;
  pnFromTTMap_[51] = pair38;
  pnFromTTMap_[52] = pair38; 
  pnFromTTMap_[53] = pair49;
  pnFromTTMap_[54] = pair49;
  pnFromTTMap_[55] = pair49;
  pnFromTTMap_[56] = pair49;
  pnFromTTMap_[57] = pair49;
  pnFromTTMap_[58] = pair49;
  pnFromTTMap_[59] = pair49;
  pnFromTTMap_[60] = pair49;
  pnFromTTMap_[61] = pair49;
  pnFromTTMap_[62] = pair49; 
  pnFromTTMap_[63] = pair49;
  pnFromTTMap_[64] = pair49;
  pnFromTTMap_[65] = pair49;
  pnFromTTMap_[66] = pair49;
  pnFromTTMap_[67] = pair49;
  pnFromTTMap_[68] = pair49;

}


double EcalLaserShapeTools::fitpj(double * parout, int num_event ,
				  const std::vector<EBDataFrame >  &  theCrystalPulses, 
				  const std::vector<float> & thePnAmplitudes , 
				  std::vector< pair <float,float> > & reducedData ) 
{

  // temporarily gio 
  //  float adcval[100][100];

  // ******************************************************************
  // *  Definitions of variables used in the method                  
  // ******************************************************************
  double a1,a2,b1,b2;
  int iter,nevt;
  double bi[Nevt_num][2],dbi[Nevt_num][2];
  double zi[Nevt_num][2] ;
  int    ioktk[Nevt_num],iokchi2[Nevt_num],nk,nborn_min,nborn_max ;
  double cti[Nevt_num][6],dm1i[Nevt_num][4]; 
  double aiter[10][3],chi2_iter[10];       
  double par[4],tsig[1] ;
  double amp,delta[N_samples],delta2,fun; 
  int event_ok[Nevt_num] ;
  double num_fit_min[Nevt_num],num_fit_max[Nevt_num] ;
  int i,j,k,imax[Nevt_num];
  double ampmax[Nevt_num],dt,t;
  double chi2=0.,chi2s=0.,da1[N_samples],da2[N_samples],db1[N_samples],db2[N_samples]  ;
  double deriv_a1,deriv_a2,deriv_b1,deriv_b2 ;
  double fact1,fact2,expo ;
  double albet,dtsbeta,variab,alpha,beta,puiss ;
  double unsurs1 ;
  //  double unsurs2 ;
  double fit3 ;
  int numb_a,numb_b,numb_ab,numb_b2,numb_x,ndimi,ndimj ;
  //     
  matrice DA,DAT,BK,DB,DBT,C,CT,D,DM1,CDM1,CDM1CT,Z,CDM1Z,YK,Y,B,X,XINV,RES2 ;
  matrice A_CROISS,ZMCT ;
  //  
  //
 
  double amplu[N_samples] ;

  //
  parout[0] = 0. ;
  parout[1] = 0. ;
  parout[2] = 0. ;
  //
  //  Initialisation of fit parameters 
  // 
  
  double    a1ini = 1.6419 ; // alpha parameter initilalization
  double    a2ini = 1.4755 ; // beta parameter  
  a1 = a1ini ;
  a2 = a2ini ;
  
  
  for (i=0 ; i<Nevt_num ; i++) {
    iokchi2[i]=1 ;
    for (j=0 ; j<2 ; j++ ) {
      bi[i][j] = (double)0. ;
      dbi[i][j] = (double)0. ;
      zi[i][j]=(double)0. ;
      cti[i][j]=(double)0. ;
      dm1i[i][j]=(double)0. ;
    }
  }
  //
  //
  
 
  //
  //  Matrix initialisation 
  //
  ndimi = 2 ;
  ndimj = 2 ;
  numb_a = 2 ;
  
  numb_x = 1 ;
  numb_b = 2 ;
  numb_ab = numb_a*numb_b ;
  numb_b2 = numb_b*numb_b ;
  
  DA = cree_mat(numb_a,numb_x) ;
  
  DAT = cree_mat(numb_x,numb_a) ;
  BK = cree_mat_prod(DA,DAT) ;
  DB = cree_mat(numb_b,numb_x) ;
  DBT = cree_mat(numb_x,numb_b) ;
  C = cree_mat(numb_a,numb_b) ;
  CT = cree_mat(numb_b,numb_a) ;
  D = cree_mat_prod(DB,DBT) ;
  DM1 = cree_mat_prod(DB,DBT) ;
  CDM1 = cree_mat_prod(C,DM1) ;
  CDM1CT = cree_mat_prod(CDM1,CT) ;
  Z = cree_mat(numb_b,numb_x) ;
  CDM1Z =cree_mat_prod(CDM1,Z) ;
  YK =cree_mat(numb_a,numb_x) ;
  Y =cree_mat(numb_a,numb_x) ;
  B = cree_mat_prod(DA,DAT) ;
  X = cree_mat_prod(DA,DAT) ;
  XINV = cree_mat_prod(DA,DAT) ;
  RES2=cree_mat(numb_a,numb_x) ;
  A_CROISS = cree_mat(numb_a,numb_x) ;
  ZMCT = cree_mat(numb_b,numb_x) ;
  //
  // Loop on iterations 
  // 

  for (iter=0 ; iter < nb_iter ; iter++) {
    //    
    //    mise a zero des matrices generales                             
    //                                                                      
    //printf(" Debut de l'iteration numero %d \n",iter) ;
    zero_mat(CDM1Z) ;
    zero_mat(Y) ;
    zero_mat(CDM1CT) ;
    zero_mat(B) ;
    zero_mat(X) ;
    zero_mat(CDM1) ;
    // 
    //	
    nk = -1 ;	
    aiter[iter][0] = a1 ;
    aiter[iter][1] = a2 ;


    //
    //					
    //  Loop on nevt to be read
    //   to be replaced with loop on theCrystalEvents
    //    for (nevt=0 ; nevt < num_event ; nevt++) {// OLD loop on pulses
    
    nevt =-1;
    std::vector< EBDataFrame>::const_iterator thePulse;
    //    std::vector< pair<float, int> >::const_iterator thePulse;
    for (thePulse = theCrystalPulses.begin(); thePulse != theCrystalPulses.end(); thePulse ++)
      {
	nevt ++;
	// gio
	//theCrystalEvents
	// std::vector<EBDataFrame >  &  theCrystalEvents) 
	//EcalRawDataCollection::const_iterator headerItr ;
	
	//      
	//    
	//                                           		             
	//       B1 = BI[nk,1] is the signal amplitude                   
	//       B2 = BI[nk,2] is the signal timing            
	//                                                 
	//                                                                      
	zero_mat(Z) ;
	zero_mat(YK) ;
	zero_mat(BK) ;
	zero_mat(C) ;
	zero_mat(D) ;
	//
	//
	  
	nk=nevt ;   
	ampmax[nk] = 0. ;
	imax[nk] = 0 ;
	for ( k = 0 ; k < N_samples ; k++) {     
	  amplu[k] =  ((double) (* thePulse).sample(k).adc() ) ;    // get the pulses the new way
	  amplu[k]/=  thePnAmplitudes[ nevt ];

	  // gio go away
	  //	  LogDebug("EcalLaserShapeTools") << "sample is: " << k << " ampl is: " << amplu[k] << endl;
	  
	  if (amplu[k] > ampmax[nk] ) {
	    ampmax[nk] = amplu[k] ;
	    imax[nk] = k ; 
	  }
	}// end loop on samples
       
	if( iter == 0 ) {
	  double parout[3] ;
	  //
	  // evaluate starting point with parabolic adjustement on the 3 higher samples
	  //
	  fit3 =parab(amplu,2,9,parout) ;  
	
	  num_fit_min[nevt] = (double)imax[nk] - (double)nsmin ;
	  num_fit_max[nevt] = (double)imax[nk] + (double)nsmax ;
	
	

	  bi[nk][0] = parout[0]-(amplu[0]+amplu[1])/2. ; // to remove dynamically the pedestal from the maximum
	  bi[nk][1] = parout[1] ;

	}
	else {
	  // 	                                                             
	  //  in other iterations  :                                              
	  //   increment bi[][] parameters with bdi[][]                         
	  //   calculated in previous                                           
	  //   iteration 	    			                    			    
	  // 	                            						           
	  bi[nk][0] +=  dbi[nk][0] ;
	  bi[nk][1] +=  dbi[nk][1] ;
	
	}
	b1 = bi[nk][0] ;
	b2 = bi[nk][1] ;
	
                                                               
	//                                   
	//    loop on hits in a single cluster		                    
	//            -------------------------------------------------------------------------
	//	       						                                 
	chi2 = 0. ;
	ioktk[nk] = 0 ;
	ns = N_samples ;
	nborn_min = (int)num_fit_min[nevt] ;
	nborn_max = (int)num_fit_max[nevt] ;
	//
	int kk=-1 ;
        
	for (k=nborn_min ; k <= nborn_max ; k++){
	  //
	  //	calculation of fonction used to fit 				             
	  //	
	  if(amplu[k]<=0.){// printf(" %f \n",amplu[k]) ;
	    LogDebug("EcalLaserShapeTools") << " ======>>> attention adc  = " << amplu[k] << std::endl;
	  }


	  dt =(double)k - b2 ;
	  t = (double)k ;
	  amp = amplu[k]-(amplu[0]+amplu[1])/2. ; // remove dynamically the pedestal for each samples
   

	  unsurs1 = 1./sigma_rec ;
 
	  //	                                                                   
	  // Here to use the fitted function
	  //
	
	  tsig[0] =(double)k  ;
	
	  par[0]=  b1 ;
	  par[1] = b2 ;
	  par[2] = a1 ;
	  par[3] = a2 ;

	  fun = pulseShapepj( tsig , par) ;
	  //	
	  //	
	  //	 we need to determine a1,a2,a3 which are global parameters         
	  //	  and  b1,b2 which are parameters for each individual signal       
	  //	  we first begin to calculate the derivatives used in the          
	  //    following calculation 					     
	  //	                                                                   
	  //          
	
	  alpha = a1 ;
	  beta  = a2 ;
	  //
	  albet=alpha*beta;
	  if(dt > -albet)  { 
	    variab = (double)1. + dt/albet ;
	    dtsbeta = dt/beta ;
	    expo = exp(-dtsbeta) ;	 
	    puiss = pow(variab,alpha) ;
	    fact1 = puiss*expo ;         
	    deriv_b1 = unsurs1*fun/b1 ;
	    fact2 =  fun ;
	    deriv_b2 = unsurs1*fact2*dtsbeta/(albet*variab) ;
	    deriv_a1 = unsurs1*fact2*(log(variab)-dtsbeta/(alpha*variab)) ;
	    deriv_a2 = unsurs1*fact2*dtsbeta*dtsbeta/(albet*variab) ;
	  }
	  else
	    {
	      continue;
	    }

	  kk++ ;
	
	  da1[kk] = deriv_a1 ;
	  da2[kk] = deriv_a2 ;
	  db1[kk] = deriv_b1 ;
	  db2[kk] = deriv_b2 ;
	  delta[kk] = (amp - fun)*unsurs1 ;
	  //
	  //gio
	  //	  std::LogDebug("EcalLaserShapeTools") << "function " << fun << "amplitude "<< amp << "delta " << delta[kk] << std::endl ;
	  chi2 = chi2 + delta[kk]*delta[kk]   ;
	  chi2 /= float(num_event) ; // general chi2 ...

	}
	//	
   
    
   
	for(int k1=0 ; k1<=kk ; k1++) {
		
	  DA.coeff[0][0] = da1[k1];
	  DA.coeff[1][0] = da2[k1] ;
	  DAT.coeff[0][0]= da1[k1] ;
	  DAT.coeff[0][1]= da2[k1] ;
	  DB.coeff[0][0] = db1[k1] ;
	  DB.coeff[1][0] = db2[k1] ;
	  DBT.coeff[0][0]= db1[k1] ;
	  DBT.coeff[0][1]= db2[k1] ;
	  //
	  //  Here to compute derivative matrix : matrix b[2][2]  
	  //
	  produit_mat_int(DA,DAT,BK) ;
	  //
	  //  Here to compute matrix c[2][2]	  	                    
	  //	
	  produit_mat_int(DA,DBT,C) ;
	  //	
	  //	Here to compute matrix d[2][2]	                             
	  //
	  produit_mat_int(DB,DBT,D) ;
	  //
	  //  compute matrix y[3] and z[2] depending of delta (amp-fun)        
	  //
	  //				             */
	  delta2 = delta[k1] ;

	  somme_mat_int_scale(DA,YK,delta2) ;
	  //				
	  somme_mat_int_scale(DB,Z,delta2) ;

	  ioktk[nk]++ ;
	  //
	  // end of loop on samples
	}
	//  remove event if too few samples used ...
	event_ok[nk]=1 ;
	if(ioktk[nk] < 3 ) {
	  event_ok[nk]=0 ;
	  continue ;
	}
	chi2s = chi2  ;
	chi2_iter[iter]=chi2s ;
	//
	//       	Here to transpose matrix C ---> CT                        
	//     
	transpose_mat(C,CT) ;
     
	//     
	// Here to calculate DM1 (inverse of D matrix 2x2)  	          
	//     
  
	inverse_mat(D,DM1) ;
	//	                                                            
	//  Set matrix product c*d in memory in order to compute variations    
	//   of parameters B at the end of the iteration loop                   
	//   the variations of parameters b are dependant of the variations of  
	//   parameters da[1],da[2]                                            
	//      
	cti[nk][0] = CT.coeff[0][0]  ;
	cti[nk][1] = CT.coeff[0][1]  ;
	cti[nk][2] = CT.coeff[1][0]  ;
	cti[nk][3] = CT.coeff[1][1]  ;

	//
	dm1i[nk][0] = DM1.coeff[0][0] ;
	dm1i[nk][1] = DM1.coeff[0][1] ;
	dm1i[nk][2] = DM1.coeff[1][0] ;
	dm1i[nk][3] = DM1.coeff[1][1] ; 
	//
	zi[nk][0] = Z.coeff[0][0]  ;
	zi[nk][1] = Z.coeff[1][0]  ;
	//      
    	   
     
	//       Here to sum the matrix b and y after every event            
      
	for( k=0 ; k< numb_a ; k++) Y.coeff[k][0] += YK.coeff[k][0] ;
      
	somme_mat_int(BK,B) ;
	//    
	// Here to calculate c(d-1)                                     
	//
	produit_mat(C,DM1,CDM1) ;
	//
	// Then compute c(d-1)ct                                         
	//    
	produit_mat_int(CDM1,CT,CDM1CT);
	//                                                                 
	// Then compute c(d-1)z                                          
	//                                                             
	produit_mat_int(CDM1,Z,CDM1Z) ;


	//----------------------------------------------------------------------------------      
	// end of loops on events
      }
    //   
    //   When the 2 loops ( signals and events are ended , compute b-cdm1ct*/
    //   







    diff_mat(B,CDM1CT,X) ;
    //
    //    
    //    

    inverse_mat(X,XINV) ;
    diff_mat(Y,CDM1Z,RES2) ;
    //                                                                  
    // Calculation is now easy for da[0] da[1]                         
    //                                                                
    produit_mat(XINV,RES2,A_CROISS) ;
                                     
    // now we compute variations of parameters
    //                                                                  
 
   
    for( k=0 ; k< nk ; k++) {
      //
      if(event_ok[k]==0) continue ; // remove bad events

		
      ZMCT.coeff[0][0] = zi[k][0] - (cti[k][0]*A_CROISS.coeff[0][0]+
				     cti[k][1]*A_CROISS.coeff[1][0]) ;
      ZMCT.coeff[1][0] = zi[k][1] - (cti[k][2]*A_CROISS.coeff[0][0]+
				     cti[k][3]*A_CROISS.coeff[1][0]) ;

      //
      dbi[k][0] = dm1i[k][0]*ZMCT.coeff[0][0] + dm1i[k][1]*ZMCT.coeff[1][0] ;
      dbi[k][1] = dm1i[k][2]*ZMCT.coeff[0][0] + dm1i[k][3]*ZMCT.coeff[1][0] ;

      // copy local array into global array  	 
      // [0] is  amplitude, [1] is time
      db_i[k][0] = bi[k][0]+ dbi[k][0]   ;
      db_i[k][1] = bi[k][1]+ dbi[k][1]   ;
      

    }

	
    //	
    a1 += A_CROISS.coeff[0][0] ;
    a2 += A_CROISS.coeff[1][0] ;
    LogDebug("EcalLaserShapeTools") << "iter " << iter << " alpha " << a1 << " beta " << a2 << std::endl ; 
    // end of iteration if chi2 is stable
    // uncomment this in case of APD not divided by PN
    // if(iter> 0 && ((chi2_iter[iter]-chi2_iter[iter-1])/chi2_iter[iter-1])<0.01 ) break ;


    // end of iteration loop */
  }

  
  for( k=0 ; k< ((int)theCrystalPulses.size()) ; k++) {
    //
    if(event_ok[k]==0) continue ; // remove bad events
    // [0] is  amplitude, [1] is time
    pair <float,float> theReducedDatum(db_i[k][0],db_i[k][1]);
    LogDebug("EcalLaserShapeTools")  << "[fitpj after iterations] ampli " << db_i[k][0] << " time " << db_i[k][1] << endl;
    reducedData.push_back(theReducedDatum);
  }




  parout[0] = a1 ;    // alpha
  parout[1] = a2 ;    // beta
  parout[2] = chi2s ;

  LogDebug("EcalLaserShapeTools") << "alpha is: " << a1  << " beta " << a2  << endl;

  //   LogDebug("EcalLaserShapeTools") << "cry: " << 1 << ""
  //     hitCollection_
  // 				 << " produced by " << hitProducer_ << std::endl;
  


  return chi2 ;
}

void EcalLaserShapeTools::produit_mat(matrice A , matrice B , matrice M)
{
  int i,j,k ;
  //  resultat du produit A*B = M 
  if(A.nb_colonnes != B.nb_lignes) {
    printf( " Erreur : produit de matrices de tailles incompatibles \n ");
    M.coeff = NULL ;
    return ;
  }
  M.nb_lignes = A.nb_lignes ;
  M.nb_colonnes = B.nb_colonnes ;
  zero_mat(M) ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) {
      for(k=0 ; k< A.nb_colonnes; k++){
	M.coeff[i][j] += A.coeff[i][k]*B.coeff[k][j] ;
      }
    }
  }
  return  ;
}

void EcalLaserShapeTools::produit_mat_int(matrice A , matrice B, matrice M)
{
  int i,j,k ;
  if(A.nb_colonnes != B.nb_lignes) {
    printf( " Erreur : produit de matrices de tailles incompatibles \n ");
    M.coeff = NULL ;
    return ;
  }
  M.nb_lignes = A.nb_lignes ;
  M.nb_colonnes = B.nb_colonnes ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) {
      for(k=0 ; k< A.nb_colonnes; k++){
	M.coeff[i][j] += A.coeff[i][k]*B.coeff[k][j] ;
      }
    }
  }
  return  ;
}
void EcalLaserShapeTools::diff_mat(matrice A , matrice B , matrice M)
{
  int i,j ;
  //resultat de la difference A-B = M 
  if(A.nb_lignes != B.nb_lignes) {
    printf( " Erreur : difference de matrices de tailles incompatibles \n ");
    M.coeff = NULL ;
    return ;
  }
  M.nb_lignes = A.nb_lignes ;
  M.nb_colonnes = A.nb_colonnes ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j < M.nb_colonnes ; j++) {
      M.coeff[i][j] = A.coeff[i][j] - B.coeff[i][j] ;
    }
  }
  return  ;
  
}
void EcalLaserShapeTools::copie_colonne_mat(matrice A , matrice M , int nk)
{
  int i,j ;
  int k ;
  /* resultat de la copie de A dans un vecteur colonne M */
  k = 0 ;
  for(i=0 ; i< A.nb_lignes; i++) {
    for(j=0 ; j < A.nb_colonnes ; j++) {
      M.coeff[nk][k] = A.coeff[i][j] ;
      printf(" copie nk %d  i %d j %d k %d A %e M %e \n ",nk,i,j,k,A.coeff[i][j],
	     M.coeff[nk][k]);      
      k++ ;
    }
  }
  return  ;
}

void EcalLaserShapeTools::somme_mat_int(matrice A , matrice M)
{
  int i,j;
  /* resultat de la somme integree M += A */
  if(A.nb_lignes != M.nb_lignes) {
    printf( " Erreur : somme de matrices de tailles incompatibles \n ");
    M.coeff = NULL ;
    return ;
  }
  M.nb_lignes = A.nb_lignes ;
  M.nb_colonnes = A.nb_colonnes ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) 
      M.coeff[i][j] += A.coeff[i][j] ;
  }
  return  ;
}
void EcalLaserShapeTools::somme_mat_int_scale(matrice A , matrice M , double delta)
{
  int i,j ;
  M.nb_lignes = A.nb_lignes ;
  M.nb_colonnes = A.nb_colonnes ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) M.coeff[i][j] += A.coeff[i][j]*delta ;
  }
  return  ;
}
void EcalLaserShapeTools::transpose_mat(matrice A , matrice M)
{
  int i,j;
  // resultat de la transposition = matrice M 
  for(i=0 ; i< A.nb_lignes; i++) {
    for(j=0 ; j< A.nb_colonnes ; j++) {
      M.coeff[j][i] = A.coeff[i][j]  ;
    }
  }
  return  ;
}

matrice EcalLaserShapeTools::cree_mat_prod(matrice A , matrice B)
{
  int i,j;
  matrice M ; /* resultat de la creation */
  
  M.nb_lignes = A.nb_lignes ;
  M.nb_colonnes = B.nb_colonnes ;
  M.coeff = (double**)malloc(M.nb_lignes*sizeof(double*)) ;
  for(i=0 ; i< M.nb_lignes; i++) 
    M.coeff[i]=(double*)calloc(M.nb_colonnes,sizeof(double));
  for(i=0 ; i< M.nb_lignes; i++) {

    for(j=0 ; j< M.nb_colonnes ; j++) {
      M.coeff[i][j] = 0.  ;
    }
  }
  //printf(" creation de matrice ---->  nlignes %d ncolonnes %d  \n",
  //M.nb_lignes,M.nb_colonnes) ;
  return (M) ;
}

matrice EcalLaserShapeTools::cree_mat(int n_lignes,int n_colonnes)
{
  int i,j;
  matrice M ; /* resultat de la creation */
 
  M.nb_lignes = n_lignes ;
  M.nb_colonnes = n_colonnes ;
  M.coeff = (double**)malloc(M.nb_lignes*sizeof(double*)) ;
  for(i=0 ; i< M.nb_lignes; i++) 
    M.coeff[i]=(double*)calloc(M.nb_colonnes,sizeof(double));
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) {
      M.coeff[i][j] = 0.  ;
    }
  }
  //printf(" creation de matrice --->  nlignes %d ncolonnes %d  \n",
  // M.nb_lignes,M.nb_colonnes) ;
  return (M) ;
}

void EcalLaserShapeTools::fill_mat( matrice A , matrice M)
{
  int i,j;
  /* on remplit la matrice M avec la matrice A */
 
  M.nb_lignes = A.nb_lignes ;
  M.nb_colonnes = A.nb_colonnes ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) {
      M.coeff[i][j] = A.coeff[i][j]  ;
      printf("matrice remplie %e \n",M.coeff[i][j]) ;
    }
  } 
  return ;
}
void EcalLaserShapeTools::print_mat(matrice M)
{
  int i,j ;
  if( M.coeff == NULL) 
    printf(" erreur : affichage d'une matrice vide \n") ;
  printf(" m_nli %d M_ncol %d \n",M.nb_lignes,M.nb_colonnes) ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) 
      printf(" MATRICE i= %d j= %d ---> %e \n",i,j,M.coeff[i][j]) ;
  }
  //printf(" apres passage d'impression \n") ;
  return ;
}
void EcalLaserShapeTools::zero_mat(matrice M)
{
  int i,j ;
  for(i=0 ; i< M.nb_lignes; i++) {
    for(j=0 ; j< M.nb_colonnes ; j++) M.coeff[i][j]=0. ; 
  }
  return ;
}
void EcalLaserShapeTools::zero_mat_nk(matrice M,int nk)
{
  int j ;
  for(j=0 ; j< M.nb_colonnes ; j++) M.coeff[nk][j]=0. ;
  return ;
}
void EcalLaserShapeTools::print_mat_nk(matrice M,int nk)
{
  int j ;
  if( M.coeff == NULL)
    printf(" erreur : affichage d'une matrice vide \n") ;
  printf(" nk = %d m_nli %d M_ncol %d \n",nk,M.nb_lignes,M.nb_colonnes) ;
  for(j=0 ; j< M.nb_colonnes ; j++) 
    printf(" MATRICE nk= %d j= %d  ---> %e \n",nk,j,M.coeff[nk][j]) ;    
  printf(" apres passage d'impression \n") ;
  return ;
}
void EcalLaserShapeTools::inverse_mat( matrice A , matrice M )
{
  /*   A[ligne][colonne]   B[ligne][colonne]   */
  int i , j   ;
  double  deter=0.  ;
  /*  M est la matrice inverse de A */
 
  if(A.nb_lignes != A.nb_colonnes) {
    printf( " attention matrice non inversible !!!! %d lignes %d colonnes \n",
	    A.nb_lignes,A.nb_colonnes) ;
    return ;
  }
  zero_mat(M) ;
  if(A.nb_lignes == 2) {
    deter = A.coeff[0][0]*A.coeff[1][1] - A.coeff[0][1]*A.coeff[1][0] ;
    M.coeff[0][0] = A.coeff[1][1]/deter ;
    M.coeff[0][1] = -A.coeff[0][1]/deter ;
    M.coeff[1][0] = -A.coeff[1][0]/deter ;
    M.coeff[1][1] = A.coeff[0][0]/deter ;
  }
  else if(A.nb_lignes == 3) {
    M.coeff[0][0]=A.coeff[1][1]*A.coeff[2][2]-A.coeff[2][1]*A.coeff[1][2] ;
    M.coeff[1][1]=A.coeff[0][0]*A.coeff[2][2]-A.coeff[2][0]*A.coeff[0][2] ;

    M.coeff[2][2]=A.coeff[0][0]*A.coeff[1][1]-A.coeff[0][1]*A.coeff[1][0] ;
    M.coeff[0][1]=A.coeff[2][1]*A.coeff[0][2]-A.coeff[0][1]*A.coeff[2][2] ;
    M.coeff[0][2]=A.coeff[0][1]*A.coeff[1][2]-A.coeff[1][1]*A.coeff[0][2] ;
    M.coeff[1][0]=A.coeff[1][2]*A.coeff[2][0]-A.coeff[1][0]*A.coeff[2][2] ;
    M.coeff[1][2]=A.coeff[1][0]*A.coeff[0][2]-A.coeff[0][0]*A.coeff[1][2] ;
    M.coeff[2][0]=A.coeff[1][0]*A.coeff[2][1]-A.coeff[1][1]*A.coeff[2][0] ;
    M.coeff[2][1]=A.coeff[0][1]*A.coeff[2][0]-A.coeff[0][0]*A.coeff[2][1] ;
    deter=A.coeff[0][0]*M.coeff[0][0]+A.coeff[1][0]*M.coeff[0][1]
      +A.coeff[2][0]*M.coeff[0][2] ;
    for ( i=0 ; i<3 ; i++ ) {
      for ( j=0 ; j<3 ; j++ ) M.coeff[i][j] = M.coeff[i][j]/deter  ;
    }
  }
  else {
    printf(" Attention , on ne peut inverser la MATRICE %d \n",A.nb_lignes) ;
    return ;
  }
  
  return ;
}

/*------------------------------------------------------------------*/

double EcalLaserShapeTools::inverpj(int n,double g[N_samples][N_samples],double ginv[N_samples][N_samples] )
{
  /*                                                                   */
  /*  inversion d une matrice symetrique definie positive de taille n  */
  /*  J.P. Pansart   Novembre 99                                       */
  /*                                                                   */
  int i , j , k , jj  ;
  double r ,  s  ;
  double deter=0  ;
  double al[N_samples][N_samples] , be[N_samples][N_samples]  ;
  /*   initialisation                                                  */
  for( i=0 ; i<n ; i++ ) {
    for ( j=0 ; j<n ; j++ ) {
      al[i][j] = 0.  ;
      be[i][j] = 0.  ;
    }
  }
  /*  decomposition en vecteurs sur une base orthonormee               */
  al[0][0] =  sqrt( g[0][0] )  ;
  for ( i=1 ; i<n ; i++ ) {
    al[i][0] = g[0][i] / al[0][0]  ;
    for ( j=1 ; j<=i ; j++ ) {
      s=0.   ;
      for ( k=0 ; k<=j-1 ; k++ ) {
	s=s+ al[i][k] * al[j][k]  ;
      }
      r= g[i][j] - s   ;
      if( j < i ) al[i][j] = r/al[j][j]  ;
      if( j == i ) al[i][j] =  sqrt ( r)  ;
    }
  }
  /*  inversion de la matrice al                                       */
  be[0][0] = 1./al[0][0]  ;
  for ( i=1 ; i<n ; i++ ) {
    be[i][i] = 1. / al[i][i]  ;
    for ( j=0 ; j<i ; j++ ) {
      jj=i-j-1  ;
      s=0.   ;
      for ( k=jj+1 ; k<=i ; k++ ) {
	s=s+ be[i][k] * al[k][jj]  ;
      }
      be[i][jj]=-s/al[jj][jj]  ;
    }
  }
  /*   calcul de la matrice ginv                                       */
  for ( i=0 ; i<n ; i++ ) {
    for ( j=0 ; j<n ; j++ ) {
      s=0.   ;
      for ( k=0 ; k<n ; k++ ) {
	s=s+ be[k][i] * be[k][j]  ;
      }
      ginv[i][j]=s  ;
#ifdef DEBUG
      printf("valeur de la matrice %d %d %f \n",i,j,ginv[i][j]) ;
#endif
    }
  }
  return deter ;
}
/*                                                                   */
/*  inversion d une matrice 3x3                                      */
/*                                                                   */
double EcalLaserShapeTools::inv3x3(double a[3][3] , double b[3][3] )
{
  /*   a[ligne][colonne]   b[ligne][colonne]   */
  int i , j   ;
  double  deter=0.  ;
  b[0][0]=a[1][1]*a[2][2]-a[2][1]*a[1][2] ;
  b[1][1]=a[0][0]*a[2][2]-a[2][0]*a[0][2] ;
  b[2][2]=a[0][0]*a[1][1]-a[0][1]*a[1][0] ;
  printf("a[x][x] %e %e %e %e %e %e %e \n",a[0][0],a[1][1],a[0][1],a[1][0],
	 a[0][0]*a[1][1],a[0][1]*a[1][0],b[2][2]);
  b[0][1]=a[2][1]*a[0][2]-a[0][1]*a[2][2] ;
  b[0][2]=a[0][1]*a[1][2]-a[1][1]*a[0][2] ;
  b[1][0]=a[1][2]*a[2][0]-a[1][0]*a[2][2] ;
  b[1][2]=a[1][0]*a[0][2]-a[0][0]*a[1][2] ;
  b[2][0]=a[1][0]*a[2][1]-a[1][1]*a[2][0] ;
  b[2][1]=a[0][1]*a[2][0]-a[0][0]*a[2][1] ;
  deter=a[0][0]*b[0][0] + a[1][0]*b[0][1] + a[2][0]*b[0][2] ;
  printf(" deter = %e \n",deter) ;
  for ( i=0 ; i<3 ; i++ ) {
    for ( j=0 ; j<3 ; j++ ) {
      printf(" avant division a[3][3] %d %d  %e \n",i,j,a[i][j]) ;
      printf(" avant division b[3][3] %d %d  %e %e \n",i,j,b[i][j],deter) ;
      b[i][j] = b[i][j]/deter  ;
      printf(" valeur de b[3][3] apres division %d %d  %e %e \n",i,j,b[i][j],deter) ;
    }
  }
  return deter ;
}

double EcalLaserShapeTools::pulseShapepj( Double_t *x, Double_t *par )
{

  Double_t fitfun;
  Double_t ped, h, tm, alpha, beta;
  Double_t dt, dtsbeta, albet, variab, puiss ;
  Double_t b1,b2,a1,a2 ;
  b1 = par[0] ;
  b2 = par[1] ;
  a1 = par[2] ;
  a2 = par[3] ;
  ped   =  0. ;
  h     =  b1 ;
  tm    =  b2 ;
  alpha =  a1 ;
  beta  =  a2 ;
  dt= x[0] - tm  ;
  //printf(" par %f %f %f %f dt = %f albet = %f",b1,b2,a1,a2,dt,albet) ;
  albet = alpha*beta ;
  if( albet <= 0 )return( (Double_t)0. );

  if(dt > -albet)  {
    dtsbeta=dt/beta ;
    variab=1.+dt/albet ;
    puiss=pow(variab,alpha);
    fitfun=h*puiss*exp(-dtsbeta) + ped;
    //printf(" dt = %f h = %f puiss = %f exp(-dtsbeta) %f \n",dt,h,puiss,
    // exp(-dtsbeta)) ;
  }
  else {
    fitfun = ped;
  }

  return fitfun;
}


double EcalLaserShapeTools::parab(Double_t ampl[N_samples],Int_t nmin,Int_t nmax,Double_t parout[3])
{
  /* Now we calculate the parabolic adjustement in order to get        */
  /*    maximum and time max                                           */
  
  double denom,dt,amp1,amp2,amp3 ; 
  double ampmax = 0. ;				
  int imax = 0 ;
  int k ;
  /*
   */	  
  for ( k = nmin ; k < nmax ; k++) {
    if (ampl[k] > ampmax ) {
      ampmax = ampl[k] ;
      imax = k ;
    }
  }
  amp1 = ampl[imax-1] ;
  amp2 = ampl[imax] ;
  amp3 = ampl[imax+1] ;
  denom=2.*amp2-amp1-amp3  ;
  /* 							             */	      
  if(denom>0.){
    dt =0.5*(amp3-amp1)/denom  ;
  }
  else {
    // gio
    LogDebug("EcalLaserShapeTools") << "[LaserParab] denom= " << denom << " (caused by small ampli?)" << endl;
    //    printf("denom =%f\n",denom)  ;
    dt=0.5  ;
  }
  /* 						                     */	       
  /* ampmax correspond au maximum d'amplitude parabolique et dt        */
  /* decalage en temps par rapport au sample maximum soit k + dt       */
		
  parout[0] =amp2+(amp3-amp1)*dt*0.25 ;
  parout[1] = (double)imax + dt ;
  parout[2] = (double)imax ;
  return denom ;
}

double EcalLaserShapeTools::ComputePulseWidth( double alpha_here , double beta_here ){ 

  // fixed parameters
  double amplitude   = 1.00 ;
  double offset      = alpha_here*beta_here+2.; // evaluate time of max approx...  
  double amp_max     = amplitude;

  // steps in time
  double t_min       =  offset-1.50;
  double t_max       =  offset+5.50;
  int    t_step_max  = 1000;
  double delta_t     =  (double)((t_max-t_min)/t_step_max);
  double reference=0.8 ; // computing width at this level (0.8 = 80%) of the signal ...    
  // Loop over time ( Loop 2  --> get width )
  int    t_amp_half_flag =    0;
  double t_amp_half_min  =  999.; 
  double t_amp_half_max  = -999.; 
  for (int t_step=0; t_step<t_step_max; t_step++){
    double t_val = t_min + (double)t_step*delta_t;
    double amp =  amplitude*TMath::Power( ( 1 + ( (t_val-offset) / (alpha_here*beta_here) ) ) , alpha_here ) * TMath::Exp(-1.0*((t_val-offset)/beta_here));
    if( amp > (amp_max*reference) && t_amp_half_flag == 0){
      t_amp_half_flag = 1;
      t_amp_half_min = t_val;
    }
    if( amp < (amp_max*reference) && t_amp_half_flag == 1){
      t_amp_half_flag = 2;
      t_amp_half_max = t_val;
    }          
  } // end loop over time
      
  // 4] Compute Width
  double width =   (t_amp_half_max - t_amp_half_min);
 
  return width;
} // end ComputePulseWidth
