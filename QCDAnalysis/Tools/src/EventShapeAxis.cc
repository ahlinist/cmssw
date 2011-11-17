#include "QCDAnalysis/Tools/interface/EventShapeAxis.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;

/*  \class EventShape
*
*  Class that, given the 4-momenta of the objects in the event, 
*  allows to calculate event shapes 
*
*  Authors: Matthias Weber                 Date: October 2010
*
*/

EventShapeAxis::EventShapeAxis(std::vector< TLorentzVector>& PtEtaPhiE_vector, std::vector<float> eventAxis, float eta_central, int rapidity) : Object_PtEtaPhiE(PtEtaPhiE_vector),ThrustAxis(eventAxis),eta_c(eta_central),rap(rapidity){}


vector<float> EventShapeAxis::getEventShapes(){
  this->calculate();
  return EventShapeVector;
}

vector<int> EventShapeAxis::getGrouping_C(){
  this->calculate();
  return Grouping_c;
}

vector<int> EventShapeAxis::getGrouping(){
  if(!Grouping.empty()){
    Grouping.clear();
  }
  for(unsigned int j = 0; j < Object_PtEtaPhiE.size(); j++){
    Grouping.push_back(-1);
  }
  for (unsigned int i=0; i<Object_PtEtaPhiE.size(); i++){
    if( (Object_PtEtaPhiE[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE[i].Py()*ThrustAxis[1]) >= 0){
      Grouping[i]=1;
    }
    if( (Object_PtEtaPhiE[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE[i].Py()*ThrustAxis[1]) < 0){
      Grouping[i]=2;
    }
  }
  return Grouping;
}


int EventShapeAxis::calculate(){

  unsigned int length = (unsigned int) Object_PtEtaPhiE.size();

  if (!Object_Pt.empty()){
    Object_Pt.clear();
    Object_E.clear();
    Object_Eta_Rap.clear();
    Object_Phi.clear();
  }

  if(ThrustAxis.size()<2){
    cout<<"error in event_axis dimension: Need at least 2D axis in transverse plane (in x-y-space)"<<endl;
  }

  if(!EventShapeVector.empty()){
    EventShapeVector.clear();
    Grouping.clear();
    Grouping_c.clear();
  }


  //if we have LESS than 2 objects e.g. if one or less jets in an event
  //should survive a preselection on jets in a jet collection
  //in this case an event shape gives no meaningful value
  //we need space for 14 variables initiated with -1
  for(unsigned int j = 0; j < 14; j++){
    EventShapeVector.push_back(-1.0);
  } 
  for(unsigned int j = 0; j < length; j++){
    Grouping.push_back(-1);
  }
  for(unsigned int j = 0; j < length; j++){
    Grouping_c.push_back(-1);
  }


  if(length<2){
    cout<<"WARNING!! Less than two momenta in the event given!! Event shapes won't be calculated and set = -1.0 per default!"<<endl;
    cout<<"all central group entries will be -1"<<endl;
    cout<<"global group entries will be -1"<<endl;  
  }else{
    
    //at least 2 input objects which is required for a meaningful value
    //for event-shapes

    float Pt_sum =0;
    for(unsigned int j = 0; j < length; j++){
      Object_Pt.push_back(Object_PtEtaPhiE[j].Pt());
      Object_Phi.push_back(Object_PtEtaPhiE[j].Phi());
      //only in this case eta is used in the following calculations
      if(rap==0){
	Object_Eta_Rap.push_back(Object_PtEtaPhiE[j].Eta());
      }else{//rap==2 or rap==1
	Object_Eta_Rap.push_back(Object_PtEtaPhiE[j].Rapidity());
      }
      if((rap!=0)&&(rap!=1)&&(rap!=2)){
	cout<<"ERROR!!!, The choice to use the rapidity y or the pseudorapidity eta is not set correctly! Change that please!"<<endl;
	return 0;
      }
      Pt_sum+=Object_PtEtaPhiE[j].Pt();
    }
   
    //sum of all Pt's
    
    //calculate the directly global transverse thrust dirgltrthr
    float dirgltrthr=0;
    
    //normalize nx and ny of eventAxis to ensure nx^2+ny^2=1
    float norm = sqrt(pow(ThrustAxis[0],2)+pow(ThrustAxis[1],2));

    ThrustAxis[0]=ThrustAxis[0]/norm;
    ThrustAxis[1]=ThrustAxis[1]/norm;

    float thrust=0;
    for (unsigned int i=0;i<length;i++){
      thrust+=fabs(Object_PtEtaPhiE[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE[i].Py()*ThrustAxis[1]);
    }
    dirgltrthr =1-thrust/Pt_sum;
    
    EventShapeVector[0] = dirgltrthr;  

    for (unsigned int i=0; i<length; i++){
      if( (Object_PtEtaPhiE[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE[i].Py()*ThrustAxis[1]) >= 0){
	Grouping[i]=1;
      }
      if( (Object_PtEtaPhiE[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE[i].Py()*ThrustAxis[1]) < 0){
	Grouping[i]=2;
      }
    }
   
   
    //the directly global thrust minor dirglthrmin
    float dirglthrmin =0;
    //rotate the coordinate system around the beam axis that
    //the thrust axis is the new y'-Axis - the projections are
    //simply the new y-values then
    float alpha=atan2(ThrustAxis[1],ThrustAxis[0]);
    for(unsigned int k=0;k<length;k++){
      dirglthrmin+= fabs(-sin(alpha)*Object_PtEtaPhiE[k].Px()+cos(alpha)*Object_PtEtaPhiE[k].Py());
    }
    dirglthrmin=dirglthrmin/Pt_sum;
    EventShapeVector[1] = dirglthrmin;
    
    //||||||||||||||||||||||||||||||||||||||||||||||
    //here the central event shape variables begin||
    //||||||||||||||||||||||||||||||||||||||||||||||
    
    std::vector<TLorentzVector> Object_PtEtaPhiE_in;
    std::vector<float> Object_Px_in;
    std::vector<float> Object_Py_in;
    std::vector<float> Object_Pz_in;
    std::vector<float> Object_Pt_in;
    std::vector<float> Object_E_in;
    std::vector<float> Object_Eta_Rap_in;
    std::vector<TLorentzVector> Object_PtEtaPhiE_out;
    std::vector<float> Object_Px_out;
    std::vector<float> Object_Py_out;
    std::vector<float> Object_Pz_out;
    std::vector<float> Object_E_out;
    std::vector<float> Object_Pt_out;
    std::vector<float> Object_Eta_Rap_out;

    //0 if object out, 1 if object inside of central region
    std::vector<unsigned int> Object_in_out;
    
    if (!Object_PtEtaPhiE_in.empty()){
      Object_PtEtaPhiE_in.clear();
      Object_PtEtaPhiE_out.clear();
      Object_Px_in.clear();
      Object_Py_in.clear();
      Object_Pz_in.clear();
      Object_Pt_in.clear();
      Object_E_in.clear();
      Object_Eta_Rap_in.clear();
      Object_Px_out.clear();
      Object_Py_out.clear();
      Object_Pz_out.clear();
      Object_Pt_out.clear();
      Object_E_out.clear();
      Object_Eta_Rap_out.clear();
      Object_in_out.clear();
    }
    
    float Pt_sum_c = 0;
    float Eta_cw=0;
    float RecTerm =0;
    float px_sum_in = 0;
    float py_sum_in = 0;

    unsigned int nin = 0;
    unsigned int nout = 0;
    for(unsigned int j=0;j<length;j++){
      //in the case of using pseudorapidity
      //in the DeltaR values for the event-shape calculation
      if(rap ==0){
	if(fabs(Object_PtEtaPhiE[j].Eta())<eta_c){
	  Object_in_out.push_back(1);
	  Pt_sum_c+=Object_PtEtaPhiE[j].Pt();
	  Eta_cw+=Object_PtEtaPhiE[j].Pt()*Object_PtEtaPhiE[j].Eta();
	  px_sum_in+=Object_PtEtaPhiE[j].Px();
	  py_sum_in+=Object_PtEtaPhiE[j].Py();
	  Object_PtEtaPhiE_in.push_back(Object_PtEtaPhiE[j]);
	  Object_Px_in.push_back(Object_PtEtaPhiE[j].Px());
	  Object_Py_in.push_back(Object_PtEtaPhiE[j].Py());
	  Object_Pz_in.push_back(Object_PtEtaPhiE[j].Pz());
	  Object_E_in.push_back(Object_PtEtaPhiE[j].E());
	  Object_Pt_in.push_back(Object_PtEtaPhiE[j].Pt());
	  Object_Eta_Rap_in.push_back(Object_PtEtaPhiE[j].Eta());
	  nin+=1;}
	else{
	  nout+=1;
	  Object_in_out.push_back(0);
	  Object_PtEtaPhiE_out.push_back(Object_PtEtaPhiE[j]);
	  Object_Px_out.push_back(Object_PtEtaPhiE[j].Px());
	  Object_Py_out.push_back(Object_PtEtaPhiE[j].Py());
	  Object_Pz_out.push_back(Object_PtEtaPhiE[j].Pz());
	  Object_E_out.push_back(Object_PtEtaPhiE[j].E());
	  Object_Pt_out.push_back(Object_PtEtaPhiE[j].Pt());
	  Object_Eta_Rap_out.push_back(Object_PtEtaPhiE[j].Eta());
	}
      }else if(rap ==1){
	if(fabs(Object_PtEtaPhiE[j].Eta())<eta_c){
	  Object_in_out.push_back(1);
	  Pt_sum_c+=Object_PtEtaPhiE[j].Pt();
	  Eta_cw+=Object_PtEtaPhiE[j].Pt()*Object_PtEtaPhiE[j].Rapidity();
	  px_sum_in+=Object_PtEtaPhiE[j].Px();
	  py_sum_in+=Object_PtEtaPhiE[j].Py();
	  Object_PtEtaPhiE_in.push_back(Object_PtEtaPhiE[j]);
	  Object_Px_in.push_back(Object_PtEtaPhiE[j].Px());
	  Object_Py_in.push_back(Object_PtEtaPhiE[j].Py());
	  Object_Pz_in.push_back(Object_PtEtaPhiE[j].Pz());
	  Object_E_in.push_back(Object_PtEtaPhiE[j].E());
	  Object_Pt_in.push_back(Object_PtEtaPhiE[j].Pt());
	  Object_Eta_Rap_in.push_back(Object_PtEtaPhiE[j].Rapidity());
	  nin+=1;}
	else{
	  nout+=1;
	  Object_in_out.push_back(0);
	  Object_PtEtaPhiE_out.push_back(Object_PtEtaPhiE[j]);
	  Object_Px_out.push_back(Object_PtEtaPhiE[j].Px());
	  Object_Py_out.push_back(Object_PtEtaPhiE[j].Py());
	  Object_Pz_out.push_back(Object_PtEtaPhiE[j].Pz());
	  Object_E_out.push_back(Object_PtEtaPhiE[j].E());
	  Object_Pt_out.push_back(Object_PtEtaPhiE[j].Pt());
	  Object_Eta_Rap_out.push_back(Object_PtEtaPhiE[j].Rapidity());
	}
      }else if(rap==2){
	if(fabs(Object_PtEtaPhiE[j].Rapidity())<eta_c){
	  Object_in_out.push_back(1);
	  Pt_sum_c+=Object_PtEtaPhiE[j].Pt();
	  Eta_cw+=Object_PtEtaPhiE[j].Pt()*Object_PtEtaPhiE[j].Rapidity();
	  px_sum_in+=Object_PtEtaPhiE[j].Px();
	  py_sum_in+=Object_PtEtaPhiE[j].Py();
	  Object_PtEtaPhiE_in.push_back(Object_PtEtaPhiE[j]);
	  Object_Px_in.push_back(Object_PtEtaPhiE[j].Px());
	  Object_Py_in.push_back(Object_PtEtaPhiE[j].Py());
	  Object_Pz_in.push_back(Object_PtEtaPhiE[j].Pz());
	  Object_E_in.push_back(Object_PtEtaPhiE[j].E());
	  Object_Pt_in.push_back(Object_PtEtaPhiE[j].Pt());
	  Object_Eta_Rap_in.push_back(Object_PtEtaPhiE[j].Rapidity());
	  nin+=1;}
	else{
	  nout+=1;
	  Object_in_out.push_back(0);
	  Object_PtEtaPhiE_out.push_back(Object_PtEtaPhiE[j]);
	  Object_Px_out.push_back(Object_PtEtaPhiE[j].Px());
	  Object_Py_out.push_back(Object_PtEtaPhiE[j].Py());
	  Object_Pz_out.push_back(Object_PtEtaPhiE[j].Pz());
	  Object_E_out.push_back(Object_PtEtaPhiE[j].E());
	  Object_Pt_out.push_back(Object_PtEtaPhiE[j].Pt());
	  Object_Eta_Rap_out.push_back(Object_PtEtaPhiE[j].Rapidity());
	}
      }else{
	cout<<"eta central not set correctly: choice 0 (pseudorapity), choice 1 (rapidty/pseudorapidity) and choice 2 (rapidity) allowed only"<<endl;
      }
    }//loop over lorentzvectors to separate into central/non-central jets
    Eta_cw=Eta_cw/Pt_sum_c;
    if(Object_PtEtaPhiE_in.size()!=nin){cout<<"ERROR!!! wrong dimension of in momenta"<<endl;}
    
    float ExpTerm =0;
    for(unsigned int j=0;j<Object_PtEtaPhiE_out.size();j++){
      if(rap ==0){
	ExpTerm+=Object_PtEtaPhiE_out[j].Pt()*exp(-fabs(Object_PtEtaPhiE[j].Eta()-Eta_cw));
      }
      else{//(rap ==1)||(rap==2)
	ExpTerm+=Object_PtEtaPhiE_out[j].Pt()*exp(-fabs(Object_PtEtaPhiE[j].Rapidity()-Eta_cw));
      }
    }
    ExpTerm = ExpTerm/Pt_sum_c;
      
    EventShapeVector[12]=ExpTerm;

    RecTerm=sqrt(pow(px_sum_in,2)+pow(py_sum_in,2))/Pt_sum_c;
      
    EventShapeVector[13]=RecTerm;

    //unsigned int nout = length - nin; 
    if(nin<2){
      cout<<"WARNING!! Less than two momenta in the central region!! Central event shapes won't be calculated and set = -1.0 per default!"<<endl;
      //set the values to 10 - the initialization is the same
      for(unsigned int j = 0; j < length; j++){
	Grouping_c[j]=-1;
      }
    }
    //more than two central input momenta
    if (nin>1){      
      //the central global transverse thrust centrthr is calculated
      float centrthr=0;
      float centhrmin=0;
      unsigned int group1_up_c=0;
      unsigned int group2_down_c=0;
      
      //define in case there is no momenta out of the central region
      float alpha_c=atan2(ThrustAxis[1],ThrustAxis[0]);

      if(nout==0){
	for(unsigned int i=0;i<length;i++){
	  //all objects are in the central region
	  Grouping_c[i]=Grouping[i];
	  if(Grouping_c[i]==1){
	    group1_up_c+=1;
	  }else if(Grouping_c[i]==2){
	    group2_down_c+=1;
	  }
	}
	centrthr=dirgltrthr;
	centhrmin=dirglthrmin;
      }else{//momenta outside of central region 
	for (unsigned int i=0;i<nin;i++){
	  centrthr+=fabs(Object_PtEtaPhiE_in[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE_in[i].Py()*ThrustAxis[1]);
	}
	   
	//the variable which gets resummed is not thrust 
	//but tau = 1 - thrust - see calculation
	centrthr=1-centrthr/Pt_sum_c;	
	
	std::vector<int>grouping_temp;
	
	//the central grouping has the same dimension as the input vectors
	//not only the dimension of the central jet momenta
	for (unsigned int i=0;i<nin;i++){
	  if( (Object_PtEtaPhiE_in[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE_in[i].Py()*ThrustAxis[1]) >= 0){
	    grouping_temp.push_back(1);
	    group1_up_c+=1;
	  }else{
	    grouping_temp.push_back(2);
	    group2_down_c+=1;
	  }
	}   
	if(grouping_temp.size()!=nin){
	  cout<<"big error in central grouping: "<<grouping_temp.size()<<"/"<<nin<<endl;
	}

	unsigned int count_central=0;
	//fill in the central grouping 
	if(length!=Object_in_out.size()){
	  cout<<"error in in_out_vector length: "<<length<<"/"<<Object_in_out.size()<<endl;
	}
	for(unsigned int i=0;i<length;i++){
	  //the object is IN the central region
	  if(Object_in_out[i]==1){
	    Grouping_c[i]=grouping_temp[count_central];
	    count_central+=1;
	  }
	}       
	
	//the central thrust minor centhrmin
	//rotate the coordinate system around the beam axis that
	//the thrust axis is the new y'-Axis - the projections are
	//simply the new y-values then
	//redefinition of alpha_c if momenta outside of central region
	alpha_c=atan2(ThrustAxis[1],ThrustAxis[0]);
	for(unsigned int i=0;i<nin;i++){
	  centhrmin+= fabs(-sin(alpha_c)*Object_PtEtaPhiE_in[i].Px()+cos(alpha_c)*Object_PtEtaPhiE_in[i].Py());
	}
	centhrmin=centhrmin/Pt_sum_c;
      }

      EventShapeVector[2] = centrthr;  
      EventShapeVector[3] = centhrmin;      
      //central jet masses
      //define two jet masses in region U and D
      
      float cenjm_up=0;
      float cenjm_down=0;
      float dot_product =0;
      
      std::vector<float> up_sum;
      std::vector<float> down_sum;
      for(unsigned int j=0; j<4;j++){
	up_sum.push_back(0.);
	down_sum.push_back(0.);
      }
      for(unsigned int i=0;i<nin;i++){
	dot_product = Object_PtEtaPhiE_in[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE_in[i].Py()*ThrustAxis[1];
	if(dot_product>=0){
	  up_sum[0]+=Object_PtEtaPhiE_in[i].Px();
	  up_sum[1]+=Object_PtEtaPhiE_in[i].Py();
	  up_sum[2]+=Object_PtEtaPhiE_in[i].Pz();
	  up_sum[3]+=Object_PtEtaPhiE_in[i].E();
	}else{
	  down_sum[0]+=Object_PtEtaPhiE_in[i].Px();
	  down_sum[1]+=Object_PtEtaPhiE_in[i].Py();
	  down_sum[2]+=Object_PtEtaPhiE_in[i].Pz();
	  down_sum[3]+=Object_PtEtaPhiE_in[i].E();
	}
      }
      cenjm_up = lorentz_sp(up_sum,up_sum)/pow(Pt_sum_c,2);
      cenjm_down = lorentz_sp(down_sum,down_sum)/pow(Pt_sum_c,2);
      
      //central total jet mass centotjm
      float centotjm=0;
      centotjm = cenjm_up+cenjm_down;
      
      EventShapeVector[4]=centotjm;
      //central heavy jet mass cenheavjm
      float cenheavjm=0;
      cenheavjm = max(cenjm_up,cenjm_down);
      
      EventShapeVector[5]=cenheavjm;

      
      //the central jet broadenings in the up and down
      //region
      float cenbroad_up=0;
      float cenbroad_down=0;
      float lambda_1_U=0;
      float lambda_2_U=0;
      float lambda_1_D=0;
      float lambda_2_D=0;      

      float eta_up=0;
      unsigned int num_up=0;
      float eta_down =0;
      unsigned int num_down =0;
      float phi_temp =0;
      float phi_up_aver =0;
      float phi_down_aver =0;
      float Pt_sum_up =0;
      float Pt_sum_down =0;
      float dot_product_b =0;
      vector<float> phi_up;
      vector<float> phi_down;
      float py_rot =0;
      float px_rot =0;
     
      float matrixU[2][2];
      matrixU[0][0]=0;
      matrixU[0][1]=0;
      matrixU[1][0]=0;
      matrixU[1][1]=0;
      float matrixD[2][2];
      matrixD[0][0]=0;
      matrixD[0][1]=0;
      matrixD[1][0]=0;
      matrixD[1][1]=0;

      //if one of the hemispheres contains more than two momenta
      if((group1_up_c>1)||(group2_down_c>1)){
	for(unsigned int i=0;i<nin;i++){
	  //thrust-axis is here the predefined axis of interest
	  dot_product_b = Object_PtEtaPhiE_in[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE_in[i].Py()*ThrustAxis[1];
	  if(dot_product_b>=0){
	    Pt_sum_up+=Object_PtEtaPhiE_in[i].Pt();
	    //rotate the coordinate system so that
	    //the central thrust axis is e_x
	    px_rot = cos(alpha_c)*Object_PtEtaPhiE_in[i].Px()+sin(alpha_c)*Object_PtEtaPhiE_in[i].Py();
	    py_rot = - sin(alpha_c)*Object_PtEtaPhiE_in[i].Px()+cos(alpha_c)*Object_PtEtaPhiE_in[i].Py();
	    //calculate the eta and phi in the rotated system
	    if(rap==0){
	      eta_up+=Object_PtEtaPhiE_in[i].Pt()*Object_PtEtaPhiE[i].Eta();
	    }else{//rap==1 and rap==2
	      eta_up+=Object_PtEtaPhiE_in[i].Pt()*Object_PtEtaPhiE[i].Rapidity();
	    }	   
	    phi_temp =atan2(py_rot,px_rot);	   
	    if(phi_temp>M_PI/2){
	      phi_temp = phi_temp - M_PI/2;}
	    if (phi_temp<-M_PI/2){
	      phi_temp = phi_temp + M_PI/2;
	    }
	    phi_up.push_back(phi_temp);
	    phi_up_aver+=Object_PtEtaPhiE_in[i].Pt()*phi_temp;
	    num_up+=1;
	  }else{
	    if(rap==0){
	      eta_down+=Object_PtEtaPhiE_in[i].Pt()*Object_PtEtaPhiE[i].Eta();
	    }else{//rap==1 or rap ==2
	      eta_down+=Object_PtEtaPhiE_in[i].Pt()*Object_PtEtaPhiE[i].Rapidity();
	    }
	    Pt_sum_down+=Object_PtEtaPhiE_in[i].Pt();
	    px_rot = cos(alpha_c)*Object_PtEtaPhiE_in[i].Px()+sin(alpha_c)*Object_PtEtaPhiE_in[i].Py();
	    py_rot = - sin(alpha_c)*Object_PtEtaPhiE_in[i].Px()+cos(alpha_c)*Object_PtEtaPhiE_in[i].Py();
	    phi_temp =atan2(py_rot,px_rot);
	    if(phi_temp>M_PI/2){
	      //if phi is bigger than pi/2 in the new system calculate 
	      //the difference to the thrust axis 
	      phi_temp = M_PI -phi_temp;}
	    if (phi_temp<-M_PI/2){
	      //if phi is smaller than 
	      phi_temp = -M_PI-phi_temp;}
	    phi_down.push_back(phi_temp);
	    //calculate the pt-weighted phi
	    phi_down_aver+=Object_PtEtaPhiE_in[i].Pt()*phi_temp;	
	    num_down+=1;
	  }
	}

	if (num_up!=0){
	  eta_up=eta_up/Pt_sum_up;
	  phi_up_aver=phi_up_aver/Pt_sum_up;}
	if(num_down!=0){
	  eta_down = eta_down/Pt_sum_down;
	  phi_down_aver=phi_down_aver/Pt_sum_down;}	
	unsigned int index_up=0;
	unsigned int index_down=0;	
	for(unsigned int i=0;i<nin;i++){
	  dot_product_b =Object_PtEtaPhiE_in[i].Px()*ThrustAxis[0]+Object_PtEtaPhiE_in[i].Py()*ThrustAxis[1];
	  if(dot_product_b>=0){
	    //fill the upper matrix elements
	    //calculate the broadenings of the regions with the rotated system
	    //and the pt-weighted average of phi in the rotated system
	    if(rap==0){
	      cenbroad_up+=Object_PtEtaPhiE_in[i].Pt()*sqrt(pow(Object_PtEtaPhiE[i].Eta()-eta_up,2)+pow(DeltaPhi(phi_up[index_up],phi_up_aver),2));
	      matrixU[0][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*pow(Object_PtEtaPhiE[i].Eta()-eta_up,2);
	      matrixU[0][1]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Eta()-eta_up)*(phi_up[index_up]-phi_up_aver);
	      matrixU[1][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Eta()-eta_up)*(phi_up[index_up]-phi_up_aver);
	    }else{//rap==1 and rap==2
	      cenbroad_up+=Object_PtEtaPhiE_in[i].Pt()*sqrt(pow(Object_PtEtaPhiE[i].Rapidity()-eta_up,2)+pow(DeltaPhi(phi_up[index_up],phi_up_aver),2));
	      matrixU[0][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*pow(Object_PtEtaPhiE[i].Rapidity()-eta_up,2);
	      matrixU[0][1]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Rapidity()-eta_up)*(phi_up[index_up]-phi_up_aver);
	      matrixU[1][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Rapidity()-eta_up)*(phi_up[index_up]-phi_up_aver);
	    }
	    matrixU[1][1]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(phi_up[index_up]-phi_up_aver)*(phi_up[index_up]-phi_up_aver);
	    if(fabs(phi_up[index_up]-phi_up_aver)>M_PI){
	      cout<<"problem in up phi: "<<fabs(phi_up[index_up]-phi_up_aver);
	    }
	    index_up+=1;
	  }else{
	    if(rap==0){
	      cenbroad_down+=Object_Pt_in[i]*sqrt(pow(Object_PtEtaPhiE[i].Eta()-eta_down,2)+pow(DeltaPhi(phi_down[index_down],phi_down_aver),2));
	      matrixD[0][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*pow(Object_PtEtaPhiE[i].Eta()-eta_down,2);
	      matrixD[0][1]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Eta()-eta_down)*(phi_down[index_down]-phi_down_aver);
	      matrixD[1][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Eta()-eta_down)*(phi_down[index_down]-phi_down_aver);
	    }else{//rap==1 or rap==2
	      cenbroad_down+=Object_Pt_in[i]*sqrt(pow(Object_PtEtaPhiE[i].Rapidity()-eta_down,2)+pow(DeltaPhi(phi_down[index_down],phi_down_aver),2));
	      matrixD[0][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*pow(Object_PtEtaPhiE[i].Rapidity()-eta_down,2);
	      matrixD[0][1]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Rapidity()-eta_down)*(phi_down[index_down]-phi_down_aver);
	      matrixD[1][0]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(Object_PtEtaPhiE[i].Rapidity()-eta_down)*(phi_down[index_down]-phi_down_aver);
	    }
	    if(fabs(phi_down[index_down]-phi_down_aver)>M_PI){
	      cout<<"problem in down phi: "<<fabs(phi_down[index_down]-phi_down_aver);
	    }
	    matrixD[1][1]+=Object_PtEtaPhiE_in[i].Pt()/Pt_sum_c*(phi_down[index_down]-phi_down_aver)*(phi_down[index_down]-phi_down_aver);
	    index_down+=1;
	  }
	}
      }//broadening values calculated for events with 3 and more input objects

      if(group1_up_c<2){
       cenbroad_up=0;
     }
     if(group2_down_c<2){
       cenbroad_down=0;
     }
     if(group1_up_c>1){
       lambda_1_U=(matrixU[0][0]+matrixU[1][1])/2.0+sqrt( pow( (matrixU[0][0]-matrixU[1][1])/2.0  ,2)+matrixU[0][1]*matrixU[1][0]);
       lambda_2_U=(matrixU[0][0]+matrixU[1][1])/2.0-sqrt( pow( (matrixU[0][0]-matrixU[1][1])/2.0  ,2)+matrixU[0][1]*matrixU[1][0]);
     }
     
     if(group2_down_c>1){
       lambda_1_D=(matrixD[0][0]+matrixD[1][1])/2.0+sqrt( pow( (matrixD[0][0]-matrixD[1][1])/2.0  ,2)+matrixD[0][1]*matrixD[1][0]);
       lambda_2_D=(matrixD[0][0]+matrixD[1][1])/2.0-sqrt( pow( (matrixD[0][0]-matrixD[1][1])/2.0  ,2)+matrixD[0][1]*matrixD[1][0]);
     }

 
     EventShapeVector[8]=max (lambda_1_U,lambda_1_D);
     EventShapeVector[9]=min (lambda_1_U,lambda_1_D);
     EventShapeVector[10]=max (lambda_2_U,lambda_2_D);
     EventShapeVector[11]=min (lambda_2_U,lambda_2_D);
     
     cenbroad_up=cenbroad_up/(2*Pt_sum_c);
     cenbroad_down=cenbroad_down/(2*Pt_sum_c);
     //central total jet broadening
     float centotbroad=0;
     centotbroad = cenbroad_up+cenbroad_down;
     EventShapeVector[6]=centotbroad;
     //central wide jet broadening
     float cenwidbroad=0;
     cenwidbroad = max(cenbroad_up,cenbroad_down);
     
     EventShapeVector[7]=cenwidbroad;

     
    }//more than two central momenta - calculate broadenings
    //cental evshp
      
  }
      
  return 1;
}

//||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||
//here functions begin||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||

float EventShapeAxis::DeltaPhi(float phi1, float phi2)
{
  float delta_phi = fabs(phi2 - phi1);
  if (delta_phi > M_PI){ 
    delta_phi = 2*M_PI-delta_phi;
  } 
  return delta_phi;
} 

float EventShapeAxis::max (float a, float b)
{ if(a>=b){return a;}
 else{return b;}
}

float EventShapeAxis::min (float a, float b)
{ if(a<=b){return a;}
 else{return b;}
}

//returns the scalar product between two 4 momenta
float EventShapeAxis::lorentz_sp(std::vector<float> a, std::vector<float> b){
  unsigned int dim = (unsigned int) a.size();
  if(a.size()!=b.size()){
    cout<<"ERROR!!! Dimension of input vectors are different! Change that please!"<<endl;
    return 0;}
  else{float l_dot_product=a[dim-1]*b[dim-1];
  for(unsigned int i=0; i<dim-1;i++){
    l_dot_product-=a[i]*b[i];
  }
  return l_dot_product;
  }
}

