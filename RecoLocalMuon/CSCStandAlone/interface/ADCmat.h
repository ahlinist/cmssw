//Authors: S Durkin

class ADCmat{

 public:

   ADCmat(float *adc){
    for(int i=0;i<12;i++)adcl[i]=adc[i];
    pass=adcl;
  }
  ~ADCmat(){}
  float * array(){pass=adcl;return pass;}

 private:
  
  float adcl[12];
  float *pass;
};

