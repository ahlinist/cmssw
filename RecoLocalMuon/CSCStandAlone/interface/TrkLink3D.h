//Written by S Durkin

#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include <stdio.h>

class TrkLink3D
{
 public:

  TrkLink3D(){};
 ~TrkLink3D(){};


  void TrkLink3D_fill(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {

    int vmecrate = data.dmbHeader().crateID();
    int dmb = data.dmbHeader().dmbID();
    int tmb = -1;
    int endcap = -1;
    int station = -1;
    int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
    CSCDetId cid(id);
    int sid=cid.station()*1000+cid.ring()*100+cid.chamber();

    for(int i3d=0;i3d<fit.size();i3d++){
      float cslp=fit.trk3D_slp(i3d);
      float cintr=fit.trk3D_intr(i3d);
      float chi2=fit.trk3D_chi2(i3d);
      float s1=fit.trk3D_s1(i3d);
      float sx=fit.trk3D_sx(i3d);
      float sx2=fit.trk3D_sx2(i3d);
      float sy=fit.trk3D_sy(i3d);
      float sxy=fit.trk3D_sxy(i3d); 
      int at=fit.trk3D_apnt(i3d);
      float aslp=atrk.atrk_slp(at);
      float aintr=atrk.atrk_intr(at);
      trk_id.push_back(sid);
      trk_cslp.push_back(cslp);
      trk_cint.push_back(cintr);
      trk_chi2.push_back(chi2);
      trk_s1.push_back(s1);
      trk_sx.push_back(sx);
      trk_sx2.push_back(sx2);
      trk_sy.push_back(sy);
      trk_sxy.push_back(sxy);
      trk_aslp.push_back(aslp);
      trk_aint.push_back(aintr);
     }
  }

  std::vector<int> ltrk_id(){return trk_id;}
  std::vector<float> ltrk_cslp(){return trk_cslp;}
  std::vector<float> ltrk_cint(){return trk_cint;}
  std::vector<float> ltrk_chi2(){return trk_chi2;}
  std::vector<float> ltrk_s1(){return trk_s1;}
  std::vector<float> ltrk_sx(){return trk_sx;}
  std::vector<float> ltrk_sx2(){return trk_sx2;}
  std::vector<float> ltrk_sy(){return trk_sy;}
  std::vector<float> ltrk_sxy(){return trk_sxy;}
  std::vector<float> ltrk_aslp(){return trk_aslp;}
  std::vector<float> ltrk_aint(){return trk_aint;}


 private:
 
  std::vector<int> trk_id;
  std::vector<float> trk_cslp;
  std::vector<float> trk_cint;
  std::vector<float> trk_chi2;
  std::vector<float> trk_s1;
  std::vector<float> trk_sx;
  std::vector<float> trk_sx2;
  std::vector<float> trk_sy;
  std::vector<float> trk_sxy;
  std::vector<float> trk_aslp;
  std::vector<float> trk_aint;

};

