{
gROOT->Reset();
gROOT->SetStyle("Plain");

gStyle->SetOptStat(1111);
gStyle->SetOptFit(111);
   Float_t plmeang_nn[50][73][5][5],plnoise[50][73][5][5];
   Float_t minmeang_nn[50][73][5][5],minnoise[50][73][5][5];
   Float_t excluded_min[50][73][5][5];
   
   FILE *Out1 = fopen("coef_with_noise_check.txt", "w+");  
   FILE *Out2 = fopen("coef_without_noise_10mln_check.txt", "w+");
   
    for(Int_t k=1; k<50; k++) 
    {
    for(Int_t i=1; i<73; i++) 
    {  
    for(Int_t j=1; j<5; j++) 
    {  
    for(Int_t l=1; l<5; l++) 
    {  
      excluded_min[k][i][j][l] = 0.;
    } // l  
    } // j
    } // i
    } // k 
   
   std::string line;
   
   std::ifstream in21( "HB_exclusion.txt" );

   while( std::getline( in21, line)){
   istringstream linestream(line);

   int eta,phi,dep;
   linestream>>dep>>eta>>phi;
    cout<<" Eta="<<eta<<endl;
   if( eta > 0 )
   {
     cout<<" eta > 0 "<<endl;
   } else
   {
     excluded_min[abs(eta)][phi][dep][1] = 1;
   }
  }


   cout<<" Read "<<endl;
   
   std::ifstream in20( "NOISE_TEST/var_noise_2mln.txt" );
   
   while( std::getline( in20, line)){
   istringstream linestream(line);
   Float_t var,err;
   int subd,eta,phi,dep;
   linestream>>subd>>eta>>phi>>dep>>var;
   
   
   if( eta > 0 )
   {
     plnoise[eta][phi][dep][subd] = var;
   } else
   {
      minnoise[abs(eta)][phi][dep][subd] = var;
   }
   
//   if( eta == 1) cout<<"Noise subd "<<subd<<" "<<eta<<" "<<phi<<" "<<dep<<" "<<plnoise[eta][phi][dep][subd]<<endl;   
      
   }
   cout<<" End of noise read "<<endl;
   
   std::ifstream in21( "MINBIAS_TEST/var_minbias_10mln_check.txt" );
   
   while( std::getline( in21, line)){
   istringstream linestream(line);
   Float_t var,err;
   int subd,eta,phi,dep;
   linestream>>subd>>eta>>phi>>dep>>var>>err;
   if( eta > 0 )
   {
     plmeang_nn[eta][phi][dep][subd] = var;
   } else
   {
      minmeang_nn[abs(eta)][phi][dep][subd] = var;
   }

   if(subd == 1 && eta == 1) cout<<"Minbias subd "<<subd<<" "<<eta<<" "<<phi<<" "<<dep<<" "<<plmeang_nn[eta][phi][dep][subd]<<endl;   
   if( subd == 1 && eta == -1) cout<<"Minbias subd "<<subd<<" "<<eta<<" "<<phi<<" "<<dep<<" "<<minmeang_nn[abs(eta)][phi][dep][subd]<<endl;   
   }
// 
// Choose depth
//   
//   Int_t idep = 1;
   
   Float_t plmean1[50][5][5]; 
   Float_t minmean1[50][5][5];
   Float_t plmean1_nn[50][5][5]; 
   Float_t minmean1_nn[50][5][5];
   Int_t numchan[50][5][5];
   
   for(Int_t k=1; k<5; k++) 
   {
   
    for(Int_t idep0=1; idep0<5; idep0++) 
   {  
    for(Int_t i=1; i<50; i++) {
   
//    Int_t nch1 = 72.;
//    if( i > 20 ) nch1 = 36.; 
    
     plmean1[i][idep0][k] = 0.; 
     minmean1[i][idep0][k] = 0.;
     plmean1_nn[i][idep0][k] = 0.; 
     minmean1_nn[i][idep0][k] = 0.;

     Int_t nch1 = 0; 
     Int_t nchp = 0; 
     Int_t nchm = 0;
       
     for(Int_t j=1; j<73; j++){
      if(minnoise[i][j][idep0][k]>0.) nch1++;
      plmean1[i][idep0][k] = plmean1[i][idep0][k] + plmeang_nn[i][j][idep0][k];
      minmean1[i][idep0][k] = minmean1[i][idep0][k] + minmeang_nn[i][j][idep0][k];
      plmean1_nn[i][idep0][k] = plmean1_nn[i][idep0][k] + plmeang_nn[i][j][idep0][k] - plnoise[i][j][idep0][k];
      minmean1_nn[i][idep0][k] = minmean1_nn[i][idep0][k] + minmeang_nn[i][j][idep0][k] - minnoise[i][j][idep0][k];
      if(plnoise[i][j][idep0][k]>0.) nchp++;
      if(minnoise[i][j][idep0][k]>0.) nchm++;
      } // j   
  
    numchan[i][idep0][k] = nch1;    
    if(nch1 == 0) continue;
    //cout<<" i, idep0, k "<<i<<" "<<idep0<<" "<<k<<" "<<nch1<<" "<<nchp<<" "<<nchm<<endl;
          
    plmean1[i][idep0][k] = plmean1[i][idep0][k]/nchp;
    minmean1[i][idep0][k] = minmean1[i][idep0][k]/nchm;
    plmean1_nn[i][idep0][k] = plmean1_nn[i][idep0][k]/nchp;
    minmean1_nn[i][idep0][k] = minmean1_nn[i][idep0][k]/nchm;
    cout<<" i, idep0, k "<<i<<" "<<idep0<<" "<<k<<" "<<nch1<<" "<<nchp<<" "<<nchm<<" "<<plmean1_nn[i][idep0][k]<<" "<<
    minmean1_nn[i][idep0][k]<<endl;
//    cout<<" k, idep0, i, nch1= "<<k<<" "<<idep0<<" "<<i<<" "<<nch1<<
//    " "<<plmeang_nn[i][j][idep0][k]<<" "<<plnoise[i][j][idep0][k]<<endl;
    // Do not calibrate HO
    Float_t err = 0.00001;
    if( k != 3 ) {
    for(Int_t j=1;j<73;j++) {
       if( plnoise[i][j][idep0][k] > 0) {       
       Float_t tt0, tt1; 
       if( i != 16 && i != 15 ) {
         tt0 = sqrt(plmean1_nn[i][idep0][k]/(plmeang_nn[i][j][idep0][k] - plnoise[i][j][idep0][k]));
         tt1 = sqrt(plmean1[i][idep0][k]/plmeang_nn[i][j][idep0][k]);
       } else {       
	                  tt0 = 1.; tt1 = 1.;
       }
       fprintf(Out2,"%d %d %d %d %.5f %.5f\n",k,idep0,i,j,tt0,err);
       fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,i,j,tt1,err);       
       } // plnoise
    } // j
    cout<<" Here "<<endl;
    for(Int_t j=1;j<73;j++) {   
       if( minnoise[i][j][idep0][k] > 0 ) {       
       Float_t tt0, tt1;        
       if( i != 16 && i != 15) {
       tt0 = sqrt(minmean1_nn[i][idep0][k]/(minmeang_nn[i][j][idep0][k] - minnoise[i][j][idep0][k]));
       tt1 = sqrt(minmean1[i][idep0][k]/minmeang_nn[i][j][idep0][k]);
       if(k==1 && i==1) cout<<" tt0 "<<tt0<<" "<<minmeang_nn[i][j][idep0][k]<<" "<<minnoise[i][j][idep0][k]<<endl;
        } else {       
	 tt0 = 1.; tt1 = 1.;
       }      
       Int_t ieta = -1*i;
       fprintf(Out2,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt0,err);
       fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt1,err);
       } // minnoise
     } // j
    } else { 
        for(Int_t j=1;j<73;j++){   
           Float_t tt0=1.; 
           Float_t tt1=1.; 
           Int_t ieta = i;
           fprintf(Out2,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt0,err);
           fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt1,err);
        }// j
         
        for(Int_t j=1;j<73;j++){   
           Float_t tt0=1.; 
           Float_t tt1=1.; 
           Int_t ieta = -1*i;
           fprintf(Out2,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt0,err);
           fprintf(Out1,"%d %d %d %d %.5f %.5f\n",k,idep0,ieta,j,tt1,err);
        }// j
     } // k = 3
      cout<<" End "<<endl;
   } // i
   } // idep0
   } // k   
    
   fclose(Out1);
   fclose(Out2);     
}
