//int main(){;}
void DrawGraphs()
{
  cout<<"moving to the event: "<<++event<<endl;
  //can.cd(1);
  //TH1F a("aa","aa",33,0,10);a.Draw();
  the_event = IntToString(event);
  can.Clear();
  can.Divide(width,height);
  can.Update();
  for(int k=0 ; k<width*height ; k++){
    string xtal = IntToString(windCry[k]);
    string name = "Graph_"+the_event+"_"+xtal;
    //graphs.push_back((TGraph*) f.Get(name.c_str()));
    TGraph* gra = (TGraph*) f.Get(name.c_str()));
  int canvas_num = width*height - (k%height)*width - width + 1 + k/height;
  //cout<<name<<endl;
  can.cd(canvas_num);
  if( gra != NULL ){
    //cout<<"AAAAAAAAAAAAAA"<<endl;
    gra->Draw("A*");
    can.Update();
  }
  //else{gPad->Clear();}
}
 can.cd((width*height+1)/2);
 can.Update();
//return the_event;

}
