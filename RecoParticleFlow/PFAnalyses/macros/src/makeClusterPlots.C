{
        gSystem->Load("libFWCoreFWLite.so");
        // // AutoLibraryLoader::enable();
        gSystem->Load("libCintex.so");
        ROOT::Cintex::Cintex::Enable();
		gROOT->ProcessLine(".x src/clusterMultiplicity.C+");
						  
}
