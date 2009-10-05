void treeView(std::string file) {
	//std::string file("/tmp/PFlowTB_Tree_100GeV_endcaps.root");
	//gSystem->Load("TTreeViewer");
	TFile* source = TFile::Open(file.c_str());
	TTree* tree = (TTree *) gDirectory->FindObjectAny("Extraction");
	std::cout << "Tree loaded: " << tree << std::endl;
	TTreeViewer* tv = new TTreeViewer(tree);
	//tv->SetTreeName(tree->GetName());

}


