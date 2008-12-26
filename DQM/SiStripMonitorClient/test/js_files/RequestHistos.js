var RequestHistos = {};
//
// -- Get list of histogram names according to the option selected
//
RequestHistos.RequestHistoList = function() 
{
  var queryString;
  var url = WebLib.getApplicationURLWithLID();
  if ($('module_histos').checked) {
    queryString = "RequestID=SingleModuleHistoList";
    var obj = $('mod_struc_name');
    var sname    = obj.options[obj.selectedIndex].value;
    queryString += '&FolderName='+sname;    
    url        += queryString; 
    var retVal = new Ajax.Request(url,                    
 	 	                 {			  
 	 		          method: 'get',	  
 			          parameters: '', 
 			          onSuccess: RequestHistos.FillModuleHistoList
 			         });
//    WebLib.makeRequest(url, RequestHistos.FillModuleHistoList);
    CommonActions.ShowProgress("visible", "Module Histogram List");     
  } else if ($('global_histos').checked) {
    queryString = "RequestID=GlobalHistoList";    
    var obj = $('ghisto_path');
    var value =  obj.value;
    queryString += '&GlobalFolder='+value;
    url        += queryString;
    var retVal = new Ajax.Request(url,                    
 	 	                 {			  
 	 		          method: 'get',	  
 			          parameters: '', 
 			          onSuccess: RequestHistos.FillGlobalHistoList
 			         });
//    WebLib.makeRequest(url, RequestHistos.FillGlobalHistoList);     
    CommonActions.ShowProgress("visible", "Global Histogram List");
  }
}
//

//
RequestHistos.RequestSummaryHistoList = function()
{
  var queryString;
  var url      = WebLib.getApplicationURLWithLID();
  queryString  = "RequestID=SummaryHistoList";
  var obj      = $('summ_struc_name');
  var sname    = obj.options[obj.selectedIndex].value;
  queryString += '&StructureName='+sname;
  url         += queryString; 
  var retVal = new Ajax.Request(url,
                               {           
                  		method: 'get',	  
 			        parameters: '', 
 			        onSuccess: RequestHistos.FillSummaryHistoList
 			       });
//  WebLib.makeRequest(url, RequestHistos.FillSummaryHistoList);     
  CommonActions.ShowProgress("visible", "Summary Histogram Tree");
}
//
// -- Request the alarm tree
//
RequestHistos.RequestAlarmList = function()
{
  var queryString;
  var url      = WebLib.getApplicationURLWithLID();
  queryString  = "RequestID=AlarmList";
  var obj      = $('alarm_struc_name');
  var sname    = obj.options[obj.selectedIndex].value;
  queryString += '&StructureName='+sname;
  url         += queryString; 
  var retVal = new Ajax.Request(url,
                               {           
                  		method: 'get',	  
 			        parameters: '', 
 			        onSuccess: RequestHistos.FillAlarmList
 			       });

//  WebLib.makeRequest(url, RequestHistos.FillAlarmList);     
  CommonActions.ShowProgress("visible", "Alarm Tree");
}
//
// -- Fill list of modules and histogram names in th list area
//
RequestHistos.FillModuleHistoList = function(transport) 
{
    CommonActions.ShowProgress("hidden");
    try 
    {
      var doc   = transport.responseXML;
      var root  = doc.documentElement;
        
      // Module Number select box
      var aobj  = $('module_numbers');

      aobj.options.length = 0;
        
       var mrows = root.getElementsByTagName('ModuleNum');
//        alert(" rows = " + mrows.length);
      for (var i = 0; i < mrows.length; i++) {
        var mnum = mrows[i].childNodes[0].nodeValue;
        var aoption = new Option(mnum, mnum);
        try 
        {
          aobj.add(aoption, null);
        }
        catch (e) {
          aobj.add(aoption, -1);
        }
      }
      // Select the first option and set to editable text  
      var cobj = $('module_number_edit');
      if (cobj != null) {
        cobj.value = aobj.options[0].value;;
      }   
      // Histogram  select box
      var bobj = $('histolistarea');
      bobj.options.length = 0;

      var hrows = root.getElementsByTagName('Histo');
      // alert(" rows = " + hrows.length);
      for (var j = 0; j < hrows.length; j++) {
        var name  = hrows[j].childNodes[0].nodeValue;
        var boption = new Option(name, name);
        try {
          bobj.add(boption, null);
        }
        catch (e) {
          bobj.add(boption, -1);
        }
      }
    }
    catch (err) {
      alert ("[RequestHistos.FillModuleHistoList] Error detail: " + err.message); 
    }
}
//
// -- Fill names of the global histogram
//
RequestHistos.FillGlobalHistoList = function(transport) 
{
    CommonActions.ShowProgress("hidden");
    try 
    {
      var doc  = transport.responseXML;
      var root = doc.documentElement;
       
      // Histogram  select box
      var bobj = $('histolistarea');
      bobj.options.length = 0;

      var hrows = root.getElementsByTagName('GHisto');
      // alert(" rows = " + hrows.length);
      for (var j = 0; j < hrows.length; j++) {
        var name    = hrows[j].childNodes[0].nodeValue;
        var boption = new Option(name, name);
        try 
        {
          bobj.add(boption, null);
        }
        catch (e) {
          bobj.add(boption, -1);
        }
      }
    }
    catch (err) {
      alert ("[RequestHistos.FillGlobalHistoList] Error detail: " + err.message); 
    }
}
//
// -- Fill the summary tree in the list area
//
RequestHistos.FillSummaryHistoList = function(transport) 
{
    CommonActions.ShowProgress("hidden");
    try {
      var text = transport.responseText;
      var obj  = $('tree_list');
      if (obj != null) {
        obj.innerHTML = text;
        initTree();
      }       
    }
    catch (err) {
    // alert ("[RequestHistos.FillSummaryHistoList] Error detail: " + err.message); 
    }
}
//
// -- Fill alarm tree in the list area
//
RequestHistos.FillAlarmList = function(transport) 
{
    CommonActions.ShowProgress("hidden");
    try {
      var text = transport.responseText;
      var obj = $('alarm_list');
      if (obj != null) {
        obj.innerHTML = text;
        initTree();
      }       
    }
    catch (err) {
    // alert ("[RequestHistos.FillAlarmList] Error detail: " + err.message); 
    }
}
//
// -- Draw selected histograms from the list area
//
RequestHistos.DrawSelectedHistos = function() 
{
  var queryString;
  var url = WebLib.getApplicationURLWithLID();
  if ($('module_histos').checked) {
    queryString = "RequestID=PlotAsModule";
    // Get Module Number
    var obj      = $('module_number_edit');
    var value    = obj.value;
    queryString += '&ModId='+value;
  } else if ($('global_histos').checked) {
    queryString  = "RequestID=PlotGlobalHisto";    
    var obj = $('ghisto_path');
    var value =  obj.value;
    queryString += '&GlobalFolder='+value;
  }
  var hist_opt   = RequestHistos.SetHistosAndPlotOption();
  if (hist_opt == " ") return;
  queryString   += hist_opt;
//  IMGC.computeCanvasSize();
  queryString += '&width='+IMGC.BASE_IMAGE_WIDTH+
                 '&height='+IMGC.BASE_IMAGE_HEIGHT;
  url += queryString;
  var getMEURLS = new Ajax.Request(url,                    
 	 		         {			  
 	 		          method: 'get',	  
 			          parameters: '', 
 			          onComplete: IMGC.processImageURLs // <-- call-back function
 			         });
//  CommonActions.ShowProgress('visible', 'Selected Plot');
//  setTimeout('RequestHistos.UpdatePlot()', 2000);   
}
//
//  -- Set Histograms and plotting options 
//    
RequestHistos.SetHistosAndPlotOption = function() {
   var dummy = " ";
   var qstring = " ";
  // Histogram Names 
  var hist_obj   = $('histolistarea');
  var nhist = hist_obj.length;
  if (nhist == 0) {
    alert("Histogram List Area Empty!");
    return dummy;
  } else {
    for (var i = 0; i < nhist; i++) {
      if (hist_obj.options[i].selected) {
	if (qstring == " ") qstring  = '&histo='+ hist_obj.options[i].value;
        else        qstring += '&histo='+ hist_obj.options[i].value;
      }
    }
  }
  // Plot options for single histogram 
  if (nhist == 1) {
    // logy option
    if ($('logy').checked) {
      qstring += '&logy=true';
    }
  } 
  // Drawing option
  var obj1 = $('drawing_options');
  var value1 =  obj1.options[obj1.selectedIndex].value;
  qstring += '&drawopt='+value1;
  return qstring;
} 
//
// -- Get last plot from the server
// 
RequestHistos.UpdatePlot = function()
{
  var canvas = $('drawingcanvas');

  var queryString = "RequestID=UpdatePlot";
  var url = WebLib.getApplicationURLWithLID();
  url = url + queryString;
  url = url + '&t=' + Math.random();
  canvas.src = url; 
  CommonActions.ShowProgress('hidden');
}
//
// -- Draw single histogram from path
// 
RequestHistos.DrawSingleHisto = function(path)
{
  var url      = WebLib.getApplicationURLWithLID();
  queryString  = 'RequestID=PlotHistogramFromPath';
  queryString += '&Path='+path;
  queryString += '&histotype=summary';
//  IMGC.computeCanvasSize();
  queryString += '&width='+IMGC.BASE_IMAGE_WIDTH+
                 '&height='+IMGC.BASE_IMAGE_HEIGHT;

  url         += queryString;

  var getMEURLS = new Ajax.Request(url,                    
 	 		         {			  
 	 		          method: 'get',	  
 			          parameters: '', 
 			          onComplete: IMGC.processImageURLs // <-- call-back function
 			         });

//  CommonActions.ShowProgress('visible', 'Selected Plot');   
//  setTimeout('RequestHistos.UpdatePlot()', 2000);     
}
//
// -- Read status message from QTest
//
RequestHistos.ReadStatus = function(path) 
{
  var url      = WebLib.getApplicationURLWithLID();
  queryString  = 'RequestID=ReadQTestStatus';
  queryString += '&Path='+path;
  queryString += '&width='+IMGC.BASE_IMAGE_WIDTH+
                 '&height='+IMGC.BASE_IMAGE_HEIGHT;
  url         += queryString;
  CommonActions.ShowProgress('visible', 'Status Message');
  var retVal = new Ajax.Request(url,
                               {           
                  		method: 'get',	  
 			        parameters: '', 
 			        onSuccess: RequestHistos.FillStatus
 			       });
}
//
// -- Fill status message from QTest in the status list area
//
RequestHistos.FillStatus = function(transport) {
   CommonActions.ShowProgress('hidden');
   try {
      var doc = transport.responseXML;
      var root = doc.documentElement;
      var mrows = root.getElementsByTagName('Status');
      if (mrows.length > 0) {
        var stat  = mrows[0].childNodes[0].nodeValue;
        var obj = $('status_area');
        if (obj != null) {
          obj.innerHTML = stat;
        }       
      }
      mrows = root.getElementsByTagName('HPath');
      if (mrows.length > 1) {
        var hpath  = mrows[0].childNodes[0].nodeValue;
        if (hpath != "NONE")  {
          var tempImages = new Array() ;
          var tempTitles = new Array() ; 
          var date = new Date() ;
          date = date.toString() ;
          date = date.replace(/\s+/g,"_") ;
          var url = WebLib.getApplicationURLWithLID();
  
          for (var i = 1; i < mrows.length; i++) {
            var name = mrows[i].childNodes[0].nodeValue;
            tempImages[i-1] = url + "RequestID=GetIMGCImage&Path=" + 
                           hpath + "/" + name + "&Date=" + date;
            tempTitles[i-1] = hpath + "|" + name;
          }
        }
        $('imageCanvas').imageList     = tempImages;
	$('imageCanvas').titlesList    = tempTitles;
        setTimeout('IMGC.computeCanvasSize()',10000) ;	
      }
    }
    catch (err) {
//      alert ("Error detail: " + err.message); 
    }
}
//
// -- Draw selected group plot from shifter page
//
RequestHistos.DrawSelectedSummary = function() 
{
  var tobj      = $('summary_plot_type');
  var url = WebLib.getApplicationURL() ;
  var urlTitleList = url + '/images/' + tobj.value +'_titles.lis';
  var urlImageList = url + '/images/' + tobj.value +'.lis';
  var getTitles = new Ajax.Request(urlTitleList,	   // Load titles first, because they are
  				  {			   // used by the IMGC.processImageList
  				   method: 'get',	   // which fires later on
  				   parameters: '', 
  				   onComplete: IMGC.processTitlesList // <-- call back function
  				  });
  var getFiles  = new Ajax.Request(urlImageList, 
  				  {
  				   method: 'get', 
  				   parameters: '', 
  				   onComplete: IMGC.processImageList  // <-- call back function
  				  });
} 
//
// Check Quality Test Results (Lite)
//
RequestHistos.CheckQualityTestResultsLite = function() 
{
  var queryString  = "RequestID=CheckQTResults";
  queryString     += '&InfoType=Lite';
  var url          = WebLib.getApplicationURLWithLID();
  url              = url + queryString; 
  var retVal = new Ajax.Request(url,
                               {           
                  		method: 'get',	  
 			        parameters: '', 
 			        onSuccess: RequestHistos.FillTextStatus
 			       });
}
//
// Check Quality Test Results (Expert)
//
RequestHistos.CheckQualityTestResultsDetail = function() {
  var queryString  = "RequestID=CheckQTResults";
  queryString     += '&InfoType=Detail';
  var url          = WebLib.getApplicationURLWithLID();
  url              = url + queryString; 
  var retVal = new Ajax.Request(url,
                               {           
                  		method: 'get',	  
 			        parameters: '', 
 			        onSuccess: RequestHistos.FillTextStatus
 			       });
}
//
// -- Fill the status of QTest in the status list area
//
RequestHistos.FillTextStatus = function(transport) 
{
  try {
    var text = transport.responseText;
      CommonActions.FillText("summary_status_area", text);
    }
    catch (err) {
//      alert ("Error detail: " + err.message); 
    }
}
//
// -- Request Readout/Control Tree
//
RequestHistos.RequestNonGeomeHistoList = function()
{
  var queryString;
  var url      = WebLib.getApplicationURLWithLID();
  queryString  = "RequestID=NonGeomHistoList";
  var obj      = $('type_tag');
  var fname    = obj.options[obj.selectedIndex].value;
  queryString += '&FolderName='+fname;
  url         += queryString; 
  var retVal = new Ajax.Request(url,
                               {           
                  		method: 'get',	  
 			        parameters: '', 
 			        onSuccess: RequestHistos.FillNonGeomHistoList
 			       });
  CommonActions.ShowProgress("visible", "Readout/Control Tree");
}
//
// -- Fill the readout/control tree in the list area
//
RequestHistos.FillNonGeomHistoList = function(transport) 
{
    CommonActions.ShowProgress("hidden");
    try {
      var text = transport.responseText;
      var obj  = $('non_geo_hlist');
      if (obj != null) {
        obj.innerHTML = text;
        initTree();
      }       
    }
    catch (err) {
    // alert ("[RequestHistos.FillNonGeometricHistoList] Error detail: " + err.message); 
    }
}
//
// -- Draw CondDB Histos for Module
//
RequestHistos.DrawModuleCondDBHisto = function()
{
  if ($('global_histos').checked) {
    alert("Global Plot option is selected!! Select Modules");
    return;
  }
  var queryString;
  var url = WebLib.getApplicationURLWithLID();
  queryString = "RequestID=PlotModuleCondDBHistos";
  // Get Module Number
  var obj      = $('module_number_edit');
  var value    = obj.value;
  queryString += '&ModId='+value;
  var option = 'PedestalFromCondDB,NoiseFromCondDB,GainFromCondDB';
  queryString += '&option='+option;  
  url += queryString;
  var getMEURLS = new Ajax.Request(url,                    
 	 		         {			  
 	 		          method: 'get',	  
 			          parameters: '', 
 			          onComplete: IMGC.processImageURLs // <-- call-back function
 			         });
}
//
// -- Draw CondDB Histos for Module
//
RequestHistos.DrawLayerCondDBHisto = function()
{
  var queryString;
  var url = WebLib.getApplicationURLWithLID();
  queryString = "RequestID=PlotLayerCondDBHistos";
  var obj      = $('summ_struc_name');
  var sname    = obj.options[obj.selectedIndex].value;
  queryString += '&StructureName='+sname;
  var option = 'PedestalFromCondDB,NoiseFromCondDB,GainFromCondDB';
  queryString += '&option='+option;  
  url += queryString;
  var getMEURLS = new Ajax.Request(url,                    
 	 		         {			  
 	 		          method: 'get',	  
 			          parameters: '', 
 			          onComplete: IMGC.processImageURLs // <-- call-back function
 			         });
}
//
// Check Shifter Report
//
RequestHistos.CheckShiftReport = function() {
  var url          = WebLib.getApplicationURL();
  url              = url + "/sistrip_shift_report.txt"; 
  var retVal = new Ajax.Request(url,
                               {           
                  		method: 'get',	  
 			        parameters: '', 
 			        onSuccess: RequestHistos.FillTextStatus
 			       });
}
