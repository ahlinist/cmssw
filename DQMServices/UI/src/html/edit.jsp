<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>

<%
  MessageUser user = MessageUser.get(request);
%>

<script type="text/javascript">

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
      "Delete Run": function() { },
      "Apply Run": function() { },
      "Finish Run": function() { },
    }
  });

  $("button:contains('Delete Run')").attr("id", "btn-run-delete");
  $("button:contains('Apply Run')").attr("id", "btn-run-apply");
  $("button:contains('Finish Run')").attr("id", "btn-run-finish");
  $("#btn-run-delete").text("Delete");
  $("#btn-run-apply").text("Apply");
  $("#btn-run-finish").text("Finish");

  $("#edit_sub_tabs").tabs();
  $("#edit_l1t_tabs").tabs();
  $("#edit_info_tabs").tabs();
  
  var textareaValue = function(name) {
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
  
   $("#expertsCommentSub").change(function(){ 
	  textareaValue(this.value)
    });

  var validateSubSelectExp = function(c) {
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

  var expertsSel = function(value) {
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
 
   $("#expertCommentSel").change(function(){ 
	  expertsSel(this.value);
    });
  
  var updateExpertsComment = function(comment) {
    var field = document.getElementById('expertsCommentSub').value+'_comment';
    document.getElementById(field).value=comment;
    var sell = document.getElementById(field);
    var c = $(sell).parent().parent();
    validateSubSelectExp(c);
  }
  
   $("#expertsComment").keyup(function(){ 
	  updateExpertsComment(this.value);
    });

   $("#commentLink").click(function(){ 
	  textareaValue($("#expertsCommentSub").val());
    });

  var isTrue = function (s) {
    if (!s) return false;
    if ((s == "null") || (s == "")) return false;
    return true;
  }

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

  var build_xml = function (next_status) {

    var number = $("#edit form input[name=RUN_NUMBER]").val();
    var msg = { RUN_NUMBER: number };
    var status = $('#edit').attr("current_status");

    status = (status != "" ? status : "ONLINE");

    $("#edit .edit_info input[type=text], #edit .edit_info textarea").each(function (i, o) {
      if ($(o).attr("readonly")) return;
      $(msg).attr($(o).attr("name"), $(o).val());
    });

    if (next_status) {
      if (status == "ONLINE") status = "OFFLINE";
      else if (status == "OFFLINE") status = "SIGNOFF";
      else if (status == "SIGNOFF") status = "COMPLETED";
    } else {
      $("#edit .edit_info select[name=RUN_STATUS] option:selected").each(function () {
        if ($(this).attr("readonly")) return;
        status = $(this).val();
      });
    }
    $(msg).attr("RUN_STATUS", status);


    $("#edit .edit_l1t input[type=checkbox]").each(function (i, o) {
      if ($(o).attr("readonly")) return;
      var name = $(o).attr("name");
      $(msg).attr(name, $(o).is(":checked"));
      if ($(o).is(":checked")) {
        $(msg).attr(name + "_comment", $("#" + name + "_comment").val());
      }
    });

    $("#edit .edit_sub select").each(function (i, o) {
      if ($(o).attr("readonly")) return;
      if ($(o).attr("disabled")) return;
      var name = $(o).attr("name").replace("sub_", "");
      $(msg).attr(name, $(o).val());
      $(msg).attr(name + "_comment", $("#sub_" + name + "_comment").val());
    });

    if (status == "COMPLETED") {
      var tag = getTag();
      if (tag == null || tag == '') return false;
      $(msg).attr("TAG", tag);
    }

    return msg;
  };

  var sendEditMessage = function (o) {
    var message = $.toJSON( o );

    $.ajax({
      type: "POST",
      url: "editprovider",
      data: message,
      processData: false,
      dataType: "xml",

      error: function(o) {
        messageBox(o);
      },

      success: function(ret) {
        $("#edit").dialog("close");
        $("#flex1").flexReload();
      }

    });

  };

  $.showRunEditForm = function (number) {

    $('#edit_overlay').show();
    $('#edit').dialog('open');
    $('#edit').dialog('option', 'title', 'View/Edit Run #' + number);

    $("#btn-run-finish").hide();   
    $("#btn-run-apply").hide();
    $("#btn-run-delete").hide();

    $.ajax({
      type: "GET",
      url: "runeditdata?qtype=RUN_NUMBER&format=json&query=" + number,
      processData: false,
      async: true,
      dataType: "json",

      error: function(o) {
        messageBox(o);
        $('#edit').dialog('close');
      },

      success: function(json) {
	  
        if (json.rows.length == 0) {
          messageBox("Run not found");
          $('#edit').dialog('close');
          return;
        }

        var run = json.rows[0];

        $("#edit input").attr("readonly", "true");
        $("#edit select").attr("readonly", "true");
        $("#edit textarea").attr("readonly", "true"); 
        $("#edit .edit_l1t textarea").attr("disabled", "true");
        $("#edit .edit_l1t input[type=checkbox]").removeAttr("checked");
        $("#edit form .edit_sub .sub_online > option").remove();
        $("#edit .edit_info select[name=RUN_STATUS]").empty();
        
        $("#edit form input[name=RUN_NUMBER]").val(number);

        jQuery.each(run, function (n, v) {

          $("#edit form .edit_sub select[name=sub_" + n + "]").each(function (i, sel) {
            $("#edit form .edit_sub textarea[name=sub_" + n + "_comment]").val($(run).attr(n + "_comment"));
          });

          $("#edit form .edit_sub select[name=sub_" + n + "][class=sub_online]").each(function (i, sel) {
            if (isTrue(v) && v != "EXCL") {
              $(sel).append("<option value=\"GOOD\">GOOD</option>");
              $(sel).append("<option value=\"BAD\">BAD</option>");
              if (v == "BAD") 
                $(sel).val("BAD");
              else 
                $(sel).val("GOOD");
            } else {
              $(sel).append("<option value=\"EXCL\">EXCL</option>");
            };
          });

          $("#edit form .edit_sub select[name=sub_" + n + "][class=sub_offline]").each(function (i, sel) {
            if (isTrue(v)) {
              if (v == "BAD") $(sel).val("BAD");
              if (v == "GOOD") $(sel).val("GOOD");
              if (v == "NOTSET") $(sel).val("NOTSET");
            } else {
              $(sel).val("NOTSET");
            };
          });

          $("#edit form .edit_l1t input[name=" +n+ "]").each(function (i, sel) {
            if (isTrue(v)) {
              $(sel).attr("checked", true);
              $("#edit form .edit_l1t textarea[name=" +n+ "_comment]").val($(run).attr(n + "_comment")); //$(run).attr(n + "_comment")
            }
          });

          if (n == "RUN_STATUS" && v != "" && v != null) {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option readonly=\"true\" value=\"+ $(o).text() +\">"+ v +"</option>"));
          }

          $("#edit form .edit_info input[name=" + n + "]").val(v);
          $("#edit form .edit_info textarea[name=" + n + "]").val(v);

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
        var status = $(run).attr("RUN_STATUS"); 
        if (status == null) status = "";
		
        $('#edit').attr("current_status", status);

        if ((status == 'COMPLETED') || (status == '') || (status == null))  {
          $("#btn-run-finish").hide();
        } else {
          $("#btn-run-finish").text('Finish ' + status + ' DQM');
        }

        var count_tags = parseInt($(run).attr("RUN_COUNT_TAGS")); 
        count_tags--;

        if (status != "COMPLETED") {
          if (<%= user.hasLoggedRole(WebUtils.EXPERT) %>) real_role = "EXPERT";
          else if (<%= user.hasLoggedRole(WebUtils.ONLINE) %> && (status == "ONLINE" || status == ""))
            real_role = "ONLINE";
          else if (<%= user.hasLoggedRole(WebUtils.OFFLINE) %> && status == "OFFLINE")
            real_role = "OFFLINE";
        }
 
        if ($(run).attr("RUN_EVENTS") == "" ||  
            $(run).attr("RUN_START_TIME") == "" ||
            $(run).attr("RUN_RATE") == "" ||
            $(run).attr("RUN_L1KEY") == "" ||
            $(run).attr("RUN_HLTKEY") == "") {
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
            $("#btn-run-delete").show();
          }
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("disabled");
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("readonly");

          if (status != "") $("#btn-run-finish").show();
          $("#btn-run-apply").show();

        } else if (real_role == "OFFLINE")  {

          $("#edit form .edit_info .edit_offline input").removeAttr("readonly");
          $("#edit form .edit_info .edit_offline textarea").removeAttr("readonly");
          
          $("#edit form .edit_sub_offline textarea").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("disabled");

          $("#edit form .edit_sub_online textarea").removeAttr("readonly");
          $("#edit form .edit_sub_online select").removeAttr("disabled");
          $("#edit form .edit_sub_online select").removeAttr("readonly");
			
          $("#edit form .edit_sub_online_exp textarea").removeAttr("readonly");
          $("#edit form .edit_sub_online_exp select").removeAttr("disabled");
          $("#edit form .edit_sub_online_exp select").removeAttr("readonly");
          
          $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"SIGNOFF\">to SIGNOFF</option>"));
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("disabled");
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("readonly");

          if (status != "") $("#btn-run-finish").show();
          $("#btn-run-apply").show();

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

          if (status != "") $("#btn-run-finish").show();
          $("#btn-run-apply").show();
          $("#btn-run-delete").show();
          
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

        $("#btn-run-apply:visible").unbind("click").click(function () {
          var err = $("#edit .error");
          if (err.length > 0) {
            $(err[0]).focus();
            alert($(err[0]).attr("error"));
            return;
          };
          var msg = build_xml(false);
          if (msg) sendEditMessage(msg);
        });
		
        $("#btn-run-finish:visible").unbind("click").click(function () {
          var err = $("#edit .error");
          if (err.length > 0) {
            $(err[0]).focus();
            alert($(err[0]).attr("error"));
            return;
          };
          var msg = build_xml(true);
          if (msg) sendEditMessage(msg);
        }); 

        $("#btn-run-delete:visible").unbind("click").click(function () {
          var message = { 
            RUN_NUMBER: $("#edit form input[name=RUN_NUMBER]").val(),
            DELETE: true
          };
          if(!confirm("Do you really want to delete run #" + message.RUN_NUMBER +"?")) return;          
          sendEditMessage(message);
        });

        $('#edit_overlay').hide('fast');

      }
    });
  }

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
	  <li><a href="#edit_exp_com" id="commentLink"><span>Comments</span></a></li>
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
                  <select style="width: 80px"  name="expertsCommentSub" disabled="true" id="expertsCommentSub">
                  <dqm:listSubsystems type="ONLINE">				  
                    <option  value="sub_${sub_abbr}">${sub_abbr}</option> 
                  </dqm:listSubsystems>
                  <dqm:listSubsystems type="OFFLINE">
                    <option  value="sub_${sub_abbr}">${sub_abbr}</option>
                  </dqm:listSubsystems>
                    </select>
                    <b>Status: <b/> 
                      <select style="width: 80px" name="expertCommentSel" id="expertCommentSel" class="sub_offline" disabled="true"/>
                  </div>
              </td>
            </tr>
            <tr>
              <td class="edit_col" width="100%">
                <div class="edit_sub edit_sub_online">
                  <b>Comments: <b/>
                  <textarea name="expertsComment" style="width:100%;height:300px" readonly="true" id="expertsComment"></textarea>
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
