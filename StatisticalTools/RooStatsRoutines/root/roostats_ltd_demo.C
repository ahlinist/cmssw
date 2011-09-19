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

  LimitResult limitResult = cls_limit("ws_cl95.root",
				      "ws",
				      "observed_data");

  // all done
  std::cout << "[roostats_ltd_demo]: done." << std::endl;
}
