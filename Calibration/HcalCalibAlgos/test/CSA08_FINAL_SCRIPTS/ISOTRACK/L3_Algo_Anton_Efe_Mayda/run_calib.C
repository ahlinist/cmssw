TDatime _time;
TStopwatch _timer;

void run_calib() {
    
    gROOT->LoadMacro("sIsoTrackCalib.cpp++");
    
    _timer.Start();
       
    std::cout << "\n\n\n";
    std::cout << " --- Running sIsoTrackCalib --- \n";
    std::cout << "\n\n\n";

    sIsoTrackCalib();
       
    _timer.Stop();
    std::cout << "\n\tCPU time:  " << _timer.CpuTime()  << std::endl;
    std::cout << "\tReal time: " << _timer.RealTime() << std::endl << std::endl ;
        
    return;
}

