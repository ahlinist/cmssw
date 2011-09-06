{
  // load the library
  gSystem->Load("roostats_ltd_C.so");

  // instantiate calculator
  LimitCalc calc;

  // load workspace
  calc.LoadWorkspace("ws_cl95.root", "ws");
  calc.GetWorkspace()->Print();

  calc.LoadData("observed_data");

  LimitResult limitResult = calc.ComputeInverterLimit(true,
						      5,
						      0,
						      0.5,
						      1000);

  std::cout << " observed limit: " << limitResult.GetExpectedLimit() << std::endl;
  std::cout << " expected limit (median): " << limitResult.GetExpectedLimit() << std::endl;
  std::cout << " expected limit (-1 sig): " << limitResult.GetOneSigmaLowRange() << std::endl;
  std::cout << " expected limit (+1 sig): " << limitResult.GetOneSigmaHighRange() << std::endl;
  std::cout << " expected limit (-2 sig): " << limitResult.GetTwoSigmaLowRange() << std::endl;
  std::cout << " expected limit (+2 sig): " << limitResult.GetTwoSigmaHighRange() << std::endl;

  // all done
  std::cout << "[roostats_ltd_demo]: done." << std::endl;
}
