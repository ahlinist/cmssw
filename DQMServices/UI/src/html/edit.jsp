<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>

<script type="text/javascript">

function textCounter(field,  maxlimit) {
   if (field.value.length > maxlimit) {
    field.value=field.value.substring(0, maxlimit);
    alert('You can write up to ' + maxlimit + ' simbols in this field.');
   }
}

function fitToText()
{
 if (this.scrollHeight>this.clientHeight)
 {
   this.style.height=(this.scrollHeight+20)+"px";
 }
 else
 {
 this.style.height="10px";
 this.style.height=(this.scrollHeight+20)+"px";
 }
}



$(document).ready( function () {

  var rpcSubmit = function (xml) {
    //alert(xml);
    return "<?xml version=\"1.0\"?>\n"+xml;
  }

  var errorMessage = function (o) {
    alert("Error! Changes where not saved. Reason:\n\n" + o.responseText + "\n\nPlease copy above text and post ELOG message.");
  }

  var isTrue = function (s) {
    if (!s) return false;
    if ((s == "null") || (s == "")) return false;
    return true;
  }

  var escapeXML = function (s) {
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
    //keista
    //var inp = $(c).find("input");
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
        //keista
        txa.attr("error", "Comment for BAD subsystem must be set.");
        txa.addClass("error");
        //inp.attr("error", "Comment for BAD subsystem must be set.");
        // inp.addClass("error");
      }
    } else if (sel.val() == "EXCL") {
      sel.css("color", "blue");
      //keista
      if (txa.val() == "") {
        txa.attr("error", "Comment for EXCL subsystem must be set.");
        txa.addClass("error");

      
     // if (inp.val() == "") {
     //   inp.attr("error", "Comment for EXCL subsystem must be set.");
     //   inp.addClass("error");
      }
    }
  };

  var validateL1T = function (c) {
      var sel = $(c).find("input[type=checkbox]");
      //keista
      // var inp = $(c).find("input[type=text]");
      var txa = $(c).find("textarea");

      if (sel.is(":checked")) {
        //keista
        // inp.removeAttr("disabled");
        txa.removeAttr("disabled");
 
      } else {
        //keista
        // inp.attr("disabled", "true");
        txa.attr("disabled", "true");

      }
 
      if ($("#edit .edit_l1t input:checkbox:checked").length == 0) {
        //keista
       //$("#edit .edit_l1t input").addClass("error");
       //$("#edit .edit_l1t input").attr("error", "At least one L1 source must be set.");
        $("#edit .edit_l1t textarea").addClass("error");
        $("#edit .edit_l1t textarea").attr("error", "At least one L1 source must be set.");

      } else {
        //keista
        // $("#edit .edit_l1t input").removeClass("error");
        // $("#edit .edit_l1t input").removeAttr("error");
        $("#edit .edit_l1t textarea").removeClass("error");
        $("#edit .edit_l1t textarea").removeAttr("error");

      }
  };

  $("#edit form .edit_info input[type=text]").unbind("change").change(function () { validateInfo(); });
  $("#edit form .edit_info input[type=text]").unbind("keypress").keypress(function () { validateInfo(); });
  $("#edit form .edit_sub select").unbind("change").change(function () { validateSubSelect($(this).parent()); });
  $("#edit form .edit_sub input[type=text]").unbind("change").change(function () { validateSubSelect($(this).parent()); });
  $("#edit form .edit_l1t input[type=checkbox]").unbind("change").change(function () { validateL1T($(this).parent()); });
  $("#edit button.close_button").unbind("click").click(function () { $.hideRunEditForm(); });
  $("#edit form .edit_info input[name=RUN_GLOBALNAME]").autocomplete([<dqm:listGlobalNamesJS/>]);
  //keista
  $("#edit form .edit_info textarea").unbind("change").change(function () { validateInfo(); });
  $("#edit form .edit_info textarea").unbind("keypress").keypress(function () { validateInfo(); });
  $("#edit form .edit_sub textarea").unbind("change").change(function () { validateSubSelect($(this).parent()); });
  //$("#edit form .edit_l1t textarea").unbind("change").change(function () { validateL1T($(this).parent()); });



  var build_xml = function () {
    var xml = "";
    var status = "";

    $("#edit .edit_info input[type=text], #edit .edit_info textarea").each(function (i, o) {
            if ($(o).attr("readonly")) return;
            xml += "\t<" +$(o).attr("name")+ ">" + escapeXML($(o).val()) + "</" +$(o).attr("name")+ ">\n";
    });

    $("#edit .edit_info select[name=RUN_STATUS] option:selected").each(function () {
      if ($(this).attr("readonly")) return;
      status = $("#edit .edit_info select[name=RUN_STATUS]").val();
      xml += "\t<RUN_STATUS>"+ status  +"</RUN_STATUS>\n";
    });

    $("#edit .edit_l1t input[type=checkbox]").each(function (i, o) {
             
            if ($(o).attr("readonly")) return;
            
            var name = $(o).attr("name");
            xml += "\t<" +name+ ">" + $(o).is(":checked") + "</" +name+ ">\n";
            
            if ($(o).is(":checked"))
                 //keista 
                 //xml += "\t<" +name+ "_comment>" + escapeXML($(o).siblings("input[type=text]").val()) + "</" +name+ "_comment>\n";
                 xml += "\t<" +name+ "_comment>" + escapeXML($(o).siblings("textarea").val()) + "</" +name+ "_comment>\n";

    });

    $("#edit .edit_sub select").each(function (i, o) {
            if ($(o).attr("readonly")) return;
            if ($(o).attr("disabled")) return;
            if (! $(o).parent().is(":visible")) return;

            var name = $(o).attr("name").replace("sub_", "");
            xml += "\t<" +name+ ">" + $(o).val() + "</" +name+ ">\n";
            //keista
            //xml += "\t<" +name+ "_comment>" + escapeXML($(o).siblings("input[type=text]").val()) + "</" +name+ "_comment>\n";
            xml += "\t<" +name+ "_comment>" + escapeXML($(o).siblings("textarea").val()) + "</" +name+ "_comment>\n";
     
    });

    if (status == "COMPLETED") {
      var tag = getTag();
      xml += "\t<TAG>"+ tag + "</TAG>\n";
    }

    return xml
  };

  $.showRunEditForm = function (number) {

    $("#edit").hide();
    $("#edit_overlay").show();
    $("#edit_loader").show();
    $("#edit button.submit_button").text("Save");

    $.ajax({
      type: "GET",
      url: "runeditdata?qtype=RUN_NUMBER&query=" + number,
      processData: false,
      dataType: "xml",

      error: function(o) {
        errorMessage(o);
        $.hideRunEditForm();
      },

      success: function(ret) {
        var xml = $(ret).find("RESULTS");

        if ($(xml).find("ROW").length == 0) {
          alert("Run not found?");
          $.hideRunEditForm();
          return;
        }

        /* Pagrindine inicializacija */
        $("#edit input").attr("readonly", "true");
        $("#edit select").attr("readonly", "true");
        //keista
        $("#edit textarea").attr("readonly", "true"); 
        $("#edit button.submit_button, #edit button.delete_button").hide()
        // $("#edit .edit_l1t input[type=text]").attr("disabled", "true");
        //keista
        $("#edit .edit_l1t textarea").attr("disabled", "true");
        $("#edit .edit_l1t input[type=checkbox]").removeAttr("checked");
        //$("#edit .edit_sub_online select").parent().hide();
        //$(sel).parent().show();
        $("#edit form .edit_sub .sub_online > option").remove();
        $("#edit .edit_info select[name=RUN_STATUS]").empty();
        
        $("#edit form input[name=RUN_NUMBER]").val(number);

        $(xml).find("ROW").children().each(function (i, o) {

          $("#edit form .edit_sub select[name=sub_" +o.nodeName+ "]").each(function (i, sel) {
            //keista
            $("#edit form .edit_sub textarea[name=sub_" +o.nodeName+ "_comment]").val($(xml).find(o.nodeName+ "_comment").text());
            //$("#edit form .edit_sub input[name=sub_" +o.nodeName+ "_comment]").val($(xml).find(o.nodeName+ "_comment").text());
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
         //$("#edit form .edit_l1t textarea[name=" +o.nodeName+ "]").each(function (i, sel) {
            if (isTrue($(o).text())) {
            
              $(sel).attr("checked", true);
              //keista
              $("#edit form .edit_l1t textarea[name=" +o.nodeName+ "_comment]").val($(xml).find(o.nodeName+ "_comment").text());
              // $("#edit form .edit_l1t input[name=" +o.nodeName+ "_comment]").val($(xml).find(o.nodeName+ "_comment").text());
            }
          });

          if (o.nodeName == "RUN_STATUS" && $(o).text() != "") {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option readonly=\"true\" value=\"+ $(o).text() +\">"+ $(o).text() +"</option>"));
          }

          $("#edit form .edit_info input[name=" + o.nodeName + "]").val($(o).text());
          $("#edit form .edit_info textarea[name=" + o.nodeName + "]").val($(o).text());

        });

        validateInfo();
        $("#edit form .edit_l1t input[type=checkbox]").each(function () { validateL1T($(this).parent()); });
        $("#edit form .edit_sub select").each(function () { validateSubSelect($(this).parent()); });

        var real_role = "";
        var status = $(xml).find("RUN_STATUS").text();
        var count_tags = parseInt($(xml).find("RUN_COUNT_TAGS").text());
        count_tags--;
     
        if (status != "COMPLETED") {
          if (<%= WebUtils.hasLoggedRole(request, WebUtils.EXPERT) %>) real_role = "EXPERT";
          else if (<%= WebUtils.hasLoggedRole(request, WebUtils.ONLINE) %> && (status == "ONLINE" || status == ""))
            real_role = "ONLINE";
          else if (<%= WebUtils.hasLoggedRole(request, WebUtils.OFFLINE) %> && status == "OFFLINE")
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

          //$("#edit form .edit_l1t input").removeAttr("readonly");
          //$("#edit form .edit_l1t input").removeAttr("disabled");
          //keista
          $("#edit form .edit_l1t textarea").removeAttr("readonly");
          $("#edit form .edit_l1t textarea").removeAttr("disabled");


          //$("#edit form .edit_sub_online input").removeAttr("readonly");
          //keista
          $("#edit form .edit_sub_online textarea").removeAttr("readonly");
          $("#edit form .edit_sub_online select").removeAttr("disabled");
          $("#edit form .edit_sub_online select").removeAttr("readonly");
          
          if (status == "") {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"ONLINE\">ONLINE</option>"));
            $("#edit form .edit_info input[name='RUN_ONLINE_SHIFTER']").val("<%=WebUtils.getLoggedUser(request)%>");
          } else {
            $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"OFFLINE\">to OFFLINE</option>"));
            $("#edit button.delete_button").show();
          }
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("disabled");
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("readonly");

          $("#edit button.submit_button").show();

        } else if (real_role == "OFFLINE")  {

          $("#edit form .edit_info .edit_offline input").removeAttr("readonly");
          $("#edit form .edit_info .edit_offline textarea").removeAttr("readonly");
          
          //keista
          $("#edit form .edit_sub_offline textarea").removeAttr("readonly");
          // $("#edit form .edit_sub_offline input").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("disabled");

          if (count_tags != 0) {
            //keista
            $("#edit form .edit_sub_online textarea").removeAttr("readonly");
            //$("#edit form .edit_sub_online input").removeAttr("readonly");
            $("#edit form .edit_sub_online select").removeAttr("disabled");
            $("#edit form .edit_sub_online select").removeAttr("readonly");
          }
          
          $("#edit .edit_info select[name=RUN_STATUS]").append($("<option value=\"SIGNOFF\">to SIGNOFF</option>"));
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("disabled");
          $("#edit .edit_info select[name=RUN_STATUS]").removeAttr("readonly");

          $("#edit button.submit_button").show();

        } else if (real_role == "EXPERT") {

          $("#edit form .edit_l1t input").removeAttr("readonly");
          $("#edit form .edit_l1t input").removeAttr("disabled");
          //keista
          $("#edit form .edit_l1t textarea").removeAttr("readonly");
          $("#edit form .edit_l1t textarea").removeAttr("disabled");


          $("#edit form .edit_info .edit_online input").removeAttr("readonly");
          $("#edit form .edit_info .edit_online textarea").removeAttr("readonly");

          $("#edit form .edit_info .edit_offline input").removeAttr("readonly");
          $("#edit form .edit_info .edit_offline textarea").removeAttr("readonly");
          
          //keista
          $("#edit form .edit_sub_online textarea").removeAttr("readonly");
          //$("#edit form .edit_sub_online input").removeAttr("readonly");
          $("#edit form .edit_sub_online select").removeAttr("readonly");
          $("#edit form .edit_sub_online select").removeAttr("disabled");
          
          //keista
          $("#edit form .edit_sub_offline textarea").removeAttr("readonly");
          //$("#edit form .edit_sub_offline input").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("readonly");
          $("#edit form .edit_sub_offline select").removeAttr("disabled");

          $("#edit button.submit_button").show();
          $("#edit button.delete_button").show();
          
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
          //keista
          $("#edit form .edit_l1t textarea").attr("readonly", true);
          //$("#edit form .edit_l1t input").attr("readonly", true);

          $("#edit .edit_info select[name=RUN_STATUS]").attr("disabled", true);
    
          $("#edit form .edit_sub_online select").attr("disabled", true);
          $("#edit form .edit_sub_offline select").attr("disabled", true);

          $("#edit form .edit_info .edit_online textarea").attr("readonly", true);
          $("#edit form .edit_info .edit_offline textarea").attr("readonly", true);
        }

        $("#edit button.submit_button, #edit button.delete_button").unbind("click").click(function () {

          var number = $("#edit form input[name=RUN_NUMBER]").val();
          var xml = "<RUN>\n";
          xml += "\t<RUN_NUMBER>" + number + "</RUN_NUMBER>\n";
          
          if ($(this).hasClass("delete_button")) {
            if(!confirm("Do you really want to delete run #" + number +"?")) return;          
            xml += "\t<DELETE>true</DELETE>";
            $("#edit button.delete_button").text("Deleting....");
          } else {
            xml += build_xml();
            var err = $("#edit .error");
            if (err.length > 0) {
              $(err[0]).focus();
              alert($(err[0]).attr("error"));
              return;
            };
            $("#edit button.submit_button").text("Saving....");
          }
          xml += "</RUN>";

          $.ajax({
            type: "POST",
            url: "editprovider",
            data: rpcSubmit(xml),
            processData: false,
            dataType: "xml",
            error: function(o) {
              errorMessage(o);
              $("#edit button.submit_button").text("Save");
              $("#edit button.delete_button").text("Delete");
            },
            success: function(ret) {
              $("#edit button.submit_button").text("Save");
              $("#edit button.delete_button").text("Delete");
              $.hideRunEditForm();
              $("#flex1").flexReload();
              return;
            }
          });
        });

        $("#edit_loader").hide();
        $("#edit").show("fast");
      }
    });
  };

  $.hideRunEditForm = function () {
    $("#edit_overlay").hide();
    $("#edit").hide();
    $("#edit_loader").hide();
  };

  $("#edit_sub_tabs").tabs({ selected: 0 });
  $("#edit_l1t_tabs").tabs({ selected: 0 });
  $("#edit_info_tabs").tabs({ selected: 0 });

});
</script>

<div id="edit_overlay" style="display: none;" > </div>
<div id="edit_loader" style="display: none;" >
  <img src="media/img/load.gif"/>
</div>
<div id="edit" style="display: none;" >
<form>

  <table id="edit_table" width="100%" border="0">
    <tr>
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
      <td class="edit_info" valign="top">

        <ul id="edit_info_tabs">
          <li><a href="#edit_info_tab"><span>General Information</span></a></li>
          <li><a href="#edit_comp_tab"><span>Components</span></a></li>
        </ul>
        <div id="edit_info_tab">
          <table width="90%">
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
              <td colspan=3 class="edit_online"><textarea  name="RUN_STOP_REASON" readonly="true" maxlength="90" onKeyDown="texCounter(this.form.RUN_STOP_REASON,
1000)"
onKeyUp="textCounter(this.form.RUN_STOP_REASON, 1000)" oninput="fitToText.call(this)" onpopertychange="if (event.popertyName.toLowerCase()=='value') fitToText.call(this)"  /></textarea></td>
            </tr>
            <tr>
              <td><label for="RUN_ONLINE_COMMENT"> Online comment:  </label></td>
              <td colspan=3 class="edit_online"><textarea name="RUN_ONLINE_COMMENT" readonly="true" maxlength="90" onKeyDown="textCounter(this.form.RUN_ONLINE_COMMENT, 1000)"
onKeyUp="textCounter(this.form.RUN_NLINE_COMMENT, 1000);" oninput="fitToText.call(this)" onpropertychange="if (event.propertyName.toLowerCase()=='value') fitToText.call(this)"
></textarea></td>
            </tr>
            <tr>
              <td><label for="RUN_OFFLINE_COMMENT"> Offline comment:  </label></td>
              <td colspan=3 class="edit_offline"><textarea name="RUN_OFFLINE_COMMENT" id="RUN_OFFLINE_COMMENT" readonly="true" maxlength="90"
onKeyDown="textCounter(this.form.RUN_OFFLINE_COMMENT,1000)" onKeyUp="textCounter(this.form.RUN_OFFLINE_COMMENT,1000)" oninput="fitToText.call(this)"
onpropertychange="if (event.propertyName.toLowerCase()=='value') fitToText.call(this)" ></textarea></td>
            </tr>

          </table>
        </div>

        <div id="edit_comp_tab">
          <table>
            <tr>
              <td class="edit_col" width="50%">

                <ul id="edit_l1t_tabs">
                  <li><a href="#edit_l1t_tab"><span>L1 Triggers</span></a></li>
                </ul>
                <div class="edit_l1t" id="edit_l1t_tab">
                  <dqm:listL1Sources>
                  <div>
                    <label for="l1s_${l1s_abbr}"> ${l1s_abbr} </label>
                    <input name="l1s_${l1s_abbr}" type="checkbox" disabled="true" readonly="true"/> 
                    <!--<input name="l1s_${l1s_abbr}_comment" type="text" value="" readonly="true" maxlength="90" />-->
                    <textarea name="l1s_${l1s_abbr}_comment" id="l1s_${l1s_abbr}_comment" readonly="true"  style="height:20px;" maxlength="90"
onKeyDown="textCounter(this.form.sub_${sub_abbr}_comment, 1000)" onKeyUp="textCounter(this.form.sub_${sub_abbr}_comment, 1000)" oninput="fitToText.call(this)" onpropertychange="if
(event.propertyN$fitToText.call(this)"></textarea>
                  </div>
                  </dqm:listL1Sources>
                </div>
  
              </td>
              <td class="edit_col" width="50%">

                <ul id="edit_sub_tabs">
                  <li><a href="#edit_sub_online_tab"><span>DPG</span></a></li>
                  <li><a href="#edit_sub_offline_tab"><span>POG</span></a></li>
                </ul>
                <div class="edit_sub edit_sub_online" id="edit_sub_online_tab">
                  <dqm:listSubsystems type="ONLINE">
                  <div>
                    <label for="sub_${sub_abbr}"> ${sub_abbr} </label>
                    <select style="width: 80px" name="sub_${sub_abbr}" class="sub_online" disabled="true">
                    </select>
                   <!-- <input name="sub_${sub_abbr}_comment" type="text" value="" readonly="true" maxlength="90" />-->
                    <textarea name="sub_${sub_abbr}_comment" id="sub_${sub_abbr}_comment" readonly="true"  style="height:20px;" maxlength="90"
onKeyDown="textCounter(this.form.sub_${sub_abbr}_comment, 1000)" onKeyUp="textCounter(this.form.sub_${sub_abbr}_comment, 1000)" oninput="fitToText.call(this)" onpropertychange="if 
(event.propertyN$fitToText.call(this)"></textarea>

                  </div>
                  </dqm:listSubsystems>
                </div>
                <div class="edit_sub edit_sub_offline" id="edit_sub_offline_tab">
                  <dqm:listSubsystems type="OFFLINE">
                    <div>
                      <label for="sub_${sub_abbr}"> ${sub_abbr} </label>
                      <select style="width: 80px" name="sub_${sub_abbr}" class="sub_offline" disabled="true">
                        <option value="NOTSET">NOTSET</option>
                        <option value="GOOD">GOOD</option>
                        <option value="BAD">BAD</option>
                      </select>
                     <!-- <input name="sub_${sub_abbr}_comment" type="text" value="" readonly="true" maxlength="90" /> -->
                      <textarea name="sub_${sub_abbr}_comment" id="sub_${sub_abbr}_comment" readonly="true"  style="height:20px;" maxlength="90" 
onKeyDown="textCounter(this.form.sub_${sub_abbr}_comment, 1000)" onKeyUp="textCounter(this.form.sub_${sub_abbr}_comment, 1000)" oninput="fitToText.call(this)" onpropertychange="if (event.propertyName.toLowerCase()=='value') 
fitToText.call(this)"></textarea>

                    </div>
                  </dqm:listSubsystems>
                </div>

              </td>
            </tr>
          </table>

        </div>

      </td>
    </tr>
    <tr>
      <td align="center" valign="bottom">

        <button type="button" class="submit_button">Save</button>
        <button type="button" class="close_button">Close</button>
        <button type="button" class="delete_button">Delete</button>

      </td>
    </tr>
  </table>

</form>
</div>

