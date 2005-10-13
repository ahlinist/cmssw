#ifndef Hepevt_h
#define Hepevt_h
#ifdef _WIN32
# pragma once
# ifdef DLLEXPORT
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllexport)
#  endif
# else
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllimport)
#  endif
# endif
#endif
#ifndef DLLLINKAGE
# define DLLLINKAGE
#endif

#define _lenIsthep 4000
#define _lenIdhep  4000
#define _lenJmohep 4000
#define _depthJmohep 2
#define _lenJdahep 4000
#define _depthJdahep 2
#define _lenPhep 4000
#define _depthPhep 5
#define _lenVhep 4000
#define _depthVhep 4

/**
 * Imported for C++ access of HEPEVT common block.
 */
class DLLLINKAGE Hepevt
{
public:
  Hepevt();
  ~Hepevt();
  int &nevhep();
  int &nhep();
  int &isthep(const int i);
  int &idhep(const int i);
  int &jmohep(const int i,const int j);
  int &jdahep(const int i,const int j);
  double &phep(const int i,const int j);
  double &vhep(const int i,const int j);
  void fillFromPyjets();

private:
  void init(void);

  static int nDummy;
  static double dDummy;

  //  const static int _lenIsthep = 4000;
  //  const static int _lenIdhep  = 4000;
  //  const static int _lenJmohep = 4000;
  //  const static int _depthJmohep = 2;
  //  const static int _lenJdahep = 4000;
  //  const static int _depthJdahep = 2;
  //  const static int _lenPhep = 4000;
  //  const static int _depthPhep = 5;
  //  const static int _lenVhep = 4000;
  //  const static int _depthVhep = 4;

  /// map on block HEPEVT
  struct _hepevt {
    int nevhep;
    int nhep;
    int isthep[_lenIsthep];
    int idhep[_lenIdhep];
    int jmohep[_depthJmohep][_lenJmohep];
    int jdahep[_depthJdahep][_lenJdahep];
    double phep[_depthPhep][_lenPhep];
    double vhep[_depthVhep][_lenVhep];
  };
  static struct _hepevt * __hepevt;

};
#endif
