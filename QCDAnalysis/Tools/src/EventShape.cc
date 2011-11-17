#include "QCDAnalysis/Tools/interface/EventShape.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;

/*  \class EventShape
*
*  Class that, given the 4-momenta of the objects in the event, 
*  allows to calculate event shapes 
*
*  Authors: Matthias Weber                 Date: July/October 2007
*
*/

EventShape::EventShape(vector<double>& Px_vector, vector<double>& Py_vector, vector<double>& Pz_vector, vector<double>& E_vector, double eta_central, int rapidity) : Object_Px(Px_vector),Object_Py(Py_vector), Object_Pz(Pz_vector), Object_E(E_vector),eta_c(eta_central),rap(rapidity){}


vector<double> EventShape::getEventShapes(){
  this->calculate();
  return EventShapes;
}

vector<double> EventShape::getThrustAxis(){
  this->calculate();
  return ThrustAxis;
}

vector<double> EventShape::getThrustAxis_C(){
  this->calculate();
  return ThrustAxis_c;
}

vector<int> EventShape::getGrouping(){
  this->calculate();
  return Grouping;
}

vector<int> EventShape::getGrouping_C(){
  this->calculate();
  return Grouping_c;
}

int EventShape::calculate(){
  unsigned int length = (unsigned int) Object_Px.size();
  if(((unsigned int) Object_Py.size()!=length)||((unsigned int) Object_Pz.size()!=length)||((unsigned int) Object_E.size()!=length)){
    cout << "ERROR!!!! Input vectors differ in size! Change that please!" << endl;
    cout<<"py_size: "<<Object_Py.size()<<" ,pz_size: "<<Object_Pz.size()<<" ,px_size: "<<Object_Px.size()<<" ,E_size: "<<Object_E.size()<<endl;
    return 0;
  }
  
  if (!Object_Pt.empty()){
    Object_Pt.clear();
    Object_Eta_Rap.clear();
    Object_Phi.clear();
  }

    //Object_eta vector gets only filled if rapidity 
    //rap==1 is chosen
  if(!Object_Eta.empty()){
    Object_Eta.clear();
  }

  if(!EventShapes.empty()){
    EventShapes.clear();
    ThrustAxis.clear();
    ThrustAxis_c.clear();
    Grouping.clear();
    Grouping_c.clear();
  }


  //if we have LESS than 2 objects e.g. if one or less jets in an event
  //should survive a preselection on jets in a jet collection
  //in this case an event shape gives no meaningful value

  for(unsigned int j = 0; j < 30; j++){
    EventShapes.push_back(-1.0);
  } 
  for(unsigned int j = 0; j < 3; j++){
    ThrustAxis.push_back(10.);
  }
  for(unsigned int j = 0; j < length; j++){
    Grouping.push_back(-1);
  }
  for(unsigned int j = 0; j < 3; j++){
    ThrustAxis_c.push_back(10.);
  }


  for(unsigned int j = 0; j < length; j++){
    Grouping_c.push_back(-1);
  }
  //cout<<"evshp class grouping size: "<<Grouping_c.size()<<endl;

  if(length<2){
    for(unsigned int j = 0; j < length; j++){
      Grouping[j]=1;
    }   
    //cout<<"WARNING!! Less than two momenta in the event given!! Event shapes won't be calculated and set = -1.0 per default!"<<endl;
    //cout<<"The global and central thrust axis are set to T = (10,10,10) per default"<<endl;
    //cout<<"all central group entries will be -1"<<endl;
    //cout<<"global group entries will be 1"<<endl;
    //cout<<"the info about the hemisphere axes are set to -1"<<endl;    
  }else{
    
    //at least 2 input objects which is required for a meaningful value
    //for event-shapes
    for(unsigned int j = 0; j < length; j++){
      Object_Pt.push_back(0.);
      Object_Eta_Rap.push_back(0.);
      Object_Phi.push_back(0.);
      //only in this case eta is used in the following calculations
      if(rap==1){
	Object_Eta.push_back(0.);
      }
    }

    double theta=0;
    for(unsigned int k =0; k<length; k++){
      Object_Pt[k] = sqrt(pow(Object_Px[k],2) + pow(Object_Py[k],2));
      //to prevent a division by zero
      if (fabs(Object_Pz[k]) > 1E-4 ) {
	theta = atan(Object_Pt[k]/(Object_Pz[k]));
      }
      else {theta = M_PI/2;}
      if(theta<0.){theta = theta + M_PI;}
      if(rap==0){	
	Object_Eta_Rap[k] = - log(tan(0.5*theta));
      }
      if(rap==1){
	if(Object_Pz[k]==Object_E[k]){
	  cout<<"ERROR!!! Object "<<k<<" has Pz "<< Object_Pz[k]<<" which is equal to E = "<<Object_E[k]<<endl;
	  return 0;}
	Object_Eta_Rap[k]=0.5*log((Object_E[k]+Object_Pz[k])/(Object_E[k]-Object_Pz[k]));
	Object_Eta[k] = - log(tan(0.5*theta));
      }
      if((rap!=0)&&(rap!=1)){
	cout<<"ERROR!!!, The choice to use the rapidity y or the pseudorapidity eta is not set correctly! Change that please!"<<endl;
	return 0;}
      Object_Phi[k] = atan2(Object_Py[k], Object_Px[k]);
    }
    
    //sum of all Pt's
    double Pt_sum =0;
    for(unsigned int i=0;i<length;i++){
      Pt_sum+=sqrt(pow(Object_Px[i],2)+pow(Object_Py[i],2));
    }
    
    //calculate the directly global transverse thrust dirgltrthr
    double dirgltrthr=0;
    
    std::vector<double> Thrust;
    Thrust = Thrust_calculate(Object_Px, Object_Py);
    //the variable thrust is tau = 1-thrust 
    dirgltrthr =Thrust[3];
    
    EventShapes[0] = dirgltrthr;  
    
    for(unsigned int k=0;k<3;k++){
      ThrustAxis[k] = Thrust[k];
    }

    unsigned int group1_up=0;
    unsigned int group2_down=0;           
    for (unsigned int i=0; i<length; i++){
      if( (Object_Px[i]*ThrustAxis[0]+Object_Py[i]*ThrustAxis[1]) >= 0){
	Grouping[i]=1;
	group1_up+=1;
      }
      if( (Object_Px[i]*ThrustAxis[0]+Object_Py[i]*ThrustAxis[1]) < 0){
	Grouping[i]=2;
	group2_down+=1;
      }
    }

    //the directly global thrust minor dirglthrmin
    double dirglthrmin =0;
    //rotate the coordinate system around the beam axis that
    //the thrust axis is the new y'-Axis - the projections are
    //simply the new y-values then
    double alpha=atan2(ThrustAxis[1],ThrustAxis[0]);
    for(unsigned int k=0;k<length;k++){
      dirglthrmin+= fabs(-sin(alpha)*Object_Px[k]+cos(alpha)*Object_Py[k]);
    }
    dirglthrmin=dirglthrmin/Pt_sum;
    EventShapes[1] = dirglthrmin;
    
    
    //the directly global three-jet resolution 
    //threshold dirgly3
    double dirgly3 =0;
    
    if(length<3){
      //cout<<"WARNING: less than three input objects!! The directly global three-jet resolution threshold will be set to -1.0 per default!"<<endl; 
      dirgly3 =-1.0;}
    else{
      dirgly3 = three_jet_res(Object_Px, Object_Py, Object_Pz, Object_E, rap);
    }
    
    EventShapes[2] = dirgly3;
   
    //||||||||||||||||||||||||||||||||||||||||||||||
    //here the central event shape variables begin||
    //||||||||||||||||||||||||||||||||||||||||||||||
    
    std::vector<double> Object_Px_in;
    std::vector<double> Object_Py_in;
    std::vector<double> Object_Pz_in;
    std::vector<double> Object_Pt_in;
    std::vector<double> Object_E_in;
    std::vector<double> Object_Eta_Rap_in;
    std::vector<double> Object_Px_out;
    std::vector<double> Object_Py_out;
    std::vector<double> Object_Pz_out;
    std::vector<double> Object_E_out;
    std::vector<double> Object_Pt_out;
    std::vector<double> Object_Eta_Rap_out;

    //0 if object out, 1 if object inside of central region
    std::vector<unsigned int> Object_in_out;
    
    if (!Object_Px_in.empty()){
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
    }
    
    unsigned int nin = 0;
    for(unsigned int j=0;j<length;j++){
      //in the case of using pseudorapidity
      //in the DeltaR values for the event-shape calculation
      if(rap ==0){
	if(fabs(Object_Eta_Rap[j])<eta_c){
	  Object_in_out.push_back(1);
	  Object_Px_in.push_back(Object_Px[j]);
	  Object_Py_in.push_back(Object_Py[j]);
	  Object_Pz_in.push_back(Object_Pz[j]);
	  Object_E_in.push_back(Object_E[j]);
	  Object_Pt_in.push_back(sqrt(pow(Object_Px[j],2)+pow(Object_Py[j],2)));
	  Object_Eta_Rap_in.push_back(Object_Eta_Rap[j]);
	  nin+=1;}
	else{
	  Object_in_out.push_back(0);
	  Object_Px_out.push_back(Object_Px[j]);
	  Object_Py_out.push_back(Object_Py[j]);
	  Object_Pz_out.push_back(Object_Pz[j]);
	  Object_E_out.push_back(Object_E[j]);
	  Object_Pt_out.push_back(sqrt(pow(Object_Px[j],2)+pow(Object_Py[j],2)));
	  Object_Eta_Rap_out.push_back(Object_Eta_Rap[j]);
	}
      }
      if(rap ==1){
	if(fabs(Object_Eta[j])<eta_c){
	  Object_in_out.push_back(1);
	  Object_Px_in.push_back(Object_Px[j]);
	  Object_Py_in.push_back(Object_Py[j]);
	  Object_Pz_in.push_back(Object_Pz[j]);
	  Object_E_in.push_back(Object_E[j]);
	  Object_Pt_in.push_back(sqrt(pow(Object_Px[j],2)+pow(Object_Py[j],2)));
	  Object_Eta_Rap_in.push_back(Object_Eta_Rap[j]);
	  nin+=1;}
	else{
	  Object_in_out.push_back(0);
	  Object_Px_out.push_back(Object_Px[j]);
	  Object_Py_out.push_back(Object_Py[j]);
	  Object_Pz_out.push_back(Object_Pz[j]);
	  Object_E_out.push_back(Object_E[j]);
	  Object_Pt_out.push_back(sqrt(pow(Object_Px[j],2)+pow(Object_Py[j],2)));
	  Object_Eta_Rap_out.push_back(Object_Eta_Rap[j]);
	}
      }
    }
    if(Object_Px_in.size()!=nin){cout<<"ERROR!!! wrong dimension of in momenta"<<endl;}
    
    unsigned int nout = length - nin; 
    if(nin<2){
      //cout<<"WARNING!! Less than two momenta in the central region!! Central event shapes won't be calculated and set = -1.0 per default!"<<endl;
      //cout<<"The central thrust axis is set to T_c = (10,10,10)"<<endl;
      //cout<<"all additional info is set to -1 per default!"<<endl;
      for(unsigned int i=3; i<30;i++){
	EventShapes[i]=-1.0;
      }
      //set the values to 10 - the initialization is the same
      for(unsigned int l=0; l<3; l++){
	ThrustAxis_c[l]=10;
      }
      for(unsigned int j = 0; j < length; j++){
	Grouping_c[j]=-1;
      }
    }
    //at least two central input momenta
    if (nin>1){
      double Pt_sum_c = 0;
      double Eta_cw=0;
      double RecTerm =0;
      double px_sum_in = 0;
      double py_sum_in = 0;
      for(unsigned int j=0;j<nin;j++){
	Pt_sum_c+=Object_Pt_in[j];
	Eta_cw+=Object_Pt_in[j]*Object_Eta_Rap_in[j];
	px_sum_in+=Object_Px_in[j];
	py_sum_in+=Object_Py_in[j];
      }
      Eta_cw=Eta_cw/Pt_sum_c;
      RecTerm=sqrt(pow(px_sum_in,2)+pow(py_sum_in,2))/Pt_sum_c;
      
      double ExpTerm =0;
      for(unsigned int j=0;j<nout;j++){
	ExpTerm+=Object_Pt_out[j]*exp(-fabs(Object_Eta_Rap_out[j]-Eta_cw));
      }
      ExpTerm = ExpTerm/Pt_sum_c;
      
      
      //the central global transverse thrust centrthr is calculated
      double centrthr=0;
      //the central thrust minor centhrmin
      double centhrmin =0;

      unsigned int group1_up_c=0;
      unsigned int group2_down_c=0; 

      //if no momenta is outside of the central region
      //dirgl values == central values
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
	for(unsigned int l=0; l<3; l++){
	  ThrustAxis_c[l]=ThrustAxis[l];
	}
	centrthr=dirgltrthr;
	centhrmin=dirglthrmin;

      }else{
	std::vector<double> Thrust_central = Thrust_calculate(Object_Px_in, Object_Py_in);
	
	for(unsigned int l=0; l<3; l++){
	  ThrustAxis_c[l]=Thrust_central[l];
	}   
	//the variable which gets resummed is not thrust 
	//but tau = 1 - thrust - see calculation
	centrthr=Thrust_central[3];

	//rotate the coordinate system around the beam axis that
	//the thrust axis is the new y'-Axis - the projections are
	//simply the new y-values then
	double alpha_cen=atan2(ThrustAxis_c[1],ThrustAxis_c[0]);
	for(unsigned int i=0;i<nin;i++){
	  centhrmin+= fabs(-sin(alpha_cen)*Object_Px_in[i]+cos(alpha_cen)*Object_Py_in[i]);
	}
	centhrmin=centhrmin/Pt_sum_c;

	//information about the grouping
	//in both hemispheres  
	
	std::vector<int>grouping_temp;
	
	//the central grouping has the same dimension as the input vectors
	//not only the dimension of the central jet momenta
	for (unsigned int i=0;i<nin;i++){
	  if( (Object_Px_in[i]*ThrustAxis_c[0]+Object_Py_in[i]*ThrustAxis_c[1]) >= 0){
	    grouping_temp.push_back(1);
	    group1_up_c+=1;
	  }
	  if( (Object_Px_in[i]*ThrustAxis_c[0]+Object_Py_in[i]*ThrustAxis_c[1]) < 0){
	    grouping_temp.push_back(2);
	    group2_down_c+=1;
	  }
	}   
	
	unsigned int count_central=0;
	//fill in the central grouping 
	for(unsigned int i=0;i<length;i++){
	  //the object is IN the central region
	  if(Object_in_out[i]==1){
	    Grouping_c[i]=grouping_temp[count_central];
	    count_central+=1;
	  }
	  //cout<<"object: "<<i<<" grouping: "<<Grouping[i]<<" ,central grouping: "<<Grouping_c[i]<<endl;
	}
      }//calculation of central variables if nout>0

      double alpha_c=atan2(ThrustAxis_c[1],ThrustAxis_c[0]);


      EventShapes[3] = centrthr;

      //the central transverse thrust with exponentially
      //suppressed forward term
      //centrthr is again centau = 1 - centrthr
      EventShapes[4]=centrthr+ExpTerm;
      
      //the central transverse thrust with 
      //recoil term
      //centrthr is again centau = 1 - centrthr
      EventShapes[5]=centrthr+RecTerm;
          
      EventShapes[6] = centhrmin;
      
      //the central thrust minor with exponentially
      //suppressed forward term centhrminwexp
      
      EventShapes[7]=centhrmin+ExpTerm;
      
      //the central thrust minor with 
      //recoil term centhrminwr
      EventShapes[8]=centhrmin+RecTerm;
      
      //central jet masses
      //define two jet masses in region U and D
      double cenjm_up=0;
      double cenjm_down=0;
      double dot_product =0;
      
      std::vector<double> up_sum;
      std::vector<double> down_sum;
      for(unsigned int j=0; j<4;j++){
	up_sum.push_back(0.);
	down_sum.push_back(0.);
      }
      for(unsigned int i=0;i<nin;i++){
	dot_product = Object_Px_in[i]*ThrustAxis_c[0]+Object_Py_in[i]*ThrustAxis_c[1];
	if(dot_product>=0){
	  up_sum[0]+=Object_Px_in[i];
	  up_sum[1]+=Object_Py_in[i];
	  up_sum[2]+=Object_Pz_in[i];
	  up_sum[3]+=Object_E_in[i];
	}else{
	  down_sum[0]+=Object_Px_in[i];
	  down_sum[1]+=Object_Py_in[i];
	  down_sum[2]+=Object_Pz_in[i];
	  down_sum[3]+=Object_E_in[i];
	}
      }
      cenjm_up = lorentz_sp(up_sum,up_sum)/pow(Pt_sum_c,2);
      cenjm_down = lorentz_sp(down_sum,down_sum)/pow(Pt_sum_c,2);
      
      
      //central total jet mass centotjm
      double centotjm=0;
      centotjm = cenjm_up+cenjm_down;
      
      EventShapes[9]=centotjm;
      
      //central total jet mass with exponentially
      //suppressed forward term centotjmwexp
      double centotjmwexp=0;
      centotjmwexp = centotjm+ExpTerm;
      
      EventShapes[10]=centotjmwexp;
      
      //central total jet mass with recoil term
      //centotjmwr
      double centotjmwr=0;
      centotjmwr = centotjm+RecTerm;
      
      EventShapes[11]=centotjmwr;
      
      //central heavy jet mass cenheavjm
      double cenheavjm=0;
      cenheavjm = max(cenjm_up,cenjm_down);
      
      EventShapes[12]=cenheavjm;
      
      //central heavy jet mass with exponentially
      //suppressed forward term cenheavjmwexp
      double cenheavjmwexp=0;
      cenheavjmwexp = cenheavjm+ExpTerm;
      
      EventShapes[13]=cenheavjmwexp;
      
      //central heavy jet mass with recoil term
      //cenheavjmwr
      
      double cenheavjmwr=0;
      cenheavjmwr = cenheavjm+RecTerm;
      
      EventShapes[14]=cenheavjmwr;
      
      double centrjm_up=0;
      double centrjm_down=0;	
      std::vector<double> upsum;
      std::vector<double> downsum;
      for(unsigned int j=0; j<3;j++){
	upsum.push_back(0.);
	downsum.push_back(0.);
      }
      //if one of the hemispheres contains more than two momenta - else the centrjm are 0 anyway
      if((group1_up_c>1)||(group2_down_c>1)){
	for(unsigned int i=0;i<nin;i++){
	  dot_product = Object_Px_in[i]*ThrustAxis_c[0]+Object_Py_in[i]*ThrustAxis_c[1];
	  if(dot_product>=0){
	    upsum[0]+=Object_Px_in[i];
	    upsum[1]+=Object_Py_in[i];
	    upsum[2]+=Object_Pt_in[i];
	  }else{
	    downsum[0]+=Object_Px_in[i];
	    downsum[1]+=Object_Py_in[i];
	    downsum[2]+=Object_Pt_in[i];
	  }
	}
      }

      centrjm_up= lorentz_sp(upsum,upsum)/pow(Pt_sum_c,2);
      centrjm_down= lorentz_sp(downsum,downsum)/pow(Pt_sum_c,2);

      //if one of the groups contains only 1 object 
      if(group1_up_c<2){
	centrjm_up=0;
      }
      if(group2_down_c<2){
	centrjm_down=0;
      }	
      double centottrjm = centrjm_up + centrjm_down;
      double cenheavtrjm = max(centrjm_up,centrjm_down);
      
      EventShapes[24] = centottrjm;
      EventShapes[25] = centottrjm + ExpTerm;
      EventShapes[26] = centottrjm + RecTerm;
      
      EventShapes[27] = cenheavtrjm;
      EventShapes[28] = cenheavtrjm + ExpTerm;
      EventShapes[29] = cenheavtrjm + RecTerm;
      
      double ceny3=0;

      
      //central three-jet resolution threshold
      
      
      //central three-jet resolution threshold with
      //exponentially suppressed forward term
      double ceny3wexp=0;
      
      //central three-jet resolution threshold
      //with recoil term
      double ceny3wr=0;
      
      if(nin<3){
	//cout<<"WARNING: less than three central input momenta!! The central three-jet resolution threshold variables will be set to -1.0 per default!"<<endl; 
	ceny3 =-1.0;
	ceny3wexp =-1.0;
	ceny3wr =-1.0;}
      else{
	if(nout==0){
	  ceny3=dirgly3;
	}else{
	  ceny3 = three_jet_res(Object_Px_in, Object_Py_in, Object_Pz_in, Object_E_in,rap);
	}
	//this time we need to add the squares of the recoil- and exp-terms
	ceny3wexp = ceny3+pow(ExpTerm,2);
	ceny3wr = ceny3+pow(RecTerm,2);
      }
      EventShapes[15] = ceny3;
      EventShapes[16] = ceny3wexp;
      EventShapes[17] = ceny3wr;
     
      
      //the central jet broadenings in the up and down
      //region
      double cenbroad_up=0;
      double cenbroad_down=0;
      
      double eta_up=0;
      unsigned int num_up=0;
      double eta_down =0;
      unsigned int num_down =0;
      double phi_temp =0;
      double phi_up_aver =0;
      double phi_down_aver =0;
      double Pt_sum_up =0;
      double Pt_sum_down =0;
      double dot_product_b =0;
      vector<double> phi_up;
      vector<double> phi_down;
      double py_rot =0;
      double px_rot =0;
     

      //if one of the hemispheres contains more than two momenta
      if((group1_up_c>1)||(group2_down_c>1)){
	for(unsigned int i=0;i<nin;i++){
	  dot_product_b =Object_Px_in[i]*ThrustAxis_c[0]+Object_Py_in[i]*ThrustAxis_c[1];
	  if(dot_product_b>=0){
	    Pt_sum_up+=Object_Pt_in[i];
	    //rotate the coordinate system so that
	    //the central thrust axis is e_x
	    px_rot = cos(alpha_c)*Object_Px_in[i]+sin(alpha_c)*Object_Py_in[i];
	    py_rot = - sin(alpha_c)*Object_Px_in[i]+cos(alpha_c)*Object_Py_in[i];
	    //calculate the eta and phi in the rotated system
	    eta_up+=Object_Pt_in[i]*Object_Eta_Rap_in[i];
	    phi_temp =atan2(py_rot,px_rot);
	    
	    if(phi_temp>M_PI/2){
	      phi_temp = phi_temp - M_PI/2;}
	    if (phi_temp<-M_PI/2){
	      phi_temp = phi_temp + M_PI/2;
	    }
	    phi_up.push_back(phi_temp);
	    phi_up_aver+=Object_Pt_in[i]*phi_temp;
	    num_up+=1;
	  }else{
	    eta_down+=Object_Pt_in[i]*Object_Eta_Rap_in[i];
	    Pt_sum_down+=Object_Pt_in[i];
	    px_rot = cos(alpha_c)*Object_Px_in[i]+sin(alpha_c)*Object_Py_in[i];
	    py_rot = - sin(alpha_c)*Object_Px_in[i]+cos(alpha_c)*Object_Py_in[i];
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
	    phi_down_aver+=Object_Pt_in[i]*phi_temp;	
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
	  dot_product_b =Object_Px_in[i]*ThrustAxis_c[0]+Object_Py_in[i]*ThrustAxis_c[1];
	  if(dot_product_b>=0){
	    //calculate the broadenings of the regions with the rotated system
	    //and the pt-weighted average of phi in the rotated system
	    cenbroad_up+=Object_Pt_in[i]*sqrt(pow(Object_Eta_Rap_in[i]-eta_up,2)+pow(DeltaPhi(phi_up[index_up],phi_up_aver),2));
	    index_up+=1;
	  }else{
	    cenbroad_down+=Object_Pt_in[i]*sqrt(pow(Object_Eta_Rap_in[i]-eta_down,2)+pow(DeltaPhi(phi_down[index_down],phi_down_aver),2));
	    index_down+=1;
	  }
	}
      }

      if(group1_up_c<2){
	cenbroad_up=0;
      }
      if(group2_down_c<2){
	cenbroad_down=0;
      }
      cenbroad_up=cenbroad_up/(2*Pt_sum_c);
      cenbroad_down=cenbroad_down/(2*Pt_sum_c);

      //central total jet broadening
      double centotbroad=0;
      centotbroad = cenbroad_up+cenbroad_down;
      
      EventShapes[18]=centotbroad;
      
      //central total jet broadening with exponentially
      //suppressed forward term
      double centotbroadwexp=0;
      centotbroadwexp =centotbroad + ExpTerm;
      
      EventShapes[19]=centotbroadwexp;
      
      //central total jet broadening with
      //recoil term
      double centotbroadwr=0;
      centotbroadwr = centotbroad + RecTerm;
      
      EventShapes[20]=centotbroadwr;
      
      //central wide jet broadening
      double cenwidbroad=0;
      cenwidbroad = max(cenbroad_up,cenbroad_down);
      
      EventShapes[21]=cenwidbroad;
      
      //central wide jet broadening with exponentially
      //suppressed forward term
      double cenwidbroadwexp=0;
      cenwidbroadwexp =cenwidbroad + ExpTerm;
      
      EventShapes[22]=cenwidbroadwexp;
      
      //central wide jet broadening with
      //recoil term
      double cenwidbroadwr=0;
      cenwidbroadwr = cenwidbroad + RecTerm;
      
      EventShapes[23]=cenwidbroadwr;
      
    }
  }
  return 1;
}

//||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||
//here functions begin||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||
//||||||||||||||||||||||||||||||||||||||||||||||||||||

double EventShape::DeltaPhi(double phi1, double phi2)
{
  double delta_phi = fabs(phi2 - phi1);
  if (delta_phi > M_PI){ 
    delta_phi = 2*M_PI-delta_phi;
  } 
  return delta_phi;
} 

double EventShape::max (double a, double b)
{ if(a>=b){return a;}
 else{return b;}
}

double EventShape::min (double a, double b)
{ if(a<=b){return a;}
 else{return b;}
}

//returns the scalar product between two 4 momenta
double EventShape::lorentz_sp(std::vector<double> a, std::vector<double> b){
  unsigned int dim = (unsigned int) a.size();
  if(a.size()!=b.size()){
    cout<<"ERROR!!! Dimension of input vectors are different! Change that please!"<<endl;
    return 0;}
  else{double l_dot_product=a[dim-1]*b[dim-1];
  for(unsigned int i=0; i<dim-1;i++){
    l_dot_product-=a[i]*b[i];
  }
  return l_dot_product;
  }
}

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//function which calculates the three-jet resolution thresholds|||
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
double EventShape::three_jet_res(std::vector<double> In_Object_Px, std::vector<double> In_Object_Py, std::vector<double> In_Object_Pz, std::vector<double> In_Object_E, int rap){

  unsigned int y3_length = (unsigned int)In_Object_Px.size();
  if(((unsigned int) In_Object_Py.size()!=y3_length)||((unsigned int) In_Object_Pz.size()!=y3_length)||(In_Object_E.size()!=y3_length)){
    cout << "ERROR!!!! Input vectors differ in size! Change that please!" << endl;
    cout<<"py_size: "<<In_Object_Py.size()<<" ,pz_size: "<<In_Object_Pz.size()<<" ,px_size: "<<In_Object_Px.size()<<" , E_size: "<<In_Object_E.size()<<endl;
    return 0;}

  std::vector<double> In_Object_Pt;
  std::vector<double> In_Object_Eta_Rap;
  std::vector<double> In_Object_Phi;

   if (!In_Object_Pt.empty()){
     In_Object_Pt.clear();
     In_Object_Eta_Rap.clear();
     In_Object_Phi.clear();
   }


   for(unsigned int j = 0; j < y3_length; j++){
     In_Object_Pt.push_back(0.);
     In_Object_Eta_Rap.push_back(0.);
     In_Object_Phi.push_back(0.);
   }
  double theta_y3_1st=0;
  for(unsigned int k =0; k<y3_length; k++){
    In_Object_Pt[k] = sqrt(pow(In_Object_Px[k],2) + pow(In_Object_Py[k],2));
    //calculates the pseudorapidity
    //to prevent a division by zero
    if(rap==0){
      if (fabs(In_Object_Pz[k]) > 1E-5) {
	theta_y3_1st = atan(In_Object_Pt[k]/(In_Object_Pz[k]));
      }
      else {theta_y3_1st = M_PI/2;}
      if(theta_y3_1st<0.){theta_y3_1st = theta_y3_1st + M_PI;}
      In_Object_Eta_Rap[k] = - log(tan(0.5*theta_y3_1st));
    }
    //calculates the real rapidity
    if(rap==1){
      In_Object_Eta_Rap[k]=0.5*log((In_Object_E[k]+In_Object_Pz[k])/(In_Object_E[k]-In_Object_Pz[k]));
    }
    In_Object_Phi[k] = atan2(In_Object_Py[k], In_Object_Px[k]);
  }

  //the three-jet resolution 
  //threshold y3
  double y3 =0;

  //vector which will be filled with the 
  //minimum of the distances
  double max_dmin_temp=0;

  double max_dmin =0;

  //distance input object k, beam
  double distance_jB=0;
  double distance_jB_min=0;
  //distance of input object k to l
  double distance_jk=0;
  double distance_jk_min =0;
  //as we search the minimum of the distances
  //give them values which are for sure higher
  //than those we evaluate first in the for-loups


  unsigned int index_jB=0;

  unsigned int index_j_jk = 0;
  unsigned int index_k_jk =0;

  //to decide later if the minmum is a jB or jk
  int decide_jB=-1;

  std::vector<double> input_Pt;
  std::vector<double> input_Px;
  std::vector<double> input_Py;
  std::vector<double> input_Pz;
  std::vector<double> input_E;
  std::vector<double> input_Phi;
  std::vector<double> input_Eta_Rap;

   if (!input_Pt.empty()){
     input_Pt.clear();
     input_Px.clear();
     input_Px.clear();
     input_Pz.clear();
     input_E.clear();
     input_Phi.clear();
     input_Eta_Rap.clear();
   }

  for(unsigned int j=0; j<y3_length;j++){
    input_Pt.push_back(In_Object_Pt[j]);
    input_Px.push_back(In_Object_Px[j]);
    input_Py.push_back(In_Object_Py[j]);
    input_Pz.push_back(In_Object_Pz[j]);
    input_E.push_back(In_Object_E[j]);
    input_Phi.push_back(In_Object_Phi[j]);
    //if rap==0 the pseudorapidity is filled
    //if rap==1 then the real rapidity is filled
    input_Eta_Rap.push_back(In_Object_Eta_Rap[j]);
  }
  if(y3_length<3){
    //cout<<"WARNING: less than three input momenta!! The directly global three-jet resolution threshold will be set to -1.0 per default!"<<endl; 
    y3 =-1.0;}
  else{
    unsigned int rest = y3_length;
    for(unsigned int i = 0; i<y3_length; i++){
      //make the minima at the initialization step
      //of each louping bigger than the first values
      distance_jB_min = pow(input_Pt[0],2) + 10;
      //DELTA PHIs wanted not the pure difference
      //choose as D-parameter D=0.6
      distance_jk_min=min(pow(input_Pt[1],2),pow(input_Pt[0],2))*(pow(input_Eta_Rap[1]-input_Eta_Rap[0],2)+pow(DeltaPhi(input_Phi[1],input_Phi[0]),2))/0.36 + 10;
      //do the procedure only until we have only 2 objects left anymore
      if(rest>2){
	for(unsigned int j=0; j<rest;j++){
	  //calculate the distance between object j and the beam
	  distance_jB = pow(input_Pt[j],2);
	  if(distance_jB<distance_jB_min){
	    distance_jB_min = distance_jB;
	    index_jB = j;}	
	  if(j>0){
	    for(unsigned int k=0; k<j;k++){
	      //calculate the distance in delta eta/rap and delta phi
	      //between object i and object j
	      //choose as D-parameter D=0.6
	      distance_jk = min(pow(input_Pt[j],2),pow(input_Pt[k],2))*(pow(input_Eta_Rap[j]-input_Eta_Rap[k],2)+pow(DeltaPhi(input_Phi[j],input_Phi[k]),2))/0.36;
	      if(distance_jk<distance_jk_min){
		distance_jk_min = distance_jk;
		index_j_jk = j;	      
		index_k_jk =k;
	      }
	    }
	    //cout<<"distance jk min "<<distance_jk_min<<endl;
	  }
	}
	//decide if the minimum is from a jB 
	//or jk combination
	if(distance_jk_min<distance_jB_min){
	  max_dmin_temp = max(distance_jk_min,max_dmin_temp);
	  decide_jB = 0;
	}else{
	  max_dmin_temp = max(distance_jB_min,max_dmin_temp);
	  decide_jB=1;}	
	//if we have only three jets left calculate
	//the maxima of the dmin's
	//if the minimum is a jB eliminate the input object
	if(decide_jB==1){
	  //if index_jB is the last one nothing is to do
	  if(index_jB!=rest-1){
	    for(unsigned int i=index_jB; i<rest-1;i++){
	      input_Pt[i]=input_Pt[i+1];
	      input_Phi[i]=input_Phi[i+1];
	      input_Eta_Rap[i]=input_Eta_Rap[i+1];
	      input_Px[i]=input_Px[i+1];
	      input_Py[i]=input_Py[i+1];
	      input_Pz[i]=input_Pz[i+1];
	      input_E[i]=input_E[i+1];
	    }
	  }
	}
	//if the minimum is a jk combine both input objects
	// put the new object at the index of index_k_jk (with index_k_jk<index_j_jk)
	if(decide_jB==0){
	  input_Px[index_k_jk] = input_Px[index_k_jk]+input_Px[index_j_jk];
	  input_Py[index_k_jk] = input_Py[index_k_jk]+input_Py[index_j_jk];
	  input_Pz[index_k_jk] = input_Pz[index_k_jk]+input_Pz[index_j_jk];
	  input_E[index_k_jk] = input_E[index_k_jk]+input_E[index_j_jk];
	  //calculate the pt, eta/rap and phi of the new combined momenta k_jk
	  input_Pt[index_k_jk] = sqrt(pow(input_Px[index_k_jk],2)+pow(input_Py[index_k_jk],2));
	  //in the case of pseudorapidity
	  if(rap==0){
	    double theta_new =0;
	    if (fabs(input_Pz[index_k_jk]) > 1E-5){
	      theta_new = atan(input_Pt[index_k_jk]/(input_Pz[index_k_jk]));
	    }
	    else {theta_new = M_PI/2;}
	    if(theta_new<0.){theta_new = theta_new + M_PI;}
	    input_Eta_Rap[index_k_jk] = - log(tan(0.5*theta_new));
	  }
	  //if the rapidity is chosen in the delta R quantity
	  if(rap==1){
	    input_Eta_Rap[index_k_jk]=0.5*log((input_E[index_k_jk]+input_Pz[index_k_jk])/(input_E[index_k_jk]-input_Pz[index_k_jk]));
	  }
	  input_Phi[index_k_jk] = atan2( input_Py[index_k_jk],input_Px[index_k_jk]);
	  if(index_j_jk!=rest-1){
	    //overwrite object index_j_jk (which was bigger than index_k_jk)
	    for(unsigned int i=index_j_jk; i<rest-1;i++){
	      input_Pt[i]=input_Pt[i+1];
	      input_Phi[i]=input_Phi[i+1];
	      input_Eta_Rap[i]=input_Eta_Rap[i+1];
	      input_Px[i]=input_Px[i+1];
	      input_Py[i]=input_Py[i+1];
	      input_Pz[i]=input_Pz[i+1];
	      input_E[i]=input_E[i+1];
	    }
	  }
	}
      }
      if(rest==3){
	max_dmin=max_dmin_temp;
      }
      //cout<<"rest "<<rest<<endl;
      rest=rest-1;
    }
  }
  
  double Et2=0;
  Et2= input_Pt[0]+input_Pt[1];
  
  y3=max_dmin/pow(Et2,2);

  return y3;
}

//|||||||||||||||||||||||||||||||||||||||||
//function which calculates the thrusts||||
//|||||||||||||||||||||||||||||||||||||||||

std::vector<double> EventShape::Thrust_calculate (std::vector<double> Input_Px, std::vector<double> Input_Py){
  //cout<<"begin of thrust calculation"<<endl;
  double thrustmax_calc =0;
  double temp_calc =0;
  unsigned int length_thrust_calc =0;
  std::vector<double> ThrustValues;
  std::vector<double> Thrust_Axis_calc;
  std::vector<double> p_thrust_max_calc;
  std::vector<double> p_dec_1_calc;
  std::vector<double> p_dec_2_calc;
  std::vector<double> p_pt_beam_calc;

   if (!ThrustValues.empty()){
     ThrustValues.clear();
     Thrust_Axis_calc.clear();
     p_thrust_max_calc.clear();
     p_dec_1_calc.clear();
     p_dec_2_calc.clear();
     p_pt_beam_calc.clear();
   }

  for(unsigned int j = 0; j < 3; j++){
    p_pt_beam_calc.push_back(0.);
    p_dec_1_calc.push_back(0.);
    p_dec_2_calc.push_back(0.);
    p_thrust_max_calc.push_back(0.);
    Thrust_Axis_calc.push_back(0.);
  }
  
  for(unsigned int j =0;j<4;j++){
    ThrustValues.push_back(0.);
  }
  length_thrust_calc = Input_Px.size();
  if(Input_Py.size()!=length_thrust_calc){
    cout<<"ERROR in thrust calculation!!! Size of Input vectors differs. Change that please!"<<endl;
    return ThrustValues;}

  double Pt_sum_calc =0;

  for(unsigned int k=0;k<length_thrust_calc;k++){
    Pt_sum_calc+=sqrt(pow(Input_Px[k],2)+pow(Input_Py[k],2)); 
    for(unsigned int j = 0; j < 3; j++){
      p_thrust_max_calc[j]=0;
    }
    //get a vector perpendicular to the beam axis and 
    //perpendicular to the momentum of particle k
    //per default beam axis b = (0,0,1)   
    p_pt_beam_calc[0] = Input_Py[k]*1; 
    p_pt_beam_calc[1] = - Input_Px[k]*1;
    p_pt_beam_calc[2] = 0.;
    for(unsigned int i=0;i<length_thrust_calc;i++){
      if(i!=k){
	if((Input_Px[i]*p_pt_beam_calc[0]+Input_Py[i]*p_pt_beam_calc[1])>=0){
	  p_thrust_max_calc[0]= p_thrust_max_calc[0]+Input_Px[i];
	  p_thrust_max_calc[1]= p_thrust_max_calc[1]+Input_Py[i];
	}
	else{
	  p_thrust_max_calc[0]= p_thrust_max_calc[0]-Input_Px[i];
	  p_thrust_max_calc[1]= p_thrust_max_calc[1]-Input_Py[i];
	}
      }
    }
    p_dec_1_calc[0]=p_thrust_max_calc[0]+Input_Px[k];
    p_dec_1_calc[1]=p_thrust_max_calc[1]+Input_Py[k];
    p_dec_1_calc[2]=0;
    p_dec_2_calc[0]=p_thrust_max_calc[0]-Input_Px[k];
    p_dec_2_calc[1]=p_thrust_max_calc[1]-Input_Py[k];
    p_dec_2_calc[2]=0;
    temp_calc = pow(p_dec_1_calc[0],2)+pow(p_dec_1_calc[1],2);
    if(temp_calc>thrustmax_calc){
      thrustmax_calc =temp_calc;
      for(unsigned int i=0;i<3;i++){
	Thrust_Axis_calc[i]=p_dec_1_calc[i]/sqrt(thrustmax_calc);
      }
    }
    temp_calc = pow(p_dec_2_calc[0],2)+pow(p_dec_2_calc[1],2);
    if(temp_calc>thrustmax_calc){
      thrustmax_calc =temp_calc;
      for(unsigned int i=0;i<3;i++){
	Thrust_Axis_calc[i]=p_dec_2_calc[i]/sqrt(thrustmax_calc);
      }
    }
  }
  for(unsigned int j=0;j<3;j++){
    ThrustValues[j]=Thrust_Axis_calc[j];
  }
  //cout<<"thrust axis px/py/pz/phi/prel thrust: "<<Thrust_Axis_calc[0]<<"/"<<Thrust_Axis_calc[1]<<"/"<<Thrust_Axis_calc[2]<<"/"<<atan2(Thrust_Axis_calc[1],Thrust_Axis_calc[0])<<"/"<<sqrt(thrustmax_calc)/Pt_sum_calc<<endl;
  double thrust_calc=0;
  thrust_calc = sqrt(thrustmax_calc)/Pt_sum_calc;

  //the variable which gets resummed is not the thrust
  //but tau=1-thrust
  ThrustValues[3]=1-thrust_calc;
  
  return ThrustValues;
}


