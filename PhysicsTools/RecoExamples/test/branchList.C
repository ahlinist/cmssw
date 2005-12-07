{
TObjArray * branches = Events->GetListOfBranches();
int n = branches->GetEntries();
for( int i = 0; i < n; ++i ) {
  TBranch * branch = (TBranch*) branches->At( i );
  std::cout << ">>> " << branch->GetName() << std::endl;
}
}
