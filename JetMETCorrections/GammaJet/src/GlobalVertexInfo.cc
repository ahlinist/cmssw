#include "JetMETCorrections/GammaJet/interface/GlobalVertexInfo.h"

void GlobalVertexInfo::fillInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  vtx_xyz.clear();
  tk_p4.clear();

  //Dump tracks  
  tk_n = 0;
  edm::Handle<reco::TrackCollection> tkH;
  iEvent.getByLabel("generalTracks", tkH);
  for(unsigned int i=0; i<tkH->size(); i++) {
    if (tk_n >= MAX_TRACKS) {
      std::cout << "GLobeTracks: WARNING TOO MANY TRACK: " << tkH->size() << " (allowed " << MAX_TRACKS << ")" << std::endl;
      break;
    }
    reco::TrackRef tk(tkH, i);
    //    new ((*tk_p4)[tk_n]) TLorentzVector();
    TLorentzVector a;
    a.SetXYZT(tk->px(), tk->py(), tk->pz(), tk->p());
    tk_p4.push_back(a);
    tk_pterr[tk_n] = tk->ptError();
    tk_quality[tk_n] = tk->qualityMask();
    tk_n++;
  } // end loop overtracks
    //Dump vertex
  edm::Handle<reco::VertexCollection> vtxH;
  iEvent.getByLabel("offlinePrimaryVertices", vtxH);
  vtx_tkind.clear();
  vtx_tkweight.clear();
  vtx_n = 0;

  for(unsigned int i=0; i<vtxH->size(); i++) {
    if (vtx_n >= MAX_VERTICES) {
      std::cout << "GlobeVertex: WARNING TOO MANY VERTEX: " << vtxH->size() << " (allowed " << MAX_VERTICES << ")" << std::endl;
      break;
    }
    //    std::cout << "vtx" << i << std::endl;
    reco::VertexRef vtx(vtxH, i);
    // Any cut on vertex?? gCUT ??  
    TVector3 a;
    a.SetXYZ(vtx->x(), vtx->y(), vtx->z());
    vtx_xyz.push_back(a);
    vtx_ntks[vtx_n] = vtx->tracksSize();
      
    std::vector<reco::TrackBaseRef>::const_iterator tk;
      
    std::vector<unsigned short> temp;
    std::vector<float> temp_float;
      
    for(tk=vtx->tracks_begin();tk!=vtx->tracks_end();++tk) {
      int index = 0;
      bool ismatched = false; 
      for(reco::TrackCollection::size_type j = 0; j<tkH->size(); ++j) {
	reco::TrackRef track(tkH, j);
	if (&(**tk) == &(*track)) {
	  //std::cout << "MWL: " << k << " " << index << " " << vtx->trackWeight(track) << std::endl;
	  temp.push_back(index);
	  temp_float.push_back(vtx->trackWeight(track));
	  ismatched = true;
	  break;
	}
	index++;
      }
       if( ! ismatched ) {
	 std::cout << "This should not happen" << std::endl;
// 	temp.push_back(-9999);
// 	temp_float.push_back(-9999);
       }
    }
      
    vtx_tkind.push_back(temp);
    vtx_tkweight.push_back(temp_float);
    vtx_n++;
  }  //end loop over vertices
}
