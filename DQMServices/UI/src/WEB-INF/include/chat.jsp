<%@ page import="cms.dqm.workflow.*" %>

<%
  User user = User.get(request);
  String mediaurl = WebUtils.getMediaURL();
%>

<script type="text/javascript">

  var openMessageBoard;
  var messageBoardResize;
  var doNotify;
  var _chat_blink = false;
  var _chat;

  $(document).ready( function () {

    _chat = $("#chat");
    var _chat_table = $("table", _chat);
    var _chat_message = $("#chat_message");
    var _chat_messages = $("#chat_messages");
    var _chat_users = $("#chat_users");
    var _chat_priority = $("#chat_priority");
    var _chat_notification = $("#chat_notification");
    var _chat_open_menu = $("#chat_open_menu");
    var _chat_send;
    var _chat_clear;

    var _chat_notification_title = "New message...!";
    var _chat_default_title = "";
    var _chat_notification_default_color = "#679CFB";
    var _chat_notification_notify_color = "white";

    doNotify = function(run) {
      
      var last = $("#chat").data("last");
      var read = $.cookie("last_read_message");
      if (!read) read = -1;

      if (last <= read) {
        _chat_blink = false;
        return; 
      }

      if (run) {
        _chat_blink = true;
        notify();
        if ($("div.ui-layout-south").css("display") == 'block') {
          doNotify(false);
        }
      } else {
        _chat_blink = false;
        $.cookie("last_read_message", last);
      }

    };

    var notify = function(n) {
      if (_chat_notification) {
        setTimeout(function() { 
          if (top.document.title != _chat_notification_title && top.document.title != _chat_default_title) {
            _chat_default_title = top.document.title;
          }
          if (n) {
            if (jQuery.fx.off) {
              _chat_notification.css("backgroundColor", _chat_notification_default_color);
            } else {
              _chat_notification.animate({ backgroundColor: _chat_notification_default_color }, 400);
            }
            top.document.title = _chat_default_title;
            if (!_chat_blink) return;
          } else {
            if (jQuery.fx.off) {
              _chat_notification.css("backgroundColor", _chat_notification_notify_color); 
            } else {
              _chat_notification.animate({ backgroundColor: _chat_notification_notify_color }, 400);
            }
            top.document.title = _chat_notification_title;
          }
          notify(!n);
        }, 500);
      }
    };

    messageBoardResize = function () {
      var h = $(_chat).parent().innerHeight() - 30;
      _chat_users.height(1);
      _chat_messages.height(1);
      _chat_table.height(h);
      h = _chat_users.parent().height() - 10;
      _chat_users.height(h);
      _chat_messages.height(h);
      _chat_messages.attr("scrollTop", _chat_messages.attr("scrollHeight"));
    };

    _chat.data("mtext", 0);
    _chat.data("mprio", 1);
    _chat.data("last", 0);
    _chat.data("busy", 0);
    _chat.data("last_user", 0);
    
    _chat_clear = $("button:contains('Clear message')");
    _chat_send = $("button:contains('Send message')");

    _chat.bind("sendMessage", function() {
      _chat.data("mtext", $.trim(_chat_message.val()));
      _chat.data("mprio", parseInt(_chat_priority.val()));
      _chat.trigger("getMessages");
      _chat_clear.trigger("click");
    });

    _chat.bind("getMessages", function() {

      if ($(this).data("busy") == 1) return;
      $(this).data("busy", 1);

      var url = "messageboard?";

      var mtext = $(this).data("mtext");
      if (mtext != 0) {
        url += "msg=" + escape(
          $.toJSON(
            { 
              text:     mtext, 
              priority: $(this).data("mprio") 
            }
          )
        );
        $(this).data("mtext", 0);
      } else {
        url += "msg=";
      }
      url += "&last=" + $(this).data("last");
      url += "&last_user=" + $(this).data("last_user");

      $("#chat_busy").show();

      $.ajax({
        type: "GET",
        url: url,
        processData: true,
        async: true,
        dataType: "json",

        error: function(o) {
          messageBox(o, "Please contact CMS DQM expert.");
          _chat.data("busy", 0);
          $("#chat_busy").hide();
        },

        success: function(data) {

          var last_user = _chat.data("last_user");
          var msg_changed = false;
          var usr_changed = false;
      
          <% if (user.isLogged()) { %>
          if (!data.logged) {
            messageBox("Session has expired", "Now you will be logged out. You can log in again afterwards.");
            logoutUser();
            return;
          }
          <% } %>

          jQuery.each(data.msgs, function (i, o) {
            var m = "";
            if (o.NAME == "RR" && !o.ROLES && !o.LOC ) {
              m += "<div class=\"rr_line rr_priority" + o.PRIO + "\">[" + o.TIME + "] " + o.TEXT + "</div>";
            } else {
              m += "<div class=\"chat_line_header\">[" + o.TIME + "] " + o.NAME + " " + (o.ROLES ? "(" + o.ROLES + ")" : "") + (o.LOC ? "@" + o.LOC : "") + "</div>";
              m += "<div class=\"chat_line_text chat_priority" + o.PRIO + "\">" + o.TEXT + "</div>";
            }
            _chat_messages.append("<p class=\"chat_line\" last=\"" + data.last_msg + "\"/>");
            _chat_messages.find("p:last").append(m);
            msg_changed = true;
          });
      
          if (last_user != data.last_user) {
            _chat_users.empty();
            jQuery.each(data.users, function (i, o) {
              var m = "<div class=\"chat_line_text\">" + o.name + "</div>";
              m += "<div class=\"chat_line_header\">" + (o.roles ? "(" + o.roles + ")": "") + (o.loc ? "@" + o.loc : "") + "</div>";
              _chat_users.append("<p class=\"chat_line\" last_user=\"" + data.last_user + "\">");
              _chat_users.find("p:last").append(m);
              usr_changed = true;
            });
          }

          _chat.data("last", data.last_msg);
          _chat.data("last_user", data.last_user);
          _chat.data("busy", 0);
          $("#chat_busy").hide();
          $('#chat_overlay').hide();

          if (msg_changed || usr_changed) {
            _chat_messages.attr("scrollTop", _chat_messages.attr("scrollHeight"));
            _chat_users.attr("scrollTop", _chat_users.attr("scrollHeight"));
            doNotify(msg_changed);
          }

          setTimeout(function() { _chat.trigger("getMessages"); }, 10000);

        }

      });

    });

    _chat_message.bind("keyup", function(e) {
      var v = $.trim(_chat_message.val());
      if (!v) {
        _chat_send.attr("disabled", "true");
        _chat_clear.attr("disabled", "true");
      } else {
        _chat_send.removeAttr("disabled");
        _chat_clear.removeAttr("disabled");
        if (e.keyCode == 13) _chat.trigger("sendMessage");
      }
    });

    _chat.height($(window).height() - 15);
    _chat_send.click(function(){
      _chat.trigger("sendMessage");
    });
    _chat_clear.click(function(){
      _chat_message.val("");
      _chat_send.attr("disabled", "true");
      _chat_clear.attr("disabled", "true");
    });

    messageBoardResize();

    _chat.trigger("getMessages");
    //$(document).everyTime(10000, function(i) { _chat.trigger("getMessages"); });

  });

</script>

<div id="chat" title="Message Board">
  <div id="chat_overlay"></div>
  <table border="0" width="100%">

    <tr height="100%">
      <td valign="top" width="100%"><div id="chat_messages"></div></td>
      <td valign="top" width="1%"><div id="chat_users"></div></td>
    </tr>

    <% if (user.isLogged()) { %>

    <tr height="0">
      <td width="100%" align="left" colspan="2">
      <label style="align: left;">Priority</label>
        <select id="chat_priority" style="width: 100px">
          <option value="2">High</option>
          <option value="1" selected>Normal</option>
          <option value="0">Low</option>
        </select>
        <button disabled="true" class="ui-state-default ui-corner-all ui-state-focus ui-state-hover">Send message</button>
        <button disabled="true" class="ui-state-default ui-corner-all ui-state-focus ui-state-hover">Clear message</button>
        <span id="chat_busy"><img src="<%=mediaurl%>img/load.gif" align="absmiddle"/></span>
      </td>
    </tr>
    <tr height="0">
      <td align="left" colspan="2">
        <textarea id="chat_message" style="width: 100%; height: 50px"></textarea>
      </td>
    </tr>

    <% } %>

  </table>
</div>
