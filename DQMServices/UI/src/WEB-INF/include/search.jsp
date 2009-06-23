<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>

<%
  String mediaurl = WebUtils.getMediaURL();
%>

<script type="text/javascript">

var addSearchRow;
var addSearchRow2;
var showSearchForm;

$(document).ready( function () {

  var sel_1 = function (sitems, sel_) {	
    var sopt_ = "";
    for (var s = 0; s < sitems.length; s++) {
      sel = '';
      if (sel_ == sitems[s].name) sel = 'selected="selected"';
      sopt_ += "<option value='" + sitems[s].name + "' " + sel + " >" + sitems[s].display + "&nbsp;&nbsp;</option>";
    }
    return sopt_;
  }

  var sel_2 = function (sitems, sel_) {
    var sopt_ = "";
    for (var s = 0; s < sitems.length; s++) {		
      sel = '';
      if (sel_ == sitems[s].name) sel =  'selected="selected"';
      sopt_ += "<option value='" + sitems[s].name + "' " + sel + " >" + sitems[s].display + "&nbsp;&nbsp;</option>";
    }
    return sopt_;
  }

  var SearchValues = function (operator) {

    var formVals = new Array();

    function formValue(fiel, val) {
      this.field = fiel;
      this.value = val;
      return this;
    }

    function formValueRange(fiel, ran_f, ran_t) {
      this.field = fiel;
      this.range_from = ran_f;
      this.range_to = ran_t;
      return this;
    }

    $("div .sDiv1").each(function (i, o) {
      var field = null;
      var value = null;
      field = $(o).children("select").val();
      value = $(o).children("input").val();
      if (value!=null) {
      if ((value.length != 0 ) && (value != null))  formVals.push(new formValue(field, value)); }
    });

		var error = 0;
    $("div .sDiv2").each(function (i, o) {
			if (error) return;
      var field = '';
      var from = '';
      var to = '';
      field = $(o).children("select").val();
      from = $(o).children("input[name='from']").val();
      to = $(o).children("input[name='to']").val();
      if ((from != '') && (to != '') && (from != undefined) && (to != undefined)) {
				formVals.push(new formValueRange(field,  from, to)); 
			} else if ((from == '') && (to != '')) {
				messageBox("From field is not provided", "Please fill in the missing field and try again."); 
				error = 1;
			} else if ((from != '') && (to == '')) {
				messageBox("To field is not provided", "Please fill in the missing field and try again."); 
				error = 1;
			}
    });

		if (error) return 0;
		   
    var encoded = $.toJSON(formVals);
		var formv = new Array();
    if (encoded != "[]")  { formv.query = encoded; }
    formv.operator = operator;
    return formv;

  }

   var p = {
      searchitems : [
        {display: 'RUN#', name : 'RUN_NUMBER', isdefault: true},
        {display: 'Online Shifter', name : 'RUN_ONLINE_SHIFTER'},
        {display: 'Offline Shifter', name : 'RUN_OFFLINE_SHIFTER'},
        {display: 'Global Name', name : 'RUN_GLOBALNAME'},
        {display: 'Status', name : 'RUN_STATUS'},
        {display: 'Current Tag', name : 'RUN_TAG'},
        {display: 'Tags', name : 'RUN_COUNT_TAGS'},
        {display: 'L1 Key', name: 'RUN_L1KEY'},
        {display: 'HLT Key', name: 'RUN_HLTKEY'},
        {display: 'Started', name : 'RUN_START_TIME'},
        {display: 'Stopped', name : 'RUN_END_TIME'},
        {display: 'Events', name : 'RUN_EVENTS'},
        {display: 'Rate, hz', name : 'RUN_RATE'},
        <dqm:listSubsystems type="ONLINE">
        {display: '${sub_abbr}', name: '${sub_abbr}'},
        </dqm:listSubsystems>
        <dqm:listSubsystems type="OFFLINE">
        {display: '${sub_abbr}', name: '${sub_abbr}'},
        </dqm:listSubsystems>
        {display: 'Created', name : 'RUN_CREATE_TIME'}
      ]
    };
	
  var r = {
    searchitems : [
      {display: 'RUN#', name : 'RUN_NUMBER', isdefault: true},
//    {display: 'Started', name : 'RUN_START_TIME'},
//    {display: 'Stopped', name : 'RUN_END_TIME'},
      {display: 'Events', name : 'RUN_EVENTS'},
      {display: 'Rate, hz', name : 'RUN_RATE'}      
//    {display: 'Created', name : 'RUN_CREATE_TIME'}
    ]
  };

  sitems = p.searchitems;
  sitems2 = r.searchitems;

  var sopt = sel_1(sitems, '');
  var sopt2 = sel_2(sitems2, '');

  if (p.qtype=='') p.qtype = sitems[0].name;
  var query = "";
  if (p.query) query = p.query;

  addSearchRow = function () {
    $("div .sfield").append("<div class='sDiv1'><input type='text' size='30' name='q' class='qsbox' value='"+query+"'/><select name='qtype'>"+sopt+"</select>  <img onclick=\"this.parentNode.innerHTML ='';\" src=\"<%=mediaurl%>minus.png\"/></div>");
  }
	
  addSearchRow2 = function () {
    $("div .sfield2").append("<div class='sDiv2'><label>  From: </label><input type='text' style='width:150px;' size='30' name='from' class='qsbox' value=''/><label>  To: </label><input type='text' style='width:150px;' size='30' name='to' class='qsbox' value=''/><select name='qtype'>"+sopt2+"</select>  <img onclick=\"this.parentNode.innerHTML ='';\" src=\"<%=mediaurl%>minus.png\"/></div>");
  }


  $('#searchf').dialog({
    autoOpen: false,
    width: 680,
    height: 420,
    modal: false,
    buttons: {
      "Close": function() { 
        $(this).dialog("close"); 
      },
      "Clear": function() { $("#flex1").flexClear(); },
      "Search any": function() { 
				var sv = SearchValues('OR');
				if (sv.operator != undefined) $("#flex1").flexSearch(sv); 
			},
      "Search all": function() {
				var sv = SearchValues('AND');
				if (sv.operator != undefined) $("#flex1").flexSearch(sv); 
			},
    }
  });

  $("#searchf_info_tabs").tabs();
  $("#searchf_info_tabs2").tabs();

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

  showSearchForm = function (number) {
    var _querya = $.cookie("flex_querya");
    var json_querya = new Array();
    if (_querya != '') json_querya = eval('(' + _querya + ')');
	
///////////////////////////////////////////////////////////////////////////

    $("div .sfield").empty();
    $("div .sfield2").empty();

    $("div .sfield").append("<div class='sDiv1'><input type='text' size='30' name='q' class='qsbox' value=''/><select name='qtype'>"+sopt+"</select>  <img onclick=\"addSearchRow();\" src=\"<%=mediaurl%>plus.png\"/></div>");
    $("div .sfield2").append("<div class='sDiv2'> From: <input type='text' style='width:150px;' size='30' name='from' class='qsbox' value=''/>  To: <input type='text' style='width:150px;' size='30' name='to' class='qsbox' value=''/><select name='qtype'>"+sopt2+"</select>  <img onclick=\"addSearchRow2();\" src=\"<%=mediaurl%>plus.png\"/></div>");

//////////////////////////////////////////////////////////////////////////

    for(var i=0; i < json_querya.length; i++) {
      if ((json_querya[i].value != '') && (json_querya[i].value != undefined)) {
        $("div .sfield").append("<div class='sDiv1'><input type='text' size='30' name='q' class='qsbox' value='"+json_querya[i].value+"'/><select name='qtype'>"+sel_1(sitems,json_querya[i].field)+"</select>  <img onclick=\"this.parentNode.innerHTML ='';\" src=\"<%=mediaurl%>minus.png\"/></div>");
      }
      if ((json_querya[i].range_from != '') && (json_querya[i].range_to != '') && (json_querya[i].range_from != undefined) && (json_querya[i].range_to != undefined)) {
        if (sitems2,json_querya[i].field == 'RUN_NUMBER') {
          var pos = json_querya[i].range_from.lastIndexOf('.');
          if (pos != -1) json_querya[i].range_from  = json_querya[i].range_from.substring(0,pos);
          var pos2 = json_querya[i].range_to.lastIndexOf('.');
          if (pos2 != -1) json_querya[i].range_to  = json_querya[i].range_to.substring(0,pos2);
        }
        $("div .sfield2").append("<div class='sDiv2'><label>  From: </label><input type='text' style='width:150px;' size='30' name='from' class='qsbox' value='"+json_querya[i].range_from +"'/><label>  To: </label><input type='text' style='width:150px;' size='30' name='to' class='qsbox' value='"+json_querya[i].range_to +"'/><select name='qtype'>"+sel_2(sitems2,json_querya[i].field)+"</select>  <img onclick=\"this.parentNode.innerHTML ='';\" src=\"<%=mediaurl%>minus.png\"/></div>");
      }
    }

    $('#searchf_overlay').show();
    $('#searchf').dialog('open');
    $('#searchf').dialog('option', 'title', 'Advanced search');
  
    $("button:contains('Apply')").hide();
    $("button:contains('Delete')").hide();
        
    $("button:contains('Apply')").show();
    $("button:contains('Delete')").show();

    $('#edit_overlay').hide('fast');
  };

});

</script>

<div id="searchf" title="Edit Run" style="display: none">
  <div id="searchf_overlay"></div>
<form name="adSearch" id="adSearch" >

  <table id="searchf_table" width="100%" border="0">
    <tr>
      <td id="searchf_info_tabs"  valign="top">
        <ul>
          <li><a href="#search_info_tab"><span>All fields</span></a></li>
        </ul>
        <div id="search_info_tab">
          <table width="100%">
            <tr>
              <td class="searchf_col">
                <div class="searchf_sub searchf_sub_online" id="searchf_sub_online_tab">
                  <div class="sfield"/>
                </div>				
              </td>
            </tr>
          </table>

        </div>
      </td>
    </tr>
  </table>
  
  <table id="searchf_table2" width="100%" border="0">
    <tr>
      <td id="searchf_info_tabs2"  valign="top">
        <ul>
          <li><a href="#search_info_tab2"><span>Numeric fields</span></a></li>
        </ul>
        <div id="search_info_tab2">
          <table width="100%">
            <tr>
              <td class="searchf_col">
                <div class="searchf_sub searchf_sub_offline" id="searchf_sub_offline_tab">
				 					<div class="sfield2"/>
								</div>				
              </td>
            </tr>
          </table>

        </div>
      </td>
    </tr>
  </table>
</form>
</div>
