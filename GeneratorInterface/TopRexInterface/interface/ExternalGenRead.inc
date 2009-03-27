
 // read External Generator parameters
{   ParameterSet generator_params = 
    pset.getParameter<ParameterSet>("GeneratorParameters") ;
     vector<string> pars = 
      generator_params.getParameter<vector<string> >("generator");
     cout << "----------------------------------------------" << endl;
     cout << "Read External Generator parameter set "  << endl;
     cout << "----------------------------------------------" << endl;
   for( vector<string>::const_iterator  
	   itPar = pars.begin(); itPar != pars.end(); ++itPar ) 
       {
      call_txgive(*itPar);          
       }
}


