<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>

<%
  MessageUser user = MessageUser.get(request);
%>

<script type="text/javascript">

function textareaValue (name) {
  var field = name + '_comment';
  field = document.getElementById(field).value;
  sel = document.getElementById(name);
  selectbox = document.getElementById('expertCommentSel');
  selectbox.innerHTML = sel.innerHTML; 
  for (var i=0; i < selectbox.length; i++) {
    if (selectbox[i].value == sel.value) {
      selectbox[i].selected = true;
    }
  }
  document.getElementById('expertsComment').value=field;
  sel = document.getElementById(name);
}

function validateSubSelectExp(c) {
    var sel = $(c).find("select");
    var txa = $(c).find("textarea");

    txa.removeAttr("error")
    txa.removeClass("error");
    if (sel.val() == "GOOD") {
      sel.css("color", "green");
    } else if (sel.val() == "NOTSET") {
      sel.css("color", "blue");
    } else if (sel.val() == "BAD") {
      sel.css("color", "red");
      if (txa.val() == "") {
        txa.attr("error", "Comment for BAD subsystem must be set.");
        txa.addClass("error");
      }
    } else if (sel.val() == "EXCL") {
      sel.css("color", "blue");
    //  if (txa.val() == "") {
    //    txa.attr("error", "Comment for EXCL subsystem must be set.");
    //    txa.addClass("error");
    // }
    }
}

function expertsSel(value) {
  var name = document.getElementById('expertsCommentSub').value;
  for (var i=0; i < sel.length; i++) {
    if (sel[i].value == value) {
      sel[i].selected = true;
    }
  }
  var sell = document.getElementById(name);
  var c = $(sell).parent().parent();
  validateSubSelectExp(c);
}

function updateExpertsComment(comment) {
  var field = document.getElementById('expertsCommentSub').value+'_comment';
  document.getElementById(field).value=comment;
  var sell = document.getElementById(field);
  var c = $(sell).parent().parent();
  validateSubSelectExp(c);
}

var errorMessage = function (o) {
	alert("Error! Changes where not saved. Reason:\n\n" + o.responseText + "\n\nPlease copy above text and post ELOG message.");
}

$(document).ready( function () {

  $('#edit').dialog({
    autoOpen: false,
    width: 880,
		height: 620,
    modal: false,
    buttons: {
      "Close": function() { 
        $(this).dialog("close"); 
      },
      "Delete": function() { },
      "Apply": function() { },
	  "Finish DQM": function() { },
    }
  });

  $("#edit_sub_tabs").tabs();
  $("#edit_l1t_tabs").tabs();
  $("#edit_info_tabs").tabs();

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

  var validateInfo = function() {
    var glob = $("#edit .edit_info input[name=RUN_GLOBALNAME]");

    glob.removeAttr("error")
    glob.removeClass("error");

    if (!glob.val() || ( glob.val() == "")) {
      glob.attr("error", "Global Name must be set.");
      glob.addClass("error");
    }
  };

  var validateSubSelect = function(c) {
    var sel = $(c).find("select");
    var txa = $(c).find("textarea");

    txa.removeAttr("error")
    txa.removeClass("error");
    if (sel.val() == "GOOD") {
      sel.css("color", "green");
    } else if (sel.val() == "NOTSET") {
      sel.css("color", "blue");
    } else if (sel.val() == "BAD") {
      sel.css("color", "red");
      if (txa.val() == "") {
        txa.attr("error", "Comment for BAD subsystem must be set.");
        txa.addClass("error");
      }
    } else if (sel.val() == "EXCL") {
      sel.css("color", "blue");
     // if (txa.val() == "") {
     //   txa.attr("error", "Comment for EXCL subsystem must be set.");
     //   txa.addClass("error");
     // }
    }
  };

  var validateL1T = function (c) {
      var sel = $(c).find("input[type=checkbox]");
      var txa = $(c).find("textarea");

      if (sel.is(":checked")) {
        txa.removeAttr("disabled");
      } else {
        txa.attr("disabled", "true");
      }
 
      if ($("#edit .edit_l1t input:checkbox:checked").length == 0) {
        $("#edit .edit_l1t textarea").addClass("error");
        $("#edit .edit_l1t textarea").attr("error", "At least one L1 source must be set.");

      } else {
        $("#edit .edit_l1t textarea").removeClass("error");
        $("#edit .edit_l1t textarea").removeAttr("error");

      }
  };

  $("#edit form .edit_info input[type=text]").unbind("change").change(function () { validateInfo(); });
  $("#edit form .edit_info input[type=text]").unbind("keypress").keypress(function () { validateInfo(); });
  $("#edit form .edit_info textarea").unbind("change").change(function () { validateInfo(); });
  $("#edit form .edit_info textarea").unbind("keypress").keypress(function () { validateInfo(); });
  $("#edit form .edit_info input[name=RUN_GLOBALNAME]").autocomplete([<dqm:listGlobalNamesJS/>]);

  $("#edit form .edit_sub select").unbind("change").change(function () { validateSubSelect($(this).parent().parent()); });
  $("#edit form .edit_sub input[type=text]").unbind("change").change(function () { validateSubSelect($(this).parent().parent()); });
  $("#edit form .edit_sub textarea").unbind("change").change(function () { validateSubSelect($(this).parent().parent()); });

  $("#edit form .edit_l1t input[type=checkbox]").unbind("change").change(function () { validateL1T($(this).parent().parent()); });

  var build_xml = function (stat) {
    var xml = "";
    var status = "";

    $("#edit .edit_info input[type=text], #edit .edit_info textarea").each(function (i, o) {
            if ($(o).attr("readonly")) return;
            xml += "\t<" +$(o).attr("name")+ ">" + escapeXML($(o).val()) + "</" +$(o).attr("name")+ ">\n";
    });
    if (stat!=null)
	{
	  if (stat=='OFFLINE') {status = 'SIGNOFF';}
	  	else if (stat=='ONLINE') {status = 'OFFLINE';}
		else if (stat=='SIGNOFF') {status = 'COMPLETED';}
	  xml += "\t<RUN_STATUS>"+ status  +"</RUN_STATUS>\n";
	
	}else {
				$("#edit .edit_info select[name=RUN_STATUS] option:selected").each(function () {
				  if ($(this).attr("readonly")) return;
				  status = $("#edit .edit_info select[name=RUN_STATUS]").val();
				  xml += "\t<RUN_STATUS>"+ status  +"</RUN_STATUS>\n";
				});
			}

    $("#edit .edit_l1t input[type=checkbox]").each(function (i, o) {
			if ($(o).attr("readonly")) return;
			var name = $(o).attr("name");
			xml += "\t<" +name+ ">" + $(o).is(":checked") + "</" +name+ ">\n";
			if ($(o).is(":checked"))
				xml += "\t<" +name+ "_comment>" + $("#" + name + "_comment").val() + "</" +name+ "_comment>\n";
    });

    $("#edit .edit_sub select").each(function (i, o) {
			if ($(o).attr("readonly")) return;
			if ($(o).attr("disabled")) return;
			var name = $(o).attr("name").replace("sub_", "");
			xml += "\t<" +name+ ">" + $(o).val() + "</" +name+ ">\n";
			xml += "\t<" +name+ "_comment>" + $("#sub_" + name + "_comment").val() + "</" +name+ "_comment>\n";
    });

    if (status == "COMPLETED") {
      var tag = getTag();
      xml += "\t<TAG>"+ tag + "</TAG>\n";
    }

    return xml
  };

  $.showRunEditForm = function (number) {

    $('#edit_overlay').show();
    $('#edit').dialog('open');
    $('#edit').dialog('option', 'title', 'View/Edit Run #' + number);

	$("button:contains('Finish')").hide();   
    $("button:contains('Apply')").hide();
    $("button:contains('Delete')").hide();

    $.ajax({
      type: "GET",
      url: "runeditdata?qtype=RUN_NUMBER&query=" + number,
      processData: false,
      async: true,
      dataType: "xml",

      error: function(o) {
        errorMessage(o);
        $('#edit').dialog('close');
      },

      success: function(ret) {
        var xml = $(ret).find("RESULTS");

        if ($(xml).find("ROW").length == 0) {
          alert("Run not found?");
          $('#edit').dialog('close');
          return;
        }

        /* Pagrindine inicializacija */
        $("#edit input").attr("readonly", "true");
        $("#edit select").attr("readonly", "true");
        $("#edit textarea").attr("readonly", "true"); 
        $("#edit .edit_l1t textarea").attr("disabled", "true");
        $("#edit .edit_l1t input[type=checkbox]").removeAttr("checked");
        $("#edit form .edit_sub .sub_online > option").remove();
        $("#edit .edit_info select[name=RUN_STATUS]").empty();
        
        $("#edit form input[name=RUN_NUMBER]").val(number);

        $(xml).find("ROW").children().each(function (i, o) {

          $("#edit form .edit_sub select[name=sub_" +o.nodeName+ "]").each(function (i, sel) {
            $("#edit form .edit_sub textarea[name=sub_" +o.nodeName+ "_comment]").val($(xml).find(o.nodeName+ "_comment").text());
          });

          $("#edit form .edit_sub select[name=sub_" +o.nodeName+ "][class=sub_online]").each(function (i, sel) {
            if (isTrue($(o).text()) && $(o).text() != "EXCL") {
              $(sel).append("<option value=\"GOOD\">GOOD</option>");
              $(sel).append("<option value=\"BAD\">BAD</option>");
              if ($(o).text() == "BAD") 
                $(sel).val("BAD");
              else 
                $(sel).val("GOOD");
            } else {
              $(sel).append("<option value=\"EXCL\">EXCL</option>");
            };
          });

          $("#edit form .edit_sub select[name=sub_" +o.nodeName+ "][class=sub_offline]").each(function (i, sel) {
            if (isTrue($(o).text())) {
              if ($(o).text() == "BAD") $(sel).val("BAD");
              if ($(o).text() == "GOOD") $(sel).val("GOOD");
              if ($(o).text() == "NOTSET") $(sel).val("NOTSET");
            } else {
              $(sel).val("NOTSET");
            };
          });

          $("#edit form .edit_l1t input[name=" +o.nodeName+ "]").each(function (i, sel) {
            if (isTrue($(o).text())) {
              $(sel).attr("checked", true);
              $("#edit form .edit_l1t textarea[name=" +o.nodeName+ "_comment]").val($(xml).find(o.nodeName+ "_comment").text());
            }
          });

          if (o.nodeName == "RUN_STATUS" && $(o).text() != "") {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option readonly=\"true\" value=\"+ $(o).text() +\">"+ $(o).text() +"</option>"));
          }

          $("#edit form .edit_info input[name=" + o.nodeName + "]").val($(o).text());
          $("#edit form .edit_info textarea[name=" + o.nodeName + "]").val($(o).text());

        });
		
        document.getElementById('expertsCommentSub').selectedIndex = 0;
        var field = 'sub_CSC_comment';

        field = document.getElementById(field).value;
        sel = document.getElementById('sub_CSC');
        selectbox = document.getElementById('expertCommentSel');
        selectbox.innerHTML = sel.innerHTML;
        for (var i=0; i < selectbox.length; i++) {
          if (selectbox[i].value == sel.value) {
            selectbox[i].selected = true;
          }
        }
        document.getElementById('expertsComment').value = field;

        validateInfo();
        $("#edit form .edit_l1t input[type=checkbox]").each(function () { validateL1T($(this).parent().parent()); });
        $("#edit form .edit_sub select").each(function () { validateSubSelect($(this).parent().parent()); });

        var real_role = "";
        var status = $(xml).find("RUN_STATUS").text();
		if (status =='COMPLETED') 
			{$("button:contains('Finish')").hide();}
		else {
				$("button:contains('Finish')").text('Finish '+status+' DQM');
				$("button:contains('Finish')").val('Finish '+status+' DQM');
			}

        var count_tags = parseInt($(xml).find("RUN_COUNT_TAGS").text());
        count_tags--;
     
        if (status != "COMPLETED") {
          if (<%= user.hasLoggedRole(WebUtils.EXPERT) %>) real_role = "EXPERT";
          else if (<%= user.hasLoggedRole(WebUtils.ONLINE) %> && (status == "ONLINE" || status == ""))
            real_role = "ONLINE";
          else if (<%= user.hasLoggedRole(WebUtils.OFFLINE) %> && status == "OFFLINE")
            real_role = "OFFLINE";
        }
 
        if ($(xml).find("RUN_EVENTS").text() == "" || 
            $(xml).find("RUN_START_TIME").text() == "" ||
            $(xml).find("RUN_RATE").text() == "" ||
            $(xml).find("RUN_L1KEY").text() == "" ||
            $(xml).find("RUN_HLTKEY").text() == "") {
          real_role = "";
        }

        if (real_role == "ONLINE") {

          $("#edit form .edit_info .edit_online input").removeAttr("readonly");
          $("#edit form .edit_info .edit_online textarea").removeAttr("readonly");

          $("#edit form .edit_l1t input").removeAttr("readonly");
          $("#edit form .edit_l1t input").removeAttr("disabled");
          $("#edit form .edit_l1t textarea").removeAttr("readonly");
          $("#edit form .edit_l1t textarea").removeAttr("disabled");


          $("#edit form .edit_sub_online textarea").removeAttr("readonly");
          $("#edit form .edit_sub_online select").removeAttr("disabled");
          $("#edit form .edit_sub_online select").removeAttr("readonly");
		  
	  $("#edit form .edit_sub_online_exp textarea").removeAttr("readonly");
          $("#edit form .edit_sub_online_exp select").removeAttr("disabled");
          $("#edit form .edit_sub_online_exp select").removeAttr("readonly");
          
          if (status == "") {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"ONLINE\">ONLINE</option>"));
            $("#edit form .edit_info input[name='RUN_ONLINE_SHIFTER']").val("<%=user.getName()%>");
          } else {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"OFFLINE\">to OFFLINE</option>"));
            $("button:contains('Delete')").show();
          }
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("disabled");
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("readonly");

          $("button:contains('Finish')").show();
		  $("button:contains('Apply')").show();

        } else if (real_role == "OFFLINE")  {

          $("#edit form .edit_info .edit_offline input").removeAttr("readonly");
          $("#edit form .edit_info .edit_offline textarea").removeAttr("readonly");
          
          $("#edit form .edit_sub_offline textarea").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("disabled");

          if (count_tags != 0) {
            $("#edit form .edit_sub_online textarea").removeAttr("readonly");
            $("#edit form .edit_sub_online select").removeAttr("disabled");
            $("#edit form .edit_sub_online select").removeAttr("readonly");
			
            $("#edit form .edit_sub_online_exp textarea").removeAttr("readonly");
            $("#edit form .edit_sub_online_exp select").removeAttr("disabled");
            $("#edit form .edit_sub_online_exp select").removeAttr("readonly");
          }
          
          $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"SIGNOFF\">to SIGNOFF</option>"));
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("disabled");
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("readonly");

          $("button:contains('Finish')").show();
		  $("button:contains('Apply')").show();

        } else if (real_role == "EXPERT") {

		  
          $("#edit form .edit_l1t input").removeAttr("readonly");
          $("#edit form .edit_l1t input").removeAttr("disabled"); 
          $("#edit form .edit_l1t textarea").removeAttr("readonly");
          $("#edit form .edit_l1t textarea").removeAttr("disabled");

          $("#edit form .edit_info .edit_online input").removeAttr("readonly");
          $("#edit form .edit_info .edit_online textarea").removeAttr("readonly");

          $("#edit form .edit_info .edit_offline input").removeAttr("readonly");
          $("#edit form .edit_info .edit_offline textarea").removeAttr("readonly");
          
          $("#edit form .edit_sub_online textarea").removeAttr("readonly");
          $("#edit form .edit_sub_online select").removeAttr("readonly");
          $("#edit form .edit_sub_online select").removeAttr("disabled");
		  
          $("#edit form .edit_sub_online_exp textarea").removeAttr("readonly");
          $("#edit form .edit_sub_online_exp select").removeAttr("readonly");
          $("#edit form .edit_sub_online_exp select").removeAttr("disabled");
		  
	  $("#edit form .expertSub select").removeAttr("readonly");
          $("#edit form .expertSub select").removeAttr("disabled")
          
          $("#edit form .edit_sub_offline textarea").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("disabled");

          $("button:contains('Finish')").show();
		  $("button:contains('Apply')").show();
          $("button:contains('Delete')").show();
          
          if (status == "") {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"ONLINE\">ONLINE</option>"));
          } else {
            if (status != "OFFLINE") {
              $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"OFFLINE\">to OFFLINE</option>"));
            }
            if (status != "SIGNOFF") {
              $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"SIGNOFF\">to SIGNOFF</option>"));
            }
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"COMPLETED\">to COMPLETED</option>"));
          }
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("disabled");
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("readonly");

        } else {

          $("#edit form .edit_l1t input[type=checkbox]").attr("disabled", true);
          $("#edit form .edit_l1t textarea").attr("readonly", true);

          $("#edit .edit_info select[name=RUN_STATUS]").attr("disabled", true);
    
          $("#edit form .edit_sub_online select").attr("disabled", true);
          $("#edit form .edit_sub_offline select").attr("disabled", true);

          $("#edit form .edit_info .edit_online textarea").attr("readonly", true);
          $("#edit form .edit_info .edit_offline textarea").attr("readonly", true);
        }

        $("button:contains('Apply'):visible").unbind("click").click(function () {
          var number = $("#edit form input[name=RUN_NUMBER]").val();
          var xml = "<RUN><RUN_NUMBER>" + number + "</RUN_NUMBER>" + build_xml(null);
					var err = $("#edit .error");
					if (err.length > 0) {
						$(err[0]).focus();
						alert($(err[0]).attr("error"));
						return;
					};
					xml = "<?xml version=\"1.0\"?>\n" + xml + "</RUN>";

          $.ajax({
            type: "POST",
            url: "editprovider",
            data: xml,
            processData: false,
            dataType: "xml",
            error: function(o) {
              errorMessage(o);
            },
            success: function(ret) {
              $('#edit').dialog('close');
              $("#flex1").flexReload();
              return;
            }
          });

        });
		
        $("button:contains('Finish'):visible").unbind("click").click(function () {
          var number = $("#edit form input[name=RUN_NUMBER]").val();
          var xml = "<RUN><RUN_NUMBER>" + number + "</RUN_NUMBER>" + build_xml(status);
					var err = $("#edit .error");
					if (err.length > 0) {
						$(err[0]).focus();
						alert($(err[0]).attr("error"));
						return;
					};
					xml = "<?xml version=\"1.0\"?>\n" + xml + "</RUN>";


          $.ajax({
            type: "POST",
            url: "editprovider",
            data: xml,
            processData: false,
            dataType: "xml",
            error: function(o) {
              errorMessage(o);
            },
            success: function(ret) {
              $('#edit').dialog('close');
              $("#flex1").flexReload();
              return;
            }
          }); 

        });

        $("button:contains('Delete'):visible").unbind("click").click(function () {
          var number = $("#edit form input[name=RUN_NUMBER]").val();
					if(!confirm("Do you really want to delete run #" + number +"?")) return;          
          var xml = "<RUN><RUN_NUMBER>" + number + "</RUN_NUMBER>";
					xml = "<?xml version=\"1.0\"?>\n" + xml + "\t<DELETE>true</DELETE></RUN>";
          $.ajax({
            type: "POST",
            url: "editprovider",
            data: xml,
            processData: false,
            dataType: "xml",
            error: function(o) {
              errorMessage(o);
            },
            success: function(ret) {
              $('#edit').dialog('close');
              $("#flex1").flexReload();
              return;
            }
          });

        });

				$('#edit_overlay').hide('fast');

      } 

    });

  };

});

</script>

<div id="edit" title="Edit Run">
  <div id="edit_overlay"></div>
  <form>
  <table id="edit_table" width="100%" height="100%" border="0">
    <tr height="10">
      <td class="run_number_info" valign="top">
        <table width="45%">
          <tr>
            <td><label for="RUN_NUMBER"> Run number: </label></td>
            <td><input name="RUN_NUMBER" type="text" value="" readonly="true" /></td>
          </tr>
        </table>
      </td>
    </tr>
    <tr>
      <td id="edit_info_tabs" class="edit_info" valign="top">
        <ul>
          <li><a href="#edit_info_tab"><span>General Information</span></a></li>
          <li><a href="#edit_comp_tab"><span>Components</span></a></li>
	  <li><a href="#edit_exp_com"><span>Comments</span></a></li>
        </ul>
        <div id="edit_info_tab" width="100%" height="100%">
          <table width="100%">
            <tr>
              <td><label for="RUN_ONLINE_SHIFTER"> Online Shifter: </label></td>
              <td><input name="RUN_ONLINE_SHIFTER" type="text" value="" readonly="true" /></td>
              <td><label for="RUN_OFFLINE_SHIFTER"> Offline Shifter: </label></td>
              <td><input name="RUN_OFFLINE_SHIFTER" type="text" value="" readonly="true" /></td>
            </tr>
            <tr>
              <td><label for="RUN_GLOBALNAME"> Global name: </label></td>
              <td class="edit_online"><input name="RUN_GLOBALNAME" type="text" value="" readonly="true" maxlength="90" /></td>
              <td><label for="RUN_STATUS"> Status: </label></td>
              <td>
                <select name="RUN_STATUS" disabled="true"></select>
              </td>
            </tr>
            <tr>
              <td><label for="RUN_RATE"> Rate: </label></td>
              <td><input name="RUN_RATE" type="text" value="" readonly="true" /></td>
              <td><label for="RUN_START_TIME"> Started: </label></td>
              <td><input name="RUN_START_TIME" type="text" value="" readonly="true" /></td>
            </tr>
            <tr>
              <td><label for="RUN_EVENTS"> Events: </label></td>
              <td><input name="RUN_EVENTS" type="text" value="" readonly="true" /></td>
              <td><label for="RUN_END_TIME"> Stopped: </label></td>
              <td><input name="RUN_END_TIME" type="text" value="" readonly="true" /></td>
            </tr>
            <tr>
              <td><label for="RUN_BFIELD"> B Field (Tesla): </label></td>
              <td><input name="RUN_BFIELD" type="text" value="" readonly="true" /></td>
              <td><label for="RUN_BFIELD_COMMENT"> B Field Notes: </label></td>
              <td class="edit_online"><input name="RUN_BFIELD_COMMENT" type="text" value="" readonly="true" /></td>
            </tr>
            <tr>
              <td><label for="RUN_L1KEY"> L1 Key: </label></td>
              <td colspan=3><input name="RUN_L1KEY" type="text" value="" readonly="true" /></td>
            </tr>
            <tr>
              <td><label for="RUN_HLTKEY"> HLT Key: </label></td>
              <td colspan=3><input name="RUN_HLTKEY" type="text" value="" readonly="true" /></td>
            </tr>
                      <tr>
              <td><label for="RUN_STOP_REASON"> Stop reason: </label></td>
              <td colspan=3 class="edit_online"><textarea  name="RUN_STOP_REASON" readonly="true" maxlength="90"></textarea></td>
            </tr>
            <tr>
              <td><label for="RUN_ONLINE_COMMENT"> Online comment:  </label></td>
              <td colspan=3 class="edit_online"><textarea name="RUN_ONLINE_COMMENT" readonly="true" maxlength="90"></textarea></td>
            </tr>
            <tr>
              <td><label for="RUN_OFFLINE_COMMENT"> Offline comment:  </label></td>
              <td colspan=3 class="edit_offline"><textarea name="RUN_OFFLINE_COMMENT" id="RUN_OFFLINE_COMMENT" readonly="true" maxlength="90"></textarea></td>
            </tr>

          </table>
        </div>

        <div id="edit_comp_tab" width="100%" height="100%">
          <table>
            <tr>
              <td id="edit_l1t_tabs" class="edit_col" width="50%">
                <ul>
                  <li><a href="#edit_l1t_tab"><span>L1 Triggers</span></a></li>
                </ul>
                <div class="edit_l1t" id="edit_l1t_tab" width="50%" height="100%">
                  <table width="100%">
                    <dqm:listL1Sources>
                    <tr>
                      <td><label for="l1s_${l1s_abbr}"> ${l1s_abbr} </label></td>
                      <td><input name="l1s_${l1s_abbr}" type="checkbox" disabled="true" readonly="true"/></td>
                      <td><textarea name="l1s_${l1s_abbr}_comment" id="l1s_${l1s_abbr}_comment" readonly="true" style="height:20px;" maxlength="90"></textarea></td>
                    </tr>
                  </dqm:listL1Sources>
                  </table>
                </div>
              </td>
              <td id="edit_sub_tabs" class="edit_col" width="50%" height="100%">
                <ul>
                  <li><a href="#edit_sub_online_tab"><span>DPG</span></a></li>
                  <li><a href="#edit_sub_offline_tab"><span>POG</span></a></li>
                </ul>
                <div class="edit_sub edit_sub_online" id="edit_sub_online_tab" width="100%" height="100%">
                  <table width="100%">
                    <dqm:listSubsystems type="ONLINE">
                      <tr>
                        <td><label for="sub_${sub_abbr}"> ${sub_abbr} </label></td>
                        <td><select style="width: 80px" name="sub_${sub_abbr}" id="sub_${sub_abbr}" class="sub_online" disabled="true"></select></td>
                        <td><textarea name="sub_${sub_abbr}_comment" id="sub_${sub_abbr}_comment" readonly="true"  style="height:20px;" maxlength="90"></textarea></td>
                      </tr>
                    </dqm:listSubsystems>
                  </table>
                </div>
                <div class="edit_sub edit_sub_offline" id="edit_sub_offline_tab" width="100%" height="100%">
                  <table width="100%">
                    <dqm:listSubsystems type="OFFLINE">
                      <tr>
                        <td><label for="sub_${sub_abbr}"> ${sub_abbr} </label></td>
                        <td><select style="width: 80px" name="sub_${sub_abbr}" id="sub_${sub_abbr}" class="sub_offline" disabled="true">
                          <option value="NOTSET">NOTSET</option>
                          <option value="GOOD">GOOD</option>
                          <option value="BAD">BAD</option>
                        </select></td>
                        <td><textarea name="sub_${sub_abbr}_comment" id="sub_${sub_abbr}_comment" readonly="true"  style="height:20px;" maxlength="90"></textarea></td>
                      </tr>
                    </dqm:listSubsystems>
                  </table>
                </div>

              </td>
            </tr>
          </table>

        </div>
        <div id="edit_exp_com" width="100%" height="100%">
          <table width="100%">
            <tr>
              <td class="edit_col" width="100%">
                <div class="edit_sub_exp edit_sub_online_exp" id="edit_sub_online_tab">
                  <b>Subsystem: </b>
                  <select style="width: 80px"  name="expertsCommentSub" disabled="true" id="expertsCommentSub" onchange="textareaValue(this.value);">
                  <dqm:listSubsystems type="ONLINE">				  
                    <option  value="sub_${sub_abbr}">${sub_abbr}</option> 
                  </dqm:listSubsystems>
                  <dqm:listSubsystems type="OFFLINE">
                    <option  value="sub_${sub_abbr}">${sub_abbr}</option>
                  </dqm:listSubsystems>
                    </select>
                    <b>Status: <b/> 
                      <select style="width: 80px" name="expertCommentSel" id="expertCommentSel" class="sub_offline" disabled="true" onchange="expertsSel(this.value);"/>
                  </div>
              </td>
            </tr>
            <tr>
              <td class="edit_col" width="100%">
								<div class="edit_sub edit_sub_online">
									<b>Comments: <b/>
									<textarea name="expertsComment" style="width:100%;height:300px" readonly="true" id="expertsComment" onchange="updateExpertsComment(this.value)"></textarea>
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
