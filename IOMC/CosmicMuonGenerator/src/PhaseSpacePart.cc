#include "IOMC/CosmicMuonGenerator/interface/PhaseSpacePart.h"

void PhaseSpacePart::setBorders(double lE, double uE, double lT, double uT){
  El = lE;  Eu = uE;
  Tl = lT;  Tu = uT;
}

void PhaseSpacePart::setIntAndMax(unsigned int Nstep){
  Integral = 0.;
  int ordMag = int(log10(Eu)-log10(El)) + 1;
  int Estep = Nstep*ordMag;
  double dE = (Eu-El)/double(Estep);
  double dT = (Tu-Tl)/double(Nstep);
  for (int iE=0; iE<Estep; ++iE){// E
    for (unsigned int iT=0; iT<Nstep; ++iT){// theta
      double E = El + (0.5+double(iE))*dE;
      double T = Tl + (0.5+double(iT))*dT;
      double N = dNdEdT(E,T); if (N > Maximum) Maximum = N;
      Integral += N*dE*dT;
    }
  }
  // check maximum at borders separately
  for (int iE=0; iE<(Estep+1); ++iE){// E
    double E = double(iE)*(Eu-El)/double(Estep) + El;
    double N = dNdEdT(E,Tl); if (N > Maximum) Maximum = N;
           N = dNdEdT(E,Tu); if (N > Maximum) Maximum = N;
  }
  for (unsigned int iT=0; iT<(Nstep+1); ++iT){// theta
    double Theta = double(iT)*(Tu-Tl)/double(Nstep) + Tl;
    double N = dNdEdT(El,Theta); if (N > Maximum) Maximum = N;
           N = dNdEdT(Eu,Theta); if (N > Maximum) Maximum = N;
  }
}

void PhaseSpacePart::setProbabilities(double lP, double uP){
  Pl = lP; Pu = uP;
}

double PhaseSpacePart::loEnergy(){ return El; }

double PhaseSpacePart::upEnergy(){ return Eu; }

double PhaseSpacePart::loTheta(){ return Tl; }

double PhaseSpacePart::upTheta(){ return Tu; }

double PhaseSpacePart::maximum(){ return Maximum; }

double PhaseSpacePart::integral(){ return Integral; }

double PhaseSpacePart::loProb(){ return Pl; }

double PhaseSpacePart::upProb(){ return Pu; }
