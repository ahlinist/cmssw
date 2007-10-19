#include <RecoLocalMuon/CSCRecHitB/src/Chi2Probab.h>

extern double Chi2Probab(int& ndof, double& chisq){

//constants
  static double srtopi=2.0/sqrt(2.0*M_PI);
  static double upl=100.0;

  double prob=0.0;
  if ( ndof<=0 ) { return prob; }
  if ( chisq<0.0 ) { return prob; }

  if( ndof <= 60 ) {
  //full treatment
    if ( chisq>upl ) { return prob; }
    double sum = exp(-0.5*chisq);
    double term = sum;

    int m = ndof/2;
    // If ndof is even
    if ( 2*m == ndof ){
      if ( m==1 ) { return sum;}
      for (int i=2; i<=m;++i ){
        term = 0.5*term*chisq/(i-1);
        sum += term;
      }
      return sum;

    } else {
    // if ndof is odd

      double srty = sqrt(chisq);
      double temp = srty/M_SQRT2;
      prob = erfc(temp);
      if ( ndof==1 ) { return prob; }
      if ( ndof==3 ) { return (srtopi*srty*sum+prob); }
      m--;
      for (int i=1; i<=m; ++i ){
        term = term*chisq/(2*i+1);
        sum += term;
      }
      return (srtopi*srty*sum+prob);
    }

  } else {
  //asymtotic Gaussian approx
    double srty = sqrt(chisq)-sqrt(ndof-0.5);
    if ( srty<12.0 ) prob=0.5*erfc(srty);
    return prob;
  }

 return prob;
}
