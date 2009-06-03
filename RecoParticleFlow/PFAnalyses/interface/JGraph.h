#ifndef JGRAPH_H_
#define JGRAPH_H_

#include <TGraph.h>
#include <TGraphErrors.h>
#include <vector>
#include <string>
#include <ostream>
class JGraph {
public:
	JGraph(std::string rootName);

	virtual ~JGraph();

	void addPoint(double x, double y);

	void addPoint(double x, double ex, double y, double ey);

	TGraph finalise();

	TGraphErrors finaliseWithErrors();

	std::ostream& operator<<(std::ostream& s);
private:
	bool hasErrors_;
	std::string rootName_;
	std::vector<double> xvals_;
	std::vector<double> exvals_;
	std::vector<double> yvals_;
	std::vector<double> eyvals_;
};

#endif /*JGRAPH_H_*/
