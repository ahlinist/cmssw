<%@ page import="cms.dqm.workflow.*" %>
<%@ taglib prefix="dqm" uri="/WEB-INF/cmsdqmworkflow.tld" %>

<%
  User user = User.get(request);
  String mediaurl = WebUtils.getMediaURL();
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

  <meta http-equiv="content-type" content="text/html; charset=utf-8" />
  <title>CMS DQM Run Registry</title>

  <link rel="stylesheet" type="text/css" href="<%=mediaurl%>index.css" />
  <link type="text/css" href="<%=mediaurl%>smoothness/jquery-ui-1.7.1.custom.css" rel="stylesheet" />

  <script type="text/javascript" src="<%=mediaurl%>jquery-1.3.2.min.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>jquery-ui-1.7.1.custom.min.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>jquery.timers.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>jquery.cookie.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>jquery.json-1.3.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>utils.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>jquery.layout.min.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>jquery.menu.min.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>ui.progressbar.js"></script>
  <script type="text/javascript" src="<%=mediaurl%>flot/jquery.flot.pack.js"></script>

<script type="text/javascript">

  var toggleAnimation;
  var logoutUser;
  var getTag;
  var globalResize;

  $(document).ready(function() {

    globalResize = function() {
      try {
        mainFrame.$("#flex1").flexHeight();
      } catch(e) { }
      messageBoardResize();
    };

    $(window).resize(function() {
      globalResize();
    });

    $("#logout_link").click(function() { window.top.logoutUser(); });

    $("#login_link").click(function(){
      window.top.location.href = window.top.location.href.replace(/^http:\/\//,'https:\/\/');
    });

    var Layout = $('body').layout(layoutSettings);

    toggleAnimation = function (value) {
      if (value != 0 && value != 1) value = 0;
      jQuery.fx.off = (value == 1 ? false : true); 
      $.cookie("animation", value);
      $("a.animation_option").text("Turn animation " + (jQuery.fx.off ? "on" : "off"));
      $("span.animation_value").text((jQuery.fx.off ? "OFF" : "ON"));
    };

    logoutUser = function() {
      frames['logout'].location.href = "https://login.cern.ch/adfs/ls/?wa=wsignout1.0";
      setTimeout("window.top.location.href = 'logout.jsp?redirect=' + window.top.location.href;", 200);
    };

    getTag = function() {
      return prompt('Please type in Tag value for COMPLETED run(s) (required)');
    };

    Layout.addToggleBtn('#chat_toggler', 'south');

    $("#dumpdatamenu").menu({
      hoverOpenDelay: 200 
    });

    $("#batchupdatemenu").menu({
      hoverOpenDelay: 200 
    });

    $("#helpmenu").menu({
      hoverOpenDelay: 200 
    });

    $("#optionsmenu").menu({
      hoverOpenDelay: 200 
    });

    toggleAnimation($.cookie("animation"));

    $("div.menu-item > a.animation_option").parent().click(function() { 
      toggleAnimation((jQuery.fx.off ? 1 : 0));
    });

    $("#batch_updater_progressbar").progressBar({ barImage: '<%=mediaurl%>img/progressbg_red.gif', boxImage: '<%=mediaurl%>img/progressbar.gif', showText: true});

    $("div.menu-item > a[change-status]").parent().click(function(){ 
      var status = $(this).find("a").attr("change-status");
      changeStatusTo(status);
    });

    $("div.menu-item > a[dump-intpl]").parent().click(function(){ 
      var intpl = $(this).find("a").attr("dump-intpl");
      var tpl   = $(this).find("a").attr("dump-tpl");
      var mime  = $(this).find("a").attr("dump-mime");
      mainFrame.dumpData(intpl, tpl, mime);
    });

    $(".ui-layout-toggler-south").attr("id", "chat_notification").hover(
      function() {
        $(this).css("backgroundColor", "#5386DF");
      },
      function() {
        $(this).css("backgroundColor", "#679CFB");
      }
    );

  });

  var layoutSettings = {
    name: "layout",
      defaults: {
        closable: true,
        resizable: true,
        slidable: true,
        size: "auto",
      },
      north: {
        slidable: false, 
        closable: false,
        resizable: false,
        size: 55,
        spacing_open: 0,
      },
      south: {
        slidable: true, 
        togglerLength_closed: '500',
        spacing_closed: 20,
        maxSize: Math.floor(screen.availHeight / 1.5),
        minSize: 200,
        size: 350,
        onresize_end: function () { globalResize(); },
        onshow_end: function () { doNotify(false); },
        onopen_end: function () { globalResize(); doNotify(false); },
        onclose_end: function () { globalResize(); doNotify(false); },
        togglerTip_open: "Hide Message Board",
        togglerTip_closed: "Open Message Board",
        resizerTip_open: "Resize Message Board",
        slideTrigger_open: "mouseover",
        initClosed: true,
      }
  };

</script>
</head>

<body>

  <jsp:include page="WEB-INF/include/messageBox.html" />

  <div class="ui-layout-north">

    <table width="100%">
      <tr>
        <td id="logo_img"></td>
        <td id="logo">CMS DQM Run Registry</td>
        <td id="login">
  
          <a id="chat_toggler">Message Board</a>
  
          &nbsp;|&nbsp;
  
  <% if (user.hasRole(User.EXPERT)) { %>
  
          <span id="batch_updater" style="display: none;" >
            Updating: 
            <span id="batch_updater_progressbar"></span>
          </span>
  
          <span id="batchupdatemenu"><a href="#">Update Selected</a>
            <ul>
              <li><a change-status="ONLINE">To ONLINE</a></li>
              <li><a change-status="OFFLINE">To OFFLINE</a></li>
              <li><a change-status="SIGNOFF">To SIGNOFF</a></li>
              <li><a change-status="COMPLETED">To COMPLETED</a></li>
            </ul>
          </span>
  
          &nbsp;|&nbsp;
  
  <% } %>        
  
          <a href="#" onclick="drawChart()">Plot Chart</a>
  
          &nbsp;|&nbsp;
      
          <a id="advanced_search" href="#" onClick="mainFrame.showSearchForm()">Advanced search</a>
  
          &nbsp;|&nbsp;
  
          <span id="dumpdatamenu"><a href="#">Dump Data</a>
            <ul>
              <li><a dump-intpl="xml,elog"     dump-tpl="" dump-mime="text/plain">Email</a></li>
              <li><a dump-intpl="xml,twiki"    dump-tpl="" dump-mime="text/plain">TWiki</a></li>
              <li><a dump-intpl="xml,text_csv" dump-tpl="" dump-mime="text/plain">Text&nbsp;(CSV)</a></li>
              <li><a dump-intpl="xml,text_tsv" dump-tpl="" dump-mime="text/plain">Text&nbsp;(TSV)</a></li>
              <li><a dump-intpl="xml"          dump-tpl="" dump-mime="text/xml">XML</a></li>
              <li><a dump-intpl="xml,table"    dump-tpl="" dump-mime="text/html">HTML&nbsp;Table</a></li>
            </ul>
          </span>
  
          &nbsp;|&nbsp;
  
  <% if (user.isLogged() && !user.isAuthorized()) { %>

          <a href="accrequest.jsp" target="mainFrame">Request Access</a>

          &nbsp;|&nbsp;

  <% } %>

          <span id="optionsmenu"><a href="#">Options</a>
            <ul>
              <li><a class="animation_option">Animation</a></li>
              <li><a href="cache.jsp" target="_blank">View cache</a></li>
            </ul>
          </span>
  
          &nbsp;|&nbsp;
  
          <span id="helpmenu"><a href="#">Help</a>
            <ul>
              <li><a href="help.html" target="_blank">Quick Help</a></li>
              <li><a href="https://twiki.cern.ch/twiki/bin/view/CMS/CMSDQMRunRegistry" target="_blank">Tutorial</a></li>
              <li><a href="https://twiki.cern.ch/twiki//bin/view/CMS/CMSDQMRunRegistryDev" target="_blank">Developer Notes</a></li>
            </ul>
          </span>
  
          &nbsp;|&nbsp;
  
          <% if (user.isLogged()) { %>
            Logged in as <b><%= user.getMessageLine() %></b>
            <a href="#" id="logout_link">Logout</a>
          <% } else { %>
            <a href="#" id="login_link">Login</a>
          <% } %>
  
        </td>
      </tr>
    </table>
  </div>

  <iframe 
    style="overflow: hide"
    id="mainFrame" 
    name="mainFrame" 
    class="ui-layout-center" 
    width="100%" 
    height="100%" 
    frameborder="0" 
    scrolling="no"
    src="main.jsp"></iframe>

  <DIV class="ui-layout-south" style="background-color: #DDDDDD">

    <jsp:include page="WEB-INF/include/chat.jsp" />
    
  </DIV>

  <iframe name="logout" width="1" height="1" src="" style="display:none;"></iframe>

  <jsp:include page="WEB-INF/include/plot.jsp" />

</body>
</html>


