#ifndef OPERATIONS_H_
#define OPERATIONS_H_
#include <iostream>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>

template<typename T> std::string obj2str(T n) {
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	return s;
}

template<typename T> T* vecToArr(std::vector<T> input) {
	T* a = new T[input.size()];
	std::copy(input.begin(), input.end(), a);
	return a;
}

//This does work.
template<typename u> std::vector<u> arrToVec(u a[], int size) {
	std::vector<u> ans(a, a + size);
	return ans;
}

namespace pftools {
double deltaR(const double& eta1, const double& eta2, const double& phi1,
		const double& phi2);

}

#endif
