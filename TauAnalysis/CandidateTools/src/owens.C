# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <cmath>
# include <ctime>

using namespace std;

# include "TauAnalysis/CandidateTools/interface/owens.h"

//****************************************************************************80

void bivariate_normal_cdf_values ( int *n_data, double *x, double *y,
  double *r, double *fxy )

//****************************************************************************80
//
//  Purpose:
//
//    BIVARIATE_NORMAL_CDF_VALUES returns some values of the bivariate normal CDF.
//
//  Discussion:
//
//    FXY is the probability that two variables A and B, which are
//    related by a bivariate normal distribution with correlation R,
//    respectively satisfy A <= X and B <= Y.
//
//    Mathematica can evaluate the bivariate normal CDF via the commands:
//
//      <<MultivariateStatistics`
//      cdf = CDF[MultinormalDistribution[{0,0}{{1,r},{r,1}}],{x,y}]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 May 2009
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    National Bureau of Standards,
//    Tables of the Bivariate Normal Distribution and Related Functions,
//    NBS, Applied Mathematics Series, Number 50, 1959.
//
//  Parameters:
//
//    Input/output, int *N_DATA.  The user sets N_DATA to 0 before the
//    first call.  On each call, the routine increments N_DATA by 1, and
//    returns the corresponding data; when there is no more data, the
//    output value of N_DATA will be 0 again.
//
//    Output, double *X, *Y, the parameters of the function.
//
//    Output, double *R, the correlation value.
//
//    Output, double *FXY, the value of the function.
//
{
#  define N_MAX 27

  double fxy_vec[N_MAX] = {
  0.02260327218569867E+00,
  0.1548729518584100E+00,
  0.4687428083352184E+00,
  0.7452035868929476E+00,
  0.8318608306874188E+00,
  0.8410314261134202E+00,
  0.1377019384919464E+00,
  0.1621749501739030E+00,
  0.1827411243233119E+00,
  0.2010067421506235E+00,
  0.2177751155265290E+00,
  0.2335088436446962E+00,
  0.2485057781834286E+00,
  0.2629747825154868E+00,
  0.2770729823404738E+00,
  0.2909261168683812E+00,
  0.3046406378726738E+00,
  0.3183113449213638E+00,
  0.3320262544108028E+00,
  0.3458686754647614E+00,
  0.3599150462310668E+00,
  0.3742210899871168E+00,
  0.3887706405282320E+00,
  0.4032765198361344E+00,
  0.4162100291953678E+00,
  0.6508271498838664E+00,
  0.8318608306874188E+00 };
  double r_vec[N_MAX] = {
     0.5,  0.5,  0.5,  0.5,  0.5,
     0.5, -0.9, -0.8, -0.7, -0.6,
    -0.5, -0.4, -0.3, -0.2, -0.1,
     0.0,  0.1,  0.2,  0.3,  0.4,
     0.5,  0.6,  0.7,  0.8,  0.9,
     0.673,  0.5 };
  double x_vec[N_MAX] = {
    -2.0, -1.0,  0.0,  1.0,  2.0,
     3.0, -0.2, -0.2, -0.2, -0.2,
    -0.2, -0.2, -0.2, -0.2, -0.2,
    -0.2, -0.2, -0.2, -0.2, -0.2,
    -0.2, -0.2, -0.2, -0.2, -0.2,
     1.0,  2.0 };
  double y_vec[N_MAX] = {
     1.0,  1.0,  1.0,  1.0,  1.0,
     1.0,  0.5,  0.5,  0.5,  0.5,
     0.5,  0.5,  0.5,  0.5,  0.5,
     0.5,  0.5,  0.5,  0.5,  0.5,
     0.5,  0.5,  0.5,  0.5,  0.5,
     0.5,  1.0 };

  if ( *n_data < 0 )
  {
    *n_data = 0;
  }

  *n_data = *n_data + 1;

  if ( N_MAX < *n_data )
  {
    *n_data = 0;
    *r = 0.0;
    *x = 0.0;
    *y = 0.0;
    *fxy = 0.0;
  }
  else
  {
    *r = r_vec[*n_data-1];
    *x = x_vec[*n_data-1];
    *y = y_vec[*n_data-1];
    *fxy = fxy_vec[*n_data-1];
  }

  return;
# undef N_MAX
}
//****************************************************************************80

double bivprb ( double h, double k, double r )

//****************************************************************************80
//
//  Purpose:
//
//    BIVPRB computes a bivariate normal CDF for correlated X and Y.
//
//  Discussion:
//
//    This routine computes P( H < X, K < Y ) for X and Y standard normal
//    variables with correlation R.
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    Original version by Mike Patefield, David Tandy.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Mike Patefield, David Tandy,
//    Fast and Accurate Calculation of Owen's T Function,
//    Journal of Statistical Software,
//    Volume 5, Number 5, 2000, pages 1-25.
//
//  Parameters:
//
//    Input, double H, K, the lower limits for X and Y.
//    0 <= H, 0 <= K.
//
//    Input, double R, the correlation between X and Y.
//
//    Output, double BIVPRB, the requested probability.
//
{
  double ri;
  double rr;
  double rroot2 = 0.70710678118654752440;
  double rtwopi = 0.15915494309189533577;
  double value;

  if ( r == 0.0 )
  {
    value = znorm2 ( h ) * znorm2 ( k );
  }
  else
  {
    rr = 1.0 - r * r;

    if ( 0.0 < rr )
    {
      ri = 1.0 / sqrt ( rr );

      if ( 0.0 < h && 0.0 < k )
      {
        value = q ( h, ( k / h - r ) * ri ) + q ( k, ( h / k - r ) * ri );
      }
      else if ( 0.0 < h )
      {
        value = q ( h, - r * ri );
      }
      else if ( 0.0 < k )
      {
        value = q ( k, - r * ri );
      }
      else
      {
        value = 0.25 + rtwopi * asin ( r );
      }
    }
    else if ( 1.0 <= r )
    {
      if ( k <= h )
      {
         value = znorm2 ( h );
      }
      else
      {
        value = znorm2 ( k );
      }
    }
    else
    {
      value = 0.0;
    }
  }

  return value;
}
//****************************************************************************80

void normal_01_cdf_values ( int *n_data, double *x, double *fx )

//****************************************************************************80
//
//  Purpose:
//
//    NORMAL_01_CDF_VALUES returns some values of the Normal 01 CDF.
//
//  Discussion:
//
//    In Mathematica, the function can be evaluated by:
//
//      Needs["Statistics`ContinuousDistributions`"]
//      dist = NormalDistribution [ 0, 1 ]
//      CDF [ dist, x ]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    28 August 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Milton Abramowitz, Irene Stegun,
//    Handbook of Mathematical Functions,
//    National Bureau of Standards, 1964,
//    ISBN: 0-486-61272-4,
//    LC: QA47.A34.
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//  Parameters:
//
//    Input/output, int *N_DATA.  The user sets N_DATA to 0 before the
//    first call.  On each call, the routine increments N_DATA by 1, and
//    returns the corresponding data; when there is no more data, the
//    output value of N_DATA will be 0 again.
//
//    Output, double *X, the argument of the function.
//
//    Output, double *FX, the value of the function.
//
{
# define N_MAX 17

  double fx_vec[N_MAX] = {
     0.5000000000000000E+00,
     0.5398278372770290E+00,
     0.5792597094391030E+00,
     0.6179114221889526E+00,
     0.6554217416103242E+00,
     0.6914624612740131E+00,
     0.7257468822499270E+00,
     0.7580363477769270E+00,
     0.7881446014166033E+00,
     0.8159398746532405E+00,
     0.8413447460685429E+00,
     0.9331927987311419E+00,
     0.9772498680518208E+00,
     0.9937903346742239E+00,
     0.9986501019683699E+00,
     0.9997673709209645E+00,
     0.9999683287581669E+00 };

  double x_vec[N_MAX] = {
     0.0000000000000000E+00,
     0.1000000000000000E+00,
     0.2000000000000000E+00,
     0.3000000000000000E+00,
     0.4000000000000000E+00,
     0.5000000000000000E+00,
     0.6000000000000000E+00,
     0.7000000000000000E+00,
     0.8000000000000000E+00,
     0.9000000000000000E+00,
     0.1000000000000000E+01,
     0.1500000000000000E+01,
     0.2000000000000000E+01,
     0.2500000000000000E+01,
     0.3000000000000000E+01,
     0.3500000000000000E+01,
     0.4000000000000000E+01 };

  if ( *n_data < 0 )
  {
    *n_data = 0;
  }

  *n_data = *n_data + 1;

  if ( N_MAX < *n_data )
  {
    *n_data = 0;
    *x = 0.0;
    *fx = 0.0;
  }
  else
  {
    *x = x_vec[*n_data-1];
    *fx = fx_vec[*n_data-1];
  }

  return;
# undef N_MAX
}
//****************************************************************************80

void normp ( double z, double *p, double *q, double *pdf )

//****************************************************************************80
//
//  Purpose:
//
//    NORMP computes the cumulative density of the standard normal distribution.
//
//  Discussion:
//
//    This is algorithm 5666 from Hart, et al.
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Alan Miller.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    John Hart, Ward Cheney, Charles Lawson, Hans Maehly,
//    Charles Mesztenyi, John Rice, Henry Thacher,
//    Christoph Witzgall,
//    Computer Approximations,
//    Wiley, 1968,
//    LC: QA297.C64.
//
//  Parameters:
//
//    Input, double Z, divides the real line into two semi-infinite
//    intervals, over each of which the standard normal distribution
//    is to be integrated.
//
//    Output, double *P, *Q, the integrals of the standard normal
//    distribution over the intervals ( - Infinity, Z] and
//    [Z, + Infinity ), respectively.
//
//    Output, double *PDF, the value of the standard normal
//    distribution at Z.
//
{
  double cutoff = 7.071;
  double expntl;
  double p0 = 220.2068679123761;
  double p1 = 221.2135961699311;
  double p2 = 112.0792914978709;
  double p3 = 33.91286607838300;
  double p4 = 6.373962203531650;
  double p5 = 0.7003830644436881;
  double p6 = 0.03526249659989109;
  double q0 = 440.4137358247522;
  double q1 = 793.8265125199484;
  double q2 = 637.3336333788311;
  double q3 = 296.5642487796737;
  double q4 = 86.78073220294608;
  double q5 = 16.06417757920695;
  double q6 = 1.755667163182642;
  double q7 = 0.08838834764831844;
  double root2pi = 2.506628274631001;
  double zabs;

  zabs = r8_abs ( z );
//
//  37 < |Z|.
//
  if ( 37.0 < zabs )
  {
    *pdf = 0.0;
    *p = 0.0;
  }
//
//  |Z| <= 37.
//
  else
  {
    expntl = exp ( - 0.5 * zabs * zabs );
    *pdf = expntl / root2pi;
//
//  |Z| < CUTOFF = 10 / sqrt(2).
//
    if ( zabs < cutoff )
    {
      *p = expntl * ((((((
          p6   * zabs
        + p5 ) * zabs
        + p4 ) * zabs
        + p3 ) * zabs
        + p2 ) * zabs
        + p1 ) * zabs
        + p0 ) / (((((((
          q7   * zabs
        + q6 ) * zabs
        + q5 ) * zabs
        + q4 ) * zabs
        + q3 ) * zabs
        + q2 ) * zabs
        + q1 ) * zabs
        + q0 );
    }
//
//  CUTOF <= |Z|.
//
    else
    {
      *p = *pdf / (
        zabs + 1.0 / (
        zabs + 2.0 / (
        zabs + 3.0 / (
        zabs + 4.0 / (
        zabs + 0.65 )))));
    }
  }

  if ( z < 0.0 )
  {
    *q = 1.0 - *p;
  }
  else
  {
    *q = *p;
    *p = 1.0 - *q;
  }
  return;
}
//****************************************************************************80

void owen_values ( int *n_data, double *h, double *a, double *t )

//****************************************************************************80
//
//  Purpose:
//
//    OWEN_VALUES returns some values of Owen's T function.
//
//  Discussion:
//
//    Owen's T function is useful for computation of the bivariate normal
//    distribution and the distribution of a skewed normal distribution.
//
//    Although it was originally formulated in terms of the bivariate
//    normal function, the function can be defined more directly as
//
//      T(H,A) = 1 / ( 2 * pi ) *
//        Integral ( 0 <= X <= A ) e^(H^2*(1+X^2)/2) / (1+X^2) dX
//
//    In Mathematica, the function can be evaluated by:
//
//      fx = 1/(2*Pi) * Integrate [ E^(-h^2*(1+x^2)/2)/(1+x^2), {x,0,a} ]
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    10 December 2004
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Stephen Wolfram,
//    The Mathematica Book,
//    Fourth Edition,
//    Cambridge University Press, 1999,
//    ISBN: 0-521-64314-7,
//    LC: QA76.95.W65.
//
//  Parameters:
//
//    Input/output, int *N_DATA.  The user sets N_DATA to 0 before the
//    first call.  On each call, the routine increments N_DATA by 1, and
//    returns the corresponding data; when there is no more data, the
//    output value of N_DATA will be 0 again.
//
//    Output, double *H, a parameter.
//
//    Output, double *A, the upper limit of the integral.
//
//    Output, double *T, the value of the function.
//
{
# define N_MAX 22

  double a_vec[N_MAX] = {
    0.5000000000000000E+00,
    0.1000000000000000E+01,
    0.2000000000000000E+01,
    0.3000000000000000E+01,
    0.5000000000000000E+00,
    0.1000000000000000E+01,
    0.2000000000000000E+01,
    0.3000000000000000E+01,
    0.5000000000000000E+00,
    0.1000000000000000E+01,
    0.2000000000000000E+01,
    0.3000000000000000E+01,
    0.5000000000000000E+00,
    0.1000000000000000E+01,
    0.2000000000000000E+01,
    0.3000000000000000E+01,
    0.5000000000000000E+00,
    0.1000000000000000E+01,
    0.2000000000000000E+01,
    0.3000000000000000E+01,
    0.1000000000000000E+02,
    0.1000000000000000E+03 };

  double h_vec[N_MAX] = {
    0.1000000000000000E+01,
    0.1000000000000000E+01,
    0.1000000000000000E+01,
    0.1000000000000000E+01,
    0.5000000000000000E+00,
    0.5000000000000000E+00,
    0.5000000000000000E+00,
    0.5000000000000000E+00,
    0.2500000000000000E+00,
    0.2500000000000000E+00,
    0.2500000000000000E+00,
    0.2500000000000000E+00,
    0.1250000000000000E+00,
    0.1250000000000000E+00,
    0.1250000000000000E+00,
    0.1250000000000000E+00,
    0.7812500000000000E-02,
    0.7812500000000000E-02,
    0.7812500000000000E-02,
    0.7812500000000000E-02,
    0.7812500000000000E-02,
    0.7812500000000000E-02 };

  double t_vec[N_MAX] = {
    0.4306469112078537E-01,
    0.6674188216570097E-01,
    0.7846818699308410E-01,
    0.7929950474887259E-01,
    0.6448860284750376E-01,
    0.1066710629614485E+00,
    0.1415806036539784E+00,
    0.1510840430760184E+00,
    0.7134663382271778E-01,
    0.1201285306350883E+00,
    0.1666128410939293E+00,
    0.1847501847929859E+00,
    0.7317273327500385E-01,
    0.1237630544953746E+00,
    0.1737438887583106E+00,
    0.1951190307092811E+00,
    0.7378938035365546E-01,
    0.1249951430754052E+00,
    0.1761984774738108E+00,
    0.1987772386442824E+00,
    0.2340886964802671E+00,
    0.2479460829231492E+00 };

  if ( *n_data < 0 )
  {
    *n_data = 0;
  }

  *n_data = *n_data + 1;

  if ( N_MAX < *n_data )
  {
    *n_data = 0;
    *h = 0.0;
    *a = 0.0;
    *t = 0.0;
  }
  else
  {
    *h = h_vec[*n_data-1];
    *a = a_vec[*n_data-1];
    *t = t_vec[*n_data-1];
  }

  return;
# undef N_MAX
}
//****************************************************************************80

double q ( double h, double ah )

//****************************************************************************80
//
//  Purpose:
//
//    Q computes (1/2) * p(H<Z) - T(H,AH).
//
//  Discussion:
//
//    The routine computes Q = (1/2) * P( H < Z ) - T ( H, AH ).
//
//    The result for Q is non-negative.
//
//    Warning : Q is computed as the difference between two terms;
//    When the two terms are of similar value this may produce
//    error in Q.
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Mike Patefield, David Tandy.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Mike Patefield, David Tandy,
//    Fast and Accurate Calculation of Owen's T Function,
//    Journal of Statistical Software,
//    Volume 5, Number 5, 2000, pages 1-25.
//
//  Parameters:
//
//    Input, double H, the lower limit for Z.
//    0 < H.
//
//    Input, double AH, one of the arguments for the T function.
//
//    Output, double Q, the desired quantity.
//
{
  double ahh;
  double rroot2 = 0.70710678118654752440;
  double value;
  double x;

  if ( 1.0 < ah )
  {
    ahh = ah * h;
    value = tfun ( ahh, 1.0 / ah, h ) - znorm2 ( ahh ) * znorm1 ( h );
  }
  else
  {
    value = 0.5 * znorm2 ( h ) - t ( h, ah );
  }

  return value;
}
//****************************************************************************80

double r8_abs ( double x )

//****************************************************************************80
//
//  Purpose:
//
//    R8_ABS returns the absolute value of an R8.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    14 November 2006
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double X, the quantity whose absolute value is desired.
//
//    Output, double R8_ABS, the absolute value of X.
//
{
  double value;

  if ( 0.0 <= x )
  {
    value = x;
  }
  else
  {
    value = -x;
  }
  return value;
}
//****************************************************************************80

double t ( double h, double a )

//****************************************************************************80
//
//  Purpose:
//
//    T computes Owen's T function for arbitrary H and A.
//
//  Discussion:
//
//    As printed, this routine had an error in the second case.
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Mike Patefield, David Tandy.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Mike Patefield, David Tandy,
//    Fast and Accurate Calculation of Owen's T Function,
//    Journal of Statistical Software,
//    Volume 5, Number 5, 2000, pages 1-25.
//
//  Parameters:
//
//    Input, double H, A, the arguments.
//
//    Output, double T, the value of Owen's T function.
//
{
  double absa;
  double absh;
  double ah;
  double cut = 0.67;
  double normah;
  double normh;
  double rroot2 = 0.70710678118654752440;
  double value;
  double x;

  absh = r8_abs ( h );
  absa = r8_abs ( a );
  ah = absa * absh;

  if ( absa <= 1.0 )
  {
    value = tfun ( absh, absa, ah );
  }
//
//  In the printed paper, the formula for T that follows was incorrect.
//
  else if ( absh <= cut )
  {
    value = 0.5 * znorm1 ( absh ) + 0.5 * znorm1 ( ah )
    - znorm1 ( absh ) * znorm1 ( ah ) -  tfun ( ah, 1.0 / absa, absh );
  }
  else
  {
    normh = znorm2 ( absh );
    normah = znorm2 ( ah );
    value = 0.5 * ( normh + normah ) - normh * normah
    - tfun ( ah, 1.0 / absa, absh );
  }

  if ( a < 0.0 )
  {
    value = - value;
  }

  return value;
}
//****************************************************************************80

double tfun ( double h, double a, double ah )

//****************************************************************************80
//
//  Purpose:
//
//    TFUN computes Owen's T function for a restricted range of parameters.
//
//  Discussion:
//
//    This routine computes Owen's T-function of H and A.
//
//    This routine, originally named "TF", was renamed "TFUN" to avoid
//    a conflict with a built in MATLAB function.
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by Mike Patefield, David Tandy.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Mike Patefield, David Tandy,
//    Fast and Accurate Calculation of Owen's T Function,
//    Journal of Statistical Software,
//    Volume 5, Number 5, 2000, pages 1-25.
//
//  Parameters:
//
//    Input, double H, the H argument of the function.
//    0 <= H.
//
//    Input, double A, the A argument of the function.
//    0 <= A <= 1.
//
//    Input, double AH, the value of A*H.
//
//    Output, double TF, the value of Owen's T function.
//
{
  double ai;
  double aj;
  double arange[7] = {
    0.025, 0.09, 0.15, 0.36, 0.5,
    0.9, 0.99999 };
  double as;
  double c2[21] = {
                                   0.99999999999999987510,
     -0.99999999999988796462,      0.99999999998290743652,
     -0.99999999896282500134,      0.99999996660459362918,
     -0.99999933986272476760,      0.99999125611136965852,
     -0.99991777624463387686,      0.99942835555870132569,
     -0.99697311720723000295,      0.98751448037275303682,
     -0.95915857980572882813,      0.89246305511006708555,
     -0.76893425990463999675,      0.58893528468484693250,
     -0.38380345160440256652,      0.20317601701045299653,
     -0.82813631607004984866E-01,  0.24167984735759576523E-01,
     -0.44676566663971825242E-02,  0.39141169402373836468E-03 };
  double dhs;
  double dj;
  double gj;
  double hrange[14] = {
    0.02, 0.06, 0.09, 0.125, 0.26,
    0.4,  0.6,  1.6,  1.7,   2.33,
    2.4,  3.36, 3.4,  4.8 };
  double hs;
  int i;
  int iaint;
  int icode;
  int ifail;
  int ihint;
  int ii;
  int j;
  int jj;
  int m;
  int maxii;
  int meth[18] = {
    1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 4, 4, 4, 4, 5, 6 };
  double normh;
  int ord[18] = {
    2, 3, 4, 5, 7,10,12,18,10,20,30,20, 4, 7, 8,20,13, 0 };
  double pts[13] = {
                                   0.35082039676451715489E-02,
      0.31279042338030753740E-01,  0.85266826283219451090E-01,
      0.16245071730812277011,      0.25851196049125434828,
      0.36807553840697533536,      0.48501092905604697475,
      0.60277514152618576821,      0.71477884217753226516,
      0.81475510988760098605,      0.89711029755948965867,
      0.95723808085944261843,      0.99178832974629703586 };
  double r;
  double rroot2 = 0.70710678118654752440;
  double rrtpi = 0.39894228040143267794;
  double rtwopi = 0.15915494309189533577;
  int select[15*8] = {
    1, 1, 2,13,13,13,13,13,13,13,13,16,16,16, 9,
    1, 2, 2, 3, 3, 5, 5,14,14,15,15,16,16,16, 9,
    2, 2, 3, 3, 3, 5, 5,15,15,15,15,16,16,16,10,
    2, 2, 3, 5, 5, 5, 5, 7, 7,16,16,16,16,16,10,
    2, 3, 3, 5, 5, 6, 6, 8, 8,17,17,17,12,12,11,
    2, 3, 5, 5, 5, 6, 6, 8, 8,17,17,17,12,12,12,
    2, 3, 4, 4, 6, 6, 8, 8,17,17,17,17,17,12,12,
    2, 3, 4, 4, 6, 6,18,18,18,18,17,17,17,12,12 };
  double value;
  double vi;
  double wts[13] = {
                                   0.18831438115323502887E-01,
      0.18567086243977649478E-01,  0.18042093461223385584E-01,
      0.17263829606398753364E-01,  0.16243219975989856730E-01,
      0.14994592034116704829E-01,  0.13535474469662088392E-01,
      0.11886351605820165233E-01,  0.10070377242777431897E-01,
      0.81130545742299586629E-02,  0.60419009528470238773E-02,
      0.38862217010742057883E-02,  0.16793031084546090448E-02 };
  double x;
  double y;
  double yi;
  double z;
  double zi;
//
//  Determine appropriate method from t1...t6
//
  ihint = 15;

  for ( i = 1; i <= 14; i++ )
  {
    if ( h <= hrange[i-1] )
    {
      ihint = i;
      break;
    }
  }

  iaint = 8;

  for ( i = 1; i <= 7; i++ )
  {
    if ( a <= arange[i-1] )
    {
      iaint = i;
      break;
    }
  }

  icode = select[ihint-1+(iaint-1)*15];
  m = ord[icode-1];
//
//  t1(h, a, m) ; m = 2, 3, 4, 5, 7, 10, 12 or 18
//  jj = 2j - 1 ; gj = exp(-h*h/2) * (-h*h/2)**j / j//
//  aj = a**(2j-1) / (2*pi)
//
  if ( meth[icode-1] == 1 )
  {
    hs = - 0.5 * h * h;
    dhs = exp ( hs );
    as = a * a;
    j = 1;
    jj = 1;
    aj = rtwopi * a;
    value = rtwopi * atan ( a );
    dj = dhs - 1.0;
    gj = hs * dhs;

    for ( ; ; )
    {
      value = value + dj * aj / ( double ) ( jj );

      if ( m <= j )
      {
        return value;
      }
      j = j + 1;
      jj = jj + 2;
      aj = aj * as;
      dj = gj - dj;
      gj = gj * hs / ( double ) ( j );
    }
  }
//
//  t2(h, a, m) ; m = 10, 20 or 30
//  z = (-1)**(i-1) * zi ; ii = 2i - 1
//  vi = (-1)**(i-1) * a**(2i-1) * exp[-(a*h)**2/2] / sqrt(2*pi)
//
  else if ( meth[icode-1] == 2 )
  {
    maxii = m + m + 1;
    ii = 1;
    value = 0.0;
    hs = h * h;
    as = - a * a;
    vi = rrtpi * a * exp ( - 0.5 * ah * ah );
    z = znorm1 ( ah ) / h;
    y = 1.0 / hs;

    for ( ; ; )
    {
      value = value + z;

      if ( maxii <= ii )
      {
        value = value * rrtpi * exp ( - 0.5 * hs );
        return value;
      }
      z = y * ( vi - ( double ) ( ii ) * z );
      vi = as * vi;
      ii = ii + 2;
    }
  }
//
//  t3(h, a, m) ; m = 20
//  ii = 2i - 1
//  vi = a**(2i-1) * exp[-(a*h)**2/2] / sqrt(2*pi)
//
  else if ( meth[icode-1] == 3 )
  {
    i = 1;
    ii = 1;
    value = 0.0;
    hs = h * h;
    as = a * a;
    vi = rrtpi * a * exp ( - 0.5 * ah * ah );
    zi = znorm1 ( ah ) / h;
    y = 1.0 / hs;

    for ( ; ; )
    {
      value = value + zi * c2[i-1];

      if ( m < i )
      {
        value = value * rrtpi * exp ( - 0.5 * hs );
        return value;
      }
      zi = y  * ( ( double ) ( ii ) * zi - vi );
      vi = as * vi;
      i = i + 1;
      ii = ii + 2;
    }
  }
//
//  t4(h, a, m) ; m = 4, 7, 8 or 20;  ii = 2i + 1
//  ai = a * exp[-h*h*(1+a*a)/2] * (-a*a)**i / (2*pi)
//
  else if ( meth[icode-1] == 4 )
  {
    maxii = m + m + 1;
    ii = 1;
    hs = h * h;
    as = - a * a;
    value = 0.0;
    ai = rtwopi * a * exp ( - 0.5 * hs * ( 1.0 - as ) );
    yi = 1.0;

    for ( ; ; )
    {
      value = value + ai * yi;

      if ( maxii <= ii )
      {
        return value;
      }
      ii = ii + 2;
      yi = ( 1.0 - hs * yi ) / ( double ) ( ii );
      ai = ai * as;
    }
  }
//
//  t5(h, a, m) ; m = 13
//  2m - point gaussian quadrature
//
  else if ( meth[icode-1] == 5 )
  {
    value = 0.0;
    as = a * a;
    hs = - 0.5 * h * h;
    for ( i = 1; i <= m; i++ )
    {
      r = 1.0 + as * pts[i-1];
      value = value + wts[i-1] * exp ( hs * r ) / r;
    }
    value = a * value;
  }
//
//  t6(h, a);  approximation for a near 1, (a<=1)
//
  else if ( meth[icode-1] == 6 )
  {
    normh = znorm2 ( h );
    value = 0.5 * normh * ( 1.0 - normh );
    y = 1.0 - a;
    r = atan ( y / ( 1.0 + a ) );

    if ( r != 0.0 )
    {
      value = value - rtwopi * r * exp ( - 0.5 * y * h * h / r );
    }
  }
  return value;
}
//****************************************************************************80

void timestamp ( void )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    24 September 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  size_t len;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
//****************************************************************************80

double znorm1 ( double z )

//****************************************************************************80
//
//  Purpose:
//
//    ZNORM1 evaluates the normal CDF from -oo to Z.
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double Z, the upper limit.
//
//    Output, double ZNORM1, the probability that a standard
//    normal variable will lie between -oo and Z.
//
{
  double p;
  double pdf;
  double q;

  normp ( z, &p, &q, &pdf );

  return p;
}
//****************************************************************************80

double znorm2 ( double z )

//****************************************************************************80
//
//  Purpose:
//
//    ZNORM2 evaluates the normal CDF from Z to +oo.
//
//  Modified:
//
//    04 February 2008
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double Z, the lower limit.
//
//    Output, double ZNORM2, the probability that a standard
//    normal variable will lie between Z and +oo.
//
{
  double p;
  double pdf;
  double q;

  normp ( z, &p, &q, &pdf );

  return q;
}

