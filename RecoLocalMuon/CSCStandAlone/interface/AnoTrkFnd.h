/* Calman Filter using Anode strip positions to determine
   what hits belong to a Track 
   Written By S Durkin
*/

#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"



class AnoTrkFnd
{
 public:

 AnoTrkFnd(const CSCEventData & data) {

  int use[6];
  int debug=0;
  float atrk_dist=1.0;
  natrk=0;

  for(int layer=1;layer<=6;layer++){
    std::vector<CSCWireDigi> wires =data.wireDigis(layer);
    for(unsigned i=0;i<wires.size();i++){
      int wire_group=wires[i].getWireGroup();
      int wire_tbin=wires[i].getBeamCrossingTag();
      int old=0;
      for(unsigned k=0;k<wire[layer-1].size();k++){
	if(wire[layer-1][k]==wire_group)old=1;
      }
      if(old==0){
         wire[layer-1].push_back(wire_group);
         twire[layer-1].push_back(wire_tbin);
         int tmp=0;
         wuse[layer-1].push_back(tmp);
      }
    }
  }

    
  if(debug>0){
    for(int layer=0;layer<=5;layer++){
      printf(" layer %d size %d:",layer,wire[layer].size());
      for(unsigned hit=0;hit<wire[layer].size();hit++){
        printf(" %d ",wire[layer][hit]);
      }
      printf("\n");
    }
  }

  // fit some tracks
  int nhits_left=wire[0].size()+wire[1].size()+wire[2].size()+wire[3].size()+wire[4].size()+wire[5].size();

  float x[6],y[6];
  float intr=0.;
  float slp=0;
  int np=0;
  int ns=0;

  for(int i = 6; i > 2; i--){
    // Loop over all the hits on all the layers.  This loop looks a little
    // strange, since the range is one more than the number of hits.  This
    // is just a clever way to write the algorithm; there is more explanation
    // below
    for (unsigned i0 = 0; i0 < wire[0].size()+1;i0++){
    if(i0 == wire[0].size()){use[0]=0;}else{use[0]=wuse[0][i0];}
    if(use[0]==0){
     for (unsigned i1 = 0; i1 < wire[1].size()+1;i1++){
     if(i1 == wire[1].size()){use[1]=0;}else{use[1]=wuse[1][i1];}
     if(use[1]==0){
      for (unsigned i2 = 0; i2 < wire[2].size()+1;i2++){
      if(i2 == wire[2].size()){use[2]=0;}else{use[2]=wuse[2][i2];}
      if(use[2]==0){  
       for (unsigned i3 = 0; i3 < wire[3].size()+1;i3++){
       if(i3 == wire[3].size()){use[3]=0;}else{use[3]=wuse[3][i3];}
       if(use[3]==0){
	for (unsigned i4 = 0; i4 < wire[4].size()+1;i4++){
        if(i4 == wire[4].size()){use[4]=0;}else{use[4]=wuse[4][i4];}
        if(use[4]==0){
         for (unsigned i5 = 0; i5 < wire[5].size()+1;i5++){ 
         if(i5 == wire[5].size()){use[5]=0;}else{use[5]=wuse[5][i5];}
         if(use[5]==0){ 
		 
	   // The first part of this conditional only allows the search 
	   // to continue if the hit has not already been used in a track.
	   // This speeds up the algorithm since, double looping is 
	   // avoided.  But if all your hits have already been used to
	   // make tracks, you still need to be able to continue the 
	   // search for unused hits on other layers.
			    
		  // This is just a temporary flag to let the 
		  // algorithm know which layers have hits which
		  // still have not been used in tracks.
                  int u[6];
		  for (int l = 0; l < 6; l++)u[l] = 1; 
		  if (i0 ==  wire[0].size())u[0] = 0; 
		  if (i1 ==  wire[1].size())u[1] = 0; 
		  if (i2 ==  wire[2].size())u[2] = 0; 
		  if (i3 ==  wire[3].size())u[3] = 0; 
		  if (i4 ==  wire[4].size())u[4] = 0; 
		  if (i5 ==  wire[5].size())u[5] = 0;
                  //printf(" try %d %d %d %d %d %d \n",i0,i1,i2,i3,i4,i5); 
                  //printf(" u sss  %d %d %d %d %d %d \n",u[0],u[1],u[2],u[3],u[4],u[5]);
		  
		  // Double check that count of layers with
		  // available hits is consistent
		  if (u[0] + u[1] + u[2] + u[3] + u[4] + u[5] == i){ 
		    
		    // If hit is still available to be used in 
		    // track hunt, store layer and track number.
		    if (u[0] == 1){x[0]=0.0;y[0]=(float)wire[0][i0];}
		    if (u[1] == 1){x[1]=1.0;y[1]=(float)wire[1][i1];}
		    if (u[2] == 1){x[2]=2.0;y[2]=(float)wire[2][i2];}
		    if (u[3] == 1){x[3]=3.0;y[3]=(float)wire[3][i3];}
		    if (u[4] == 1){x[4]=4.0;y[4]=(float)wire[4][i4];}
		    if (u[5] == 1){x[5]=5.0;y[5]=(float)wire[5][i5];}
		
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
		    for(int j = 0; j < 6; j++){
                      for(unsigned hit=0;hit<wire[j].size();hit++){
                      if(wuse[j][hit]==0){
			yy = slp*x[j] + intr;
                        float w=wire[j][hit];
			diff = w - yy;
			if (diff < 0.0){ diff = -diff; }
			
			// if position of wire is within certain range 
			// (q.0), then accept the hit, and increase number
			// of potentially useful hits by one.
			if (diff < atrk_dist){ np = np + 1; }
		      }
                      }      
		    }
		    if (debug > 0){
		      std::cout << " Number of hits within 1.0 of fitted track = " 
			   << np << std::endl;
		      std::cout << " Number of layers with hits = " << ns << std::endl;
		    }
		    // If the number of accepted hits is greater than or equal
		    // to the number of layers, then store the track and 
		    // store the track and layer hits for that track.
		    if (np >= ns){
	               np = 0;
                       if(natrk<10){
                         trk_slp[natrk]=slp;
                         trk_intr[natrk]=intr;
                       }
		       natrk=natrk+1;
		      // This is a repeat of the previous loop, but this time
		      // the hits and layers are recorded into an array, 
		      // designating them as being used in a track.
		       float yy,diff;
		      for (int j = 0; j < 6; j++){
                        for(unsigned hit=0;hit<wire[j].size();hit++){
			if(wuse[j][hit]==0){
			  yy = slp*x[j] + intr;
                          float w=wire[j][hit];
			  diff = w - yy;
			  if (diff < 0.0){ diff = -diff; }
			  if (diff < atrk_dist){
	                     trk_layer[natrk-1].push_back(j);
                             int ww=(int)w;
	                     trk_wire[natrk-1].push_back(ww);
                             int tw=twire[j][hit];
	                     trk_twire[natrk-1].push_back(tw);
			    if (debug > 0){
			      if (j == 0){ 
				std::cout << "--- Saved Anode hits" << std::endl; 
			      }
			      std::cout << " Layer " << j << " wire "<< w <<std::endl;
			    }
			    
			    // Keep track of which hits are used.
			    wuse[j][hit] = 1;
			    
			    // If hit is used in track, the total number
			    // of hits available for other tracks decreases 
			    // by one.
			    nhits_left = nhits_left - 1;
			    np = np + 1;
			  }
			}
                        }      
		      }  

		      if (debug > 0){
			std::cout << "--- Number of found anode tracks =  " 
			     << natrk << std::endl;
			std::cout << " Hits used in track = " << np 
			     << ", hits left = " << nhits_left << std::endl;
		      }
		      // Quit if number of leftover hits is less than 3.
		      if (nhits_left <= 2)goto Terminate;
                      if(natrk==10)goto Terminate;
		    }
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

 virtual ~AnoTrkFnd(){}

 int size(){return natrk;}

 // hits along a track
 std::vector<int> atrk_layer(int trk){return trk_layer[trk];}  //layer
 std::vector<int> atrk_wire(int trk){return trk_wire[trk];} //wire
 std::vector<int> atrk_twire(int trk){return trk_twire[trk];} //time wire
 float atrk_slp(int trk){return trk_slp[trk];}  //slope
 float atrk_intr(int trk){return trk_intr[trk];}  //slope

 
 // raw hits in chamber
 std::vector<int> araw_wire(int layer){return wire[layer];} // wire
 std::vector<int> araw_twire(int layer){return twire[layer];} // wire time
 std::vector<int> araw_use(int layer){return wuse[layer];} // used in track

private:


  std::vector<int>  wire[6];
  std::vector<int>  twire[6];
  std::vector<int> wuse[6];
  int use[6];

  int natrk;
  float trk_intr[10];
  float trk_slp[10];
  std::vector<int> trk_layer[10];                                    
  std::vector<int> trk_wire[10];
  std::vector<int> trk_twire[10];

};
