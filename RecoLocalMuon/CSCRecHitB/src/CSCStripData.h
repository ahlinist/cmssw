#ifndef CSCRecHitB_CSCStripData_h
#define CSCRecHitB_CSCStripData_h

/** \class CSCStripData
 *
 * Hold strip data while building strip hits.
 *
 * \author Dominique Fortin
 */	

class CSCStripData
{	
 public:	
  /* The default ctor initializes all elements of thePulseHeightMap for
   * which explicit digis do not exist.  Thus the y's must be 0
   * Use sentinel value for x and t. 
   */
  CSCStripData() : x_(-1.), y0_( 0. ), y_( 0. ), y2_( 0. ), y3_( 0. ), t_(-1) {};
  CSCStripData(    float x, float y0,  float y,  float y2,  float y3,  int t) : 
                     x_(x),  y0_(y0),    y_(y),   y2_(y2),   y3_(y3),   t_(t) {};
  
  float x()   const {return x_;}
  float y0()  const {return y0_;}
  float y()   const {return y_;}
  float y2()  const {return y2_;}
  float y3()  const {return y3_;}
  int   t()   const {return t_;}
  bool operator<( const CSCStripData & data ) const { return y_ < data.y_; }
  
  void operator+=( float* addend) {
    y0_ += addend[0];
    y_  += addend[1];
    y2_ += addend[2];
    y3_ += addend[3];
  }

  void operator*=( float addend) {
    y0_ *= addend;
    y_  *= addend;
    y2_ *= addend;
    y3_ *= addend;
  }

  
 private:
  
  float x_;
  float y0_;
  float y_;
  float y2_;
  float y3_;
  int   t_;
  
};

#endif

