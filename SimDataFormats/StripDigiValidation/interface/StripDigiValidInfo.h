#ifndef SIMDATAFORMATS_SISTRIPDIGIVALIDATION_STRIPDIGITEST_H
#define SIMDATAFORMATS_SISTRIPDIGIVALIDATION_STRIPDIGITEST_H

class StripDigiValidInfo {
public:


  StripDigiValidInfo() 
  : strip_(0), adc_(0), x_(0.0), y_(0.0), z_(0.0) {}

  StripDigiValidInfo( int strip, int adc, float x, float y , float z) 
   : strip_(strip), adc_(adc), x_(x), y_(y), z_(z) {}

  StripDigiValidInfo( short strip, short adc, float x, float y , float z) 
  : strip_(strip), adc_(adc), x_(x), y_(y), z_(z) {}

  int strip() const   {return strip_;}
  int adc() const     {return adc_;}
  float x() const     {return x_;}
  float y() const     {return y_;}
  float z() const     {return z_;}


private:

  short strip_;
  short adc_;
  float x_;
  float y_;
  float z_;
  
};

#endif
