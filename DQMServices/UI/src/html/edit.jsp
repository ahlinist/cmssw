<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>

<%
  MessageUser user = MessageUser.get(request);
%>

<script type="text/javascript">

$(document).ready( function () {

  jQuery.fx.off = true;

  var _edit = $("#edit");
  var _edit_form = $("form", _edit);
  var _edit_info = $("td.edit_info", _edit_form);
  var _edit_l1t  = $("div.edit_l1t", _edit_form);
  var _edit_sub  = $("div.edit_sub", _edit_form);
  var _edit_sub_online  = $("#edit_sub_online_tab");
  var _edit_sub_offline = $("#edit_sub_offline_tab");
  var _edit_expert = $("#edit_expert_comments");

  $(_edit).dialog({
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

  var _btn_run_delete = $("button:contains('Delete Run')");
  $(_btn_run_delete).text("Delete");
  var _btn_run_apply  = $("button:contains('Apply Run')");
  $(_btn_run_apply).text("Apply");
  var _btn_run_finish = $("button:contains('Finish Run')");
  $(_btn_run_finish).text("Finish");

  $("#edit_sub_tabs").tabs();
  $("#edit_l1t_tabs").tabs();
  $("#edit_info_tabs").tabs();
  
  var expertCommentValue = function(name) {
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
    document.getElementById('expertComment').value = field;
    sel = document.getElementById(name);
  }
  
  $("#expertCommentSub").change(function(){ 
    expertCommentValue(this.value);
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
        txa.attr("error", "Comment for BAD subsystem must be set.").addClass("error");
      }
    } else if (sel.val() == "EXCL") {
      sel.css("color", "blue");
    }
  }

  var expertSel = function(value) {
    var name = document.getElementById('expertCommentSub').value;
    for (var i=0; i < sel.length; i++) {
      if (sel[i].value == value) {
        sel[i].selected = true;
      }
    }
    var sell = document.getElementById(name);
    var c = $(sell).parent().parent();
    validateSubSelectExp(c);
  } 
 
  $("#expertCommentSel", _edit_expert).change(function(){ 
    expertSel(this.value);
  });
  
  var updateexpertComment = function(comment) {
    var field = document.getElementById('expertCommentSub').value + '_comment';
    document.getElementById(field).value = comment;
    var sell = document.getElementById(field);
    var c = $(sell).parent().parent();
    validateSubSelectExp(c);
  }
  
  $("#expertComment", _edit_expert).keyup(function(){ 
    updateexpertComment(this.value);
  });

  $("#expertCommentTab").click(function(){ 
    expertCommentValue($("#expertCommentSub", _edit_expert).val());
  });

  var isTrue = function (s) {
    if (!s) return false;
    if ((s == "null") || (s == "")) return false;
    return true;
  }

  var validateInfo = function() {
    var glob = $("input[name=RUN_GLOBALNAME]", _edit_info);
    glob.removeAttr("error").removeClass("error");
    if (!glob.val() || ( glob.val() == "")) {
      glob.attr("error", "Global Name must be set.").addClass("error");
    }
  };

  var validateSubSelect = function(c) {
    var sel = $(c).find("select");
    var txa = $(c).find("textarea");

    txa.removeAttr("error").removeClass("error");
    if (sel.val() == "GOOD") {
      sel.css("color", "green");
    } else if (sel.val() == "NOTSET") {
      sel.css("color", "blue");
    } else if (sel.val() == "BAD") {
      sel.css("color", "red");
      if (txa.val() == "") {
        txa.attr("error", "Comment for BAD subsystem must be set.").addClass("error");
      }
    } else if (sel.val() == "EXCL") {
      sel.css("color", "blue");
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
 
    if ($("input:checkbox:checked", _edit_l1t).length == 0) {
      $("textarea", _edit_l1t).addClass("error").attr("error", "At least one L1 source must be set.");
    } else {
      $("textarea", _edit_l1t).removeClass("error").removeAttr("error");
    }

  };

  $("input[type=text]", _edit_info)
    .unbind("change")
    .change(function () { validateInfo(); })
    .unbind("keypress")
    .keypress(function () { validateInfo(); });
  $("textarea", _edit_info)
    .unbind("change")
    .change(function () { validateInfo(); })
    .unbind("keypress")
    .keypress(function () { validateInfo(); });

  $("select", _edit_sub).unbind("change").change(function () { validateSubSelect($(this).parent().parent()); });
  $("input[type=text]", _edit_sub).unbind("change").change(function () { validateSubSelect($(this).parent().parent()); });
  $("textarea", _edit_sub).unbind("change").change(function () { validateSubSelect($(this).parent().parent()); });

  $("#edit form .edit_l1t input[type=checkbox]").unbind("change").change(function () { validateL1T($(this).parent().parent()); });

  var build_xml = function (next_status) {

    var number = $("input[name=RUN_NUMBER]", _edit_form).val();
    var msg = { RUN_NUMBER: number };
    var status = $(_edit).data("current_status");

    status = (status != "" ? status : "ONLINE");

    $("input[type=text], textarea", _edit_info).each(function (i, o) {
      if ($(o).attr("readonly")) return;
      $(msg).attr($(o).attr("name"), $(o).val());
    });

    if (next_status) {
      if (status == "ONLINE") status = "OFFLINE";
      else if (status == "OFFLINE") status = "SIGNOFF";
      else if (status == "SIGNOFF") status = "COMPLETED";
    } else {
      $("select[name=RUN_STATUS] option:selected", _edit_info).each(function () {
        if ($(this).attr("readonly")) return;
        status = $(this).val();
      });
    }
    $(msg).attr("RUN_STATUS", status);

    $("input[type=checkbox]", _edit_l1t).each(function (i, o) {
      if ($(o).attr("readonly")) return;
      var name = $(o).attr("name");
      $(msg).attr(name, $(o).is(":checked"));
      if ($(o).is(":checked")) {
        $(msg).attr(name + "_comment", $("#" + name + "_comment").val());
      }
    });

    $("select", _edit_sub).each(function (i, o) {
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
        messageBox(o, "Please contact CMS DQM expert");
      },

      success: function(ret) {
        $(_edit).dialog("close");
        $("#flex1").flexReload();
      }

    });

  };

  $.showRunEditForm = function (number) {

    $('#edit_overlay').show();
    $(_edit).dialog('open');
    $(_edit).dialog('option', 'title', 'View/Edit Run #' + number);

    $(_btn_run_finish).hide();   
    $(_btn_run_apply).hide();
    $(_btn_run_delete).hide();

    $.ajax({
      type: "GET",
      url: "runeditdata?qtype=RUN_NUMBER&format=json&query=" + number,
      processData: false,
      async: true,
      dataType: "json",

      error: function(o) {
        messageBox(o, "Please contact CMS DQM expert");
        $(_edit).dialog('close');
      },

      success: function(json) {
	  
        if (json.rows.length == 0) {
          messageBox("Run not found", "Select a run and try again. If you experience this problem constantly - please contact CMS DQM expert.");
          $(_edit).dialog('close');
          return;
        }

        var run = json.rows[0];

        $("input", _edit).attr("readonly", "true");
        $("select", _edit).attr("readonly", "true");
        $("textarea", _edit).attr("readonly", "true"); 
        $("textarea", _edit_l1t).attr("disabled", "true");
        $("input[type=checkbox]", _edit_l1t).removeAttr("checked");
        $("select > option", _edit_sub_online).remove();
        $("select[name=RUN_STATUS]", _edit_info).empty();
        
        $("input[name=RUN_NUMBER]", _edit_form).val(number);

        jQuery.each(run, function (n, v) {

          $("select[name=sub_" + n + "]", _edit_sub).each(function (i, sel) {
            $("textarea[name=sub_" + n + "_comment]", _edit_sub).val($(run).attr(n + "_comment"));
          });

          $("select[name=sub_" + n + "][class=sub_online]", _edit_sub).each(function (i, sel) {
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

          $("select[name=sub_" + n + "][class=sub_offline]", _edit_sub).each(function (i, sel) {
            if (isTrue(v)) {
              if (v == "BAD") $(sel).val("BAD");
              if (v == "GOOD") $(sel).val("GOOD");
              if (v == "NOTSET") $(sel).val("NOTSET");
            } else {
              $(sel).val("NOTSET");
            };
          });

          $("input[name=" +n+ "]", _edit_l1t).each(function (i, sel) {
            if (isTrue(v)) {
              $(sel).attr("checked", true);
              $("textarea[name=" + n + "_comment]", _edit_l1t).val($(run).attr(n + "_comment"));
            }
          });

          if (n == "RUN_STATUS" && v != "" && v != null) {
            $("select[name=RUN_STATUS]", _edit_info).append($("<option readonly=\"true\" value=\" + $(o).text() + \">" + v + "</option>"));
          }

          $("input[name=" + n + "]", _edit_info).val(v);
          $("textarea[name=" + n + "]", _edit_info).val(v);

        });
		
        document.getElementById('expertCommentSub').selectedIndex = 0;
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
        document.getElementById('expertComment').value = field;

        validateInfo();
        $("input[type=checkbox]", _edit_l1t).each(function () { validateL1T($(this).parent().parent()); });
        $("select", _edit_sub).each(function () { validateSubSelect($(this).parent().parent()); });
		

        var real_role = "";
        var status = $(run).attr("RUN_STATUS"); 
        if (status == null) status = "";
		
        $(_edit).data("current_status", status);

        if ((status == 'COMPLETED') || (status == '') || (status == null))  {
          $(_btn_run_finish).hide();
        } else {
          $(_btn_run_finish).text("Finish " + status + " DQM");
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

          $(".edit_online input", _edit_info).removeAttr("readonly");
          $(".edit_online textarea", _edit_info).removeAttr("readonly");

          $("input", _edit_l1t).removeAttr("readonly").removeAttr("disabled");
          $("textarea", _edit_l1t).removeAttr("readonly").removeAttr("disabled");

          $("textarea", _edit_sub_online).removeAttr("readonly").removeAttr("disabled");
          $("select", _edit_sub_online).removeAttr("disabled").removeAttr("readonly");
		  
	  $("textarea", _edit_expert).removeAttr("readonly").removeAttr("disabled");
          $("select", _edit_expert).removeAttr("disabled").removeAttr("readonly");
          
          if (status == "") {
            $("select[name=RUN_STATUS]", _edit_info).append($("<option value=\"ONLINE\">ONLINE</option>"));
            $("input[name='RUN_ONLINE_SHIFTER']", _edit_info).val("<%=user.getName()%>");
          } else {
            $("select[name=RUN_STATUS]", _edit_info).append($("<option value=\"OFFLINE\">to OFFLINE</option>"));
            $(_btn_run_delete).show();
          }
          $("select[name=RUN_STATUS]", _edit_info).removeAttr("disabled").removeAttr("readonly");

          if (status != "") $(_btn_run_finish).show();
          $(_btn_run_apply).show();

        } else if (real_role == "OFFLINE")  {

          $(".edit_offline input", _edit_info).removeAttr("readonly");
          $(".edit_offline textarea", _edit_info).removeAttr("readonly");
          
          $("textarea", _edit_sub_offline).removeAttr("readonly");
          $("select", _edit_sub_offline).removeAttr("readonly").removeAttr("disabled");

          $("textarea", _edit_sub_online).removeAttr("readonly").removeAttr("disabled");
          $("select", _edit_sub_online).removeAttr("disabled").removeAttr("readonly");
			
          $("textarea", _edit_expert).removeAttr("readonly").removeAttr("disabled");
          $("select", _edit_expert).removeAttr("disabled").removeAttr("readonly");
          
          $("select[name=RUN_STATUS]", _edit_info).append($("<option value=\"SIGNOFF\">to SIGNOFF</option>"));
          $("select[name=RUN_STATUS]", _edit_info).removeAttr("disabled").removeAttr("readonly");

          if (status != "") $(_btn_run_finish).show();
          $(_btn_run_apply).show();

        } else if (real_role == "EXPERT") {

          $("input", _edit_l1t).removeAttr("readonly").removeAttr("disabled"); 
          $("textarea", _edit_l1t).removeAttr("readonly").removeAttr("disabled");

          $(".edit_online input", _edit_info).removeAttr("readonly");
          $(".edit_online textarea", _edit_info).removeAttr("readonly");

          $(".edit_offline input", _edit_info).removeAttr("readonly");
          $(".edit_offline textarea", _edit_info).removeAttr("readonly");
          
          $("textarea", _edit_sub_online).removeAttr("readonly").removeAttr("disabled");
          $("select", _edit_sub_online).removeAttr("readonly").removeAttr("disabled");
		  
          $("textarea", _edit_expert).removeAttr("readonly").removeAttr("disabled");
          $("select", _edit_expert).removeAttr("readonly").removeAttr("disabled");
		  
          $("textarea", _edit_sub_offline).removeAttr("readonly");
          $("select", _edit_sub_offline).removeAttr("readonly").removeAttr("disabled");

          if (status != "") $(_btn_run_finish).show();
          $(_btn_run_apply).show();
          $(_btn_run_delete).show();
          
          if (status == "") {
            $("select[name=RUN_STATUS]", _edit_info).append($("<option value=\"ONLINE\">ONLINE</option>"));
          } else {
            if (status != "OFFLINE") {
              $("select[name=RUN_STATUS]", _edit_info).append($("<option value=\"OFFLINE\">to OFFLINE</option>"));
            }
            if (status != "SIGNOFF") {
              $("select[name=RUN_STATUS]", _edit_info).append($("<option value=\"SIGNOFF\">to SIGNOFF</option>"));
            }
            $("select[name=RUN_STATUS]", _edit_info).append($("<option value=\"COMPLETED\">to COMPLETED</option>"));
          }
          $("select[name=RUN_STATUS]", _edit_info).removeAttr("disabled").removeAttr("readonly");

        } else {

          $("input[type=checkbox]", _edit_l1t).attr("disabled", true);
          $("textarea", _edit_l1t).attr("readonly", true);

          $("select[name=RUN_STATUS]", _edit_info).attr("disabled", true);
    
          $("select", _edit_sub_online).attr("disabled", true);
          $("select", _edit_sub_offline).attr("disabled", true);

          $(".edit_online textarea", _edit_info).attr("readonly", true);
          $(".edit_offline textarea", _edit_info).attr("readonly", true);
        }


        $(_btn_run_apply).unbind("click").click(function () {
          var err = $("#edit .error");
          if (err.length > 0) {
            $(err[0]).focus();
            messageBox($(err[0]).attr("error"), "Please correct data provided and try again.");
            return;
          };
          var msg = build_xml(false);
          if (msg) sendEditMessage(msg);
        });
		
        $(_btn_run_finish).unbind("click").click(function () {
          var err = $(".error", _edit);
          if (err.length > 0) {
            $(err[0]).focus();
            messageBox($(err[0]).attr("error"), "Please correct data provided and try again.");
            return;
          };
          var msg = build_xml(true);
          if (msg) sendEditMessage(msg);
        }); 

        $(_btn_run_delete).unbind("click").click(function () {
          var message = { 
            RUN_NUMBER: $("input[name=RUN_NUMBER]", _edit_form).val(),
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
	  <li><a href="#edit_expert_comments" id="expertCommentTab"><span>Comments</span></a></li>
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
              <td class="edit_online">
              <input name="RUN_GLOBALNAME" type="text" value="" readonly="true" maxlength="90" /></td>
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
        <div id="edit_expert_comments" width="100%" height="100%">
          <table width="100%">
            <tr>
              <td class="edit_col" width="100%">
                <div class="edit_sub_expert">
                  <b>Subsystem: </b>
                  <select style="width: 80px" disabled="true" id="expertCommentSub">
                    <dqm:listSubsystems type="ONLINE">				  
                      <option  value="sub_${sub_abbr}">${sub_abbr}</option> 
                    </dqm:listSubsystems>
                    <dqm:listSubsystems type="OFFLINE">
                      <option  value="sub_${sub_abbr}">${sub_abbr}</option>
                    </dqm:listSubsystems>
                  </select>
                  <b>Status: <b/> 
                    <select style="width: 80px" id="expertCommentSel" disabled="true"/>
                  </div>
              </td>
            </tr>
            <tr>
              <td class="edit_col" width="100%">
                <div class="edit_sub">
                  <b>Comments: <b/>
                  <textarea style="width:100%;height:300px" readonly="true" id="expertComment"></textarea>
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
