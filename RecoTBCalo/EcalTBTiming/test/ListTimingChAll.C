{
char name[500];
TH1F * h = 0; 
int count = 0;
ofstream file("out.txt");
for(int ch=1; ch< 1701; ch++){
  sprintf(name, "TDCTime_Cry_%04d_sum",ch);
  h = 0;
  h = (TH1F*) _file0->Get(name);
  if(h == 0){continue;}
  float mean = 0, RMS = 0;
  int nent = 0;
  nent = h->GetEntries();
  mean = h->GetMean();
  RMS  = h->GetRMS();
  if(nent > 100 && RMS < 0.05){
    count++;
    //   cout<<ch<<" \t "<<mean<<" \t "<<RMS<<" \t "<<nent<<endl;
    file<<ch<<" \t "<<mean<<" \t "<<endl;
  }
  else if(nent > 100){
    cout<<ch<<" \t "<<mean<<" \t "<<RMS<<" \t "<<nent<<endl;
  }
 
}
file.close();
cout<<"Found: "<<count<<" good Xtals"<<endl;
}
