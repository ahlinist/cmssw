{
  string filename;
  cout<<"filename: "<<endl;
  cin >> filename;
//filename = "testLaser803.root";
//cout<<" aaa "<<filename<<endl;
  TFile f(filename.c_str());
  string the_event; 
  int Xtal_num; 

  int width; 
  int height;
  cout<<"Xtal: "; cin>>Xtal_num;
  cout<<"width: ";cin>>width;
  cout<<"height: ";cin>>height;
//Xtal_num=803; width=3; height =3;
  int mX= width;
  int nY= height;
  if(!(mX>0 && mX%2 == 1 )||!(nY>0 && nY%2 == 1 )){cout<<" width and height has to be greater than 0 and odd!!!!"<<endl;return 3;}
  int* windCry = new int[mX*nY];
  H4Geom SMGeom; SMGeom.init();
  
  if(Xtal_num<1 || Xtal_num > 1700){cout<<" xtal ragne 1->1700"<<endl;}
  
  SMGeom.getWindow(windCry, Xtal_num, mX, nY);

  int pippo = 0;
  int event = 0; 
  
    the_event = IntToString(event);

    TCanvas can("ped","ped",100,50,1100,1250);
    can.Divide(width,height);
    
    for(int kk=0 ; kk<width*height ; kk++){
      string xtal = IntToString(windCry[kk]);
      string name = "Graph_"+the_event+"_"+xtal;
      cout<<name<<endl;
      TGraph* gra = (TGraph*) f.Get(name.c_str());
      int canvas_num = width*height - (kk%height)*width - width + 1 + kk/height;
       can.cd(canvas_num);
       if( gra != NULL ){
	 gra ->Draw("A*");
	 can.Update();
       }
    }
    can.cd((width*height+1)/2);
    can.Update();
    TCanvas canB("butt","next",10,50,40,90);
    TButton *but = new TButton ("but",".x ./test/DrawGraphs.C",0,0,1,1);
    but->Draw();
}
//return 0;
  
}
