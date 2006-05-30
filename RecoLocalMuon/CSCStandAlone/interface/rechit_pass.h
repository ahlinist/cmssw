//Authors N Ippolito, S Durkin

#ifndef rechitPass_h
#define rechitPass_h

class rechit_pass{
  
 public:
  rechit_pass(){}
  ~rechit_pass(){}
  void rechit_fill(int layer,float x,float y){
    arechit[layer-1].push_back(y);
    srechit[layer-1].push_back(x);
  }
  
  std::vector<float> rechita(int pln){return arechit[pln];}
  std::vector<float> rechitb(int pln){return srechit[pln];}
 private:
  std::vector<float> arechit[6];
  std::vector<float> srechit[6];
};

#endif
