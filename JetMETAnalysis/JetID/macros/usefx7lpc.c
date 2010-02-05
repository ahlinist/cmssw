{
gSystem->CompileMacro("fillXsec.c");
TFillXsec fx;
fx.SetFieldNames("w","pthat"); 
fx.SetTreeName("ak5JetID/t");
fx.ChooseUpdateType(2);
fx.LoadFlatFile("counts_7djd.txt");
cout<<"For each file simply use fx.doFile (\"FileName\")"<<endl;
}
