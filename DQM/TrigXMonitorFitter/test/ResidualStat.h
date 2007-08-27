

#if !defined(Sum_H)
 #define Sum_H

typedef int Int_t;
typedef double Double_t;

class ResidualStat
 {
  
 public:
   ResidualStat(): TLI_sumN(0), TLI_sumS(0.), TLI_sumS2(0.), TLI_sumBad(0){}  
     
     Int_t    sumN()   { return TLI_sumN; }
     Double_t sumS()   { return TLI_sumS; }
     Double_t sumS2()  { return TLI_sumS2; }
     Int_t    sumBad() { return TLI_sumBad; }
     
     
     //void addSumN()                  { ++TLI_sumN; }           
     //void addSumS(const Double_t dS) { TLI_sumS += dS; }
     //void addSumS2(const doule dS2)  { TLI_sumS2 += dS2; }
     //void addSumBad()                { ++TLI_sumBad; }
     
     void Add(Double_t resid, Int_t threshHold = 1);         
       Double_t CalculateMean();
       Double_t CalculateRMS();
       Double_t CalculateStdDev();
       Double_t CalculateSDOM();

       
 private:
       Int_t TLI_sumN;
       Double_t TLI_sumS;              
       Double_t TLI_sumS2;
       Int_t TLI_sumBad;   
 };

#endif // Sum_H
