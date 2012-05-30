#include <vector>
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/RefToBase.h" 

#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "Analysis/VertexAnalysis/interface/HggVertexAnalyzer.h"

#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <iostream>

#define MAX_VERTICES_GLBVTX 500
#define MAX_TRACKS_GLBVTX 5000

class GlobalVertexInfo : public VertexInfoAdapter
{
 public:
  GlobalVertexInfo() {
/*     vtx_xyz=new TClonesArray("TVector3", MAX_TRACKS_GLBVTX); */
/*     tk_p4 = new TClonesArray("TLorentzVector", MAX_TRACKS_GLBVTX); */
  };
  
  virtual int nvtx() const    { return vtx_n; };
  virtual int ntracks() const { return tk_n; };
  
  virtual bool hasVtxTracks() const { return true; }
  virtual const unsigned short * vtxTracks(int ii) const { return &(vtx_tkind)[ii][0]; };
  virtual int vtxNTracks(int ii) const { return vtx_ntks[ii]; };
  virtual const float * vtxTkWeights(int ii) const { return &(vtx_tkweight)[ii][0]; };
  
  virtual float tkpx(int ii) const { return tk_p4[ii].Px(); };
  virtual float tkpy(int ii) const { return tk_p4[ii].Py(); };
  virtual float tkpz(int ii) const { return tk_p4[ii].Pz(); };
  
  virtual float tkPtErr(int ii) const { return tk_pterr[ii]; };
  virtual int   tkVtxId(int ii) const { return -1; };
  
  virtual float tkWeight(int ii, int jj) const { return (vtx_tkweight)[jj][ii]; };
  
  virtual float vtxx(int ii) const { return vtx_xyz[ii].X(); };
  virtual float vtxy(int ii) const { return vtx_xyz[ii].Y(); };
  virtual float vtxz(int ii) const { return vtx_xyz[ii].Z(); };
  
  virtual float tkd0(int ii, int jj) const { return 0.; } // FIXME
  virtual float tkd0Err(int ii, int jj) const { return 0.; };  // FIXME
  
  virtual float tkdz(int ii, int jj) const { return 0.; };  // FIXME
  virtual float tkdzErr(int ii, int jj) const { return 0.; };  // FIXME
  
  virtual bool tkIsHighPurity(int ii) const { return ( tk_quality[ii] & (1<<2) ) >> 2; };
  
  virtual ~GlobalVertexInfo() 
    {   
    };

  void fillInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup);

private:
  std::vector<std::vector<unsigned short> > vtx_tkind;
  std::vector<std::vector<float> > vtx_tkweight;
  std::vector<TVector3> vtx_xyz;
  int vtx_n;
  int vtx_ntks[MAX_VERTICES_GLBVTX];
  
  int tk_n;
  int tk_quality[MAX_TRACKS_GLBVTX];
  int tk_pterr[MAX_TRACKS_GLBVTX];
  std::vector<TLorentzVector> tk_p4;	
};
