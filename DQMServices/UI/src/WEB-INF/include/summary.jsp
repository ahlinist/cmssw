<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>
<%
  String mediaurl = WebUtils.getMediaURL();
%>


<script type="text/javascript">



var addfields = true;
$(document).ready( function () {

  var headr = '';
 
  $('#summary').dialog({
    autoOpen: false,
    width: 550,
		height: 620,
    modal: false,
    buttons: {
      "Close": function() { 
        $(this).dialog("close"); 
      }
    }
  });

  var isTrue = function (s) {
    if (!s) return false;
    if ((s == "null") || (s == "")) return false;
    return true;
  }
 
  var escapeXML = function (s) {
		if (s == undefined || s == '') return s;
    var LT = new RegExp("<", "g");
    var GT = new RegExp(">", "g");
    var AMP = new RegExp("&", "g");
    var TAB = new RegExp("\t", "g");
    return s.replace(AMP,"&amp;").replace(LT, "&lt;").replace(GT, "&gt;").replace(TAB, "    ");
  };

function roundNumber(number,decimals) {
	var newString;// The new rounded number
	decimals = Number(decimals);
	if (decimals < 1) {
		newString = (Math.round(number)).toString();
	} else {
		var numString = number.toString();
		if (numString.lastIndexOf(".") == -1) {// If there is no decimal point
			numString += ".";// give it one at the end
		}
		var cutoff = numString.lastIndexOf(".") + decimals;// The point at which to truncate the number
		var d1 = Number(numString.substring(cutoff,cutoff+1));// The value of the last decimal place that we'll end up with
		var d2 = Number(numString.substring(cutoff+1,cutoff+2));// The next decimal, after the last one we want
		if (d2 >= 5) {// Do we need to round up at all? If not, the string will just be truncated
			if (d1 == 9 && cutoff > 0) {// If the last digit is 9, find a new cutoff point
				while (cutoff > 0 && (d1 == 9 || isNaN(d1))) {
					if (d1 != ".") {
						cutoff -= 1;
						d1 = Number(numString.substring(cutoff,cutoff+1));
					} else {
						cutoff -= 1;
					}
				}
			}
			d1 += 1;
		} 
		newString = numString.substring(0,cutoff) + d1.toString();
	}
	if (newString.lastIndexOf(".") == -1) {// Do this again, to the new string
		newString += ".";
	}
	var decs = (newString.substring(newString.lastIndexOf(".")+1)).length;
	for(var i=0;i<decimals-decs;i++) newString += "0";
	//var newNumber = Number(newString);// make it a number if you like
	return newString; // Output the result to the form field (change for your purposes)
}

  
  
     function createDynamicTable(tbody, data) {
		 var flag = true;
         if (tbody == null || tbody.length < 1) return;
		 jQuery.each(data, function (n, v) {

             var trow = $("<tr>");
			 if (flag == true)
			   {
			   			 $("<th style='border: solid 1px #666666; width: 90px;'>")
                         .addClass("tableCell")
                         .text('Summary values')
                         .appendTo(trow);
						  $("<th style='border: solid 1px #666666;'>")
                         .addClass("tableCell")
                         .text('DQM Online')
                         .appendTo(trow);
						 $("<th style='border: solid 1px #666666;'>")
                         .addClass("tableCell")
                         .text('DQM Offline')
                         .appendTo(trow);
						 $("<th style='border: solid 1px #666666;'>")
                         .addClass("tableCell")
                         .text('DAQ')
                         .appendTo(trow);
						 $("<th style='border: solid 1px #666666;'>")
                         .addClass("tableCell")
                         .text('DCS')
                         .appendTo(trow);
						 $("<th style='border: solid 1px #666666;'>")
                         .addClass("tableCell")
                         .text('Cert.')
                         .appendTo(trow);
						 trow.appendTo(tbody);
						 trow = $("<tr>");	   
			   			 flag = false;
			   	}
				
			 $("<td style='border: solid 1px #666666; width: 110px;'>")
                         .addClass("tableCell")
                         .text(n)
                         .appendTo(trow);
			 
			 jQuery.each(v, function (n_, v_) {

                 $("<td style='border: solid 1px #666666;'>")
                         .addClass("tableCell")
                         .text(roundNumber(v_,2))
                         .appendTo(trow);
             });
             trow.appendTo(tbody);
          });
     }   



  $.showRunSummaryForm = function (number) {

	if ($("#accordion").accordion)  $("#accordion").accordion( 'destroy' );
	$("#accordion").empty();
    $('#summary_overlay').show();
    $('#summary').dialog('open');
    $('#summary').dialog('option', 'title', 'Summary values Run #' + number);
	$("#summary_error").attr("style","display:none;");
	$('#summary').dialog('isOpen');
	

   $.ajax({
      type: "GET",
      url: "runsummaryvalues?qtype=RUN_NUMBER&intpl=summary&mime=text/plain&query=" + number, //runsummaryvalues?query=70681&qtype=RUN_NUMBER&intpl=summary&mime=text/plain
      processData: false,
      async: true,
      dataType: "json",

      error: function(o) {
        messageBox(o, "Please contact CMS DQM expert.");
        $('#summary').dialog('close');
      },

      success: function(json) {
	  	
        if (json.length == 0) {
          $("#summary_error").removeAttr("style")
          //$('#summary').dialog('close');
          return;
        } 
	 		

	 	 var s=0;
		 jQuery.each(json, function (n, v) {
		  s=s+1;
		  $("#accordion").append("<h3><a href='#'>"+ n +"</a></h3>");
		  $("#accordion").append("<div style='heigth:100px;'/>");
		  $("#accordion div:last").append("<table align='center' id='tbl_"+ s +"' style='border: solid 1px #666666; width:450px; border-collapse: collapse; font-size: 10pt; '/><tbody></tbody></table>");
		  createDynamicTable($("#tbl_"+s), v); 
		  //$("#accordion div:last").append("<ul id='gray"+ s +"' class='treeview-gray'/>");
		 /* var ul01 = $("#accordion div:last ul");
		  		jQuery.each(v.folders, function (n_, v_) {	
					 $(ul01).append("<li/>");
					 $(ul01).find("li:last").append(v_.name);
					 $(ul01).find("li:last").append("<ul>");
					 var ul02 = $(ul01).find("li:last ul");
					 jQuery.each(v_.values, function (n__, v__) {	
					 	$(ul02).append("<li>" + v__.name + "=" + v__.value + "</li>");
					 });
				}); */
				
			/*$("#gray"+s).treeview({
				control: "#treecontrol",
				//persist: "cookie",
				//cookieId: "treeview-black"
			}); */
		 });
		
		
		
    	$("#accordion").accordion({ autoHeight: false });

		
	    if ($("#accordion > h3 > a:contains('"+ headr +"')").parent().text() != '')	
		  $('#accordion').accordion( 'activate' , $("#accordion > h3 > a:contains('"+ headr +"')").parent() );
		
		$('#accordion').bind('accordionchange', function(event, ui) {
		  //alert(ui.newHeader.text()); // jQuery object, activated header
		  headr = ui.newHeader.text();
		 
		});
		
		$('#summary_overlay').hide('slow');
		
	  } 
   });
 };
});

</script>

<div id="summary" title="Summary values #Run" style="display: none">
  <div id="summary_overlay"></div>
  <div id="summary_error">This run does'nt have summary values</div>
<div id="accordion">
</div>
</div>




