
/// ////////////////////////////////////////
/// Written by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, September                      ///
/// ////////////////////////////////////////

#define TRACE //std::cout<<__FILE__<<" : "<<__LINE__<<std::endl;

#ifndef LINE_FIT_H
#define LINE_FIT_H

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

	struct Line{
		double angularcoeff;
		double phi0;
		unsigned int n;
		double chi2;
	};


class LineFit {

	public:
		LineFit(){ mData.clear(); }
		LineFit( const std::vector< GlobalPoint > &aData ) : mData(aData) {}
		~LineFit(){}

		void clear()
		{
			mData.clear();
		}

		void push_back( const GlobalPoint& a )
		{
			mData.push_back( a );
		}

    //Line modifiedLeastSquaresFit() {
		std::vector<double> modifiedLeastSquaresFit() const {
TRACE
			//std::cout << __FILE__ << " : " << __LINE__ << std::endl;
			Line line;
			line.phi0=0.0;
			line.angularcoeff=0.0;
			line.n = mData.size();
			line.chi2=0.0;

			if (line.n< 3) //return line;
      {
        std::vector<double> out;
        out.push_back(line.angularcoeff);
        out.push_back(line.phi0);
        out.push_back(line.chi2);
        return out;
      }

      // phi = a + b * z
      double XY = 0;
      double XX = 0;
      double X = 0;
      double Y = 0;
      for (unsigned int j=0; j<line.n; j++) {
        double xx = mData.at(j).z();
        double yy = mData.at(j).phi();
        XY += (xx*yy);
        XX += (xx*xx);
        X += xx;
        Y += yy;      
      }
      double b = ( XY - X*Y/(double)line.n ) / ( XX - X*X/(double)line.n );
      double a = Y/(double)line.n - b * X/(double)line.n;

      // We assume in the Tracker b is always != 0
      line.angularcoeff = b;
      line.phi0 = a;

      double chi2 = 0;
      for (unsigned int j=0; j<line.n; j++) {
        double chi = 0;
        chi += mData.at(j).perp();
        chi -= a;
        chi -= ( b * mData.at(j).z() );
        chi2 += chi*chi;
      }

      line.chi2 = chi2;

      //return line;
      std::vector<double> out;
      out.push_back(line.angularcoeff); //0
      out.push_back(line.phi0);         //1
      out.push_back(line.chi2);         //2
      return out;
		}

	private:
		std::vector< GlobalPoint > mData;
};
#endif


