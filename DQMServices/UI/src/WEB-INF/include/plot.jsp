<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>
<%
  String mediaurl = WebUtils.getMediaURL();
%>


<script type="text/javascript">

	var drawChart; 

  $(document).ready( function () {

  var from=0;
  var to=0;
  
  function QueryValues(operator) {
  
		var formVals = new Array();
  
		function  formValueRange(fiel, ran_f, ran_t) {
			this.field = fiel;
			this.range_from = ran_f;
			this.range_to = ran_t;
			return this;
		}
  
		var field ='';
  
		field = 'RUN_NUMBER';

		if ((from != 0) && (to != 0)) { formVals.push(new formValueRange(field,  from, to)); }
         
		var encoded = $.toJSON(formVals);
  
		var formv = new Array();
		if (encoded!="[]")  { formv.query=encoded; }
		formv.operator=operator;
		//alert(formv.query + '\n' + formv.operator);
		return formv;
  
  }

	$('#plot').dialog({
      autoOpen: false,
      width: 800,
      height: 600,
      modal: false,
      resizable: false,
      buttons: {
        "Close": function() { 
          $("#chart").empty();
          $(this).dialog("close"); 
        },
        "Refresh": function() { 
          drawChart();
        },
        "Query": function() { 
        $("#flex1").flexSearch(QueryValues('AND'));
        },
      }
    });

  });
  
	var add_sel = true;

	drawChart = function() {

		$('#plot_overlay').show();
		$('#plot').dialog('open'); //<div style="vertical-align: middle;">
  
		if (add_sel) {
			$('#plot + .ui-dialog-buttonpane').append('<div style="vertical-align: middle; margin-top:10px;"/>');
			$('#plot + .ui-dialog-buttonpane div').append('<label>x: </label><select id="x_axis"><option value="RUN_NUMBER" selected="selected">RUN#</option><option value="TIME">Time</option></select>  ');
			$('#plot + .ui-dialog-buttonpane div').append('<label>y1: </label><select id="y1_axis"><option value="rate" selected="selected">Rate, hz</option><option value="events">Events</option><option value="bfield">B Field</option></select>  ');
			$('#plot + .ui-dialog-buttonpane div').append('<label>type </label><select id="y1_type"><option value="points">Points</option><option value="lines">Lines</option><option value="bars" selected="selected">Bars</option></select>  ');
			$('#plot + .ui-dialog-buttonpane div').append('<label>y2: </label><select id="y2_axis"><option value="">none</option><option value="rate">Rate, hz</option><option value="events" selected="selected">Events</option><option value="bfield">B Field</option></select>  ');
			$('#plot + .ui-dialog-buttonpane div').append('<label>type </label><select id="y2_type"><option value="points" selected="selected">Points</option><option value="lines">Lines</option><option value="bars">Bars</option></select>  ');
			add_sel = false;
		}
  
		from=0;
    to=0;
    
		$("#chart").empty();

		var url = "runchartdata?intpl=xml,chart&page=1&rp=100";
		if ($.cookie("flex_qtype")) url += "&qtype=" + escape($.cookie("flex_qtype"));
		if ($.cookie("flex_query")) url += "&query=" + escape($.cookie("flex_query"));
		if ($.cookie("flex_querya")) url += "&querya=" + escape($.cookie("flex_querya"));
		if ($.cookie("flex_sortname")) url += "&sortname=" + escape($.cookie("flex_sortname"));
		if ($.cookie("flex_sortorder")) url += "&sortorder=" + escape($.cookie("flex_sortorder"));
	
		$.ajax({
			type: "GET",
			url: url,
			processData: true,
			async: true,
			dataType: "json",

			error: function(o) {
				messageBox(o, "Please contact CMS DQM expert");
				$('#plot').dialog('close');
			},

			success: function(data) {
      
				var x_axis='';       
				var y1_axis='';
				var y2_axis='';
				var y1_type='';
				var y2_type='';  

				x_axis = $('#x_axis').val();
				y1_axis = $('#y1_axis').val();
				y2_axis = $('#y2_axis').val();        	
				y1_type = $('#y1_type').val();
				y2_type = $('#y2_type').val();
      
				//alert(x_axis+y1_axis+y2_axis+y1_type+y2_type);
				var y1;
				var y2;
      
				if (y1_axis == 'rate') {y1 = data.byRunNumber.rate;}
        else if  (y1_axis == 'events') {y1 = data.byRunNumber.events;}
        else if  (y1_axis == 'bfield') {y1 = data.byRunNumber.bfield;}
        else {y1 = data.byRunNumber.rate;}
        
				if (y2_axis == 'rate') {y2 = data.byRunNumber.rate;}
        else if  (y2_axis == 'events') {y2 = data.byRunNumber.events;}
        else if  (y2_axis == 'bfield') {y2 = data.byRunNumber.bfield;}
        else  {y2 = null;}
      
			 	var oy1 = { label: y1.label, data: y1.data };
				$(oy1).attr(y1_type, { show: true, fill: true });
				var oy2 = null;

				if (y2 != null) {
					oy2 = { label: y2.label, data: y2.data, yaxis: 2 };
					$(oy2).attr(y2_type, { show: true, fill: true });
					/*
						cdata = [ 
							{ label: y1.label, data: y1.data, bars:   { show: true, fill: true } },
							{ label: y2.label, data: y2.data, points: { show: true }, yaxis: 2 }
						];
					*/    
				}

				var cdata=new Array();  
				if   (y2 != null)  {cdata = [ oy1, oy2 ];}
				else {cdata = [ oy1];}    

				var options = { 
					selection: { mode: "x" },
					legend: { position: 'ne' },
					xaxis: { tickDecimals: 0 }
				};
       
				var chart = $("#chart");
				chart.bind("plotselected", function (event, ranges) {
              
					from = ranges.xaxis.from.toFixed(1);
					to = ranges.xaxis.to.toFixed(1);
					plot = $.plot(chart, cdata, 
						$.extend(true, {}, options, {
							xaxis: { min: ranges.xaxis.from, max: ranges.xaxis.to }
					}));
				});

				var plot = $.plot(chart, cdata, options);
				$('#plot_overlay').hide('fast');

			}

		});

	}

</script>

<div id="plot" title="Chart" style="text-align:center;vertical-align:middle;">
  <div id="plot_overlay"></div>
  <div id="chart" style="width:97%;height:97%"></div>
</div>
