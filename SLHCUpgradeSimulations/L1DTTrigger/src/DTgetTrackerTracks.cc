#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"


// ********************************
// *** Tracker Tracks *******
// ********************************

// 120529PLZ Get Tracker Tracks
// begin
// Framework handles for the EVENT hits, digis, etc...

using namespace cmsUpgrades;

void DTL1SimOperations::getTrackerTracks(edm::Event& event, 
					    const edm::EventSetup& eventSetup)
{
//    outAscii << "Checking tracker Tracks " << endl;
  
  edm::Handle<L1TkTrack_PixelDigi_Collection> DigiTrackHandle;
  try {
    event.getByLabel("L1TkTracksFromPixelDigis",
		     "Level1TracksHelFitVtxYes", 
		     DigiTrackHandle);
  }
  catch(...) {
    cout << "exception event.getByLabel(\"TracksFromPixelDigis\", "
	 << "DigiTracksHandle) at event " << EvtCounter << endl;
  }
 
  int tk_kill[1000]={ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  	          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  	          };
  
  const L1TkTrack_PixelDigi_Collection *DigiTracks = 
    DigiTrackHandle.product();
  L1TkTrack_PixelDigi_Collection::const_iterator DigiTrackIter;
    int ntracks = DigiTracks->size() ;
//    outAscii << " # tracks " << DigiTracks->size() << endl;
   unsigned int minForDupl = 2; 
    /// Loop over pairs Candidate L1TkTracks
    if ( DigiTracks->size() != 0 &&  DigiTracks->size() < 1000) {
      for ( unsigned int tk = 0; tk < DigiTracks->size()-1; tk++ ) {

        L1TkStub_PixelDigi_Collection tkStubs0 = (*DigiTracks).at(tk).getStubs();
       unsigned int tkNum0 = tkStubs0.size(); 
 //   outAscii << " tknum0 " << tkNum0 << endl;
        /// This prevents from reading already deleted tracks
	if (tkNum0 == 0) continue;
        if ( tk_kill[tk] == 1 ) continue;
        unsigned int tkSeed0 = (*DigiTracks).at(tk).getSeedDoubleStack();

        /// Nested loop
        if ( (*DigiTracks).size() == 1 ) continue;
       for ( unsigned int tkk = tk+1; tkk < (*DigiTracks).size(); tkk++ ) {

          L1TkStub_PixelDigi_Collection tkStubs1 = (*DigiTracks).at(tkk).getStubs();
          unsigned int tkNum1 = tkStubs1.size();
          /// This prevents from reading already deleted tracks
	  if (tkNum1 == 0) continue;
          if ( tk_kill[tkk] == 1 ) continue;
          unsigned int tkSeed1 = (*DigiTracks).at(tkk).getSeedDoubleStack();

          unsigned int numShared = 0;
          for ( unsigned int st = 0; st < tkStubs0.size(); st++ ) {
            if ( numShared >= minForDupl ) continue;
            for ( unsigned int stt = 0; stt < tkStubs1.size(); stt++ ) {
              if ( numShared >= minForDupl ) continue;
              if ( tkStubs0.at(st).getClusterRef(0) == tkStubs1.at(stt).getClusterRef(0) &&
                   tkStubs0.at(st).getClusterRef(1) == tkStubs1.at(stt).getClusterRef(1) )
                numShared++;
            }
          } /// End of check if they are shared or not

          /// Skip to next pair if they are different
          if ( numShared < minForDupl ) continue;
//	  outAscii << tk << " " << tkk << " " << numShared << endl;
          /// Reject the one with the outermost seed
//	  outAscii << " seeds " << tkSeed0 << " " << tkSeed1 << endl;
//	  outAscii << " numst " << tkNum0 << " " << tkNum1 << endl;
          if ( tkSeed1 > tkSeed0 ) tk_kill[tkk]= 1; // = L1TkTrackType();
          else if ( tkSeed1 < tkSeed0 ) tk_kill[tk]= 1; //(*DigiTracks).at(tk) = L1TkTrackType();
          else {
            /// If they are from seeds in the same layer,
            /// use goodness of fit discrimination
//	  outAscii << (*DigiTracks).at(tkk).getChi2RPhi() << " " << (*DigiTracks).at(tk).getChi2RPhi() << endl;
            if ( (*DigiTracks).at(tkk).getChi2RPhi() / tkNum1 >
                 (*DigiTracks).at(tk).getChi2RPhi() / tkNum0 ) tk_kill[tkk]= 1; //(*DigiTracks).at(tkk) = L1TkTrackType();
            else if ( (*DigiTracks).at(tkk).getChi2RPhi() / tkNum1 <
                      (*DigiTracks).at(tk).getChi2RPhi() / tkNum0 )  tk_kill[tk]= 1; //(*DigiTracks).at(tk) = L1TkTrackType();
            else std::cerr<<"*** I CAN'T BELIEVE IT!! ***"<<std::endl;
          }

        } /// End of Nested loop 
      } /// End of Loop over pairs Candidate L1TkTracks
   }
//  for (int i=0 ; i <ntracks ; i++){ outAscii  << tk_kill[i] << " ";};
//  outAscii << endl;
  
  int tk = -1;
  for( DigiTrackIter = DigiTracks->begin();
       DigiTrackIter != DigiTracks->end();
       ++DigiTrackIter ) { 
   tk++;
   if(tk_kill[tk] == 1) continue; 

     /// Take into account only > 2 // PLZ (2 what? From Nicola Pozzobon May 29th,2012)
//     outAscii << "getStubs size " << DigiTrackIter->getStubs().size() << endl ;
//       if ( DigiTrackIter->getStubs().size() < 3 ) continue;
       
    int type = DigiTrackIter->getType();

    float pt = 
      static_cast<float>(DigiTrackIter->getMomentum().perp());

    float rho = 
      static_cast<float>(DigiTrackIter->getRadius());

    float theta = 
      DigiTrackIter->getMomentum().theta();
    float phi = 
      DigiTrackIter->getMomentum().phi();
      
/*      float xv = 
      DigiTrackIter->getVertex().x();     
      float yv = 
      DigiTrackIter->getVertex().y();
      float zv = 
      DigiTrackIter->getVertex().z();*/
      float phiv = 
      DigiTrackIter->getVertex().phi();
      float thetav = 
      DigiTrackIter->getVertex().theta();
      
            
/*      float x = 
      DigiTrackIter->getAxis().x();     
      float y = 
      DigiTrackIter->getAxis().y();
      float z = 
      DigiTrackIter->getAxis().z();*/
      
    if( phi <= 0)
      phi += (2.*TMath::Pi());     
    if( phiv <= 0)
      phiv += (2.*TMath::Pi());     

    //  save Tracks and add in collection
    if (DigiTrackIter-> isGenuine()){
    bool ptFlag = false;
    float TrackptThreshold = 2.;
    if (pt > TrackptThreshold) ptFlag = true;
    TrackerTrack* Track = 
      new TrackerTrack(rho,phi,theta,pt,ptFlag);
    DTMatches->addTrack(Track);
   
    if(ptFlag)
      outAscii 
	<< "Tracker L1 Track "
	<< " type " << type 
	<< " pt " << pt 
//	<< " xv " << xv << " yv " << yv << " zv " << zv 
//	<< " x " << x << " y " << y << " z " << z 
	<< " phi " << phi << " theta " 	<< theta 
	<< " phiv " << phi << " thetav " << thetav 
	<< endl;
    }    
    
    
    /*   
	  const Track_PixelDigi_* aStackedTrackerTrackDigi =
	  reinterpret_cast<const Track_PixelDigi_*>(&(*DigiTrackIter));
	  DigiTracks[superlayer].push_back(aStackedTrackerTrackDigi);
    */
  }
}
  
