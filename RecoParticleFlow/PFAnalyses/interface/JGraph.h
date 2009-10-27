#ifndef JGRAPH_H_
#define JGRAPH_H_

#include <TGraph.h>
#include <TGraphErrors.h>
#include <vector>
#include <string>
#include <ostream>
class JGraph {
public:
	JGraph(std::string rootName, bool suppressXErrors = false, bool suppressYErrors = false);

	virtual ~JGraph();

	void addPoint(double x, double y);

	void addPoint(double x, double ex, double y, double ey);

	TGraph finalise();

	TGraphErrors finaliseWithErrors();

	void suppressPrintErrors(bool xErrorsSuppressed, bool yErrorsSuppressed) {
		suppressXErrors_ = xErrorsSuppressed;
		suppressYErrors_ = yErrorsSuppressed;
	}

	bool streamToFile(const std::string& filename, bool recreate);

	friend std::ostream& operator<<(std::ostream& s, const JGraph& q);
private:
	bool hasErrors_;
	std::string rootName_;
	std::vector<double> xvals_;
	std::vector<double> exvals_;
	std::vector<double> yvals_;
	std::vector<double> eyvals_;

	bool suppressXErrors_;
	bool suppressYErrors_;
};

std::ostream& operator<<(std::ostream& s, const JGraph& q);

#endif /*JGRAPH_H_*/
