#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

double detaR(const double& eta1, const double& eta2,
		const double& phi1, const double& phi2) {
	double deltaEta = fabs(eta1 - eta2);
	double deltaPhi = fabs(phi1 - phi2);
	if (deltaPhi > M_PI) {
		deltaPhi = 2 * M_PI- deltaPhi;
	}
	return sqrt(pow(deltaEta, 2) + pow(deltaPhi, 2));
}

