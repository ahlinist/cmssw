#include "UFAV.h"

UFAV::UFAV(TString ProjectName)
{
  gROOT->SetStyle("Plain");
  gErrorIgnoreLevel=2001;

  //Display Version of UFAV
  DisplayVersion();
  
  LocalDirectory_ = ProjectName;
  if( !LocalDirectory_.EndsWith("/") ) 
    LocalDirectory_+="/";
  
  if( LocalDirectory_.Contains(" ") )
    {
      cerr << "[ERROR]:  ProjectName cannot contain blankspaces because this will be made into a UNIX directory." ;
      RemoveSubstring(LocalDirectory_," " );
      if( !LocalDirectory_.Length() )
	LocalDirectory_ =  "Output";
      cerr << " UFAV will use ProjecName: " << LocalDirectory_ << "instead." << endl;
    }  
  
  NoDataIcon_ = "empty.png";
  
  SetLineColor(kBlack);
  SetFillStyle(1001);
  SetFillColor(kWhite);
  
  UNIXBaseDirectory_ = getenv("PWD");
  cout<< "Working in unix directory: " << UNIXBaseDirectory_ << "/"<<LocalDirectory_ << endl;

  //Set Thumbnail Properties
  SetThumbWidth();
  SetThumbHeight();
  SetThumbLeft();
  SetThumbTop();

  //Set Mode (Type of UFAV to make)
  Mode_ = 1;
  
  //Set directory layer within rootfile
  Layer_ = 0;  
  
  //Set # of standard deviations to cover in window of distributions.  Mind the tails!!!
  Sigmas_ = 8; 

  //Set Default Flags for {Print Histos,  Make Frames in HTML, Rebin Histos, ...}
  PrintHistograms_ = true;
  Rebin_ = false;
  Zoom_  = false;
  LogY_ = false;
  YMaxLinearScale_ = 1.2;
  YMaxLogScale_ = 10.;
  RebinMax_ = 5;

  Weight_ = 1;
  Errors_ = false;  
  DrawOption2D_ = "COLZ";
  Description_ = "Analysis" ;
  Verbosity_ = 1;
  UNIXCurrentDirectory_ = LocalDirectory_;
  KS_ = false;
  RootFilesValidated_ = false;
  Ext_ = "gif";

}


void UFAV::MakeUFAV()
{ 
  
  if( Mode_ == 1 && !vFile_.size())
    {
      cerr << "[ERROR]:  You requested Mode_ = 1, but did not supply any valid rootfiles for UFAV to process.  Use the AddFile() function." << endl;
      return;
    }
  else if( Mode_ == 2 && vFile_.size() < 2 )
    {
      cerr << "[ERROR]:  You requested Mode_ = 2, but did not supply at least 2 valid root files for UFAV to compare." << endl;
      return;
    }

  MakeUNIXDirectory(LocalDirectory_+"images/");
  UNIXCommand_ = "cp -r images/*.* ";
  UNIXCommand_ += LocalDirectory_+"images/";
  UNIXCommand_ += ".";
  system(UNIXCommand_);

  MakeUNIXDirectory(LocalDirectory_+"scripts/");
  UNIXCommand_ = "cp -r scripts/*.* " ;
  UNIXCommand_ += LocalDirectory_+"scripts/";
  UNIXCommand_ += ".";
  system(UNIXCommand_);


  MakeUNIXDirectory(LocalDirectory_);
  ChangeToUNIXDirectory(LocalDirectory_);
  //Initiate HTML Production for all rootfiles,directories, and plots
  InitiateHTMLProduction();    

  if( Mode_ == 1 )
    {
      for(unsigned int f = 0 ; f < vFile_.size() ; f++ )
	{
	  vFile_[f]->cd("");
	  UNIXCurrentDirectory_ = vRootFileName_[f];
	  RootFileName_ = vRootFileName_[f];
	  StripExtension(UNIXCurrentDirectory_);
	  MakeUNIXDirectory(UNIXCurrentDirectory_);
	  ChangeToUNIXDirectory(UNIXCurrentDirectory_);
	  //Print histos and compose body of html document
	  
	  if(PrintHistograms_)
	    MakePlots(f, gDirectory);

	  
	  if(vSuperimposedDirectory1.size() == vSuperimposedDirectory2.size() && vSuperimposedDirectory1.size())
	    {
	      for(unsigned int j=0; j< vSuperimposedDirectory1.size() ; j++ )
		{
		  if(vFile_[f]->cd(vSuperimposedDirectory1[j]) && vFile_[f]->cd(vSuperimposedDirectory2[j]))
		    {
		      if( Verbosity_ )
			cout << "[UFAV will be superimposing the 1D histograms of] :  " 
			     << vSuperimposedDirectory1[j]
			     << "  [with]  " 
			     << vSuperimposedDirectory2[j] << endl
			     << "  [which will be stored to directory] : "  
			     << vSuperimposedDirectoryName[j] 
			     << endl;
		      
		      vFile_[f]->cd("");
		      if(PrintHistograms_)
			MakePlotsForSuperimposedDirectories(j);
		    }
		  else if( !vFile_[f]->Get(vSuperimposedDirectory1[j]))
		    {
		      cout << "ERROR: " << vSuperimposedDirectory1[j] <<  " is not a proper directory in this root file " << endl;
		      return;
		    }
		  else if( !vFile_[f]->Get(vSuperimposedDirectory2[j]))
		    { 
		      cout << "ERROR: " << vSuperimposedDirectory2[j] << " is not a proper directory in this root file " << endl;
		      return;
		    }
		}
	    }
	  //ChangeToUNIXDirectory("..");
	  ChangeToUNIXDirectory(UNIXBaseDirectory_+"/"+LocalDirectory_);
	}
    }
  else if( Mode_ == 2 ) 
    {
      vFile_[0]->cd("");
      RootFileName_ = Description_;
      StripExtension(RootFileName_);
      UNIXCurrentDirectory_ = Description_ ;
      StripExtension(UNIXCurrentDirectory_);
      MakeUNIXDirectory(UNIXCurrentDirectory_);
      ChangeToUNIXDirectory(UNIXCurrentDirectory_);
      //Print histos and compose body of html document                                                                                                 
      if(PrintHistograms_)
	MakePlots(0, gDirectory);


      ChangeToUNIXDirectory(UNIXBaseDirectory_+"/"+LocalDirectory_);
    }
  else 
    {
      cerr << "[Error]:  User specified Mode_ :  " << Mode_ << " which is not a valid Mode_ .  Consult UFAV User's Manual." << endl;
    }
  
  
  //Close Frame Html document
  EndHTML(*Frame1HTML_); 
    
  //ChangeToUNIXDirectory( UNIXBaseDirectory_ ) ;
  gSystem->cd(UNIXBaseDirectory_);
    
}


void UFAV::BeginHTML(ofstream &html , TString webpage_title, TString bgcolor)
{

  //Get List of Histograms for This Directory
  TIter nextkey(gDirectory->GetListOfKeys());
  TKey *key;
  std::vector< TString > vHistoList;
  while( (key = (TKey*)nextkey() ))
    {
      TString class_ = key->GetClassName();
      if ( class_.BeginsWith("TH") )
	vHistoList.push_back(key->GetName());
      else continue;
    }

  html << " <html> \n "
       << "<!-- DO NOT ERASE \n"
       << "UF Analsis Viewer (v1.5) \n"
       << "Author : Ronny Remington, University of Florida \n"
       << "Inspired by technology developed by M.Schmitt, B. Scurlock, and R. Remington (UF) \n"
       << "Date : 02/12/09  -->\n"
       << "<head> \n <title> \n " ;

  if( Layer_ == 0 ) html << gDirectory->GetName() << endl;
  else html << string(webpage_title) << endl;
  html << " </title> \n"
       << " <h3 align=\"center\"> \n ";
  if( &html == &(*Frame1HTML_)  )
    {
      html << " <b><i><u><font color=\"orange\" size=\"5\" face=\"Book Antiqua\"> "
	   << string("UFAnalysisViewer")  // FIXME !!!
	   << "</font></u></i></b> \n";

	} 
  else 
    html <<  string(webpage_title) ;
  
  html << " \n   </h3> \n ";
  
  //If thumbnail feature is desired 
  //Note to user: Mike Schmitt, UF is the genius behind the thumbnail scripts

  if( Layer_ != 0 ) 
    {
      html <<" <script type=\"text/javascript\">    \n"
	   <<" function ThumbnailPlot(plot) {              \n"
	   <<" <!-- clear_l()-->                    \n"
	   <<" document.getElementById(\"thumblink_l\").href = plot     \n"
	   <<" document.getElementById(\"thumb_l\").src = plot          \n"
	   <<" document.getElementById(\"thumb_l\").width = \" "
	   <<  ThumbWidth_
	   <<" \"   \n"
	   <<" document.getElementById(\"thumb_l\").height = \" "
	   <<  ThumbHeight_
	   <<" \"    \n"
	   <<" document.getElementById(\"thumb_l\").border = \"1\"      \n"
	   <<" }    \n"
	   <<" function clear_l() {      \n"
	   <<" document.getElementById(\"thumblink_l\").href = \"#\"   \n"    
	   <<" document.getElementById(\"thumb_l\").src = \"\"         \n"
	   <<" document.getElementById(\"thumb_l\").width = \"0\"      \n"
	   <<" document.getElementById(\"thumb_l\").height = \"0\"     \n"
	   <<" document.getElementById(\"thumb_l\").border = \"0\"     \n"
	   <<" }     \n"
	   << "function ThumbnailNote(flag,id) { \n" 
	   << "if (flag==\"1\"){ \n"
	   << "if( document.layers) document.all[''+id+''].visibility = \"show\" \n"
	   << "else if ( document.all) document.all[''+id+''].visibility = \"visible\" \n"
	   << "else if ( document.getElementById) document.getElementById(''+id+'').style.visibility = \"visible\" \n"
	   << "}\n "
	   << " else \n"
	   << " if( flag==\"0\"){ \n "
	   << "if( document.layers) document.all[''+id+''].visibility = \"hide\" \n"
	   << "else if ( document.all) document.all[''+id+''].visibility = \"hidden\" \n"
	   << "else if ( document.getElementById) document.getElementById(''+id+'').style.visibility = \"hidden\" \n"
	   << "} \n"
	   << "} \n"
	   << " function ClearNotes() { \n " ;
      
      // Add division for each histogram for user to provide captions if desired
      for(unsigned int i = 0 ; i < vHistoList.size(); i++ )
	{
	  html << "document.getElementById(\"div_"<<vHistoList[i] <<"\").style.visibility=\"hidden\" \n";
	}
      html <<" } \n"
	   <<" </script>   \n" << endl;
    }
    
  if( Layer_ == 0 ) 
    {
      html <<"<!--/********************************************************************************************************** \n"
	   <<"(C) www.dhtmlgoodies.com, July 2006 \n"
	   <<"**********************************************************************************************************/ -->\n";

      html <<"<script type=\"text/javascript\" src=\"scripts/ajax.js\"></script> \n"
	   <<"<script type=\"text/javascript\" src=\"scripts/context-menu.js\"></script>   \n "
	   <<"<script type=\"text/javascript\" src=\"scripts/drag-drop-folder-tree.js\"></script> \n "
	   <<"<link rel=\"stylesheet\" href=\"scripts/drag-drop-folder-tree.css\" type=\"text/css\"></link> \n"
	   <<"<link rel=\"stylesheet\" href=\"scrips/context-menu.css\" type=\"text/css\"></link> \n"
	   <<"<style type=\"text/css\"></style>  \n " ;
      if( Mode_ != 2 )
	{
	  html << "<a href=\"#\" onclick=\"treeObj.collapseAll()\">Collapse all</a> \n"
	       << "<a href=\"#\" onclick=\"treeObj.expandAll()\">Expand all</a> \n"
	       << endl;
	}
    }
  else
    {
      
      html <<"<style type=\"text/css\">  \n "
	   <<"#thumb_gen {position: absolute; position: fixed; left: "
	   << ThumbLeft_
	   <<"; top: "
	   << ThumbTop_
	   <<"; text-align: center;} \n"
	   <<" #main_d {position: absolute; position: fixed; left: 10px;} \n";
  
      for(unsigned int i = 0 ; i < vHistoList.size(); i++ )
	{
	  html << "#div_"<<vHistoList[i] << " {position:absolute; top: "<<NoteTop_<<"; left: "<<NoteLeft_<< "; width: " << NoteWidth_ << "; visibility:hidden} \n";
	}
      html << "</style> \n";
    }
  
  
  //html << "</head >  \n  <body bgcolor=\"" <<bgcolor<<"\" > \n <br>\n ";
  html << "</head >  \n <br>\n ";
  
}

void UFAV::EndHTML(ofstream &html)
{
  if( Layer_ == 0 ) 
    {
      html << "<script type=\"text/javascript\"> \n"
	   << "treeObj = new JSDragDropTree(); \n"
	   << "treeObj.setTreeId('directory_tree') ; \n"
	   << "treeObj.setMaximumDepth(7); \n"
	   << "treeObj.setMessageMaximumDepthReached('Maximum depth reached'); \n"
	   << "treeObj.initTree(); \n"
	   << "treeObj.expandAll(); \n"
	   << "</script>  \n";
    }
  else
    {
      html << "<div id=\"thumb_gen\">"
	   << "<a href=\"#\" id=\"thumblink_l\">"
	   << "<img src=\"\" id=\"thumb_l\" width=0 height=0 border=0></a></div> \n";
    }

  html<< "</body> \n </html> \n";
  html.close();
}

void UFAV::InitiateHTMLProduction()
{
  //Get public path to webpage 
  cout << setw(34) << "UFAV results will be placed in directory:  " << UNIXBaseDirectory_ <<"/"<<LocalDirectory_<< endl;

  //Make master HTML document
  IndexHTML_ = new ofstream("index.html");
  *IndexHTML_ << "<html>  \n "
	      << "<head>   \n "
	      << "<title> UFAnalysisViewer </title> \n"  
	      << "</head> \n"
	      << "<frameset cols = \"30%, *\"> \n"
	      << "<frame src =\""
	      << "frame1.html\" />\n"
	      << "<frame name=\"content\" src =\"\" /> \n"
	      << "</frameset> \n"
	      << "</html>";

  IndexHTML_->close();

  //Make Frame1 html document (holds directory tree)
  TString HTMLName = "frame1.html";
  Frame1HTML_ = new ofstream(HTMLName);
  if(!Frame1HTML_->is_open()) 
    cerr << "[ERROR]: HTML CLOSED!!!!!!!!!!!"<< endl;  
  
  BeginHTML(*Frame1HTML_, "UFAnalysisViewer","#6699ff");
  
  if( Mode_ == 1 ) 
    {
      *Frame1HTML_ << "<ul id=\"directory_tree\" class=\"dhtmlgoodies_tree\">" << endl;
      for(unsigned int f = 0 ; f < vFile_.size() ; f++ ) 
	{
	  Layer_++;
	  vFile_[f]->cd();
	  RootFileName_ = vRootFileName_[f];
	  StripExtension(RootFileName_ );
	  UNIXCurrentDirectory_ = vRootFileName_[f];
	  StripExtension(UNIXCurrentDirectory_);
	  MakeUNIXDirectory(UNIXCurrentDirectory_);
	  ChangeToUNIXDirectory(UNIXCurrentDirectory_);
	  MakeHTML(*Frame1HTML_,f, gDirectory);       
	  Layer_--;
	  ChangeToUNIXDirectory("..");
	}
    }
  else if (Mode_ == 2)
    {
      if( vFile_.size() < 2 ) 
	{
	  cerr << "[ERROR]:  You requested Mode_ = 2, but did not supply at least 2 valid root files to compare." << endl;
	  return;
	}

      *Frame1HTML_ << "<u>Comparison of the following files</u><br> \n" ;
      for(unsigned int i = 0 ; i < vFile_.size() ; i++ )
        {
          RootFileName_ = vRootFileName_[i];
	  *Frame1HTML_ <<"File "<< i<< ":  " << RootFileName_ << "<br> \n";
	}
      *Frame1HTML_ << "<ul id=\"directory_tree\" class=\"dhtmlgoodies_tree\"> \n "
		   << "<a href=\"#\" onclick=\"treeObj.collapseAll()\">Collapse all</a> \n"
		   << "<a href=\"#\" onclick=\"treeObj.expandAll()\">Expand all</a> \n"
		   << endl;
      
      vFile_[0]->cd();
      RootFileName_ = Description_;
      StripExtension(RootFileName_ );
      UNIXCurrentDirectory_ = Description_;
      StripExtension(UNIXCurrentDirectory_);
      MakeUNIXDirectory(UNIXCurrentDirectory_);
      ChangeToUNIXDirectory(UNIXCurrentDirectory_);
      Layer_ ++;
      //MakeHTML(*Frame1HTML_, vFile_[0], gDirectory);
      MakeHTML(*Frame1HTML_, 0, gDirectory);
      Layer_ --;
      ChangeToUNIXDirectory(UNIXBaseDirectory_+"/"+LocalDirectory_);
    }
  
  *Frame1HTML_ << "</ul> <br>" << endl;
  //Close html documents
  EndHTML(*Frame1HTML_); 

} //end InitiateHTMLProduction()

//void UFAV::MakeHTML(ofstream &html, TFile* F,  TDirectory *dir)
void UFAV::MakeHTML(ofstream &html, unsigned int f,  TDirectory *dir)
{
  if( !GetDirectoryStatus(dir) ) return;//debug

  TFile *F = vFile_[f];
  // Get name of path to current directory within root file
  RootFileSubDirectoryPath_ = ( (char*) strstr( dir->GetPath(), ":" ) );

  // Remove ":/" characters 
  RootFileSubDirectoryPath_.Remove( 0, 2 );
  F->cd(RootFileSubDirectoryPath_);
  //Get list of objects in current directory
  TIter nextkey(dir->GetListOfKeys());
  TKey *key;
  
  //Containers for Directories, 1D Histograms, and 2D Histograms
  TList *DirectoryList = new TList();
  std::vector< TString > vHisto1DList;
  std::vector< TString > vHisto2DList;
  ofstream *Layer1HTML = 0;
  if( Layer_ == 1 )
    {
      Layer1HTML = new ofstream("index.html");
      //Make beginning of html document with / without thumbnail java script 
      TString label( (char*)strstr( gDirectory->GetPath(), ":/" ) );
      label = RootFileName_ + label;
      BeginHTML(*Layer1HTML,label);
      InsertLinkToDirectory(gDirectory, RootFileName_);    
    }
  //Iterate through the contents of this directory
  while( (key = (TKey*)nextkey() ))
    {
      TObject *obj;// new = TObject();
      TString class_ = key->GetClassName();
      TString name = key->GetName(); 
      if( key->GetCycle() > 1 ) continue;
      if( class_ ==  TString("TDirectoryFile") )
	{
	  obj = key->ReadObj();
	  DirectoryList->Add(obj);
	}
      else if ( class_.BeginsWith("TH1") )   //If its a 1D histogram
	{
	  vHisto1DList.push_back(name) ;
	}
      else if( class_.BeginsWith("TH2") )  // If its a 2D histogram
	{
	  vHisto2DList.push_back( name );
	}
      else if( IsItA(key,"TTree") )    // If its a Tree
	{	

	  obj = key->ReadObj();
	  TTree *tree = (TTree*) obj;	  
	  Layer_++;
	  UNIXCurrentDirectory_ = tree->GetName();
	  MakeUNIXDirectory( UNIXCurrentDirectory_ );
	  ChangeToUNIXDirectory( UNIXCurrentDirectory_);
	  ofstream *TreeHTML = new ofstream("index.html");	  
	  TString label( tree->GetName() );
	  BeginHTML(*TreeHTML,label);
	 
	  TObjArray *BranchArray = tree->GetListOfBranches();
	  TIterator *BranchIterator = BranchArray->MakeIterator();
	  TBranch *Branch;
	  TDirectory TreeDirectory(tree->GetName() , tree->GetTitle());
	  InsertLinkToDirectory(&TreeDirectory, RootFileName_+"/"+RootFileSubDirectoryPath_+"/"+tree->GetName());
	  while( (Branch = (TBranch*)BranchIterator->Next()) )
	    {
	      BranchTitle_ = Branch->GetTitle();
	      BranchName_ = Branch->GetName();
	      if(BranchTitle_.Length() == 0 ) BranchTitle_ = BranchName_;

	      DirectoryName_ = dir->GetName();
	      PrintName_ = DirectoryName_;
	      PrintName_ += BranchName_;
	      PrintName_ +="."+Ext_;
	      
	      HTMLReference_ = "";
	      HTMLReference_ += BranchName_;
	      HTMLReference_ +="."+Ext_;

	      InsertLinkToImage(*TreeHTML, BranchName_  , HTMLReference_ ,LogY_, -1.);
	    }
	  EndHTML(*TreeHTML);
	  ChangeToUNIXDirectory("..");
	  Layer_ -- ;
	}
    }
  if( Layer_ > 1 )
    {
      if(GetDirectoryStatus(gDirectory))
	InsertLinkToDirectory(gDirectory, RootFileName_+"/"+RootFileSubDirectoryPath_);      
    }
  TListIter *DirectoryIterator = new TListIter((const TList*)DirectoryList);
  TObject *obj;
  
  while( (obj = (TObject*)DirectoryIterator->Next() ) )
    {    
      TDirectory *RootFileSubDirectory = (TDirectory*)obj;
      if( !GetDirectoryStatus(RootFileSubDirectory) ) continue;
      DirectoryName_ = RootFileSubDirectory->GetName();
      //Get full root file path to current root file directory
      RootFileSubDirectoryPath_ = TString((char*)strstr((const char*) gDirectory->GetPath(),":/"));
      RootFileSubDirectoryPath_.Remove(0,2);
            
      if(RootFileSubDirectoryPath_.Length()) 
	RootFileSubDirectoryPath_ += "/" ; 
     
      RootFileSubDirectoryPath_ += DirectoryName_;
      if(Verbosity_ > 0)
	cout << "[Making HTML Documents for Directory] : " << RootFileName_+"/"+RootFileSubDirectoryPath_ << endl;
      
      F->cd(RootFileSubDirectoryPath_);
      Layer_++;
      TString TempDirectory = UNIXCurrentDirectory_;
      UNIXCurrentDirectory_ = gDirectory->GetName() ;
      MakeUNIXDirectory(UNIXCurrentDirectory_);
      ChangeToUNIXDirectory(UNIXCurrentDirectory_);
      //Create Separate html document for this directory

      ofstream *subdir_html = new ofstream("index.html");
      //Make beginning of html document with / without thumbnail java script
      TString label( (char*)strstr( gDirectory->GetPath(), ":/" ) );
      
      BeginHTML(*subdir_html,RootFileName_ + label);
	         
      //Make Body of HTML document
      //MakeHTML(*subdir_html, F, gDirectory);
      MakeHTML(*subdir_html, f, gDirectory);
      
      //Close html document for current directory
      EndHTML(*subdir_html);

      ChangeToUNIXDirectory("..");
      //Make extra directory if this one is to be superimposed with another
      label.Remove(0,2);
      SuperimposeDirectories_ = SuperimposeThisDirectory( label ) && Mode_ == 1;
      if(SuperimposeDirectories_)
	{
	  for(unsigned int j = 0 ; j < vSuperimposedDirectory1.size() ; j ++ )
	    {
	      if(RootFileSubDirectoryPath_  ==  TString( vSuperimposedDirectory1[j] ) )
		{

		  MakeUNIXDirectory(vSuperimposedDirectoryName[j]);
		  ChangeToUNIXDirectory(vSuperimposedDirectoryName[j]);
		  
		  //Create Separate html document for this directory
		  ofstream *subdir_html_si = new ofstream("index.html");

		  TString label = RootFileName_ + ":/" + vSuperimposedDirectory1[j];
		  label += " <br> Superimposed with <br> ";
		  label +=  RootFileName_ + ":/" + vSuperimposedDirectory2[j];
		  
		  BeginHTML(*subdir_html_si,label);
		  TString ParentDirectoryPath = TString((char*)strstr((const char*) gDirectory->GetMotherDir()->GetPath(),":/"));
		  ParentDirectoryPath.Remove(0,2);

		  if(Verbosity_ > 0 )
		    cout << "[Making HTML Documents for Superimposed Directory] : " << RootFileName_+"/"+ParentDirectoryPath+"/"+vSuperimposedDirectoryName[j] << endl;

		  //Make Body of HTML document
		  TDirectory *Dir2 = (TDirectory*) F -> Get(vSuperimposedDirectory2[j]);
		  MakeHTMLForSuperimposedDirectories(*subdir_html_si, gDirectory, Dir2, vSuperimposedDirectoryName[j]);
		   
		  // Close html document for current directory
		  EndHTML(*subdir_html_si);
		  ChangeToUNIXDirectory("../");
		}
	    }
	}
      
      //Restore subdirectory names for next iteration
      UNIXCurrentDirectory_ = TempDirectory;
      
      //Cd back to parent directory
      F->cd( RootFileSubDirectory->GetMotherDir()->GetPath() );
      Layer_--;
    }

  delete DirectoryIterator;
  
  for(unsigned int i = 0 ; i < vHisto1DList.size() ; i++ )
    {
      //If object is a histogram, print it out and create thumbnail link to current html document
      DirectoryName_ = dir->GetName();
      StripExtension(DirectoryName_);
      PrintName_  =  UNIXCurrentDirectory_ + "/";
      PrintName_ += vHisto1DList[i];
      PrintName_ +="."+Ext_;
      HTMLReference_ = vHisto1DList[i]; 
      HTMLReference_ +="."+Ext_; 
      if( Layer_== 1 )
	{	
	  if( KS_ && Mode_ == 2 && vFile_.size() == 2) 
	    {
	      if(!vUncommonPrefix[0].Length() && !vUncommonPrefix[1].Length())
		InsertLinkToImage(*Layer1HTML , vHisto1DList[i],HTMLReference_ , LogY_, GetKSScore(vHisto1DList[i]));
	      else
		InsertLinkToImage(*Layer1HTML , vHisto1DList[i],HTMLReference_ , LogY_, -1.);
	    }
	  else
	    InsertLinkToImage(*Layer1HTML , vHisto1DList[i],HTMLReference_ , LogY_, -1.);

	  InsertCaption(*Layer1HTML, vHisto1DList[i]);  // Insert Caption if there is one for this histograms
	}
      //If currently in a subdirectory, link image file to subdirectory html document
      if(dir->GetMotherDir()) 
	{
	  if( KS_ && Mode_ == 2 )
	    {
	      bool common = true;
	      TString HistogramPath =   ( (char*) strstr( dir->GetPath(), ":" ) );
	      // Remove ":/" characters 
	      HistogramPath.Remove( 0, 2 );
	     
	      for(unsigned int X = 0; X < vUncommonPrefix.size() ; X++)
		{
		  if ( vUncommonPrefix[X].Length() && HistogramPath.Contains(vUncommonPrefix[X]) )
		    common = false;
		}
	      if(common)
		InsertLinkToImage(html , vHisto1DList[i],HTMLReference_ , LogY_, GetKSScore(vHisto1DList[i]));
	      else
		{		  
		  TString CommonPath = HistogramPath +"/"+ vHisto1DList[i];

		  if( vUncommonPrefix[0].Length() < HistogramPath)
		    CommonPath.Remove(0,vUncommonPrefix[0].Length());
		  else
		    cout << "[Error]:  Uncommon Subdirectory Prefix: " 
			 <<  vUncommonPrefix[0] << "  is a longer string than the Histogram path: " <<  HistogramPath << endl;

		  TString HistogramPath1 = vUncommonPrefix[0]+CommonPath;
		  TString HistogramPath2 = vUncommonPrefix[1]+CommonPath;
		  TH1F *H1 = (TH1F*) vFile_[0]->Get(HistogramPath1);
		  TH1F *H2 = (TH1F*) vFile_[1]->Get(HistogramPath2);
		  if( H1 != NULL && H2 != NULL )
		    InsertLinkToImage(html , vHisto1DList[i],HTMLReference_ , LogY_, GetKSScore(*H1, *H2));
		  else
		    InsertLinkToImage(html , vHisto1DList[i],HTMLReference_ , LogY_, -1.);
		}
	    }
	  else
	    InsertLinkToImage(html, vHisto1DList[i], HTMLReference_ , LogY_, -1);
	  
	  InsertCaption(html, vHisto1DList[i]);  // Insert Caption if there is one for this histograms
	}       	   	     
    }  

  for(unsigned int i = 0 ; i < vHisto2DList.size() ; i++ )
    {
      //If object is a histogram, print it out and create thumbnail link to current html document
      PrintName_ = UNIXCurrentDirectory_;
      PrintName_ +=vHisto2DList[i];
      PrintName_ +="."+Ext_;
      
      HTMLReference_ = vHisto2DList[i]; 
      HTMLReference_ +="."+Ext_; 
      if( Layer_ == 1 )
	{
	  InsertLinkToImage(*Layer1HTML, vHisto2DList[i], HTMLReference_ ,false,-1);
	  InsertCaption(*Layer1HTML, vHisto2DList[i]);
	}
      
      //If currently in a subdirectory, link image file to subdirectory html document
      if(dir->GetMotherDir()) 
	{
	  InsertLinkToImage(html, vHisto2DList[i], HTMLReference_ , false,-1);
	  InsertCaption(html, vHisto2DList[i]);
	}
    }

  if( Layer_ == 1 ) 
    {
      EndHTML(*Layer1HTML);
      Layer1HTML->close();
      delete Layer1HTML;
    }

  if( DirectoryList->GetSize() )
    *Frame1HTML_  << "</ul> \n </li>" << endl;
}

void UFAV::MakeHTMLForSuperimposedDirectories(ofstream &html, TDirectory *dir1, TDirectory* dir2, TString SuperimposedDirectoryName)
{
  RootFileSubDirectoryPath_ = ( (char*) strstr( dir1->GetMotherDir()->GetPath(), ":" ) );
  // Remove ":/" characters  
  RootFileSubDirectoryPath_.Remove( 0, 2 );

  TString TempDirectory = UNIXCurrentDirectory_ ;
  UNIXCurrentDirectory_ = SuperimposedDirectoryName;
  //Get list of objects in current directory
  TIter nextkey1(dir1->GetListOfKeys());
  TKey *key1;
  std::vector< TString > vHistoList;
  std::vector< TH1F > vHistograms1D;
  while( (key1 = (TKey*)nextkey1()) )
    {
      TString class_ = key1->GetClassName();
      if( class_.BeginsWith("TH1") )
	{
	  vHistoList.push_back( key1->GetName() ) ;
	  TObject *obj = key1->ReadObj();
	  TH1F histo(*(TH1F*)obj);
	  TH1F h = *(TH1F*)histo.Clone(histo.GetName());
	  vHistograms1D.push_back(h);
	}
      else continue;
    }
  if( vHistoList.size() )
    {                  
      HTMLReference_=RootFileName_+"/"+RootFileSubDirectoryPath_+"/"+SuperimposedDirectoryName+"/index.html";
      InsertLinkToSuperimposedDirectory(SuperimposedDirectoryName,HTMLReference_);
    }
  for(unsigned int i = 0 ; i < vHistoList.size() ; i++ )
    {
      //If object is a histogram, print it out and create thumbnail link to current html document          
      TString HistogramName_ =  vHistoList[i];

      while(HistogramName_.Contains("_"))
	RemovePrefix(HistogramName_,"_");

      HistogramName_ = "_" + HistogramName_;
      TIter nextkey2(dir2->GetListOfKeys());
      TKey *key2;
      while ( (key2= (TKey*) nextkey2() ) )
	{
	  TString classname2 = key2->GetClassName() ;
	  if(classname2.BeginsWith("TH1"))
	    {
	      TString keyname2 = key2->GetName();
	      if(keyname2.EndsWith(HistogramName_))
		{
		  HTMLReference_ = HistogramName_;
		  if(HTMLReference_.BeginsWith("_") || HTMLReference_.BeginsWith("/"))
		    HTMLReference_.Remove(0,1);

		  if(KS_) 
		    {
		      TObject *obj = key2->ReadObj();
		      TH1F histo(*(TH1F*)obj);
		      TH1F h2 = *(TH1F*)histo.Clone(histo.GetName());
		      InsertLinkToImage(html , HTMLReference_ , HTMLReference_ + "."+Ext_ , LogY_ , GetKSScore(vHistograms1D[i],h2));
		    }
		  else
		    InsertLinkToImage(html , HTMLReference_ , HTMLReference_ + "."+Ext_ , LogY_ , -1.);
		  
		  InsertCaption(html,HistogramName_);
		}
	    }
	}
    }
  UNIXCurrentDirectory_ = TempDirectory;
} //MakeHTMLForSuperimposedDirectories


void UFAV::MakePlotsForSuperimposedDirectories(unsigned int f )
{
  TString pwd = gSystem->pwd();
  if( f >= vSuperimposedDirectory1.size() ) return;
  
  TDirectory* Directory1 = (TDirectory*)gDirectory->Get(vSuperimposedDirectory1[f]);
  TDirectory* Directory2 = (TDirectory*)gDirectory->Get(vSuperimposedDirectory2[f]);
  
  RootFileSubDirectoryPath_ = TString( (char*)strstr( Directory1->GetMotherDir()->GetPath(), ":" ) );
  // Remove ":/" characters 
  RootFileSubDirectoryPath_.Remove( 0, 2 );
  RootFileSubDirectoryPath_+="/";
  RootFileSubDirectoryPath_+=vSuperimposedDirectoryName[f];

  TString DirectoryName1 = vSuperimposedDirectory1[f];
  TString DirectoryName2 = vSuperimposedDirectory2[f];
  TString SuperimposedDirectoryName = vSuperimposedDirectoryName[f];
  while(DirectoryName1.BeginsWith("/"))
    DirectoryName1.Remove(0,1);

  MakeUNIXDirectory(DirectoryName1);
  ChangeToUNIXDirectory(DirectoryName1+"/../");
     
  DirectoryName_ = SuperimposedDirectoryName;
  MakeUNIXDirectory(DirectoryName_);
  ChangeToUNIXDirectory(DirectoryName_);

  //Get list of objects in current directory
  TIter nextkey1(Directory1->GetListOfKeys());
  TKey *key1;

  std::vector< TH1F > vHistograms1D;
  while( (key1 = (TKey*)nextkey1() ) )  //loop through list of objects
    {
      if ( IsItA(key1,"TH1"))
	{
	TObject *obj = key1->ReadObj();
	TH1F histo(*(TH1F*)obj);
	vHistograms1D.push_back(histo);
	}       
    }  
  if(vHistograms1D.size() && PrintHistograms_)
    {
      std::vector<Int_t> vRebin;
      for(unsigned int i=0; i < vHistograms1D.size(); i++)
	{
	  HistogramTitle_ = vHistograms1D[i].GetTitle();
	  HistogramName_  = vHistograms1D[i].GetName(); 
	  while(HistogramName_.Contains("_"))
	    RemovePrefix( HistogramName_, "_");

	  if(!HistogramName_.Length())
	    continue;  
	  
	  HistogramName_ = "_" + HistogramName_;

	  TIter nextkey2(Directory2->GetListOfKeys());
	  TKey *key2;

	  while( (key2 = (TKey*) nextkey2() ))
	    {
	      if( IsItA(key2,"TH1") )
		{
		  TString keyname2 = key2->GetName();
		  if(keyname2.EndsWith(HistogramName_))
		    {
		      TH1F MatchingHistogram(*((TH1F*) Directory2->Get(keyname2)));   // Get matching histogram in other directory
		      if( HistogramTitle_.Length() == 0 ) HistogramTitle_ = HistogramName_;	      		  
		      if(HistogramName_.BeginsWith("_") ) HistogramName_.Remove(0,1);

		      vHistograms1D[i].SetName(DirectoryName1);
		      vHistograms1D[i].SetTitle(HistogramName_);      
		      MatchingHistogram.SetTitle(HistogramName_);
		      MatchingHistogram.SetName(DirectoryName2);

		      float x1ndc = 0.70;
		      float x2ndc = 0.97;
		      float y1ndc = 0.68;
		      float y2ndc = 0.83;
		      float dyNDC=0.15;
		      TPaveStats* stats_1 = new TPaveStats(x1ndc, y1ndc, x2ndc,y2ndc, "NDC");
		      TPaveStats* stats_2 = new TPaveStats(x1ndc, y1ndc+dyNDC, x2ndc,  y2ndc+dyNDC, "NDC");
		      
		      if(HistogramName_.BeginsWith("_"))
			HistogramName_.Remove(0,1);

		      PrintName_ = HistogramName_;
		      
		      FormatSuperimposedHistograms(vHistograms1D[i], MatchingHistogram , 
						   vSuperimposedDirectory1Format[f], vSuperimposedDirectory2Format[f], 
						   stats_1, stats_2);
		      PrintSuperimposedHistograms(vHistograms1D[i], MatchingHistogram, 
						  vSuperimposedDirectory1Format[f], vSuperimposedDirectory2Format[f], 
						  stats_1, stats_2,PrintName_); 
		      
		      if(!HistogramName_.BeginsWith("_"))
			HistogramName_ = "_" + HistogramName_;
		      
		      delete stats_1;
		      delete stats_2;
		    }
		}
	    }
	}
      vHistograms1D.clear();
    }
  ChangeToUNIXDirectory(pwd);
}

void UFAV::MakePlots(unsigned int f_, TDirectory *dir)
{

  TFile *F = vFile_[f_];  
  if( !GetDirectoryStatus(dir) ) return;
  
  // Get name of path to current directory within root file
  RootFileSubDirectoryPath_ = TString( (char*)strstr( dir->GetPath(), ":" ) );
  
  // Remove ":/" characters 
  RootFileSubDirectoryPath_.Remove( 0, 2 );
  F->cd(RootFileSubDirectoryPath_);
  
  //Get list of objects in current directory
  TIter nextkey(dir->GetListOfKeys());
  TKey *key;

  std::vector<TH1F*> vHistograms1D;
  std::vector<TH2F*> vHistograms2D;

  while( (key = (TKey*)nextkey() ) )  //loop through list of objects
    {
      if( key->GetCycle() > 1 ) continue;

      if( IsItA(key,"TH1") ) 
	{
	  HistogramName_ = RootFileSubDirectoryPath_;
	  if( HistogramName_.Length() )  HistogramName_ += "/";
	  HistogramName_ += key->GetName() ;
	  TH1F *histo = (TH1F*) F->Get(HistogramName_); 
	  if(histo!=NULL)
	    vHistograms1D.push_back(histo);
	  else 
	    cout << "[ERROR]: Histogram : " << HistogramName_ << " is NULL" << endl; 
	}          
      else if( IsItA(key,"TH2") )
	{
          HistogramName_ = RootFileSubDirectoryPath_;
          if( HistogramName_.Length() ) HistogramName_ += "/";
          HistogramName_ += key->GetName();
          TH2F *histo = (TH2F*) F->Get(HistogramName_);
	  if(histo != NULL )
	    vHistograms2D.push_back(histo);
	  else 
	    cout << "[ERROR]: Histogram : " << HistogramName_ << " is NULL" << endl; 
	}   
      else if( IsItA(key,"TTree") )
	{
	  TObject *obj = key->ReadObj();
	  TTree *tree = (TTree*) obj;
	  MakeUNIXDirectory(tree->GetName());
	  Layer_++;
	  ChangeToUNIXDirectory(tree->GetName());
	  DirectoryName_ = dir->GetName();
	  PrintTTreeHistograms(tree);
	  Layer_--;
	}
      //If object is a directory, then link it to frame html document,
      //create a new html document for this directory, and start MakePlots() function recursively
      else if( IsItA(key, "TDirectory") )
	{
	  TObject *obj = key->ReadObj();
	  TDirectory *RootFileSubDirectory = (TDirectory*)obj;
	  if( !GetDirectoryStatus(RootFileSubDirectory) ) continue;
	  TString TheName= RootFileSubDirectory->GetName();
	  MakeUNIXDirectory( RootFileSubDirectory->GetName() ) ;

	  ChangeToUNIXDirectory ( RootFileSubDirectory->GetName() ) ;
	  Layer_++;

	  //Begin next iteration: Print histos and compose body of html document for this directory
	  MakePlots(f_, RootFileSubDirectory);

	  ChangeToUNIXDirectory("..");
	  Layer_--;

	  //Cd back to parent directory
	  //RootFileSubDirectoryPath_ = TString( (char*)strstr( RootFileSubDirectory->GetMotherDir()->GetPath(), ":" ) );
	  RootFileSubDirectoryPath_ = TString( (char*)strstr( RootFileSubDirectory->GetPath(), ":" ) );
	  RootFileSubDirectoryPath_.Remove( 0, 2 );
	  F->cd( RootFileSubDirectoryPath_ );
	}
    }

  RootFileSubDirectoryPath_ = TString( (char*)strstr( dir->GetPath(), ":" ) );
  RootFileSubDirectoryPath_.Remove( 0, 2 );
  F->cd( RootFileSubDirectoryPath_ );
  if(PrintHistograms_)
    {
      TCanvas c_;
      c_.cd();
      unsigned index = 0;
     
      for(; index < vFile_.size(); index++)  
	if(F == vFile_[index]) break;

      for(unsigned int i=0; i < vHistograms1D.size(); i++)
	{
	  if( Mode_ == 1 ) 
	    {	     
	      FormatHistogram1D(*vHistograms1D[i],index);	
	      PrintHistogram1D(*vHistograms1D[i],c_,index);
	    }
	  else if( Mode_ == 2 ) 
	    {
	      float x1ndc = 0.80;
	      float x2ndc = 0.97;
	      float y1ndc = 0.84;
	      float y2ndc = 0.99;
	      float dyNDC=0.00;
	      std::vector<TPaveStats*> vStats;
	      vStats.push_back( new TPaveStats(x1ndc, y1ndc, x2ndc,  y2ndc, "NDC") );
	      vStats[0]->SetY1NDC(y1ndc); 
	      vStats[0]->SetY2NDC(y2ndc); 
	      vStats[0]->SetX1NDC(x1ndc); 
	      vStats[0]->SetX2NDC(x2ndc); 
	      std::vector<TH1F*> vHistogramCollection;
	      vHistogramCollection.push_back( vHistograms1D[i] );
	      float ymax = vHistograms1D[i]->GetMaximum();
	      float ymin = vHistograms1D[i]->GetMinimum();
	      float nmax = (float)vHistograms1D[i]->GetEntries();
	      float nmin = nmax;
	      int xmin_bin =1;
	      int xmax_bin =vHistograms1D[i]->GetNbinsX();
	      if( vHistograms1D[i]->GetEntries() && CaptureTails_ ) 
		{
		  while(vHistograms1D[i]->GetBinContent(xmin_bin) == 0)
		    xmin_bin++;
		  while(vHistograms1D[i]->GetBinContent(xmax_bin) == 0)
		    xmax_bin--;
		}

	      float xmin = vHistograms1D[i]->GetBinLowEdge(xmin_bin);
	      float xmax = vHistograms1D[i]->GetBinLowEdge(xmax_bin+1);
	      int rebinmin = GetRebin(*vHistograms1D[i]);
	      float rmsmax =  vHistograms1D[i]->GetRMS();
	      float meanmax = vHistograms1D[i]->GetMean();
	      bool HasEntries = vHistograms1D[i]->GetEntries();
	      bool AllEmpty   = vHistograms1D[i]->GetEntries() ? false : true;
	      TString HistogramPath((char*)strstr((const char*)gDirectory->GetPath(),":/"));
	      HistogramPath.Remove(0,2);
	      if(HistogramPath.Length())  HistogramPath+="/";
	      HistogramName_= vHistograms1D[i]->GetName();
	      
	      HistogramPath+=HistogramName_;
	      FormatHistogram1D( *vHistogramCollection[0], 0 );

	      if( HistogramTitle_.Length() == 0 )
		HistogramTitle_ = HistogramName_;

	      PrintName_ = HistogramName_;
	      for(unsigned int f = 1 ; f < vFile_.size(); f++ )
		{
		  if ( vUncommonPrefix[0].Length() && HistogramPath.Contains(vUncommonPrefix[0])) //Check 0th root file for uncommon prefix
		    {
		      TString CommonPath = HistogramPath ;
		      if( vUncommonPrefix[0].Length() < CommonPath)
			CommonPath.Remove(0,vUncommonPrefix[0].Length());
		      else
			{
			  cout << "[Error]:  Uncommon Subdirectory Prefix: " 
			       <<  vUncommonPrefix[0] << "  is a longer string than the Histogram path: " <<  HistogramPath << endl;
			  continue;
			}
		      HistogramPath = vUncommonPrefix[f]+CommonPath;
		    }
		  
		  TH1F *H = (TH1F*) vFile_[f]->Get(HistogramPath);
		  if( H == NULL )
		    {
		      cerr << "[ERROR] : Histogram :  " << HistogramPath << "  does not exist in File: " << vFile_[f]->GetName() << endl;
		      continue;
		    }
		  else
		    {
		      vHistogramCollection.push_back( H );
		      if(nmax && H->GetEntries())
			{
			  if( H->GetMaximum()/H->GetEntries() > ymax/nmax )
			    {
			      ymax = H->GetMaximum();
			      nmax = H->GetEntries();
			    }
			  if( H->GetMinimum()/H->GetEntries() < ymin/nmin )
			    {
			      ymin = H->GetMinimum();
			      nmin = H->GetEntries();
			    }
			}
			  //ymax = TMath::Max(ymax, (float)H->GetMaximum());
			  //ymin = TMath::Min(ymin, (float)H->GetMinimum());
		      
		      xmin_bin = 1; xmax_bin= H->GetNbinsX();
		      if( H->GetEntries() && CaptureTails_ )
			{
			  while(H->GetBinContent(xmin_bin) == 0)
			    xmin_bin++;
			  while(H->GetBinContent(xmax_bin) == 0)
			    xmax_bin--;
			  
			  xmax = TMath::Max(xmax, (float)H->GetBinLowEdge(xmax_bin+1) );
			  xmin = TMath::Min(xmin, (float)H->GetBinLowEdge(xmin_bin) );
			}
		      rebinmin = TMath::Min(rebinmin, GetRebin(*H));
		      rmsmax = TMath::Max(rmsmax, (float)H->GetRMS());
		      meanmax = TMath::Max(meanmax, (float)H->GetMean());
		      if( !H->GetEntries() )
			HasEntries = false;
		      else
			AllEmpty = false;
		    }
		}
	      if (ymin == 0 ) ymin = 1.E-1;
	      for(unsigned int f = 1 ; f < vHistogramCollection.size(); f++ )
		{
		  FormatHistogram1D(*vHistogramCollection[f], f);
		  y2ndc = vStats.back()->GetY1NDC() - dyNDC -0.005;
		  y1ndc = y2ndc - ( vStats.back()->GetY2NDC() - vStats.back()->GetY1NDC() ) ;
		  vStats.push_back(new TPaveStats(x1ndc, y1ndc, x2ndc,  y2ndc, "NDC"));
		  vStats.back()->SetX1NDC(x1ndc);
		  vStats.back()->SetX2NDC(x2ndc);
		  vStats.back()->SetY1NDC(y1ndc);
		  vStats.back()->SetY2NDC(y2ndc);
		  vStats.back()->SetOption("NDC");
		}

	      for(unsigned int f = 0 ; f < vHistogramCollection.size(); f++ )
                {
		  vStats[f]->SetTextSize(.03);
                  vStats[f]->SetTextColor( vTH1Format[f].linecolor );
                  vStats[f]->SetShadowColor(kNone);
                  vStats[f]->SetBorderSize(6);

                  vHistogramCollection[f]->GetListOfFunctions()->Add(vStats[f]);
                  vStats[f]->SetParent(vHistogramCollection[f]->GetListOfFunctions());
		  
		  if(ymax)
		    {

		      if(rebinmin > 1 && Rebin_ )
			{
			  vHistogramCollection[f]->Rebin(rebinmin);
			  if( nmin && nmax)
			    {
			      float high =  ymax/nmax*YMaxLinearScale_ * ((float)rebinmin) * vHistogramCollection[f]->GetEntries();
			      float low  =  ymin/nmin*((float)rebinmin)* vHistogramCollection[f]->GetEntries();
			      vHistogramCollection[f]->GetYaxis()->SetRangeUser(low,high);
			    }
			  //vHistogramCollection[f]->GetYaxis()->SetRangeUser(ymin, ymax*YMaxLinearScale_ * ((float)rebinmin));

			}
		      else
			{
			  if(nmin && nmax)
			    {
			      float high =  ymax/nmax*YMaxLinearScale_ * vHistogramCollection[f]->GetEntries();
			      float low  =  ymin/nmin*vHistogramCollection[f]->GetEntries();
			      vHistogramCollection[f]->GetYaxis()->SetRangeUser(low,high);
			    }
			  //vHistogramCollection[f]->GetYaxis()->SetRangeUser(ymin, ymax*YMaxLinearScale_);
			}


		      if( f == 0 ) 
			vHistogramCollection[f]->Draw(vTH1Format[f].drawoption);
		      else 
			vHistogramCollection[f]->Draw(vTH1Format[f].drawoption+"sames");
		      
		      if(CaptureTails_)
			{
			  if( xmin < 0 ) //ensure symmetric distributions
			    {
			      xmin =TMath::Abs(xmin) > xmax ? xmin : -1.*xmax;
			      xmax = xmax > TMath::Abs(xmin) ? xmax : -1.*xmin;
			    }
			  vHistogramCollection[f]->GetXaxis()->SetRangeUser(xmin*1.1,xmax*1.1);
			}
		      else if(Zoom_)
			{
			  vHistogramCollection[f]->GetXaxis()->SetRangeUser(meanmax - Sigmas_ * rmsmax, meanmax + Sigmas_* rmsmax ); 
			}

		    }
		}
	      
	      //redraw histograms without fill pattern for better viewing
              for(unsigned int f = 0 ; f < vHistogramCollection.size(); f++ )
                {
		  if(ymax)
                    {
		      if( vTH1Format[f].fillcolor == (unsigned)kNone )
			{
			  vHistogramCollection[f]->Draw(vTH1Format[f].drawoption+"sames");
			}
		    }
		}

	      // draw stat boxes
	      for(unsigned int f = 0 ; f < vStats.size() ; f ++ ) vStats[f]->Draw("sames");
	      
	      RemoveSubstring(RootFileSubDirectoryPath_," ");
	      PrintName_.ReplaceAll(";","_");
	      if( AllEmpty || ymax <= 0. )
		LinkToIcon(NoDataIcon_, PrintName_+"."+Ext_);
	      else
		{
		  if(Verbosity_)
		    {
		      cout << "[Printing 1D Histogram] : " << RootFileSubDirectoryPath_ << "/" << PrintName_<<"."<<Ext_ << endl;
		    }
		  c_.Print(PrintName_+"."+Ext_,Ext_);
		}
	      if( LogY_ ) 
		{
		  if( ymax > 0. && ymin >= 0.  && HasEntries ) 
		    {
		      c_.SetLogy(1);
		      if(Verbosity_)
			cout << "[Printing 1D Histogram] : " << RootFileSubDirectoryPath_<<"/"<<PrintName_<<"_log."<<Ext_ << endl;


		      //Redraw Histograms with suitable ymax for log plot
		      for(unsigned int f = 0 ; f < vHistogramCollection.size(); f++ )
			{
			  
			  if( Rebin_ && rebinmin > 1 )
			    {
			      if(nmin && nmax)
				{
				  
				  float high =  ymax/nmax*YMaxLogScale_ * ((float)rebinmin) * vHistogramCollection[f]->GetEntries();
				  float low  =  ymin/nmin * ((float)rebinmin) * vHistogramCollection[f]->GetEntries();
				  vHistogramCollection[f]->GetYaxis()->SetRangeUser(low, high);
				}
			      //vHistogramCollection[f]->GetYaxis()->SetRangeUser(ymin, ymax*YMaxLogScale_*((float)rebinmin));
			    }
			  else
			    {
			      if(nmin && nmax)
				{
				  float high =  ymax/nmax*YMaxLogScale_ * vHistogramCollection[f]->GetEntries();
				  float low  =  ymin/nmin* vHistogramCollection[f]->GetEntries();
				  vHistogramCollection[f]->GetYaxis()->SetRangeUser(low, high);
				}
			      //vHistogramCollection[f]->GetYaxis()->SetRangeUser(ymin, ymax*YMaxLogScale_);
			    }


			  if( f == 0 ) 
			    vHistogramCollection[f]->Draw(vTH1Format[f].drawoption);
			  else
			    vHistogramCollection[f]->Draw(vTH1Format[f].drawoption+"sames");
			}
		      for(unsigned int f = 0 ; f < vHistogramCollection.size(); f++ )
			if( vTH1Format[f].fillcolor == (unsigned)kNone )
			  vHistogramCollection[f]->Draw(vTH1Format[f].drawoption+"sames");

		      //draw stat boxes
		      for(unsigned int f = 0 ; f < vStats.size() ; f ++ ) vStats[f]->Draw("sames");

		      //Print log plot
		      c_.Print(PrintName_+"_log."+Ext_,Ext_);
		      c_.SetLogy(0);
		    }
		  else 
		    {
		      LinkToIcon(NoDataIcon_, PrintName_+"_log."+Ext_);
		    }
		}
	      c_.Clear();
	      while(vStats.size())
		{
	      	  delete vStats.back();
		  vStats.pop_back();
		}
	      vStats.clear();
	    }
	} //for( vHistograms1D...)
      vHistograms1D.clear();
      
      
      for(unsigned int i = 0 ; i < vHistograms2D.size(); i++ )
	{
	  PrintHistogram2D(*vHistograms2D[i], c_ );
	}

      vHistograms2D.clear();
    }
}

void UFAV::MakeHTML(TString UNIXDirectory )
{

  TString HTMLName = UNIXDirectory;
  if(!UNIXDirectory.EndsWith("/"))
    HTMLName+="/";

  HTMLName+="UFAV.html";
  ofstream *BasicHTML = new ofstream(HTMLName);
  
  //Get List of image files
  UNIXCommand_  = "ls -1 " ;
  UNIXCommand_ += UNIXDirectory;
  UNIXCommand_ += "  | grep \"."+Ext_+"\" > TEMPX__.txt" ; 
  system( UNIXCommand_ );
  
  fstream imageList("TEMPX__.txt");
  if( !imageList )
    {
      cerr << "[ERROR]: UFAV could not create and open the image list for directory : " << UNIXDirectory<< endl;
      return;
    }
  
  BeginHTML(*BasicHTML, "UF Analysis Viewer");

  TString imageFile = "";
  while( imageList >> imageFile )
    {
      InsertLinkToImage(*BasicHTML, imageFile, imageFile,false, -1.);
    }
  
  //Close html document for current directory
  EndHTML(*BasicHTML);
}

void UFAV::SuperimposeDirectories( TString dir1, TString  dir2, TString dirname)
{


  TH1Format f1; 
  f1.fillcolor=kRed+1;
  f1.fillstyle=1001;
  f1.linecolor=kRed+1;
  f1.drawoption="";
  TH1Format f2;
  f2.markercolor=kBlack;
  f2.drawoption="pe1";
  SuperimposeDirectories(dir1,dir2,dirname,f1,f2);
}


void UFAV::SuperimposeDirectories( TString dir1, TString  dir2, TString dirname, TH1Format& f1, TH1Format& f2)
{
  SuperimposeDirectories_ = true;
  while(dir1.EndsWith("/"))
    dir1.Remove(dir1.Length()-1,1);
  while(dir1.BeginsWith("/"))
    dir1.Remove(0,1);
  while(dir2.EndsWith("/"))
    dir2.Remove(dir2.Length()-1,1);
  while(dir2.BeginsWith("/"))
    dir2.Remove(0,1);
    
  while(dirname.BeginsWith("/"))
    dirname.Remove(0,1);
  while(dirname.EndsWith("/"))
    dirname.Remove(dirname.Length()-1,1);

  if(dirname.Contains("/"))
    {
      cerr << "[Error]:  You cannot include backslashes in target directory name for output of superimposed directories. " ; 
      while(dirname.Contains("/"))
	{
	  dirname = TString( (const char*) strstr(dirname,"/"));
	  dirname.Remove(0,1);
	}
      
      if(!dirname.Length())
	dirname = dir1+"_vs_"+dir2 ;
      
      cout << "UFAV will use target directory name: " << dirname << " instead . " << endl;
    }      

  vSuperimposedDirectory1.push_back(dir1);
  vSuperimposedDirectory2.push_back(dir2);
  vSuperimposedDirectory1Format.push_back( f1 );
  vSuperimposedDirectory2Format.push_back( f2 );
  if(dirname.Length() == 0)
    {
      TString a=dir1;
      TString b=dir2;

      while(a.Contains("/"))
	RemovePrefix(a,"/");
      while(b.Contains("/"))
	RemovePrefix(b,"/");

      dirname=a+"_vs_"+b;
    }
  vSuperimposedDirectoryName.push_back(dirname);
}


void UFAV::FormatHistogram1D(TH1& H, unsigned int x)
{
  H.SetLineWidth(vTH1Format[x].linewidth);
  H.SetLineColor(vTH1Format[x].linecolor);
  H.SetLineStyle(vTH1Format[x].linestyle);
  H.SetFillStyle(vTH1Format[x].fillstyle);
  H.SetFillColor(vTH1Format[x].fillcolor);
  H.SetMarkerColor(vTH1Format[x].markercolor);
  H.SetMarkerSize(vTH1Format[x].markersize);
  H.SetMarkerStyle(vTH1Format[x].markerstyle);
  H.Scale(vWeight[x]);
  H.SetStats(1);
  
  if( Mode_ == 2 ) 
    {
      H.SetName(vFileDescription[x]);
    }
  else if(CaptureTails_ && H.GetEntries())
    {
      int xmin_bin = 1; int xmax_bin = H.GetNbinsX();
      while(H.GetBinContent(xmin_bin) == 0)
	xmin_bin++;
      while(H.GetBinContent(xmax_bin) == 0)
	xmax_bin--;

      float xmin = H.GetBinLowEdge( xmin_bin );
      float xmax = H.GetBinLowEdge( xmax_bin + 1 );
      
      if( xmin < 0 ) //ensure symmetric distributions                                                                                           
	{
	  xmin = TMath::Abs(xmin) > xmax ? xmin : -1.*xmax;
	  xmax = xmax > TMath::Abs(xmin) ? xmax : -1.*xmin;
	}
      
      H.GetXaxis()->SetRangeUser(xmin*1.1,xmax*1.1);
    }
  else if (Zoom_)
    H.GetXaxis()->SetRangeUser(H.GetMean() - Sigmas_ * H.GetRMS(), H.GetMean() + Sigmas_* H.GetRMS() ); 

}

Int_t UFAV::GetRebin(const TH1& histo)
{
  // calculate and set range and number of bins
  float RMS =  histo.GetRMS();
  float Nbins = histo.GetNbinsX();
  float min = histo.GetXaxis()->GetXmin();
  float max = histo.GetXaxis()->GetXmax();
  float dX = max - min;
  float dNdX = 1;
  //if (RMS>0 ) 
  if (RMS > 5.*dX/Nbins ) 
    {
      dNdX = 100. / ( 10 * RMS);
    }
  else 
    return 1;
  
  Int_t rebinning = 1;
  if ((dX * dNdX)>0) 
    rebinning = (Int_t)(double(Nbins) / (dX * dNdX));
  
  /* 
     //RCR:  Disabling this block of code which tries to enforce that the rebinning factor is
     // a divisor of the number of bins in the histogram.  Many histograms in DQM seem to have odd numbers of bins....

     while(rebinning > 1)  // To assure rebinning divides number of bins evenly and isn't greater than RebinMax
     {
     if( (Int_t)Nbins %  rebinning == 0 && rebinning <= RebinMax_  ) break;
     //if( rebinning <= RebinMax_  ) break;
     else rebinning--;
     }  
  */

  return rebinning; 
}

void UFAV::Rebin(TH1& histo)
{
  if(GetRebin(histo) > 1)
    histo.Rebin( GetRebin(histo) );
}

void UFAV::LinkToIcon(TString iconname, TString target)
{
  TString DotDotSlash = "";
  for(int j = 0; j <= Layer_ ; j++ )
    {
      DotDotSlash+="../";
      if( FileExists(DotDotSlash+"images/"+iconname) )
	break;
    }
  
  if(FileExists(target))  //remove link if it already exists
    {
      UNIXCommand_ = "rm "+target;
      system(UNIXCommand_);
    }

  UNIXCommand_ = "ln -s -f ";
  UNIXCommand_ += DotDotSlash;
  UNIXCommand_ += "images/";
  UNIXCommand_ += iconname;
  UNIXCommand_ += " ";
  UNIXCommand_ += target;

  system(UNIXCommand_);
}


void UFAV::InsertLinkToDirectory(TDirectory* dir, TString HTMLReference)
{
  TString label( (char*)strstr( dir->GetPath(), ":/" ) );
  label.Remove( 0, 2 );

  if(Layer_ == 1)
    label = HTMLReference;

  RemoveSubstring(label," ");
  RemoveSubstring(HTMLReference," " );
  
  //Check if this directory has subdirectories
  TIter nextkey(dir->GetListOfKeys());
  TKey *key;
  //Iterate through the contents of this directory 
  bool HasSubDirectories = false;
  while( (key = (TKey*)nextkey() ))
    {
      TString class_ = key->GetClassName();
      if( class_ ==  TString("TDirectoryFile") || class_ == TString("TTree") )
        {
	  HasSubDirectories = true;
        }
    }

  *Frame1HTML_ << "<li id=\""
	       << RootFileName_ << ":/" << label
	       << "\" noDrag=\"true\" noSiblings=\"true\" noDelete=\"true\" noRename=\"true\">"
	       << "<a target=\"content\" "
	       << "href=\"" ;
  if( HTMLReference.EndsWith("index.html") ) 
    *Frame1HTML_  << HTMLReference << " \" ";
  else 
    *Frame1HTML_  << HTMLReference<<"/index.html" << " \" ";
  

  if( Layer_ > 1  )
    *Frame1HTML_  << " >" << dir->GetName() << "</a> ";
  else
    *Frame1HTML_  << " >" << label << "</a> ";

  if( HasSubDirectories ) 
    *Frame1HTML_ << "\n <ul> \n";
  else
    *Frame1HTML_ << "</li> \n"; 
  
}


void UFAV::InsertLinkToSuperimposedDirectory( TString SuperimposedDirName, TString HTMLReference)
{
  RemoveSubstring(SuperimposedDirName," ");
  RemoveSubstring(HTMLReference," ");
  
  while(SuperimposedDirName.Contains("/"))
    {
      SuperimposedDirName = TString( (const char*) strstr(SuperimposedDirName,"/"));
      SuperimposedDirName.Remove(0,1);
    }

  *Frame1HTML_ << "<li id=\""
	       << SuperimposedDirName
	       << "\" noDrag=\"true\" noSiblings=\"true\" noDelete=\"true\" noRename=\"true\">"
	       << "<a target=\"content\" "
	       << "href=\"" << HTMLReference << " \" "
	       << " >"
	       << SuperimposedDirName
	       << "</a> "
	       << "</li> \n"; 
 

}


void UFAV::InsertLinkToImage(ofstream &html, TString imagename, TString HTMLReference, bool insert_log, double ks_score)
{  
  imagename.ReplaceAll(";","_");
  HTMLReference.ReplaceAll(";","_");
  int index = (int)HasCaption(imagename);
  Bool_t flag = true;
  if( index >= 0  && index < (int)vCaption.size() )
    flag = true;
  
  TString DotDotSlash = "";
  for(int j = 0; j < Layer_ ; j++ )
    {
      DotDotSlash+="../";
    }
  
  html << "<a href=\""
       << string(HTMLReference)
       << "\" onMouseOver=\"ThumbnailPlot(\'"
       << string(HTMLReference)
       << " \'), ClearNotes(),  "
       << "ThumbnailNote(" <<flag<<",'div_"<<imagename<<"'  ) \"> "
       <<"<img src=\""
       << string(imagename+"."+Ext_)
       << "\" style=\"height:22px; width:25px;\">"
       << "<small>"
       << string(imagename) ;


  html << "</small>";  
  
  if( insert_log )
    {
      html << "</a>     \n";
      TString suffix( (char*)strstr(HTMLReference,"."));
      TString base(HTMLReference);
      base.Remove( base.Length() - suffix.Length() , suffix.Length() );
      if(base.EndsWith(".")) 
	{
	  base.Remove( base.Length() - 1 , 1 );
	  suffix = "."+suffix;
	}
      base += "_log" + suffix;
      
      html << "<a href=\""
	   << string(base)
	   << "\" onMouseOver=\"ThumbnailPlot(\'"
	   << string(base)
	   << "\'), ClearNotes(), "
	   << "ThumbnailNote(" <<flag<<",'div_"<<imagename<<"'  ) \" >" 
       	   << "<small>"
	   << "(log scale)" 
	   << "</small>"
	   << "</a>  ";
      if( KS_ && ks_score >= 0. && ks_score <=1.0001 ) 
	{
          if( ks_score < 1.E-6 )
            {
              html << "<blink><span style=\"color:red\">  KS : "
		   <<  ks_score << "</span></blink>";
            }
          else
            {
              html << "<span style=\"color:green\"> KS : "
		   <<  ks_score << "</span>";
            }
	}     
      html << "<br>   \n";  
    }  
  else
    {
      html << "</a>     \n";
      TString suffix( (char*)strstr(HTMLReference,"."));
      TString base(HTMLReference);
      base.Remove( base.Length() - suffix.Length() , suffix.Length() );
      if(base.EndsWith(".")) 
	{
	  base.Remove( base.Length() - 1 , 1 );
	  suffix = "."+suffix;
	}
      TString Name;
      Name = base +"_log" + suffix;
      
      html << "<a href=\""
	   << string(Name)
	   << "\" onMouseOver=\"ThumbnailPlot(\'"
	   << string(Name)
	   << "\'), ClearNotes(), "
	   << "ThumbnailNote(" <<flag<<",'div_"<<imagename<<"'  ) \" >" 
       	   << "<small>"
	   << "(log scale)" 
	   << "</small>"
	   << "</a> \n";
      if( KS_&& ks_score >= 0. && ks_score <=1.0001 )
	{
	  html  << "&nbsp &nbsp ";
	  if( ks_score < 1.E-6 )
	    {
	      html << "<blink><span style=\"color=red\">  KS : "
		   <<  ks_score << "</span><blink>";
	    }
	  else
	    {
	      html << "<span style=\"color=green\"> KS : "
		    <<  ks_score << "</span>";
	    }
	}      
      html << "<br>   \n";        
    }      
}

void UFAV::InsertCaption(ofstream& html,TString HistogramName)
{
  html << "<div id =\"div_" << HistogramName << "\"> " ;
  int index = HasCaption(HistogramName);
  if(index >= 0 && index < (int)vCaption.size())
    {
      TString caption((char*) strstr( (const char*)vCaption[index], "::"));
      caption.Remove(0,2); //Remove delimiter
      html << caption ;
     }
  html << " </div> \n";
}

int UFAV::HasCaption( TString HistogramName ) 
{
  for(unsigned int j = 0 ; j < vCaption.size(); j++ )
    {
      TString SubString=gDirectory->GetName();
      SubString+="/";
      SubString+=HistogramName;
      SubString+="::";
      if( vCaption[j].BeginsWith(SubString) )
	return (int)j;
      else continue;
    }
  return -1;
  
}


void UFAV::MakeUNIXDirectory(TString directory_name)
{
  
  RemoveSubstring(directory_name, " ");

  UNIXCommand_ = "mkdir -p ";
  UNIXCommand_ += directory_name;
  system(UNIXCommand_);
  
  UNIXCommand_ = "chmod u+rwx " ;
  UNIXCommand_ += directory_name ;
  system(UNIXCommand_);
  
}


void UFAV::ChangeToUNIXDirectory(TString directory_name)
{
  RemoveSubstring(directory_name," ");

  UNIXCommand_ = " cd " ;
  UNIXCommand_ += directory_name;
  system( UNIXCommand_ );
  gSystem->cd(directory_name);
}

Bool_t UFAV::FileExists(TString FileName)
{
  struct stat FileInfo;
  bool exists;
  int stats;
  
  if( !FileName.BeginsWith("/") )
    {
      TString prefix = getenv("PWD");
      if(!prefix.EndsWith("/"))
	prefix+="/";
      FileName = prefix+FileName;
    }
  
  stats = stat(FileName,&FileInfo);
  if(stats == 0) 
    {
      exists = true;
    } 
  else 
    {
      exists = false;
    }
  
  return(exists);  

}

void UFAV::AddFile(TString filename,TString description, TH1Format th1format, float weight, TString UncommonSubdirectoryPrefix )
{
  //Remove preceding and succeding blank spaces          
  RemoveBlankspaces(filename);
 
  if(vFile_.size() < 8 )
    {
      if(!filename.BeginsWith("/"))
	{
	  TString prefix = getenv("PWD");
	  if(!prefix.EndsWith("/"))
	    prefix+="/";
	  filename = prefix+filename;
	}

      if(FileExists(filename))
	{
	  TFile F(filename);
	  if( F.IsZombie() )
	    {
	      cerr << "[ERROR]:  File:  " << filename << " exists but is zombie.  " << endl;	      
	      F.Close();
	      return;
	    }
	  else
	    F.Close();

	  vFile_.push_back(new TFile(filename));	  
	  if( vFile_.back() == NULL )
	    { 
	      cerr << "[ERROR]: File:  " << filename << " exists but  pointer returns NULL  " << endl;
	      delete vFile_.back();
	      vFile_.pop_back();
	      return;
	    }

	  if(filename.BeginsWith("/")) // If user gives absolute path to Root File 
	    {

	      vRootFilePath_.push_back(filename);
	      RemovePrefix( filename , "/" ) ;
	      vRootFileName_.push_back(filename);
	    }
	  else      
	    {
	      TString prefix = getenv("PWD");
	      if(!prefix.EndsWith("/"))
		prefix+="/";
	  
	      vRootFileName_.push_back(filename);
	      vRootFilePath_.push_back( prefix + filename );
	    }

	  if(vFile_.back()->IsOpen())
	    {
	      if(description.Length()==0)
		{
		  description = "File_" ;
		  description += vFile_.size();
		}
	    
	      vFileDescription.push_back(description);
	      vWeight.push_back(weight);
	      vTH1Format.push_back(th1format);

	      RemoveBlankspaces(UncommonSubdirectoryPrefix);
	      while( UncommonSubdirectoryPrefix.EndsWith("/") )
		UncommonSubdirectoryPrefix.Remove( UncommonSubdirectoryPrefix.Length()-1,1);
	      while( UncommonSubdirectoryPrefix.BeginsWith("/") )
		UncommonSubdirectoryPrefix.Remove(0,1);
	      vUncommonPrefix.push_back(UncommonSubdirectoryPrefix);
	    }
	}
      else 
	cerr << "ERROR:  "  << filename << " cannot be opened by UFAV.  Perhaps it does not exist.  " << endl;
    }
  else 
    cerr << "ERROR: UFAV Can only process up to 8 files in one go..." << endl;
}

void UFAV::AddFile(TString FileName, TString description, float weight)
{
  TH1Format th1format;
  if( vFile_.size() == 0 )th1format.linecolor= kBlack;
  if( vFile_.size() == 1 )th1format.linecolor= kBlue+1;
  if( vFile_.size() == 2 )th1format.linecolor= kRed+1;
  if( vFile_.size() == 3 )th1format.linecolor= kGreen+1;
  if( vFile_.size() == 4 )th1format.linecolor= kOrange+1;
  if( vFile_.size() == 5 )th1format.linecolor= kGray+1;
  if( vFile_.size() == 6 )th1format.linecolor= kCyan+1;
  if( vFile_.size() == 7 )th1format.linecolor= kMagenta+1;

  AddFile(FileName, description, th1format, weight, "");
}

void UFAV::RemoveSubstring( TString& string_ , TString substring_)
{
  TString y="";

  while( string_.Contains(substring_))
    {
      for(int i = 0 ; i < string_.Length() ;i++ )
	{
	  for( int j = 1; i+j <= string_.Length() ; j++ )
	    {
	      y=string_(i,j);
	      if( y.IsWhitespace() )
		{
		  string_ = string_(0,i) + string_(i+substring_.Length(), string_.Length()-substring_.Length()-1);
		}
	    }
	}
    }
}

void UFAV::RemovePrefix(TString& string_ , TString delimiter)
{
  if( string_.Contains(delimiter) )
    {
      TString temp_ = "temp";
      while( temp_.Length() )
	{
	  string_ = (char*) strstr( (const char*)string_ , (const char*)delimiter);
	  string_.Remove(0,1);
	  temp_ = (char*) strstr( (const char*)string_ , (const char*) delimiter);
	}
    }
}

void UFAV::AddCaption(TString HistoPath,TString Caption)
{
  if(HistoPath.BeginsWith("/"))
    HistoPath.Remove(0,1);
    
  TString cap= HistoPath;
  cap+="::" ; //delimiter
  cap+=Caption;
  vCaption.push_back(cap);
}


void UFAV::RemoveBackslashes(TString &d)
{
  // Remove "/" from end of directory name if it is there                                   
  while( d.EndsWith("/") )
    d.Remove(d.Length()-1 , 1 );

  //  Remove "/" from beginning of directory name if it is there         
  while( d.BeginsWith("/") )
    d.Remove(0,1);
}


void UFAV::RemoveBlankspaces(TString &d)
{
  // Get rid of blank spaces at the beginning and end of the user's input
  while(d.EndsWith(" ")) 
    d.Remove( d.Length() ,1 );  

  while( d.EndsWith(" ") )
    d.Remove(d.Length()-1 , 1 );

}

void UFAV::StripExtension(TString &d)
{
  if(d.Contains("."))
    {
      while(!d.EndsWith("."))
	d.Remove(d.Length()-1 , 1);
      
      if(d.EndsWith("."))
	d.Remove(d.Length()-1, 1);
    }
}


void UFAV::DisplayVersion()
{
  cout << endl << endl  << endl;
  cout <<"         WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW  \n"
       <<"         W                                                      W  \n"
       <<"         W                                                      W  \n"
       <<"         W                                                      W  \n"
       <<"         W          UF Analysis Viewer version 1.6              W  \n"
       <<"         W                                                      W  \n"
       <<"         W           Developed by Ronald Remington              W  \n"
       <<"         W            The University of Florida                 W  \n"
       <<"         W                    04/01/10                          W  \n"
       <<"         W                                                      W  \n"
       <<"         W                                                      W  \n"
       <<"         WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW  \n"
       << endl << endl << endl;

}


bool UFAV::SuperimposeThisDirectory(TString d) {

  TString dirname = d;
  bool status = false;

  RemoveBackslashes( dirname );
  //Check to see if user wanted this directory to be superimposed
  for( unsigned int i = 0 ; i < vSuperimposedDirectory1.size() ; i ++)
    {
      if ( vSuperimposedDirectory1[i] == TString(dirname) )
        return true;
      else
        status = false;
    }
  for( unsigned int i = 0 ; i < vSuperimposedDirectory2.size() ; i++ )
    {
      if ( vSuperimposedDirectory2[i] == TString(dirname) )
        return true;
      else
        status = false;
    }
  return status;
}

void UFAV::PrintTTreeHistograms(TTree *tree)
{
  TObjArray *BranchArray = tree->GetListOfBranches();
  TIterator *BranchIterator = BranchArray->MakeIterator();
  TBranch   *Branch;
  TCanvas c_;
  c_.cd();
  while( (Branch = (TBranch*) BranchIterator->Next()) )
    {
      BranchTitle_ = Branch->GetTitle();
      BranchName_ = Branch->GetName();
      if(Verbosity_)
	cout << "[Printing Branch] :  " << tree->GetName() <<"/"<<BranchName_ << endl;

      tree->Draw(Branch->GetName());

      if(BranchTitle_.Length() == 0 ) BranchTitle_ = BranchName_;

      PrintName_ = BranchName_;
      PrintName_ +="."+Ext_;
      
      if(Verbosity_)
	cout << "[Printing Branch] : " << tree->GetName() << "/" << PrintName_<<"."<<Ext_ << endl;

      c_.Print(PrintName_, Ext_);

      if(LogY_)
	{
	  
	  PrintName_ = UNIXCurrentDirectory_;
	  PrintName_ += BranchName_;
	  PrintName_ += "_log."+Ext_;
	  if( Branch->GetEntries())
	    {
	      c_.SetLogy(1);
	      if(Verbosity_)
		cout << "[Printing] : " << tree->GetName() << "/"<<PrintName_<< endl;

	      c_.Print(PrintName_,Ext_);
	    }
	}
      else
	{
	  LinkToIcon( NoDataIcon_, PrintName_ );
	}

      c_.SetLogy(0);
      c_.Clear();
    } 
}

void UFAV::PrintHistogram1D(TH1& H, TCanvas& c_, unsigned int index)
{
  c_.cd();
  HistogramTitle_ = H.GetTitle();
  HistogramName_ =  H.GetName();

  HistogramName_.ReplaceAll(";","_");

  if( HistogramTitle_.Length() == 0 )
    HistogramTitle_ = HistogramName_;

  DirectoryName_  = gDirectory->GetName();
  if( gDirectory->GetMotherDir() )
    {
      PrintName_ = RootFileSubDirectoryPath_ ;
      PrintName_ += "/";
    }
  if ( PrintName_ = "" )
    PrintName_ += HistogramName_;

  TString drawoption = vTH1Format[index].drawoption;
  RemoveSubstring(PrintName_," ");
  RemoveSubstring(RootFileSubDirectoryPath_," "); //debug
  H.Draw(drawoption);
  if(Errors_)
    H.Draw("e1"+drawoption);

  if(Verbosity_)
    cout << "[Printing 1D Histogram] : " << RootFileSubDirectoryPath_ << "/"<< PrintName_<<"."<<Ext_ << endl;

  c_.Print(PrintName_+"."+Ext_,Ext_);
  
  if( LogY_ )
    {
      if( H.GetEntries() && H.GetMaximum() >= 0. && H.GetMinimum() >=0.)
	{
	  c_.SetLogy(1);
	  if(Verbosity_)
	    cout << "[Printing 1D Histogram] : " << RootFileSubDirectoryPath_ << "/" <<PrintName_<<"_log."<<Ext_ << endl;
	  c_.Print(PrintName_+"_log."+Ext_,Ext_);
	}
    }
  c_.Clear();
  c_.SetLogy(0);

}

void UFAV::PrintHistogram2D(TH2 &H, TCanvas &c_)
{
  HistogramName_ = H.GetName();
  HistogramName_.ReplaceAll(";","_");
  DirectoryName_  = gDirectory->GetName();
  if( gDirectory->GetMotherDir() )
    {
      PrintName_ = RootFileSubDirectoryPath_ ;
      PrintName_ += "/";
    }
  if ( PrintName_ = "" )
    PrintName_ += HistogramName_;

  RemoveSubstring(PrintName_, " ");
  H.Draw(DrawOption2D_);                                                                                         

  if(H.GetEntries())
    {
      if(Verbosity_)
	cout << "[Printing 2D Histogram] : " << RootFileSubDirectoryPath_ << "/" <<PrintName_<<"."<<Ext_ << endl;
    
    c_.Print(PrintName_+"."+Ext_,Ext_);
    }
  else
    LinkToIcon(NoDataIcon_, PrintName_+"."+Ext_);

  c_.Clear();
}


void UFAV::PrintSuperimposedHistograms(TH1& h1, TH1& h2, TH1Format& f1, TH1Format& f2,  TPaveStats* stats_1, TPaveStats *stats_2, TString printname)
{
  printname.ReplaceAll(";","_");

  TCanvas c_("c_","c_",50,50,692,444);
  c_.cd();
  float ymax1 = h1.GetMaximum();
  float ymax2 = h2.GetMaximum();
  float ymax = TMath::Max(ymax1,ymax2);
  
  if(ymax)
    {
      h1.GetYaxis()->SetRangeUser(0.1,ymax*1.4);
      h2.GetYaxis()->SetRangeUser(0.1,ymax*1.4);
    }

  h1.Draw(f1.drawoption);
  
  if(Errors_)
    h1.Draw("e1sames"+f1.drawoption);
  
  h2.Draw("sames"+f2.drawoption);

  stats_1->Draw("SAME");
  stats_2->Draw("SAME");
  
  if(Verbosity_)
    cout << "[Printing 1D Histogram] : " << RootFileSubDirectoryPath_ << "/" <<PrintName_<<"."<<Ext_ << endl;

  c_.Print(PrintName_ + "."+Ext_, Ext_);  
  c_.Clear();
  if(LogY_)
    {
      if( h1.GetMinimum()>=0 && h1.GetMaximum() > 0
	  && h2.GetMinimum() >= 0 && h2.GetMaximum() > 0 )
	{
	  if(ymax)
	    {
	      h1.GetYaxis()->SetRangeUser(0.1,ymax*14.);
	      h2.GetYaxis()->SetRangeUser(0.1,ymax*14.);
	    }
	  
	  h1.Draw(f1.drawoption);
	  
	  if(Errors_)
	    h1.Draw("e1sames"+f1.drawoption);
	  
	  h2.Draw("sames"+f2.drawoption);

	  stats_1->Draw("SAME");
	  stats_2->Draw("SAME");
	  c_.SetLogy(1);
	  if(Verbosity_)
	    cout << "[Printing 1D Histogram] : " << RootFileSubDirectoryPath_ << "/"<<PrintName_<<"_log."<<Ext_ << endl;
	  c_.Print( PrintName_ +"_log."+Ext_, Ext_);
	}
      else
	{
	  LinkToIcon(NoDataIcon_, PrintName_+"log."+Ext_);
	}
    }
  c_.Clear();
}

void UFAV::FormatSuperimposedHistograms(TH1& h1, TH1& h2, TH1Format&f1, TH1Format&f2, TPaveStats* stats_1, TPaveStats* stats_2)
{
  h1.SetLineColor( f1.linecolor);
  h1.SetFillColor( f1.fillcolor);
  h1.SetFillStyle( f1.fillstyle);
  h1.SetLineWidth( f1.linewidth);
  h1.SetNormFactor(1);

  h2.SetLineColor( f2.linecolor);
  h2.SetFillColor( f2.fillcolor);
  h2.SetFillStyle( f2.fillstyle);
  h2.SetLineWidth( f2.linewidth);
  h2.SetNormFactor(1);

  float Mean1x = h1.GetMean(1);                                                                                 
  float Mean2x = h2.GetMean(1);                                                                                
  float Meanx = TMath::Max(Mean1x,Mean2x); 

  float rms1 = h1.GetRMS();                                                                                     
  float rms2 = h2.GetRMS();                                                                                   
  float rms = TMath::Max(rms1,rms2);        

  if( Rebin_ )
    {
      int rebinfactor = TMath::Min( GetRebin(h1), GetRebin(h2));
      if( rebinfactor > 1 ) 
	{
	  h1.Rebin(rebinfactor);
	  h2.Rebin(rebinfactor);
	}
    }

  int xmin_bin = 1; 
  int xmax_bin = h1.GetNbinsX();
  if( CaptureTails_ && h1.GetEntries() && h2.GetEntries())
    {
      while(xmin_bin < h1.GetNbinsX())
	{
	  if(h1.GetBinContent(xmin_bin) != 0 || h2.GetBinContent(xmin_bin) !=0 ) break;
	  else xmin_bin ++;
	}
      while(xmax_bin > 1 )
	{
	  if(h1.GetBinContent(xmax_bin) != 0 || h2.GetBinContent(xmax_bin) != 0) break;
	  else xmax_bin --;
	}
      float xmin = h1.GetBinLowEdge( xmin_bin );
      float xmax = h2.GetBinLowEdge( xmax_bin + 1 ) ;
      if( xmin < 0 ) //ensure symmetric distributions                                                                                           
	{
	  xmin = TMath::Abs(xmin) > xmax ? xmin : -1.*xmax;
	  xmax = xmax > TMath::Abs(xmin) ? xmax : -1.*xmin;
	}

      h1.GetXaxis()->SetRangeUser(xmin,xmax);
      h2.GetXaxis()->SetRangeUser(xmin,xmax);
    }
  else if( Zoom_ )
    {
      h1.GetXaxis()->SetRangeUser( Meanx - Sigmas_*rms, Meanx + Sigmas_*rms);
      h2.GetXaxis()->SetRangeUser( Meanx - Sigmas_*rms, Meanx + Sigmas_*rms);
    }


  double textsize1 = 0.025;
  double textsize2 = 0.025;
  TString HistogramName1 = h1.GetName();
  TString HistogramName2 = h2.GetName();
  
  RemovePrefix(HistogramName1,"/");
  RemovePrefix(HistogramName2,"/");

  h1.SetName(HistogramName1);
  h2.SetName(HistogramName2);

  if(HistogramName1.Length() < 14) 
    textsize1 = 0.030;
  else if( HistogramName1.Length() > 25 ) 
    textsize1 = 0.020;

  if(HistogramName2.Length() < 8)
    textsize2 = 0.030;  
  else if( HistogramName2.Length() > 30 )
    textsize2 = 0.020;
  
  
  stats_1->SetOption("NDC");
  stats_1->SetShadowColor(0);
  stats_1->SetBorderSize(1);
  stats_1->SetTextSize(textsize1);
  stats_1->SetTextColor(f1.linecolor);


  stats_2->SetOption("NDC");
  stats_2->SetShadowColor(0);
  stats_2->SetBorderSize(1);
  stats_2->SetTextSize(textsize2);
  stats_2->SetTextColor(f2.linecolor);
 
  h1.GetListOfFunctions()->Add(stats_1);
  h2.GetListOfFunctions()->Add(stats_2);
  stats_1->SetParent(h1.GetListOfFunctions());
  stats_2->SetParent(h2.GetListOfFunctions());
}


TH1F UFAV::GetCumulative(TH1F& H, TString opt)
{
  TH1F clone(*(TH1F*) H.Clone());
  clone.Reset();
  TString clone_name = H.GetName() ;
  clone_name +="_cumulative";
  clone.SetName(clone_name);
  TString clone_title = H.GetTitle();
  clone_title+=" (cumulative)";
  clone.SetTitle(clone_title);
  float Integral = 0.;
  opt.ToLower();
  if (opt = "ltr" || opt != "rtl")
    {
      clone.SetBinContent(1, H.Integral() );
      for( int ibin = 1 ; ibin <= H.GetNbinsX() ; ibin++ )
	{
	  Integral += H.GetBinContent(ibin);
	  clone.SetBinContent(ibin+1, H.Integral() - Integral );
	}
    }
  else
    { 
      clone.SetBinContent(H.GetNbinsX(), H.Integral());
      for(int ibin = 1 ; ibin <= H.GetNbinsX() ; ibin++ )
	{
	  Integral += H.GetBinContent(ibin);
	  clone.SetBinContent(ibin+1, Integral);
	}
    }
  return clone;
}

void UFAV::SetDirectoryStatus(TString dirname , bool allow_)
{
  RemoveSubstring(dirname," ");
  while( dirname.EndsWith("/"))
    dirname.Remove(dirname.Length()-1,1);
  while( dirname.BeginsWith("/"))
    dirname.Remove(0,1);

  if( allow_ ) 
    {
      vAllowDirectory.push_back(dirname);
    }
  else
    vVetoDirectory.push_back(dirname);  
}

bool UFAV::GetDirectoryStatus(TDirectory *dir)
{
  RootFileSubDirectoryPath_ = ((char*) strstr( dir->GetPath(), ":" ) );
  RootFileSubDirectoryPath_.Remove(0,2);  
  RemoveSubstring(RootFileSubDirectoryPath_, " " );
  
  for( unsigned int a = 0 ; a < vAllowDirectory.size(); a++ )
    {
      if( vAllowDirectory[a] == RootFileSubDirectoryPath_ )
	{
	  return true;
	}
    }
  
  //if in a directory which has constituent subdirectories enabled
  for( unsigned int a = 0 ; a < vAllowDirectory.size(); a++ )
    {
      if( vAllowDirectory[a].Contains( RootFileSubDirectoryPath_ ) )
	{
	  return true;
	}
      
      //if in a directory which has a parent directory enabled
      TDirectory* MotherDir = dir->GetMotherDir();
      TString motherpath =(char*) strstr(MotherDir->GetPath(), ":" ) ;
      motherpath.Remove(0,2);
      RemoveSubstring(motherpath," "); //bug fix RCR

      bool enabled=false;
      while(motherpath.Length())
	{
	  if(motherpath == vAllowDirectory[a] )
	    {
	      enabled = true;
	      break;
	    }
	  
	  MotherDir = MotherDir->GetMotherDir();
	  motherpath =(char*) strstr( MotherDir->GetPath(), ":" ) ;
	  motherpath.Remove(0,2);
	  RemoveSubstring(motherpath," ");	  
	}
      
      RootFileSubDirectoryPath_ = ((char*) strstr( dir->GetPath(), ":" ) );
      RootFileSubDirectoryPath_.Remove(0,2);
      RemoveSubstring(RootFileSubDirectoryPath_, " " );
      if(enabled)
	{
	  for( unsigned int v = 0 ; v < vVetoDirectory.size() ; v++ )
	    {      
	      if( vVetoDirectory[v] == RootFileSubDirectoryPath_ )
		{
		  return false;
		}
	    }
	  return true;
	}
    }

  for( unsigned int v = 0 ; v < vVetoDirectory.size() ; v++ )
    {      

      if( vVetoDirectory[v] == RootFileSubDirectoryPath_ || RootFileSubDirectoryPath_.Contains(vVetoDirectory[v]) )
	{
	  return false;
	}

    }
  
  return true;
}


double UFAV::GetKSScore(TString histoname)
{
  double ks = -999.;
  if (vFile_.size() == 2) 
    {
      TString HistogramPath((char*)strstr((const char*)gDirectory->GetPath(),":/"));
      HistogramPath.Remove(0,2);
      HistogramPath+="/"; HistogramPath+=histoname;

      TH1F *H1 = (TH1F*)vFile_[0]->Get(HistogramPath);
      TH1F *H2 = (TH1F*)vFile_[1]->Get(HistogramPath);
      
      if(H2 != NULL && H1 != NULL)
	{

	  H1->SetNormFactor(1); 
	  H2->SetNormFactor(1);

	  if(H1->Integral() == 0) 
	    return -1111.;
	  if(H2->Integral() == 0)
	    return -2222.;
 
	  if( !H2->GetEntries() && !H1->GetEntries() )
	    ks = -333.;
	  else if( !H1->GetEntries())
	    ks = -111.;
	  else if( !H2->GetEntries())
	    ks = -222;
	  else
	    ks = H1->KolmogorovTest( H2 );
	}
      else
	ks = -888.;
    }
  return ks ;
}

double UFAV::GetKSScore(TH1F& H1, TH1F& H2)
{
  double ks = -999.;

  H1.SetNormFactor(1);
  H2.SetNormFactor(1);

  if(H1.Integral() == 0 ) 
    return -1111.;
  if(H2.Integral() == 0 )
    return -2222.;

  if( !H2.GetEntries() && !H1.GetEntries() )
    ks = -333.;
  else if( !H1.GetEntries())
    ks = -111.;
  else if( !H2.GetEntries())
    ks = -222;
  else
    ks = H1.KolmogorovTest( &H2 );

  return ks ;
}




UFAV::~UFAV(){}
     

