<%@ page import="java.sql.*,java.util.*,java.util.regex.*,cms.dqm.workflow.*,java.security.cert.*" %>

<%

  MessageUser user = MessageUser.get(request);
  MessageBoardSyn.getInstance().logoutUser(user);
  
  String url = request.getRequestURL().toString();
  url = url.replaceFirst("^https://", "http://");
  url = url.replaceFirst("/[^/]+$", "/");

  response.sendRedirect(url);
  
%>
