#include "RecoVertex/KinematicFit/interface/ColinearityKinematicConstraint.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"

ColinearityKinematicConstraint::ColinearityKinematicConstraint(ConstraintDim dim)
{
  dimension = dim;
  if (dimension == Phi) size = 1;
  else size = 2;
}

AlgebraicVector  ColinearityKinematicConstraint::value(const vector<KinematicState> states,
                        const GlobalPoint& point) const
{
  if(states.size()<2) throw VertexException("ColinearityKinematicConstraint::<2 states passed");
  AlgebraicVector res(size,0);

  double field1 = states[0].magneticField()->inInverseGeV(states[0].globalPosition()).z();
  double a_1 = -0.29979246*states[0].particleCharge()*field1;
  double field2 = states[1].magneticField()->inInverseGeV(states[1].globalPosition()).z();
  double a_2 = -0.29979246*states[1].particleCharge()*field2;

  AlgebraicVector7 p1 = states[0].kinematicParameters().vector();
  AlgebraicVector7 p2 = states[1].kinematicParameters().vector();

  double p1vx = p1(3) - a_1*(point.y() - p1(1));
  double p1vy = p1(4) + a_1*(point.x() - p1(0));
  double pt1  = sqrt(p1(3)*p1(3)+p1(4)*p1(4));
  double pTot1  = sqrt(p1(3)*p1(3)+p1(4)*p1(4)+p1(5)*p1(5));

  double p2vx = p2(3) - a_2*(point.y() - p2(1));
  double p2vy = p2(4) + a_2*(point.x() - p2(0));
  double pt2  = sqrt(p2(3)*p2(3)+p2(4)*p2(4));
  double pTot2   = sqrt(p2(3)*p2(3)+p2(4)*p2(4)+p2(5)*p2(5));

  // H_phi:
  res(1)  = ((p1vx*p2vx) + (p1vy* p2vy))/(pt1*pt2) - 1 ;
  // H_theta:
  if (dimension == PhiTheta) res(2)  = ((pt1*pt2) + (p1(5)*p2(5)))/(pTot1*pTot2) - 1;
// cout << res(1) << " "<<res(2) << "\n ";// res(2)

  return res;
}

AlgebraicMatrix ColinearityKinematicConstraint::parametersDerivative(const vector<KinematicState> states,
                                      const GlobalPoint& point) const
{
  int n_st = states.size();
  if(n_st<2) throw VertexException("ColinearityKinematicConstraint::<2 states passed");
  AlgebraicMatrix res(size,n_st*7,0);

  TrackCharge ch1 = states[0].particleCharge();
  TrackCharge ch2 = states[1].particleCharge();

  double field1 = states[0].magneticField()->inInverseGeV(states[0].globalPosition()).z();
  double a_1 = -0.29979246*ch1*field1;
  double field2 = states[1].magneticField()->inInverseGeV(states[1].globalPosition()).z();
  double a_2 = -0.29979246*ch2*field2;

  AlgebraicVector7 p1 = states[0].kinematicParameters().vector();
  AlgebraicVector7 p2 = states[1].kinematicParameters().vector();

  double p1vx = p1(3) - a_1*(point.y() - p1(1));
  double p1vy = p1(4) + a_1*(point.x() - p1(0));
  double pt1 = sqrt(p1(3)*p1(3)+p1(4)*p1(4));
  double pTot1  = sqrt(p1(3)*p1(3)+p1(4)*p1(4)+p1(5)*p1(5));

  double p2vx = p2(3) - a_2*(point.y() - p2(1));
  double p2vy = p2(4) + a_2*(point.x() - p2(0));
  double pt2  = sqrt(p2(3)*p2(3)+p2(4)*p2(4));
  double pTot2   = sqrt(p2(3)*p2(3)+p2(4)*p2(4)+p2(5)*p2(5));

  // H_phi:

  //x1 and x2 derivatives: 1st and 8th elements
  res(1,1) =  -a_1*p2vy/(pt1*pt2);
  res(1,8) =  -a_2*p1vy/(pt1*pt2);

  //y1 and y2 derivatives: 2nd and 9th elements:
  res(1,2) = a_1*p2vx/(pt1*pt2);
  res(1,9) = a_2*p1vx/(pt1*pt2);

  //z1 and z2 components: 3d and 10th elmnets stay 0:
  res(1,3)  = 0.; res(1,10) = 0.;

  //px1 and px2 components: 4th and 11th elements:
  res(1,4)  = ( pt1*pt1*p2vx - p1(3)*(p1vy*p2vy + p1vx*p2vx))/(pt1*pt1*pt1*pt2);
  res(1,11) = ( pt2*pt2*p1vx - p2(3)*(p1vy*p2vy + p1vx*p2vx))/(pt1*pt2*pt2*pt2);

  //py1 and py2 components: 5th and 12 elements:
  res(1,5)  = ( pt1*pt1*p2vy - p1(4)*(p1vy*p2vy + p1vx*p2vx))/(pt1*pt1*pt1*pt2);
  res(1,12) = ( pt2*pt2*p1vy - p2(4)*(p1vy*p2vy + p1vx*p2vx))/(pt1*pt2*pt2*pt2);


  //pz1 and pz2 components: 6th and 13 elements:
  res(1,6)  = 0.; res(1,13) = 0.;
  //mass components: 7th and 14th elements:
  res(1,7)  = 0.; res(1,14) = 0.;

  if (dimension == PhiTheta)  {
    // H_theta:
    //x1 and x2 derivatives: 1st and 8th elements
    res(2,1) =  0.; res(2,8) = 0.;

    //y1 and y2 derivatives: 2nd and 9th elements:
    res(2,2) = 0.; res(2,9) = 0.;

    //z1 and z2 components: 3d and 10th elmnets stay 0:
    res(2,3) = 0.; res(2,10) = 0.;

    //px1 and px2 components: 4th and 11 elements:
    res(2,4)  = (p1(3)* p1(5)* (pt2* p1(5) - pt1* p2(5)))/(pt1 *pTot1*pTot1*pTot1*pTot2);
    res(2,11) = (p2(3)* p2(5)* (pt1* p2(5) - pt2* p1(5)))/(pt2 *pTot1*pTot2*pTot2*pTot2);

    //py1 and py2 components: 5th and 12 elements:
    res(2,5)  = (p1(4)* p1(5)* (pt2* p1(5) - pt1* p2(5)))/(pt1 *pTot1*pTot1*pTot1*pTot2);
    res(2,12) = (p2(4)* p2(5)* (pt1* p2(5) - pt2* p1(5)))/(pt2 *pTot1*pTot2*pTot2*pTot2);

    //pz1 and pz2 components: 6th and 13 elements:
    res(2,6)  = (pt1*pt1*p2(5) - p1(5)* pt1* pt2) / (pTot1*pTot1*pTot1*pTot2);
    res(2,13) = (pt2*pt2*p1(5) - p2(5)* pt1* pt2) / (pTot1*pTot2*pTot2*pTot2);

    //mass components: 7th and 14th elements:
    res(2,7)  = 0.; res(2,14) = 0.;
  }
  return res;
}

AlgebraicMatrix ColinearityKinematicConstraint::positionDerivative(const vector<KinematicState> states,
                                    const GlobalPoint& point) const
{
  AlgebraicMatrix res(size,3,0);
  if(states.size()<2) throw VertexException("ColinearityKinematicConstraint::<2 states passed");

  TrackCharge ch1 = states[0].particleCharge();
  TrackCharge ch2 = states[1].particleCharge();

  double field1 = states[0].magneticField()->inInverseGeV(states[0].globalPosition()).z();
  double a_1 = -0.29979246*ch1*field1;
  double field2 = states[1].magneticField()->inInverseGeV(states[1].globalPosition()).z();
  double a_2 = -0.29979246*ch2*field2;

  AlgebraicVector7 p1 = states[0].kinematicParameters().vector();
  AlgebraicVector7 p2 = states[1].kinematicParameters().vector();

  double p1vx = p1(3) - a_1*(point.y() - p1(1));
  double p1vy = p1(4) + a_1*(point.x() - p1(0));
  double pt1 = sqrt(p1(3)*p1(3)+p1(4)*p1(4));

  double p2vx = p2(3) - a_2*(point.y() - p2(1));
  double p2vy = p2(4) + a_2*(point.x() - p2(0));
  double pt2  = sqrt(p2(3)*p2(3)+p2(4)*p2(4));

  // H_phi:

  // xv component
  res(1,1) = (a_2*p1vy + a_1*p2vy)/(pt1*pt2);

  //yv component
  res(1,2) = -(a_2*p1vx + a_1*p2vx)/(pt1*pt2);

  //zv component
  res(1,3) = 0.;

  // H_theta: no correlation with vertex position
  if (dimension == PhiTheta) {
    res(2,1) = 0.;
    res(2,2) = 0.;
    res(2,3) = 0.;
  }

  return res;
}
