//===============================================================>
//
// roostats_ltd_demo.C
//
// Demo for using roostats_ltd.C for computing CLs limits
//
// Usage:
//        .L roostats_ltd.C+
//        .x roostats_ltd_demo.C
//
// 2011 Gena Kukartsev
//
//===============================================================>
{

  // load the library
  gSystem->Load("roostats_ltd_C.so");

  // instantiate calculator
  LimitCalc calc;

  // load workspace
  calc.LoadWorkspace("ws_cl95.root", "ws");
  //calc.GetWorkspace()->Print();
  calc.LoadData("observed_data");
  //calc.SetTestStatType(2);

  calc.SetFirstPoiMin(0.0);
  calc.SetFirstPoiMax(0.5);

  LimitResult limitResult = calc.GetClsLimit(0, 1000, true);

  // all done
  std::cout << "[roostats_ltd_demo]: done." << std::endl;
}
