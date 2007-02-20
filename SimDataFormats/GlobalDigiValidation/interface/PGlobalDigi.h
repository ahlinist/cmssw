#ifndef PGlobalDigi_h
#define PGlobalDigi_h

#include <vector>
#include <memory>

class PGlobalDigi
{
 public:

  PGlobalDigi(): nEBCalDigis(0), nEECalDigis(0), nESCalDigis(0),
    nHBCalDigis(0), nHECalDigis(0), nHOCalDigis(0), nHFCalDigis(0),
    nTIBL1Digis(0), nTIBL2Digis(0), nTIBL3Digis(0), nTIBL4Digis(0),
    nTOBL1Digis(0), nTOBL2Digis(0), nTOBL3Digis(0), nTOBL4Digis(0),
    nTIDW1Digis(0), nTIDW2Digis(0), nTIDW3Digis(0),
    nTECW1Digis(0), nTECW2Digis(0), nTECW3Digis(0), nTECW4Digis(0), 
    nTECW5Digis(0), nTECW6Digis(0), nTECW7Digis(0), nTECW8Digis(0),
    nBRL1Digis(0), nBRL2Digis(0), nBRL3Digis(0), 
    nFWD1pDigis(0), nFWD1nDigis(0), nFWD2pDigis(0), nFWD2nDigis(0),
    nMB1Digis(0), nMB2Digis(0), nMB3Digis(0), nMB4Digis(0),
    nCSCstripDigis(0), nCSCwireDigis(0) {}
  virtual ~PGlobalDigi(){}

  ////////////
  // ECal Info
  ////////////
  struct ECalDigi
  {
    ECalDigi(): maxPos(0), AEE(0), SHE(0) {}
    int maxPos;
    double AEE; //maximum analog equivalent energy
    float SHE; //simhit energy sum
  };
  typedef std::vector<ECalDigi> ECalDigiVector;
  struct ESCalDigi
  {
    ESCalDigi(): ADC0(0), ADC1(0), ADC2(0), SHE(0) {}
    float ADC0, ADC1, ADC2; //ADC counts
    float SHE; //sum simhit energy    
  };
  typedef std::vector<ESCalDigi> ESCalDigiVector;
  //put functions
  void putEBCalDigis(std::vector<int> maxpos,
		     std::vector<double> aee, std::vector<float> she);
  void putEECalDigis(std::vector<int> maxpos,
		     std::vector<double> aee, std::vector<float> she);
  void putESCalDigis(std::vector<float> adc0, std::vector<float> adc1,
		     std::vector<float> adc2, std::vector<float> she);
  //get functions
  int getnEBCalDigis() {return nEBCalDigis;}  
  int getnEECalDigis() {return nEECalDigis;}
  int getnESCalDigis() {return nESCalDigis;}  
  ECalDigiVector getEBCalDigis() {return EBCalDigis;}  
  ECalDigiVector getEECalDigis() {return EECalDigis;}
  ESCalDigiVector getESCalDigis() {return ESCalDigis;}  

  ////////////
  // HCal Info
  ////////////
  struct HCalDigi
  {
    HCalDigi(): AEE(0), SHE(0) {}
    float AEE; //sum analog equivalent energy in fC
    float SHE; //simhit energy sum
  };
  typedef std::vector<HCalDigi> HCalDigiVector;
  //put functions
  void putHBCalDigis(std::vector<float> aee, std::vector<float> she);
  void putHECalDigis(std::vector<float> aee, std::vector<float> she);
  void putHOCalDigis(std::vector<float> aee, std::vector<float> she);
  void putHFCalDigis(std::vector<float> aee, std::vector<float> she);
  //get functions
  int getnHBCalDigis() {return nHBCalDigis;}  
  int getnHECalDigis() {return nHECalDigis;}  
  int getnHOCalDigis() {return nHOCalDigis;}  
  int getnHFCalDigis() {return nHFCalDigis;}  
  HCalDigiVector getHBCalDigis() {return HBCalDigis;}  
  HCalDigiVector getHECalDigis() {return HECalDigis;}  
  HCalDigiVector getHOCalDigis() {return HOCalDigis;}  
  HCalDigiVector getHFCalDigis() {return HFCalDigis;}  

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  ///////////////
  // SiStrip info
  ///////////////
  struct SiStripDigi
  {
    SiStripDigi(): ADC(0), STRIP(0) {}
    float ADC; //adc value
    int STRIP; //strip number
  };
  typedef std::vector<SiStripDigi> SiStripDigiVector;
  //put functions
  void putTIBL1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIBL2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIBL3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIBL4Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL4Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIDW1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIDW2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIDW3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW4Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW5Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW6Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW7Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW8Digis(std::vector<float> adc, std::vector<int> strip);
  //get functions
  int getnTIBL1Digis() {return nTIBL1Digis;}  
  int getnTIBL2Digis() {return nTIBL2Digis;}  
  int getnTIBL3Digis() {return nTIBL3Digis;}  
  int getnTIBL4Digis() {return nTIBL4Digis;}  
  int getnTOBL1Digis() {return nTOBL1Digis;}  
  int getnTOBL2Digis() {return nTOBL2Digis;}  
  int getnTOBL3Digis() {return nTOBL3Digis;}  
  int getnTOBL4Digis() {return nTOBL4Digis;}
  int getnTIDW1Digis() {return nTIDW1Digis;}
  int getnTIDW2Digis() {return nTIDW2Digis;}
  int getnTIDW3Digis() {return nTIDW3Digis;} 
  int getnTECW1Digis() {return nTECW1Digis;}
  int getnTECW2Digis() {return nTECW2Digis;}
  int getnTECW3Digis() {return nTECW3Digis;}
  int getnTECW4Digis() {return nTECW4Digis;}
  int getnTECW5Digis() {return nTECW5Digis;}
  int getnTECW6Digis() {return nTECW6Digis;}
  int getnTECW7Digis() {return nTECW7Digis;}
  int getnTECW8Digis() {return nTECW8Digis;} 
  SiStripDigiVector getTIBL1Digis() {return TIBL1Digis;}  
  SiStripDigiVector getTIBL2Digis() {return TIBL2Digis;}  
  SiStripDigiVector getTIBL3Digis() {return TIBL3Digis;}  
  SiStripDigiVector getTIBL4Digis() {return TIBL4Digis;}
  SiStripDigiVector getTOBL1Digis() {return TOBL1Digis;}  
  SiStripDigiVector getTOBL2Digis() {return TOBL2Digis;}  
  SiStripDigiVector getTOBL3Digis() {return TOBL3Digis;}  
  SiStripDigiVector getTOBL4Digis() {return TOBL4Digis;}   
  SiStripDigiVector getTIDW1Digis() {return TIDW1Digis;}
  SiStripDigiVector getTIDW2Digis() {return TIDW2Digis;}
  SiStripDigiVector getTIDW3Digis() {return TIDW3Digis;} 
  SiStripDigiVector getTECW1Digis() {return TECW1Digis;}
  SiStripDigiVector getTECW2Digis() {return TECW2Digis;}
  SiStripDigiVector getTECW3Digis() {return TECW3Digis;}
  SiStripDigiVector getTECW4Digis() {return TECW4Digis;}
  SiStripDigiVector getTECW5Digis() {return TECW5Digis;}
  SiStripDigiVector getTECW6Digis() {return TECW6Digis;}
  SiStripDigiVector getTECW7Digis() {return TECW7Digis;}
  SiStripDigiVector getTECW8Digis() {return TECW8Digis;}

  ///////////////
  // SiPixel info
  ///////////////
  struct SiPixelDigi
  {
    SiPixelDigi(): ADC(0), ROW(0), COLUMN(0) {}
    float ADC; //adc value
    int ROW; //row number
    int COLUMN; //column number
  };
  typedef std::vector<SiPixelDigi> SiPixelDigiVector;
  //put functions
  void putBRL1Digis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putBRL2Digis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putBRL3Digis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD1pDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD1nDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD2pDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD2nDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  //get functions
  int getnBRL1Digis() {return nBRL1Digis;}  
  int getnBRL2Digis() {return nBRL2Digis;}  
  int getnBRL3Digis() {return nBRL3Digis;}
  int getnFWD1pDigis() {return nFWD1pDigis;}  
  int getnFWD1nDigis() {return nFWD1nDigis;}    
  int getnFWD2pDigis() {return nFWD2pDigis;}  
  int getnFWD2nDigis() {return nFWD2nDigis;}  
  SiPixelDigiVector getBRL1Digis() {return BRL1Digis;}  
  SiPixelDigiVector getBRL2Digis() {return BRL2Digis;}  
  SiPixelDigiVector getBRL3Digis() {return BRL3Digis;}  
  SiPixelDigiVector getFWD1pDigis() {return FWD1pDigis;}
  SiPixelDigiVector getFWD1nDigis() {return FWD1nDigis;} 
  SiPixelDigiVector getFWD2pDigis() {return FWD2pDigis;}
  SiPixelDigiVector getFWD2nDigis() {return FWD2nDigis;} 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  struct DTDigi
  {
    DTDigi(): WIRE(0), TIME(0) {}
    int WIRE; //layer (wire) number
    float TIME; //time of hit
  };
  typedef std::vector<DTDigi> DTDigiVector;
  //put functions
  void putMB1Digis(std::vector<int> wire, std::vector<float> time);
  void putMB2Digis(std::vector<int> wire, std::vector<float> time);
  void putMB3Digis(std::vector<int> wire, std::vector<float> time);
  void putMB4Digis(std::vector<int> wire, std::vector<float> time);
  //get functions
  int getnMB1Digis() {return nMB1Digis;}  
  int getnMB2Digis() {return nMB2Digis;}  
  int getnMB3Digis() {return nMB3Digis;}  
  int getnMB4Digis() {return nMB4Digis;}  
  DTDigiVector getMB1Digis() {return MB1Digis;}  
  DTDigiVector getMB2Digis() {return MB2Digis;}  
  DTDigiVector getMB3Digis() {return MB3Digis;}  
  DTDigiVector getMB4Digis() {return MB4Digis;}  

  /////////////////
  // CSC Strip info
  /////////////////
  struct CSCstripDigi
  {
    CSCstripDigi(): ADC(0) {}
    float ADC; //ped subtracted amplitude
  };
  typedef std::vector<CSCstripDigi> CSCstripDigiVector;
  //put functions
  void putCSCstripDigis(std::vector<float> adc);
  //get functions
  int getnCSCstripDigis() {return nCSCstripDigis;}  
  CSCstripDigiVector getCSCstripDigis() {return CSCstripDigis;}  

  /////////////////
  // CSC Wire info
  /////////////////
  struct CSCwireDigi
  {
    CSCwireDigi(): TIME(0) {}
    float TIME; //time
  };
  typedef std::vector<CSCwireDigi> CSCwireDigiVector;
  //put functions
  void putCSCwireDigis(std::vector<float> time);
  //get functions
  int getnCSCwireDigis() {return nCSCwireDigis;}  
  CSCwireDigiVector getCSCwireDigis() {return CSCwireDigis;} 

 private:

  ////////////
  // ECal info
  ////////////
  int nEBCalDigis;
  ECalDigiVector EBCalDigis;
  int nEECalDigis;
  ECalDigiVector EECalDigis;
  int nESCalDigis;
  ESCalDigiVector ESCalDigis;

  ////////////
  // HCal info
  ////////////
  int nHBCalDigis;
  HCalDigiVector HBCalDigis;
  int nHECalDigis;
  HCalDigiVector HECalDigis;
  int nHOCalDigis;
  HCalDigiVector HOCalDigis;
  int nHFCalDigis;
  HCalDigiVector HFCalDigis;

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  //////////////
  //SiStrip info
  //////////////
  int nTIBL1Digis;  
  SiStripDigiVector TIBL1Digis;
  int nTIBL2Digis;  
  SiStripDigiVector TIBL2Digis;
  int nTIBL3Digis; 
  SiStripDigiVector TIBL3Digis;
  int nTIBL4Digis;  
  SiStripDigiVector TIBL4Digis;
  int nTOBL1Digis;
  SiStripDigiVector TOBL1Digis;
  int nTOBL2Digis;  
  SiStripDigiVector TOBL2Digis;
  int nTOBL3Digis;  
  SiStripDigiVector TOBL3Digis;
  int nTOBL4Digis; 
  SiStripDigiVector TOBL4Digis;
  int nTIDW1Digis;   
  SiStripDigiVector TIDW1Digis;
  int nTIDW2Digis;
  SiStripDigiVector TIDW2Digis;
  int nTIDW3Digis;
  SiStripDigiVector TIDW3Digis; 
  int nTECW1Digis;
  SiStripDigiVector TECW1Digis;
  int nTECW2Digis;
  SiStripDigiVector TECW2Digis;
  int nTECW3Digis;
  SiStripDigiVector TECW3Digis;
  int nTECW4Digis;
  SiStripDigiVector TECW4Digis;
  int nTECW5Digis;
  SiStripDigiVector TECW5Digis;
  int nTECW6Digis;
  SiStripDigiVector TECW6Digis;
  int nTECW7Digis;
  SiStripDigiVector TECW7Digis;
  int nTECW8Digis;
  SiStripDigiVector TECW8Digis;

  //////////////
  //SiPixel info
  //////////////
  int nBRL1Digis;
  SiPixelDigiVector BRL1Digis;
  int nBRL2Digis;  
  SiPixelDigiVector BRL2Digis; 
  int nBRL3Digis; 
  SiPixelDigiVector BRL3Digis; 
  int nFWD1pDigis; 
  SiPixelDigiVector FWD1pDigis;
  int nFWD1nDigis;
  SiPixelDigiVector FWD1nDigis; 
  int nFWD2pDigis;
  SiPixelDigiVector FWD2pDigis;
  int nFWD2nDigis;
  SiPixelDigiVector FWD2nDigis; 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  int nMB1Digis;
  DTDigiVector MB1Digis; 
  int nMB2Digis;
  DTDigiVector MB2Digis; 
  int nMB3Digis;
  DTDigiVector MB3Digis; 
  int nMB4Digis; 
  DTDigiVector MB4Digis; 

  /////////////////
  // CSC Strip info
  ////////////////
  int nCSCstripDigis;
  CSCstripDigiVector CSCstripDigis;

  /////////////////
  // CSC Wire info
  ////////////////
  int nCSCwireDigis;
  CSCwireDigiVector CSCwireDigis;
 
}; // end class declaration

#endif //PGlobalDigiHit_h
