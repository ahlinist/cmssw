/* Calman Filter using Cathode half strip positions to determine
   what hits belong to a Track 
   Written by S Durkin

*/



#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCLCTData.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigi.h"
#include "RecoLocalMuon/CSCStandAlone/interface/PulseTime.h"
#include "RecoLocalMuon/CSCStandAlone/interface/ADCmat.h"

class CatTrkFnd
{
 public:

 CatTrkFnd(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,Strip_Fit_Constants & chamb_const) {

  int use[6];
  int debug=0;
  float ctrk_dist=0.6;
  float thresh=15.0;
  float strip_stagger;
  union U{
    float ld[12];
    float d[3][4];
  };


  int vmecrate = data.dmbHeader().crateID(); 
  int dmb = data.dmbHeader().dmbID();
  int tmb = -1;
  int endcap = -1;
  int station = -1;
  int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
  CSCDetId cid(id);   
  num_strips=chamb_const.num_strips(id);
  gains=chamb_const.gain(id);

  strip_stagger=0.5;
  if(cid.station()==1)strip_stagger=0.0;

  nctrk=0;

  // find 1/2 strip position above threshold


  for(int layer=1;layer<=6;layer++){
    std::vector <CSCStripDigi> digis =  data.stripDigis(layer);
    for(unsigned sid=0;sid<digis.size();sid++){
      int strip2=digis[sid].getStrip();
      int mstrp=strip2;
      unsigned tsid=sid;
      if(sid==0){tsid=sid+1;mstrp=mstrp+1;}
      if(sid==digis.size()-1){tsid=sid-1;mstrp=mstrp-1;}
      std::vector<int> adcl=digis[tsid-1].getADCCounts();
      std::vector<int> adc=digis[tsid].getADCCounts();
      std::vector<int> adch=digis[tsid+1].getADCCounts();
      float pedsuml=gains[(layer-1)*num_strips+mstrp-2]*(adcl[0]+adcl[1])/2.;
      float pedsum=gains[(layer-1)*num_strips+mstrp-1]*(adc[0]+adc[1])/2.;
      float pedsumh=gains[(layer-1)*num_strips+mstrp]*(adch[0]+adch[1])/2.;
      float adcmax=0;
      int tmax=0;
      for(unsigned time=0;time<adc.size();time++){
	 float adcnow=gains[(layer-1)*num_strips+mstrp-1]*adc[time]-pedsum;
	 if(adcnow>adcmax){tmax=time;adcmax=adcnow;}
      }
      // printf(" tmax %d adcmax %f pedsum %f \n",tmax,adcmax,pedsum);
      float tadcl=-99.;
      float tadc=-99.;
      float tadch=-99.;
      if(sid==tsid){
        tadcl=gains[(layer-1)*num_strips+mstrp-2]*adcl[tmax]-pedsuml;
        tadc=gains[(layer-1)*num_strips+mstrp-1]*adc[tmax]-pedsum;
        tadch=gains[(layer-1)*num_strips+mstrp]*adch[tmax]-pedsumh;
      }else if(sid==0){
        tadcl=0.0;
        tadc=gains[(layer-1)*num_strips+mstrp-2]*adcl[tmax]-pedsuml;
        tadch=gains[(layer-1)*num_strips+mstrp-1]*adc[tmax]-pedsum;
      }else if(sid==digis.size()-1){
	tadcl=gains[(layer-1)*num_strips+mstrp-1]*adc[tmax]-pedsum;
        tadc=gains[(layer-1)*num_strips+mstrp]*adch[tmax]-pedsumh;
        tadch=0.0;
      }
      if((tadc>tadcl)&&(tadc>tadch)){
        float pos;
        if(tadcl+tadc+tadch>thresh){ 
           if((adch[tmax]-pedsumh)>(adcl[tmax]-pedsuml)){
	     pos=strip2+0.25+layer%2*strip_stagger;
           }else{
             pos=strip2-0.25+layer%2*strip_stagger;
           }
	   hstrip[layer-1].push_back(pos);
           strip[layer-1].push_back(strip2);
           pstrip[layer-1].push_back(sid);
	   int tmp=0;
           huse[layer-1].push_back(tmp);
           time_max[layer-1].push_back(tmax);

           U d;
	   d.d[0][0]=gains[(layer-1)*num_strips+mstrp-2]*(adcl[tmax-1]-(adcl[0]+adcl[1])/2.);
           d.d[1][0]=gains[(layer-1)*num_strips+mstrp-1]*(adc[tmax-1]-( adc[0]+ adc[1])/2.);
           d.d[2][0]=gains[(layer-1)*num_strips+mstrp-0]*(adch[tmax-1]-(adch[0]+adch[1])/2.);
           d.d[0][1]=gains[(layer-1)*num_strips+mstrp-2]*(adcl[tmax]-(adcl[0]+adcl[1])/2.);
           d.d[1][1]=gains[(layer-1)*num_strips+mstrp-1]*( adc[tmax]-( adc[0]+ adc[1])/2.);
           d.d[2][1]=gains[(layer-1)*num_strips+mstrp-0]*(adch[tmax]-(adch[0]+adch[1])/2.);
           if(tmax<7){
             d.d[0][2]=gains[(layer-1)*num_strips+mstrp-2]*(adcl[tmax+1]-(adcl[0]+adcl[1])/2.);
             d.d[1][2]=gains[(layer-1)*num_strips+mstrp-1]*( adc[tmax+1]-( adc[0]+ adc[1])/2.);
             d.d[2][2]=gains[(layer-1)*num_strips+mstrp-0]*(adch[tmax+1]-(adch[0]+adch[1])/2.);
           }else{
             d.d[0][2]=0.0;
             d.d[1][2]=0.0;
             d.d[2][2]=0.0;
            } 
           if(tmax<6){
             d.d[0][3]=gains[(layer-1)*num_strips+mstrp-2]*(adcl[tmax+2]-(adcl[0]+adcl[1])/2.);
             d.d[1][3]=gains[(layer-1)*num_strips+mstrp-1]*(adc[tmax+2]-( adc[0]+ adc[1])/2.);
             d.d[2][3]=gains[(layer-1)*num_strips+mstrp-0]*(adch[tmax+2]-(adch[0]+adch[1])/2.);
           }else{
	     d.d[0][3]=0.0;
             d.d[1][3]=0.0;
             d.d[2][3]=0.0;
           }
	   if(debug>0){
             printf(" layer %d num_strips %d mstrp %d tmax %d \n",layer,num_strips,mstrp,tmax);
             printf(" gatti charges \n");
             for(int su=0;su<3;su++)printf(" %7.2f %7.2f %7.2f \n",d.d[0][su],d.d[1][su],d.d[2][su]);
	   }
           float *pass=d.ld;
           ADCmat da(pass);
           adcmat[layer-1].push_back(da);
           pedmon[layer-1].push_back(adc[1]-adc[0]);
        }
      }
    }
  }
  
  // print 1/2 strip hits    
  if(debug>0){
    for(int layer=0;layer<=5;layer++){
      printf(" layer %d size %d:",layer,hstrip[layer].size());
      for(unsigned hit=0;hit<hstrip[layer].size();hit++){
        printf(" %5.2f %d ",hstrip[layer][hit],huse[layer][hit]);
      }
      printf("\n");
    }
  }

  // fit some tracks
  int nhits_left=hstrip[0].size()+hstrip[1].size()+hstrip[2].size()+hstrip[3].size()+hstrip[4].size()+hstrip[5].size();

  float x[6],y[6];
  int usr[6];
  float intr=0.;
  float slp=0;
  int np=0;
   
  int ns=0; 
  for(int i = 6; i > 2; i--){
    // Loop over all the hits on all the layers.  This loop looks a little
    // strange, since the range is one more than the number of hits.  This
    // is just a clever way to write the algorithm; allowing planes to be missing.
    int u[6];
    for (unsigned i0 = 0; i0 < hstrip[0].size()+1;i0++){
    if(i0==hstrip[0].size()){use[0]=0;u[0]=0;}else{use[0]=huse[0][i0];u[0]=1;}
    if(use[0]==0&&u[0]+5>=i){
     for (unsigned i1 = 0; i1 < hstrip[1].size()+1;i1++){
     if(i1==hstrip[1].size()){use[1]=0;u[1]=0;}else{use[1]=huse[1][i1];u[1]=1;}
     if(use[1]==0&&u[0]+u[1]+4>=i){
      for (unsigned i2 = 0; i2 < hstrip[2].size()+1;i2++){
      if(i2==hstrip[2].size()){use[2]=0;u[2]=0;}else{use[2]=huse[2][i2];u[2]=1;}
      if(use[2]==0&&u[0]+u[1]+u[2]+3>=i){
       for (unsigned i3 = 0; i3 < hstrip[3].size()+1;i3++){
       if(i3==hstrip[3].size()){use[3]=0;u[3]=0;}else{use[3]=huse[3][i3];u[3]=1;}
       if(use[3]==0&&u[0]+u[1]+u[2]+u[3]+2>=i){
   	for (unsigned i4 = 0; i4 < hstrip[4].size()+1;i4++){
        if(i4==hstrip[4].size()){use[4]=0;u[4]=0;}else{use[4]=huse[4][i4];u[4]=1;}
        if(use[4]==0&&u[0]+u[1]+u[2]+u[3]+u[4]+1>=i){
         for (unsigned i5 = 0; i5 < hstrip[5].size()+1;i5++){
         if(i5==hstrip[5].size()){use[5]=0;u[5]=0;}else{use[5]=huse[5][i5];u[5]=1;}	 
         if(use[5]==0&&u[0]+u[1]+u[2]+u[3]+u[4]+u[5]==i){
	  
		    
		    // If hit is still available to be used in 
		    // track hunt, store layer and wire number.
	       if (u[0] == 1){x[0]=0.0;y[0]=hstrip[0][i0];usr[0]=i0;}
	       if (u[1] == 1){x[1]=1.0;y[1]=hstrip[1][i1];usr[1]=i1;}
	       if (u[2] == 1){x[2]=2.0;y[2]=hstrip[2][i2];usr[2]=i2;}
	       if (u[3] == 1){x[3]=3.0;y[3]=hstrip[3][i3];usr[3]=i3;}
	       if (u[4] == 1){x[4]=4.0;y[4]=hstrip[4][i4];usr[4]=i4;}
	       if (u[5] == 1){x[5]=5.0;y[5]=hstrip[5][i5];usr[5]=i5;}
		
                    //for(int kk=0;kk<0;kk++){
                    //  printf(" plane %d x %f y %f \n",kk,x[kk],y[kk]);
                    //}
                       
		    // Do least squares fit.
		    float s1 = 0.0; 
                    float sx = 0.0; 
                    float sy = 0.0; 
                    float sx2 = 0.0; 
                    float sxy = 0.0; 
                    ns=0;
		    for (int j = 0; j < 6; j++){
		      if (u[j] == 1){
			ns = ns + 1;
			s1 = s1 + 1;
			sx = sx + x[j];
			sx2 = sx2 + x[j]*x[j];
			sy = sy + y[j];
			sxy = sxy + x[j]*y[j];
		      }
		    }
		    float dd = s1*sx2 - sx*sx;
		    // Calculate slope and intersect for line going through
		    // hits, from least squares fit.
                    
		    if (dd > 0.0){
		      intr = (sy*sx2 - sxy*sx)/dd;
		      slp = (sxy*s1 - sy*sx)/dd;
		    }
		    np = 0;
		    
		    // Find all hits that are within an acceptable distance
		    // of the line.
                    float yy,diff;
		    for (int j = 0; j < 6; j++){
                      if(u[j]==1){
			yy = slp*x[j] + intr;
			diff = y[j] - yy;
			if (diff < 0.0){ diff = -diff; }
			
			// if position of 1/2 strip is within certain range 
			// (0.5), then accept the hit, and increase number
			// of potentially useful hits by one.
			if (diff < ctrk_dist){ np = np + 1; }
                      }      
		    }
		    if (debug > 0){
		      std::cout << " Number of hits within 0.5 of fitted track = " 
			   << np << std::endl;
		      std::cout << " Number of layers with hits = " << ns << std::endl;		     
                      std::cout << " Number hits left = " << nhits_left << std::endl;

		    }
		    // If the number of accepted hits is greater than or equal
		    // to the number of layers, then store the track and 
		    // store the wire and layer hits for that track.
		    if (np >= ns){
	               np = 0;
		       nctrk=nctrk+1;
		      // This is a repeat of the previous loop, but this time
		      // the hits and layers are recorded into an array, 
		      // designating them as being used in a track.
		       float yy,diff;
		      for (int j = 0; j < 6; j++){
                        if(u[j]==1){
			  yy = slp*x[j] + intr;
			  diff = y[j] - yy;
			  if (diff < 0.0){ diff = -diff; }
			  if (diff < ctrk_dist){
	                     trk_layer[nctrk-1].push_back(j);
	                     trk_hstrip[nctrk-1].push_back(y[j]);
                             int strp=strip[j][usr[j]];
                             trk_strip[nctrk-1].push_back(strp);
                             int tim=time_max[j][usr[j]];
                             trk_tmax[nctrk-1].push_back(tim);
                             int id=pstrip[j][usr[j]];
                             trk_pstrip[nctrk-1].push_back(id);
                             ADCmat amat=adcmat[j][usr[j]];
                             trk_adcmat[nctrk-1].push_back(amat);
                             float pmon=pedmon[j][usr[j]];
                             trk_pedmon[nctrk-1].push_back(pmon);
			    if (debug > 0){
			      if (j == 0){ 
				std::cout << "--- Saved cathode hits" << std::endl; 
			      }
			      std::cout << " Layer " << j << " strip "<< y[j] <<std::endl;
			    }
			    
			    // Keep track of which hits are used.
                            huse[j][usr[j]]=1;
			    // If hit is used in track, the total number
			    // of hits available for other tracks decreases 
			    // by one.
			    nhits_left = nhits_left - 1;
			    np = np + 1;
			  }
			}      
		      }  

		      if (debug > 0){
			std::cout << "--- Number of found anode tracks =  " 
			     << nctrk << std::endl;
			std::cout << " Hits used in track = " << np 
			     << ", hits left = " << nhits_left << std::endl;
		      }
		      // Quit if number of leftover hits is less than 3.
		      if (nhits_left <= 2)goto Terminate;
                      if(nctrk==5)goto Terminate;
		    }  
	    }}
	  }}
        }}
      }}
    }}
  }}
 }
 
 Terminate:
    use[0]=0;  // label must be followed by a statement
 }

 virtual ~CatTrkFnd(){}

 void CatTrkTime(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,Strip_Fit_Constants & chamb_const){ 
   int vmecrate = data.dmbHeader().crateID();
   int dmb = data.dmbHeader().dmbID();
   int tmb = -1;
   int endcap = -1;
   int station = -1;
   int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
   gains=chamb_const.gain(id);
   xtlkil=chamb_const.xtalk_intercept_left(id);
   xtlkir=chamb_const.xtalk_intercept_right(id);
   xtlksl=chamb_const.xtalk_slope_left(id);
   xtlksr=chamb_const.xtalk_slope_right(id);
   for(int l=0;l<size();l++){
     std::vector<int> cx=ctrk_layer(l);
     std::vector<int> strp=ctrk_strip(l);
     std::vector<int> tm=ctrk_tmax(l);
     std::vector<ADCmat> ad=ctrk_adcmat(l);
     for(unsigned cm=0;cm<cx.size();cm++){
        float *tmp=ad[cm].array();
        float dt=0.0;
        float tp,ts,tq;
        float val[4];
        float vall[4];
        float valm[4];
        float valh[4];
        float xlr[4],xhl[4],xmr[4],xml[4];
        for(int trys=0;trys<2;trys++){
        if(strp[cm]>1&&strp[cm]<num_strips){
          for(int t=0;t<4;t++){
            float tll=xtlksl[cx[cm]*num_strips+strp[cm]-2]*(dt+50.*(t-1))+xtlkil[cx[cm]*num_strips+strp[cm]-2];
	    float tlr=xtlksr[cx[cm]*num_strips+strp[cm]-2]*(dt+50.*(t-1))+xtlkir[cx[cm]*num_strips+strp[cm]-2];
            float tml=xtlksl[cx[cm]*num_strips+strp[cm]-1]*(dt+50.*(t-1))+xtlkil[cx[cm]*num_strips+strp[cm]-1];
            float tmr=xtlksr[cx[cm]*num_strips+strp[cm]-1]*(dt+50.*(t-1))+xtlkir[cx[cm]*num_strips+strp[cm]-1];
            float thl=xtlksl[cx[cm]*num_strips+strp[cm]-0]*(dt+50.*(t-1))+xtlkil[cx[cm]*num_strips+strp[cm]-0];
            float thr=xtlksr[cx[cm]*num_strips+strp[cm]-0]*(dt+50.*(t-1))+xtlkir[cx[cm]*num_strips+strp[cm]-0];
            xlr[t]=tlr/(1-tlr-tll);
            xhl[t]=thl/(1-thr-thl);
            xmr[t]=tmr/(1-tmr-tml);
            xml[t]=tml/(1-tml-tmr); 
          }
          for(int t=0;t<4;t++){vall[t]=tmp[t];valm[t]=tmp[t+4];valh[t]=tmp[t+8];}
          for(int t=0;t<4;t++)val[t]=(valm[t]-xlr[t]*vall[t]-xhl[t]*valh[t])/(1.-xhl[t]*xmr[t]-xlr[t]*xml[t]);
	  PulseTime pt(tm[cm],val);
          ts=pt.StartTime();
          tp=pt.PeakTime();
          tq=pt.PeakChg(); 
          dt=tm[cm]*50-pt.StartTime()-pt.PeakTime();
        }
        }
        if(strp[cm]==1){
          for(int t=0;t<4;t++)val[t]=vall[t];
	  PulseTime pt(tm[cm],val); 	  
          ts=pt.StartTime();
          tp=pt.PeakTime();
          tq=pt.PeakChg(); 	  
        }
        if(strp[cm]==num_strips){
          for(int t=0;t<4;t++)val[t]=valh[t];
          PulseTime pt(tm[cm],val);
          ts=pt.StartTime();
          tp=pt.PeakTime();
          tq=pt.PeakChg();
        }
        trk_t_start[l].push_back(ts);
        trk_t_peak[l].push_back(tp);
        trk_t_chg[l].push_back(tq);
     }
   }
 
 }

 int size(){return nctrk;}

 // hits along a track
 std::vector<int> ctrk_layer(int trk){return trk_layer[trk];}  //layer
 std::vector<float> ctrk_hstrip(int trk){return trk_hstrip[trk];} // 1/2 strip
 std::vector<int> ctrk_strip(int trk){return trk_strip[trk];} //strip
 std::vector<int> ctrk_pstrip(int trk){return trk_pstrip[trk];} //strip
 std::vector<ADCmat> ctrk_adcmat(int trk){return trk_adcmat[trk];} //strip
 std::vector<float> ctrk_t_start(int trk){return trk_t_start[trk];}
 std::vector<float> ctrk_t_peak(int trk){return trk_t_peak[trk];}
 std::vector<float> ctrk_t_chg(int trk){return trk_t_chg[trk];}
 std::vector<float> ctrk_pedmon(int trk){return trk_pedmon[trk];}
 std::vector<int> ctrk_tmax(int trk){return trk_tmax[trk];} 
 // raw hits in chamber
 std::vector<float> craw_hstrip(int layer){return hstrip[layer];} // 1/2 strip
 std::vector<int> craw_use(int layer){return huse[layer];} // used in track
 std::vector<int> craw_strip(int layer){return strip[layer];} // strip
 std::vector<int> craw_pstrip(int layer){return pstrip[layer];} // digi pntr strip
 

private:

  unsigned num_strips;
  float *gains;
  float *xtlkil;
  float *xtlkir;
  float *xtlksl;
  float *xtlksr;

  std::vector<float> hstrip[6];
  std::vector<int> huse[6];
  std::vector<int> strip[6];
  std::vector<int> pstrip[6];
  std::vector<int> time_max[6];
  std::vector<ADCmat> adcmat[6];
  std::vector<float> pedmon[6];

  int use[6];

  int nctrk;
  std::vector<int> trk_layer[5];
  std::vector<float> trk_hstrip[5]; 
  std::vector<int> trk_strip[5];
  std::vector<int> trk_pstrip[5];
  std::vector<float>trk_t_start[5];
  std::vector<float>trk_t_peak[5];
  std::vector<float>trk_t_chg[5];
  std::vector<int> trk_tmax[5];
  std::vector<ADCmat> trk_adcmat[5];
  std::vector<float>trk_pedmon[5];

};


