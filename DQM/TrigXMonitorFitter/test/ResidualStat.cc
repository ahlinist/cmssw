#include <math.h>
#include "ResidualStat.h"



//
//Add
void ResidualStat::Add(double residual, int threshHold)
{
  ++TLI_sumN;                        //'TLI_' is simply a constant prefix, no real meaning.
  TLI_sumS  += fabs(residual);       //abs of residual
  TLI_sumS2 += (residual * residual);//square of the residual (always positive)
  
  if ( (residual >= threshHold) || (residual <= (-1*threshHold)) )
    {
      ++TLI_sumBad;
    } // if bad residual
}


//
//Calculate Mean
Double_t ResidualStat::CalculateMean()
{
  if (TLI_sumN)
    {
      return ( TLI_sumS / TLI_sumN );
    }
  else
    {
      return 0.; 
    }
}


//
//Calculate the Root Mean Square
Double_t ResidualStat::CalculateRMS()
{
  if (TLI_sumN)
    {
      return ( sqrt( TLI_sumS2  / TLI_sumN ) );
    }
  else
    {
      return 0.;
    }
}


//
//Calculate the Standard Deviation
Double_t ResidualStat::CalculateStdDev()
{
  if (TLI_sumN < 2)
    {
      return 0.;
    } 
  else
    {
      return ( ( TLI_sumS2 - ( TLI_sumS * TLI_sumS / TLI_sumN ) ) / (TLI_sumN - 1 ) );
    }
}


//
//Calculate the Standard Deviation of Mean
Double_t ResidualStat::CalculateSDOM()
{
  if (TLI_sumN < 2)
    {
      return 0.;
    }
  else
    {
      return (  ((TLI_sumS2 - (TLI_sumS * TLI_sumS / TLI_sumN))/(TLI_sumN - 1 )) / sqrt(TLI_sumN) );
    }
}

      
